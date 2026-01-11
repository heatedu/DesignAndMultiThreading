#pragma once
#include <iostream>
#include "QuackBehaviour.h"

class Squeak : public QuackBehaviour {
public:
    void quack() override {
        std::cout << "Squeak!" << std::endl;
    }
};