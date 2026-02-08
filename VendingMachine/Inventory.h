#pragma once
#include <iostream>
#include <vector>
#include <stdexcept>
#include "ItemShelf.h"
#include "Item.h"

class Inventory
{
private:
	std::vector<ItemShelf>shelves;
public:
    Inventory(int count) 
    {
        for (int i = 0; i < count; ++i) 
        {
            shelves.emplace_back(101 + i);
        }
    }
    std::vector<ItemShelf>& getShelves() { return shelves; }

    bool hasItems() 
    {
        for (auto& s : shelves) 
        {
            if (!s.getSoldOut()) return true;
        }
        return false;
    }

    void addItem(const Item& item, int code) 
    {
        for (auto& shelf : shelves) 
        {
            if (shelf.getCode() == code) 
            {
                shelf.addItem(item);
                return;
            }
        }
        throw std::runtime_error("Invalid code");
    }

    Item getItem(int code) {
        for (auto& shelf : shelves) 
        {
            if (shelf.getCode() == code) 
            {
                if (shelf.getSoldOut()) 
                {
                    throw std::runtime_error("Item sold out");
                }
                return shelf.getItems().front();
            }
        }
        throw std::runtime_error("Invalid code");
    }

    void removeItem(int code) 
    {
        for (auto& shelf : shelves) 
        {
            if (shelf.getCode() == code) 
            {
                shelf.removeItem();
                return;
            }
        }
        throw std::runtime_error("Invalid code");
    }
};