
#include <Arduino.h>
#include "HCRL_Edu.h"

HCRL_Edu hcrl;
void setup()
{
    Serial.begin(Defalult_Baud_Rate);
}

void loop()
{
    hcrl.Update();
}