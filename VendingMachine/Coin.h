#pragma once
#include <iostream>

enum class Coin
{
    ONE_RUPEE = 1,
    TWO_RUPEES = 2,
    FIVE_RUPEES = 5,
    TEN_RUPEES = 10
};

inline int coinValue(Coin c) 
{
    return static_cast<int>(c);
}