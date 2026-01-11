#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <mutex>
#include <condition_variable>

class ReusableBarrier {
private:
    std::mutex mtx;
    std::condition_variable cv;
    int count;
    const int total;
    int generation;

public:
    explicit ReusableBarrier(int n)
        : count(0), total(n), generation(0) {
    }

    void wait() {
        std::unique_lock<std::mutex> lock(mtx);

        int my_generation = generation;
        count++;

        // Last thread arrives
        if (count == total) {
            count = 0;          // reset for next round
            generation++;       // advance generation
            cv.notify_all();    // release all threads
        }
        else {
            // Wait until generation changes
            cv.wait(lock, [this, my_generation]() {
                return generation != my_generation;
                });
        }
    }
};
void worker(int id, ReusableBarrier& barrier) {
    for (int round = 1; round <= 3; ++round) {
        // Simulate work before barrier
        std::this_thread::sleep_for(std::chrono::milliseconds(100 * id));
        std::cout << "Thread " << id
            << " reached barrier in round " << round << "\n";

        barrier.wait();  // reusable synchronization point

        // Work after barrier
        std::cout << "Thread " << id
            << " passed barrier in round " << round << "\n";
    }
}

int main() {
    const int N = 5;
    ReusableBarrier barrier(N);

    std::vector<std::thread> threads;

    for (int i = 1; i <= N; ++i) {
        threads.emplace_back(worker, i, std::ref(barrier));
    }

    for (auto& t : threads) {
        t.join();
    }

    std::cout << "All threads completed all rounds\n";
    return 0;
}
