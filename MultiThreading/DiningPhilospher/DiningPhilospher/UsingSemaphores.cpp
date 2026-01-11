#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <semaphore>

using namespace std;

const int N = 4;

// Semaphore allowing only N-1 philosophers to try eating
counting_semaphore<N> room(N - 1);

// One semaphore per fork (binary semaphore)
vector<binary_semaphore> forks(N, binary_semaphore(1));

void philosopher(int id)
{
    int left  = id;
    int right = (id + N - 1) % N;

    // Thinking
    cout << "Philosopher " << id << " is thinking\n";
    this_thread::sleep_for(chrono::milliseconds(100));

    // Enter room (limit contention)
    room.acquire();

    // Pick forks
    forks[left].acquire();
    cout << "Philosopher " << id << " picked fork " << left << "\n";

    forks[right].acquire();
    cout << "Philosopher " << id << " picked fork " << right << "\n";

    // Eating (Critical Section)
    cout << "Philosopher " << id << " is eating\n";
    this_thread::sleep_for(chrono::milliseconds(200));

    // Put down forks
    forks[right].release();
    forks[left].release();

    // Leave room
    room.release();

    cout << "Philosopher " << id << " finished eating\n";
}

int main()
{
    vector<thread> philosophers;

    for (int i = 0; i < N; i++)
        philosophers.emplace_back(philosopher, i);

    for (auto &t : philosophers)
        t.join();

    cout << "All philosophers are done.\n";
    return 0;
}