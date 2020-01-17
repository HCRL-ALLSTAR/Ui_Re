#ifndef MQTT_TASK_H
#define MQTT_TASK_H

#include <Arduino.h>
#include <WiFiClient.h>
#include "System/SystemDefaults.hpp"
#include "System/SystemMacros.hpp"
#include "MqttWrapper/PubSubClient/PubSubClient.h"
#include "MqttWrapper/MqttWrapper.h"

class MqttTask
{
private:
    MqttWrapper wrapper;
    TaskHandle_t UpdateHandle;

    boolean isNewTopic = false;

    static void UpdateCode(void *);

public:
    MqttTask(/* args */);
    ~MqttTask();
    void Begin(const char *Server, int Port, MQTT_CALLBACK_SIGNATURE);
    void Update();
    void StartSubscribe(const char *Topic);
    void Publish(const char *Topic, const char *Payload);

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

void MqttTask::StartSubscribe(const char *Topic)
{
    this->wrapper.StartSubscribe(Topic);
    this->isNewTopic = true;
}

void MqttTask::Publish(const char *Topic, const char *Payload)
{
    if (!this->wrapper.isConnected())
    {
        vTaskDelete(this->UpdateHandle);
        this->Update();
    }
    this->wrapper.Publish(Topic, Payload);
}

void MqttTask::PrintSubscribeTopic()
{
    this->wrapper.PrintSubscribeTopic();
}
void MqttTask::PrintPublishTopic()
{
    this->wrapper.PrintPublishTopic();
}
boolean MqttTask::getStatus()
{
    return this->wrapper.isConnected();
}
char *MqttTask::GetServer()
{
    return this->wrapper.GetServer();
}
int MqttTask::GetPort()
{
    return this->wrapper.GetPort();
}

#endif
