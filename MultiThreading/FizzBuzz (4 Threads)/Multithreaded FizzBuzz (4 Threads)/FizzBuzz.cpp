/*Using mutex
* 
* 
* 
#include<iostream>
#include<thread>
#include<mutex>
#include<condition_variable>
#include<atomic>
std::condition_variable cv;
std::mutex mtx;
int current = 1,n=49;
void printfizz()
{
	std::unique_lock<std::mutex>lck(mtx);
	while (current <= n) {
		while (!(current % 3 == 0 && current % 5 != 0) && current <= n) {                       //use while not if to avoid sprunious wakeup
			cv.wait(lck);
		}
		if (current > n) break;
		std::cout << "Fizz ";
		current++;
		cv.notify_all();
	}
}
void printbuzz()
{
	std::unique_lock<std::mutex>lck(mtx);
	while (current <= n) {
		while (!(current % 5 == 0 && current % 3 != 0) && current <= n) {                   //use while not if to avoid sprunious wakeup
			cv.wait(lck);
		}
		if (current > n) break;
		std::cout << "Buzz ";
		current++;
		cv.notify_all();
	}
}
void printfizzbuzz()
{
	std::unique_lock<std::mutex> lck(mtx);
	while (current <= n) {
		while (!(current % 15 == 0) && current <= n) {                                      //use while not if to avoid sprunious wakeup
			cv.wait(lck);
		}
		if (current > n) break;
		std::cout << "FizzBuzz ";
		current++;
		cv.notify_all();
	}
}
void printnumber()
{
	std::unique_lock<std::mutex>lck(mtx);
	while (current <= n) {
		while ((current % 3 == 0 || current % 5 == 0) && current <= n) {                //use while not if to avoid sprunious wakeup
			cv.wait(lck);
		}
		if (current > n) break;
		std::cout << current << " ";
		current++;
		cv.notify_all();
	}
}
int main()
{
	std::thread t1(printfizz);
	std::thread t2(printbuzz);
	std::thread t3(printfizzbuzz);
	std::thread t4(printnumber);
	t1.join();
	t2.join();
	t3.join();
	t4.join();
}
*/


/*
Using semaphore:

#include <iostream>
#include <thread>
#include <semaphore>

int n = 15;
int current = 1;

std::binary_semaphore sem_fizz(0);
std::binary_semaphore sem_buzz(0);
std::binary_semaphore sem_fizzbuzz(0);
std::binary_semaphore sem_number(1); // start with number

void release_next() {
    if (current > n) {
        sem_fizz.release();
        sem_buzz.release();
        sem_fizzbuzz.release();
        sem_number.release();
        return;
    }

    if (current % 15 == 0)
        sem_fizzbuzz.release();
    else if (current % 3 == 0)
        sem_fizz.release();
    else if (current % 5 == 0)
        sem_buzz.release();
    else
        sem_number.release();
}

void printfizz() {
    while (true) {
        sem_fizz.acquire();
        if (current > n) break;
        std::cout << "Fizz ";
        current++;
        release_next();
    }
}

void printbuzz() {
    while (true) {
        sem_buzz.acquire();
        if (current > n) break;
        std::cout << "Buzz ";
        current++;
        release_next();
    }
}

void printfizzbuzz() {
    while (true) {
        sem_fizzbuzz.acquire();
        if (current > n) break;
        std::cout << "FizzBuzz ";
        current++;
        release_next();
    }
}

void printnumber() {
    while (true) {
        sem_number.acquire();
        if (current > n) break;
        std::cout << current << " ";
        current++;
        release_next();
    }
}

int main() {
    std::thread t1(printfizz);
    std::thread t2(printbuzz);
    std::thread t3(printfizzbuzz);
    std::thread t4(printnumber);

    t1.join();
    t2.join();
    t3.join();
    t4.join();
}



*/