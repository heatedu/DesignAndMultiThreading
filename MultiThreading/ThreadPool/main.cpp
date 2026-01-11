#include "SimpleThreadPool.h"
#include <iostream>
#include <chrono>

int main() {
    SimpleThreadPool pool(3); // 3 worker threads

    for (int i = 1; i <= 5; i++) {
        pool.addTask([i] {
            std::cout << "Task " << i << " running on thread "
                      << std::this_thread::get_id()
                      << std::endl;

            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        });
    }

    std::this_thread::sleep_for(std::chrono::seconds(3));
    std::cout << "Main exit\n";
    return 0;
}
