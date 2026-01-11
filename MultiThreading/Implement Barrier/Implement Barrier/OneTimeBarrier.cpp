#include <mutex>
#include <condition_variable>
#include <iostream>
#include <thread>
#include <vector>
#include <chrono>


/*
If total > number of threads that ever call wait()`, then all threads will wait forever.
That is expected and correct behavior for a normal barrier.
Why this happens (step-by-step)
Recall the barrier condition:
count == total
The barrier only opens when:
count reaches total
i.e., exactly total threads call wait()
*/
class Barrier {
private:
    std::mutex mtx;
    std::condition_variable cv;
    int count;
    const int total;

public:
    explicit Barrier(int n) : count(0), total(n) {}

    void wait() {
        std::unique_lock<std::mutex> lock(mtx);

        count++;
        // If last thread arrives
        if (count == total) {
            cv.notify_all();     // last thread releases all
        }
        else {
            cv.wait(lock, [this]() {
                return count == total;  //Spurious wakeups are handled because cv.wait(lock, predicate) internally rechecks the condition in a loop. The outer if only determines whether the current thread should wait or release others; it does not replace the predicate-based wait.
                });
        }
    }
};
void worker(int id, Barrier& barrier) {
    // Simulate work before barrier
    std::this_thread::sleep_for(std::chrono::milliseconds(1000 * id));
    std::cout << "Thread " << id << " reached the barrier\n";

    barrier.wait();   // synchronization point

    // Work after barrier
    std::cout << "Thread " << id << " passed the barrier\n";
}

int main() {
    const int N = 5;
    Barrier barrier(N);

    std::vector<std::thread> threads;

    for (int i = 1; i <= N; ++i) {
        threads.emplace_back(worker, i, std::ref(barrier));
    }

    for (auto& t : threads) {
        t.join();
    }

    std::cout << "All threads completed\n";
    return 0;
}