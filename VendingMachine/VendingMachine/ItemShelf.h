#pragma once
#include<iostream>
#include<vector>
#include "Item.h"

class ItemShelf
{
private:
	int code;
	std::vector<Item>items;
	bool isSoldOut = false;
public:
    ItemShelf(int code) : code(code) {}

    int getCode() const { return code; }
    bool getSoldOut() const { return isSoldOut; }

    std::vector<Item>& getItems() { return items; }

    void addItem(const Item& item) 
    {
        items.push_back(item);
        isSoldOut = false;
    }

    void removeItem() 
    {
        if (!items.empty()) 
        {
            items.erase(items.begin());
            if (items.empty()) isSoldOut = true;
        }
    }
};