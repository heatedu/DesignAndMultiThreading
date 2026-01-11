#pragma once
#include<iostream>

class FlyBehaviour
{
public:
	virtual void fly() = 0;
	virtual ~FlyBehaviour() = default;
};