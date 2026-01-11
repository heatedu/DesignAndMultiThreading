#include<iostream>
#include "WeatherData.h"
#include "CurrentConditionsDisplay.h"
using namespace std;

int main()
{
    WeatherData weatherdata;
    CurrentConditionsDisplay currentconditiondisplay(&weatherdata);
    weatherdata.setMeasurements(25.0f, 65.0f, 1013.1f);
    return 0;
}