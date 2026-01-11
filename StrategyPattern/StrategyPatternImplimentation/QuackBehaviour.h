#pragma once
#include<iostream>

class QuackBehaviour
{
public:
	virtual void quack() = 0;
	virtual ~QuackBehaviour() = default;
};