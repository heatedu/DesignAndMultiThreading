#include <iostream>
#include <thread>
#include <semaphore>

int n = 5;
int current = 1;

std::binary_semaphore semZero(1); // Zero starts
std::binary_semaphore semOdd(0);
std::binary_semaphore semEven(0);

void printZero() {
    for (int i = 1; i <= n; i++) {
        semZero.acquire();
        std::cout << 0 << " ";
        if (i % 2 == 1)
            semOdd.release();
        else
            semEven.release();
    }
}

void printOdd() {
    for (int i = 1; i <= n; i += 2) {
        semOdd.acquire();
        std::cout << i << " ";
        semZero.release();
    }
}

void printEven() {
    for (int i = 2; i <= n; i += 2) {
        semEven.acquire();
        std::cout << i << " ";
        semZero.release();
    }
}

int main() {
    std::thread t1(printZero);
    std::thread t2(printOdd);
    std::thread t3(printEven);

    t1.join();
    t2.join();
    t3.join();
}
