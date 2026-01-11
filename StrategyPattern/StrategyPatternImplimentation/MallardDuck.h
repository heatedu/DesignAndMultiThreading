#pragma once
#include<iostream>
#include "Duck.h"
#include "FlyRocketPowered.h"
#include "Squeak.h"
class MallardDuck : public Duck
{
public:
	MallardDuck()
	{
		fb = new FlyRocketPowered();
		qb = new Squeak();
	}
	void display() override
	{
		cout << "Hi I am Mallard Duck" << endl;
	}
};