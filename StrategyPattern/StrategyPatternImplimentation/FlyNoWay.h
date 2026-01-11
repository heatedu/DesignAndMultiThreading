#pragma once
#include <iostream>
#include "FlyBehaviour.h"

class FlyNoWay : public FlyBehaviour {
public:
    void fly() override {
        std::cout << "I can't fly." << std::endl;
    }
};