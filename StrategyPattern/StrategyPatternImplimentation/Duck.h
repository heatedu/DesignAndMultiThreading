#pragma once
#include<iostream>
#include "FlyBehaviour.h"
#include "QuackBehaviour.h"
using namespace std; 

class Duck
{
	protected:
		FlyBehaviour* fb;
		QuackBehaviour* qb;
	public:
		virtual ~Duck() {
			delete fb;
			delete qb;
		}
		virtual void display() = 0;
		void performfly()
		{
			fb->fly();
		}
		void performquack()
		{
			qb->quack();
		}
		void swim()
		{
			cout << "All Duck swim!!" << endl;
		}
};