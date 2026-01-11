#include <iostream>
#include <mutex>
#include <thread>

class Singleton {
private:
    static Singleton* instance;
    static std::mutex mtx;

    Singleton() {
        std::cout << "Singleton constructed\n";
    }

public:
    static Singleton* getInstance() {
        if (instance == nullptr) {              // First check (no lock)
            std::lock_guard<std::mutex> lock(mtx);
            if (instance == nullptr) {          // Second check (with lock)
                instance = new Singleton();
            }
        }
        return instance;
    }

    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
};

Singleton* Singleton::instance = nullptr;
std::mutex Singleton::mtx;

void worker() {
    std::cout << Singleton::getInstance() << std::endl;
}

int main() {
    std::thread t1(worker);
    std::thread t2(worker);
    std::thread t3(worker);

    t1.join();
    t2.join();
    t3.join();

    return 0;
}
