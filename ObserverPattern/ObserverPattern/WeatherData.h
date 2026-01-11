#pragma once
#include "Subject.h"
#include<vector>
#include<algorithm>

class WeatherData:public Subject
{
private:
	std::vector<Observer*>observers;
	float temperature;
	float humidity;
	float pressure;
public:
	WeatherData():temperature(0), humidity(0), pressure(0) {}
	
    void registerObserver(Observer* o) override {
        observers.push_back(o);
    }

    void removeObserver(Observer* o) override {
        observers.erase(std::remove(observers.begin(), observers.end(), o), observers.end());
    }

    void notifyObservers() override {
        for (auto& observer : observers) 
        {
            observer->update(temperature, humidity, pressure);
        }
    }

    void measurementsChanged() {
        notifyObservers();
    }

    void setMeasurements(float temp, float hum, float pres) {
        this->temperature = temp;
        this->humidity = hum;
        this->pressure = pres;
        measurementsChanged();
    }

    float getTemperature() { return temperature; }
    float getHumidity() { return humidity; }
    float getPressure() { return pressure; }
};