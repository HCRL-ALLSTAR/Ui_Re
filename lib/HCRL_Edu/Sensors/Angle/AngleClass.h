#ifndef ANGLE_CLASS_H
#define ANGLE_CLASS_H
#include <Arduino.h>
#include "System/SystemDefaults.hpp"
#include "System/SystemMacros.hpp"
class AngleClass
{
private:
    TaskHandle_t UpdateHandle;

    static void UpdateCode(void *);
    int sensorPin = 36;
    int last_sensorValue = 100;
    int cur_sensorValue = 0;

public:
    AngleClass(/* args */);
    ~AngleClass();
    void Begin();
    int GetValue();
};

AngleClass::AngleClass(/* args */)
{
}

AngleClass::~AngleClass()
{
}

/*
    Begin angle sensor Task
*/
void AngleClass::Begin()
{
    pinMode(sensorPin, INPUT);
    dacWrite(25, 0);
    xTaskCreate(UpdateCode, "Angle Update Task", Default_Task_Stack, this, 1, &UpdateHandle);
}

/*
    Get Current Angle Value
*/
int AngleClass::GetValue()
{
    return this->last_sensorValue;
}
void AngleClass::UpdateCode(void *_this)
{
    AngleClass *task = (AngleClass *)(_this);
    for (;;)
    {
        task->cur_sensorValue = analogRead(task->sensorPin);
        if (abs(task->cur_sensorValue - task->last_sensorValue) > 10)
        {
            task->last_sensorValue = task->cur_sensorValue;
        }
        TaskDelay(delay_Time);
    }
}

#endif