#ifndef LED_STRIP_CLASS_H
#define LED_STRIP_CLASS_H
#include <Arduino.h>
#include "System/SystemMacros.hpp"
#include "System/SystemDefaults.hpp"
//#include <Adafruit_NeoPixel.h>
#include "Adafruit_NeoPixel/Adafruit_NeoPixel.h"

class RGBStrip
{
private:
    /* data */
    int LedPin = 15;
    int NumPixels = 10;
    Adafruit_NeoPixel pixels;

    TaskHandle_t UpdateHandle;
    static void UpdateCode(void *pv);

public:
    RGBStrip(/* args */);
    ~RGBStrip();
    void Begin();
    void setPixelsColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b);
    void SetBrightness(int Value);
};

void RGBStrip::SetBrightness(int Value)
{
    this->pixels.setBrightness(Value);
}
RGBStrip::RGBStrip(/* args */)
{
    this->pixels = Adafruit_NeoPixel(this->NumPixels, this->LedPin, NEO_GRB + NEO_KHZ800);
}

RGBStrip::~RGBStrip()
{
}
void RGBStrip::Begin()
{
    pixels.begin();
    xTaskCreate(UpdateCode, "Pixels Update Task", Default_Task_Stack, this, 1, &UpdateHandle);
}
void RGBStrip::UpdateCode(void *pv)
{
    RGBStrip *task = (RGBStrip *)(pv);
    for (;;)
    {

        task->pixels.show();
        TaskDelay(delay_Time);
    }
}
void RGBStrip::setPixelsColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b)
{
    this->pixels.setPixelColor(n, r, g, b);
}

#endif