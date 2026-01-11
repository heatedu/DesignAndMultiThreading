#pragma once
#include <iostream>
#include "FlyBehaviour.h"

class FlyRocketPowered : public FlyBehaviour {
public:
    void fly() override {
        std::cout << "I'm flying with a rocket!" << std::endl;
    }
};