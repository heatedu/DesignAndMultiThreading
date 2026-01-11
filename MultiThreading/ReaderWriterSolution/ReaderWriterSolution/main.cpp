#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <chrono>

using namespace std;

mutex m;
condition_variable cv;

int cntReader = 0;
int cntWriter = 0;

/*
IMPORTANT NOTES:
    1.)This is a reader-priority solution, so writers may starve.
            New readers keep entering while writers wait
            cntReader never reaches zero.
    2.)Always use notify inside the lock only.
    3.)While is used instead of if to avoid sprunious wakeups
    */

// ------------------ Reader ------------------
void reader(int id)
{
    // Entry section
    unique_lock<mutex> lock(m);
    while (cntWriter > 0) {
        cv.wait(lock);
    }
    cntReader++;
    lock.unlock();

    // Critical Section (Reading)
    cout << "Reader " << id << " is READING\n";
    this_thread::sleep_for(chrono::milliseconds(500));

    // Exit section
    lock.lock();
    cntReader--;
    if (cntReader == 0) {
        cv.notify_all();
    }
    lock.unlock();
}

// ------------------ Writer ------------------
void writer(int id)
{
    // Entry section
    unique_lock<mutex> lock(m);
    while (cntReader > 0 || cntWriter > 0) {
        cv.wait(lock);
    }
    cntWriter++;
    lock.unlock();

    /*If we keep the lock during the critical section :
        That would mean :
        All readers are blocked
        Other writers are blocked
        Even threads just checking conditions are blocked
        👉 This kills concurrency and is not required for correctness
        
        We want logical blocking, not mutex blockin,so Readers finishing cannot decrement cntReader , Waiting threads can’t even check conditions, Entire system becomes single-threaded bottleneck.
    */

    // Critical Section (Writing)
    cout << "Writer " << id << " is WRITING\n";
    this_thread::sleep_for(chrono::milliseconds(800));

    // Exit section
    lock.lock();
    cntWriter--;
    cv.notify_all();
    lock.unlock();
}

// ------------------ Main ------------------
int main()
{
    vector<thread> threads;

    // Readers
    for (int i = 1; i <= 3; i++) {
        threads.emplace_back(reader, i);
    }

    // Writers
    for (int i = 1; i <= 2; i++) {
        threads.emplace_back(writer, i);
    }

    // More Readers
    for (int i = 4; i <= 6; i++) {
        threads.emplace_back(reader, i);
    }

    // Join all threads
    for (auto& t : threads) {
        t.join();
    }

    return 0;
}
