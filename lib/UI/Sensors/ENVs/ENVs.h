#ifndef ENV_H
#define ENV_H

#include <Wire.h>

#include "System/SystemDefaults.hpp"
#include "System/SystemMacros.hpp"
#include "DHT12/DHT12.h"
#include "DHT12/DHT12.cpp"
#include "Adafruit_BMP280/Adafruit_BMP280.h"
#include "Adafruit_BMP280/Adafruit_BMP280.cpp"

class ENVs
{
private:
    /* data */
    DHT12 dht12;
    Adafruit_BMP280 bme;

    TaskHandle_t UpdateHandle;

    float temp;
    float humi;
    float pressure;
    static void UpdateCode(void *);

public:
    ENVs(/* args */);
    ~ENVs();
    void Begin();
    float GetTemp();
    float GetHumi();
    float GetPressure();
};

ENVs::ENVs(/* args */)
{
}

ENVs::~ENVs()
{
}

/*
    Begin ENV Sensor Task
*/
void ENVs::Begin()
{
    Wire.begin();
    while (!this->bme.begin(0x76))
    {
        Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    }
    xTaskCreate(UpdateCode, "ENV Update Task", Default_Task_Stack, this, 1, &UpdateHandle);
}

/*
    Get Current Temperature in c
*/
float ENVs::GetTemp()
{
    return this->temp;
}

/*
    Get Current Humidity
*/
float ENVs::GetHumi()
{
    return this->humi;
}

/*
    Get Current Pressure in Pa
*/
float ENVs::GetPressure()
{
    return this->pressure;
}

void ENVs::UpdateCode(void *_this)
{
    ENVs *task = (ENVs *)(_this);
    for (;;)
    {
        task->pressure = task->bme.readPressure();
        task->temp = task->dht12.readTemperature();
        task->humi = task->dht12.readHumidity();
        TaskDelay(delay_Time);
    }
}

#endif