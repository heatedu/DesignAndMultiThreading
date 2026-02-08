#include <iostream>
#include "VendingMachineContext.h"

void fillInventory(VendingMachineContext& vm) 
{
    for (int i = 0; i < 10; i++) 
    {
        int code = 101 + i;
        ItemType type;
        int price;

        if (i < 3) { type = ItemType::COKE;  price = 12; }
        else if (i < 5) { type = ItemType::PEPSI; price = 9; }
        else if (i < 7) { type = ItemType::JUICE; price = 13; }
        else { type = ItemType::SODA;  price = 7; }

        for (int j = 0; j < 5; j++)
            vm.getInventory().addItem(Item(type, price), code);
    }
}

int main() {
    VendingMachineContext vm;

    fillInventory(vm);

    vm.insertCoin(Coin::TEN_RUPEES);
    //vm.insertCoin(Coin::FIVE_RUPEES);

    vm.startSelection(104);
}
