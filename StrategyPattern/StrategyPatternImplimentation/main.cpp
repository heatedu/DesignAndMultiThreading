#include<iostream>
#include "Duck.h"
#include "MallardDuck.h"
using namespace std;

int main()
{
    Duck* mallard = new MallardDuck();
    mallard->display();
    mallard->performquack();
    mallard->performfly();
    mallard->swim();
    
    std::cout << "----\n";
    
    delete mallard;
    return 0;
}