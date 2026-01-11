#include<iostream>
using namespace std;

class Beverage
{
public:
    virtual void getDescription() = 0;
    virtual int cost() = 0;
};

class Expresso : public Beverage
{
public:
    void getDescription() override
    {
        cout << "Expresso";
    }
    int cost() override
    {
        return 50;
    }
};

class HouseBlend : public Beverage
{
public:
    void getDescription() override
    {
        cout << "HouseBlend";
    }
    int cost() override
    {
        return 40;
    }
};

class ToppingDecorator : public Beverage
{
protected:
    Beverage* beverage;
public:
    ToppingDecorator(Beverage* b) :beverage(b){}
};

class Milk : public ToppingDecorator 
{
public:
    Milk(Beverage* b) :ToppingDecorator(b) {}
    void getDescription()
    {
        beverage->getDescription();
        cout<<", Milk";
    }
    int cost() 
    {
        return (beverage->cost() + 10);
    }
};

class Mocha : public ToppingDecorator {
public:
    Mocha(Beverage* b) : ToppingDecorator(b) {}
    void getDescription()  
    {
        beverage->getDescription();
        cout << ", Mocha";
    }
    int cost()
    {
        return (beverage->cost() + 20);
    }
};

int main()
{
    Beverage* b = new Expresso();
    b = new Mocha(b);
    b->getDescription();
    cout << b->cost()<<endl;
    b = new Mocha(new Milk(new HouseBlend()));
    b->getDescription();
    cout << b->cost() << endl;
    return 0;
}