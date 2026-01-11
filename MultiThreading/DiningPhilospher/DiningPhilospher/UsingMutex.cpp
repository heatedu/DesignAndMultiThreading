#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <chrono>

using namespace std;

const int N = 4;
vector<mutex> forks(N);

void philosopher(int id)
{
    int left = id;
    int right = (id + N - 1) % N;

    // Thinking
    cout << "Philosopher " << id << " is thinking\n";
    this_thread::sleep_for(chrono::milliseconds(100));

    // Deadlock avoidance:
    // Philosopher 0 picks right fork first
    //Deadlock prevention is about controlling resource acquisition order, not resource release.
    if (id == 0)
    {
        forks[right].lock();
        cout << "Philosopher " << id << " picked fork " << right << "\n";

        forks[left].lock();
        cout << "Philosopher " << id << " picked fork " << left << "\n";
    }
    else
    {
        forks[left].lock();
        cout << "Philosopher " << id << " picked fork " << left << "\n";

        forks[right].lock();
        cout << "Philosopher " << id << " picked fork " << right << "\n";
    }

    // Critical Section (Eating)
    cout << "Philosopher " << id << " is eating\n";
    this_thread::sleep_for(chrono::milliseconds(200));

    // Put down forks (reverse order is good practice)
    /*
        Circular wait ❌ (we already broke this)
        🔑 Circular wait is created only while locking, not unlocking.
    */
    forks[left].unlock();
    forks[right].unlock();

    cout << "Philosopher " << id << " finished eating\n";
}

int main()
{
    vector<thread> philosophers;

    for (int i = 0; i < N; i++)
        philosophers.emplace_back(philosopher, i);

    for (auto& t : philosophers)
        t.join();

    cout << "All philosophers are done.\n";
    return 0;
}