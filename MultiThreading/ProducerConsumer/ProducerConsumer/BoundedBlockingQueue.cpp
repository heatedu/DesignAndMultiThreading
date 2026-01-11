#include <iostream>
#include <thread>
#include <deque>
#include <semaphore>

using namespace std;

class BoundedBlockingQueue {
private:
    counting_semaphore<> full;    // number of filled slots
    counting_semaphore<> empty;   // number of empty slots
    binary_semaphore mutex;       // mutual exclusion for deque

    deque<int> dq;                // NOT thread-safe by itself

public:
    BoundedBlockingQueue(int capacity)
        : full(0), empty(capacity), mutex(1) {
    }

    // enqueue (Producer)
    void enqueue(int element) {
        empty.acquire();          // wait for empty slot
        mutex.acquire();          // enter critical section

        dq.push_front(element);   // add item

        mutex.release();          // exit critical section
        full.release();           // signal item available
    }

    // dequeue (Consumer)
    int dequeue() {
        full.acquire();           // wait for available item
        mutex.acquire();          // enter critical section

        int result = dq.back();
        dq.pop_back();            // remove item

        mutex.release();          // exit critical section
        empty.release();          // signal empty slot

        return result;
    }

    // size (non-blocking, but protected)
    int size() {
        mutex.acquire();
        int result = dq.size();
        mutex.release();
        return result;
    }
};
