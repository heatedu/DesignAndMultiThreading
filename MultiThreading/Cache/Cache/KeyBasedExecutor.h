#pragma once
#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>
#include <memory>
#include <atomic>

// Key-based executor: ensures all operations for the same key run on the same thread
class KeyBasedExecutor {
private:
    struct Task {
        std::function<void()> func;
    };

    struct ExecutorThread {
        std::thread thread;
        std::queue<Task> taskQueue;
        std::mutex mutex;
        std::condition_variable cv;
        std::atomic<bool> shouldStop{false};

        void run() {
            while (true) {
                Task task;
                {
                    std::unique_lock<std::mutex> lock(mutex);
                    cv.wait(lock, [this]() { 
                        return shouldStop.load() || !taskQueue.empty(); 
                    });

                    if (shouldStop.load() && taskQueue.empty()) {
                        break;
                    }

                    if (!taskQueue.empty()) {
                        task = std::move(taskQueue.front());
                        taskQueue.pop();
                    } else {
                        continue;
                    }
                }
                
                task.func();
            }
        }

        void enqueue(Task task) {
            {
                std::lock_guard<std::mutex> lock(mutex);
                taskQueue.push(std::move(task));
            }
            cv.notify_one();
        }

        void stop() {
            shouldStop.store(true);
            cv.notify_all();
        }
    };

    std::vector<std::unique_ptr<ExecutorThread>> executors;
    int numExecutors;

public:
    explicit KeyBasedExecutor(int numExecs) : numExecutors(numExecs) 
    {
        executors.reserve(numExecutors);
        for (int i = 0; i < numExecutors; ++i) 
        {
            executors.emplace_back(std::make_unique<ExecutorThread>());
            executors.back()->thread = std::thread(&ExecutorThread::run, executors.back().get());
        }
    }

    ~KeyBasedExecutor() {
        shutdown();
    }

    // Submit task for a given key (ensures same key goes to same thread)
    template<typename K, typename Func>
    auto submitTask(const K& key, Func&& func) -> std::future<decltype(func())> {
        using ReturnType = decltype(func());
        
        int index = getExecutorIndexForKey(key);
        auto taskPtr = std::make_shared<std::packaged_task<ReturnType()>>(std::forward<Func>(func));
        std::future<ReturnType> future = taskPtr->get_future();

        Task task;
        task.func = [taskPtr]() { (*taskPtr)(); };

        executors[index]->enqueue(std::move(task));
        
        return future;
    }

    // Get executor index for a key using hash
    template<typename K>
    int getExecutorIndexForKey(const K& key) const {
        std::hash<K> hasher;
        size_t hash = hasher(key);
        return static_cast<int>(hash % numExecutors);
    }

    void shutdown() {
        for (auto& executor : executors) {
            if (executor && executor->thread.joinable()) {
                executor->stop();
                executor->thread.join();
            }
        }
    }
};

