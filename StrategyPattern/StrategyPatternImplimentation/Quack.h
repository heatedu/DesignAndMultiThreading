#pragma once
#include<iostream>
#include "QuackBehaviour.h"

class MuteQuack : public QuackBehaviour {
public:
    void quack() override {
        std::cout << "<< Quack Quack!! >>" << std::endl;
    }
};