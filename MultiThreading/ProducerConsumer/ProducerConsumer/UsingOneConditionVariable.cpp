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
    condition_variable cv;

public:
    BoundedBuffer(int n) : size(n), cnt(0), buffin(0), buffout(0) {
        buffer.resize(n);
    }

    // Producer: blocking push
    void produce(int item) {
        unique_lock<mutex> lock(mtx);

        // wait while buffer is full
        while (cnt == size) {
            cv.wait(lock);
        }

        buffer[buffin] = item;
        buffin = (buffin + 1) % size;
        cnt++;

        cout << "Produced: " << item << endl;

        cv.notify_all(); // wake up waiting consumers
    }

    // Consumer: blocking pop
    int consume() {
        unique_lock<mutex> lock(mtx);

        // wait while buffer is empty
        while (cnt == 0) {
            cv.wait(lock);
        }

        int item = buffer[buffout];
        buffout = (buffout + 1) % size;
        cnt--;

        cout << "Consumed: " << item << endl;

        cv.notify_all(); // wake up waiting producers
        return item;
    }
};

// Producer thread function
void producer(BoundedBuffer& buf) {
    for (int i = 1; i <= 10; i++) {
        buf.produce(i);
        this_thread::sleep_for(chrono::milliseconds(100));
    }
}

// Consumer thread function
void consumer(BoundedBuffer& buf) {
    for (int i = 1; i <= 10; i++) {
        buf.consume();
        this_thread::sleep_for(chrono::milliseconds(150));
    }
}

int main() {
    BoundedBuffer buffer(5); // buffer size = 5

    thread p(producer, ref(buffer));
    thread c(consumer, ref(buffer));

    p.join();
    c.join();

    return 0;
}