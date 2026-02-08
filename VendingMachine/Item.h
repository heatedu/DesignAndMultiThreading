#pragma once
#include <iostream>
#include "ItemType.h"
class Item {
public:
    ItemType type;
    int price;

    Item() = default;

    Item(ItemType type, int price)
        : type(type), price(price) {
    }
};