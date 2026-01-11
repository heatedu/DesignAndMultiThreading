#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <semaphore>

using namespace std;

class BoundedBuffer {
private:
    vector<int> buffer;
    int size;
    int buffin;
    int buffout;

    counting_semaphore<> empty; // empty slots
    counting_semaphore<> full;  // filled slots
    binary_semaphore mutex;     // mutual exclusion

public:
    BoundedBuffer(int n)
        : size(n),
        buffin(0),
        buffout(0),
        empty(n),
        full(0),
        mutex(1)
    {
        buffer.resize(n);
    }

    // Producer
    void produce(int item) {
        empty.acquire();   // wait for empty slot
        mutex.acquire();   // enter critical section

        buffer[buffin] = item;
        buffin = (buffin + 1) % size;

        cout << "Produced: " << item << endl;

        mutex.release();   // exit critical section
        full.release();    // signal item available
    }

    // Consumer
    void consume() {
        full.acquire();    // wait for available item
        mutex.acquire();   // enter critical section

        int item = buffer[buffout];
        buffout = (buffout + 1) % size;

        cout << "Consumed: " << item << endl;

        mutex.release();   // exit critical section
        empty.release();  // signal empty slot
    }
};

// Producer thread
void producer(BoundedBuffer& buf, int id) {
    for (int i = 1; i <= 5; i++) {
        buf.produce(i + id * 100);
        this_thread::sleep_for(chrono::milliseconds(100));
    }
}

// Consumer thread
void consumer(BoundedBuffer& buf) {
    for (int i = 1; i <= 5; i++) {
        buf.consume();
        this_thread::sleep_for(chrono::milliseconds(150));
    }
}

int main() {
    BoundedBuffer buffer(5);

    thread p1(producer, ref(buffer), 1);
    thread p2(producer, ref(buffer), 2);
    thread c1(consumer, ref(buffer));
    thread c2(consumer, ref(buffer));

    p1.join();
    p2.join();
    c1.join();
    c2.join();

    return 0;
}
