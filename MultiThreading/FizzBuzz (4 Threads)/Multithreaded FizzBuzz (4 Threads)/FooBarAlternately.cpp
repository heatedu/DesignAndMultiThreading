#include <iostream>
#include <thread>
#include <semaphore>

int n = 5;

std::binary_semaphore semFoo(1);  // Foo starts
std::binary_semaphore semBar(0);

void printFoo() {
    for (int i = 0; i < n; i++) {
        semFoo.acquire();
        std::cout << "Foo";
        semBar.release();
    }
}

void printBar() {
    for (int i = 0; i < n; i++) {
        semBar.acquire();
        std::cout << "Bar";
        semFoo.release();
    }
}

int main() {
    std::thread t1(printFoo);
    std::thread t2(printBar);

    t1.join();
    t2.join();
}
