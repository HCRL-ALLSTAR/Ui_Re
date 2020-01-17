#ifndef LED_RGB_CLASS_H
#define LED_RGB_CLASS_H
#include <Arduino.h>
//#include <Adafruit_NeoPixel.h>
#include "Adafruit_NeoPixel/Adafruit_NeoPixel.h"

class LedRGBClass
{
private:
    /* data */
    int LedPin = 26;
    int NumPixels = 3;
    Adafruit_NeoPixel pixels;

    TaskHandle_t UpdateHandle;
    static void UpdateCode(void *pv);

public:
    LedRGBClass(/* args */);
    ~LedRGBClass();
    void Begin();
    void setPixelsColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b);
    void SetBrightness(int Value);
};

void LedRGBClass::SetBrightness(int Value)
{
    this->pixels.setBrightness(Value);
}
LedRGBClass::LedRGBClass(/* args */)
{
    this->pixels = Adafruit_NeoPixel(this->NumPixels, this->LedPin, NEO_GRB + NEO_KHZ800);
}

LedRGBClass::~LedRGBClass()
{
}
void LedRGBClass::Begin()
{
    pixels.begin();
    xTaskCreate(UpdateCode, "Pixels Update Task", Default_Task_Stack, this, 1, &UpdateHandle);
}
void LedRGBClass::UpdateCode(void *pv)
{
    LedRGBClass *task = (LedRGBClass *)(pv);
    for (;;)
    {

        task->pixels.show();
        TaskDelay(delay_Time);
    }
}
void LedRGBClass::setPixelsColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b)
{
    this->pixels.setPixelColor(n, r, g, b);
}

#endif