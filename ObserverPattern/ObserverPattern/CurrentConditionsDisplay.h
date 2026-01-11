#pragma once
#include "Observer.h"
#include "DisplayElement.h"
#include "Subject.h"
#include <iostream>

class CurrentConditionsDisplay : public Observer, public DisplayElement {
private:
    float temperature;
    float humidity;
    Subject* weatherData;
public:
    CurrentConditionsDisplay(Subject* weatherData) {
        this->weatherData = weatherData;
        weatherData->registerObserver(this);
    }
    
    void update(float temp, float hum, float pressure) override {
        this->temperature = temp;
        this->humidity = hum;
        display();
    }

    void display() override {
        std::cout << "Current conditions: " << temperature
            << "C and " << humidity << "% humidity" << std::endl;
    }
};