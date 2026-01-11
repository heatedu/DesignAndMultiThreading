#pragma once
#include <iostream>
#include "FlyBehaviour.h"

class FlyWithWings : public FlyBehaviour {
public:
    void fly() override {
        std::cout << "I'm flying with wings!" << std::endl;
    }
};