#ifndef HCRL_Edu_H
#define HCRL_Edu_H
#include <Arduino.h>
#include "MQTT/MqttTask.h"
#include "WiFi/WiFiTask.h"
#include "Sensors/ENVs/ENVs.h"
#include "Sensors/Angle/AngleClass.h"
#include "Sensors/MotionClass/MotionClass.h"
#include "Sensors/RGBStrip/RGBStripClass.h"
#include "Sensors/LedRGBClass/LedRGBClass.h"
#include "System/SystemDefaults.hpp"
#include "System/SystemMacros.hpp"
class HCRL_Edu
{
private:
    /* data */
public:
    HCRL_Edu(/* args */);
    ~HCRL_Edu();
    /* 
    Access to ENV Sensor
     */
    ENVs ENV;
    /* 
    Access to Angle Sensor (potentiometer)
    */
    AngleClass Angle;
    /* 
    Access to control 3 led rgbs
    */
    LedRGBClass LedRGB;
    /* 
    Access to PIR or Montion Sensor
    */
    MotionClass Motion;
    /* 
    Access to control 10 rgb strip around m5
    */
    RGBStrip StripRGB;

    /* 
    Access to MQTT connection methods
    */
    MqttTask MQTT;
    /* 
    Access to wifi connection methods
    */
    WiFiTask WiFi;

    void Update();
};

HCRL_Edu::HCRL_Edu(/* args */)
{
}

HCRL_Edu::~HCRL_Edu()
{
}

/*
*** Important ***
for delay loop task and give other task running */
void HCRL_Edu::Update()
{
    TaskDelay(delay_Time);
}

#endif