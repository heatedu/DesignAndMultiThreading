#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <chrono>

using namespace std;

class BoundedBuffer {
private:
    vector<int> buffer;
    int size;
    int cnt;
    int buffin;
    int buffout;

    mutex mtx;
    condition_variable not_full;
    condition_variable not_empty;

public:
    BoundedBuffer(int n) : size(n), cnt(0), buffin(0), buffout(0) {
        buffer.resize(n);
    }

    // Producer: blocking push
    void produce(int item) {
        unique_lock<mutex> lock(mtx);

        // wait while buffer is full
        while (cnt == size) {
            not_full.wait(lock);
        }

        buffer[buffin] = item;
        buffin = (buffin + 1) % size;
        cnt++;

        cout << "Produced: " << item << endl;

        // buffer is not empty now → wake one consumer
        not_empty.notify_one();
    }

    // Consumer: blocking pop
    int consume() {
        unique_lock<mutex> lock(mtx);

        // wait while buffer is empty
        while (cnt == 0) {
            not_empty.wait(lock);
        }

        int item = buffer[buffout];
        buffout = (buffout + 1) % size;
        cnt--;

        cout << "Consumed: " << item << endl;

        // buffer has space now → wake one producer
        not_full.notify_one();
        return item;
    }
};

// Producer thread function
void producer(BoundedBuffer& buf, int id) {
    for (int i = 1; i <= 5; i++) {
        buf.produce(i + id * 100);
        this_thread::sleep_for(chrono::milliseconds(100));
    }
}

// Consumer thread function
void consumer(BoundedBuffer& buf, int id) {
    for (int i = 1; i <= 5; i++) {
        buf.consume();
        this_thread::sleep_for(chrono::milliseconds(150));
    }
}

int main() {
    BoundedBuffer buffer(5);

    thread p1(producer, ref(buffer), 1);
    thread p2(producer, ref(buffer), 2);
    thread c1(consumer, ref(buffer), 1);
    thread c2(consumer, ref(buffer), 2);

    p1.join();
    p2.join();
    c1.join();
    c2.join();

    return 0;
}