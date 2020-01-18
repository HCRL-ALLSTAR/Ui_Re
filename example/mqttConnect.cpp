#include <Arduino.h>
#include "HCRL_Edu.h"

HCRL_Edu hcrl;

// Default Callback Function
void callback(char *Topic, byte *Paylaod, unsigned int Length)
{
    Paylaod[Length] = '\0';
    String topic_str = Topic, payload_str = (char *)Paylaod;
    Serial.println("[" + topic_str + "]: " + payload_str);
}

void setup()
{
    Serial.begin(Defalult_Baud_Rate);
    hcrl.MQTT.Begin(HCRL_MQTT_SERVER, HCRL_MQTT_PORT, callback); //Start Connect to Server
    hcrl.MQTT.StartSubscribe("YOUR TOPIC");                      // Add Subscribe Topic
}

void loop()
{
    hcrl.Update();                               //Update Data
    hcrl.MQTT.Publish("YOUR TOPIC", "YOUR MSG"); //Publish MSG To Topic
}