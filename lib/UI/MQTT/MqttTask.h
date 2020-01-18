#ifndef MQTT_TASK_H
#define MQTT_TASK_H

#include <Arduino.h>
#include <WiFiClient.h>
#include "System/SystemDefaults.hpp"
#include "System/SystemMacros.hpp"
#include "MqttWrapper/MqttWrapper.h"
#include "MqttWrapper\PubSubClient\PubSubClient.h"

class MqttTask
{
private:
    MqttWrapper wrapper;
    TaskHandle_t UpdateHandle;

    boolean isNewTopic = false;

    static void UpdateCode(void *);
    void Update();
    char *UserName;
    char *Password;

public:
    MqttTask(/* args */);
    ~MqttTask();
    void Begin(const char *Server, int Port, MQTT_CALLBACK_SIGNATURE);
    void StartSubscribe(const char *Topic);
    void Publish(const char *Topic, const char *Payload);
    void SetUser(const char *UserName, const char *Password);

    void PrintSubscribeTopic();
    void PrintPublishTopic();
    boolean getStatus();
    char *GetServer();
    int GetPort();
};

MqttTask::MqttTask(/* args */)
{
}

MqttTask::~MqttTask()
{
}

/*
    Begin Mqtt Connection
    Server  -> Mqtt Server Address
    Port    -> Mqtt Server Port 
    MQTT_CALLBACK_SIGNATURE -> Callback Function in form 
    Example :
    void callback(char *Topic, byte *Paylaod, unsigned int Length)
    {
        Paylaod[Length] = '\0';
        String topic_str = Topic, payload_str = (char *)Paylaod;
        Serial.println("[" + topic_str + "]: " + payload_str);
    }
*/
void MqttTask::Begin(const char *Server, int Port, MQTT_CALLBACK_SIGNATURE)
{
    this->wrapper.Begin(Server, Port, callback);
    this->Update();
}

void MqttTask::Update()
{
    xTaskCreate(UpdateCode, MQTT_UPDATE_TASk, Default_Task_Stack, this, 1, &UpdateHandle);
}

void MqttTask::UpdateCode(void *pv)
{
    MqttTask *task = (MqttTask *)(pv);
    for (;;)
    {
        if (task->isNewTopic || task->wrapper.SubscribeTopic[0].length() == 0)
        {
            //vTaskDelete(task->UpdateHandle);
            task->wrapper.Update();
            task->isNewTopic = false;
        }
        if (task->wrapper.isConnected() && !task->isNewTopic)
        {
            task->wrapper.Update();
        }
        TaskDelay(delay_Time);
    }
}
/*
    *** Please Add After Begin Connection ***
    Start Subscribe to Your Topic 
*/
void MqttTask::StartSubscribe(const char *Topic)
{
    this->wrapper.StartSubscribe(Topic);
    this->isNewTopic = true;
}

/*  
    Start Publish To Topic 
*/
void MqttTask::Publish(const char *Topic, const char *Payload)
{
    this->wrapper.Publish(Topic, Payload);
}

/*
    Set Your UserName And Password if you need
    UserName -> UserName in Mqtt Broker
    PassWord -> Password in Mqtt Broker
*/
void MqttTask::SetUser(const char *UserName, const char *Password)
{
    this->UserName = (char *)UserName;
    this->Password = (char *)Password;
    this->wrapper.SetUser(this->UserName, this->Password);
}

/*
    Print All Subscribe Topic
*/
void MqttTask::PrintSubscribeTopic()
{
    this->wrapper.PrintSubscribeTopic();
}
/*
    Print All Publish Topic 
*/
void MqttTask::PrintPublishTopic()
{
    this->wrapper.PrintPublishTopic();
}

/*
    Get Current Mqtt Connection Status
*/
boolean MqttTask::getStatus()
{
    return this->wrapper.isConnected();
}

/*
    Get Mqtt Server Address
*/
char *MqttTask::GetServer()
{
    return this->wrapper.GetServer();
}

/*
    Get Mqtt Server Port
*/
int MqttTask::GetPort()
{
    return this->wrapper.GetPort();
}

#endif
