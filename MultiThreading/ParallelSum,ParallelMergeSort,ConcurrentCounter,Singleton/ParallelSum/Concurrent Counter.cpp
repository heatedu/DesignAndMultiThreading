#include <iostream>
#include <thread>
#include <vector>
#include <mutex>

class MutexCounter {
private:
    long long value;
    std::mutex mtx;

public:
    MutexCounter() : value(0) {}

    void increment() {
        std::lock_guard<std::mutex> lock(mtx);
        ++value;
    }

    long long get() {
        std::lock_guard<std::mutex> lock(mtx);
        return value;
    }
};

void worker_mutex(MutexCounter& counter, int iterations) {
    for (int i = 0; i < iterations; i++) {
        counter.increment();
    }
}

int main() {
    const int NUM_THREADS = 4;
    const int ITERATIONS = 1'00'000;

    MutexCounter counter;
    std::vector<std::thread> threads;

    for (int i = 0; i < NUM_THREADS; i++) {
        threads.emplace_back(worker_mutex, std::ref(counter), ITERATIONS);
    }

    for (auto& t : threads) {
        t.join();
    }

    std::cout << "[MutexCounter] Final Value = " << counter.get() << std::endl;
}


/*
    Output:
        [MutexCounter] Final Value = 400000
*/
