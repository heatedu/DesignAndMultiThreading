#include <iostream>
#include <vector>
#include <thread>
#include <numeric>

// Cache-line size is typically 64 bytes
// alignas is available in C++11
/*alignas(64) tells the compiler:
        “Place every instance of this struct at a memory address that is a multiple of 64 bytes.”*/

struct alignas(64) PartialSum {
    long long value;
    PartialSum() : value(0) {}
};

void worker(
    const std::vector<int>& data,
    size_t start,
    size_t end,
    PartialSum& result)
{
    long long local_sum = 0;

    for (size_t i = start; i < end; ++i) {
        local_sum += data[i];
    }

    // Single write → no contention
    result.value = local_sum;
}

int main() {
    const size_t N = 10 * 1000 * 1000; // 10 million
    const unsigned int num_threads =
        std::thread::hardware_concurrency() == 0
        ? 4
        : std::thread::hardware_concurrency();

    std::vector<int> data(N, 1); // sum should be N
    std::vector<std::thread> threads;
    std::vector<PartialSum> partial(num_threads);

    size_t chunk_size = N / num_threads;

    for (unsigned int i = 0; i < num_threads; ++i) {
        size_t start = i * chunk_size;
        size_t end = (i == num_threads - 1)
            ? N
            : start + chunk_size;

        threads.push_back(
            std::thread(
                worker,
                std::cref(data),
                start,
                end,
                std::ref(partial[i])
            )
        );
    }

    for (size_t i = 0; i < threads.size(); ++i) {
        threads[i].join();
    }

    // Reduce phase (single-threaded)
    long long total_sum = 0;
    for (size_t i = 0; i < partial.size(); ++i) {
        total_sum += partial[i].value;
    }

    std::cout << "Parallel sum: " << total_sum << std::endl;

    // Verification
    long long expected =
        std::accumulate(data.begin(), data.end(), 0LL);
    std::cout << "Expected sum: " << expected << std::endl;

    return 0;
}

/*
Q: Why not use a mutex?
→ Lock inside loop serializes execution.

Q: Why not atomics?
→ Atomics still cause cache-line contention.

Q: When will this stop scaling?
→ Memory bandwidth limit.

Q: Production version?
→ Thread pool / task scheduler.
    
*/
