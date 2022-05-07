#ifndef __MQTTMANAGER_H__
#define __MQTTMANAGER_H__

#include "Inkplate.h"
#include "HTTPClient.h"
#include "PubSubClient.h"
#include "WiFiClient.h"
#include "Secrets.h"

class MqttManager {
private:
    Inkplate& display;
    IPAddress mqttBroker = IPAddress(192, 168, 50, 201);
    const char* mqtt_id = "Inkplate";
    const char* mqtt_user = MQTT_USER;
    const char* mqtt_password = MQTT_PASS;

    void mqttCallback(char* topic, byte* payload, unsigned int length) { }

    WiFiClient wifiClient = WiFiClient();
    PubSubClient mqttClient = PubSubClient(mqttBroker, 1883, wifiClient);

    void publishControlMsg(const char* msg);
    
public:
    MqttManager(Inkplate& display) : display(display) { }
    ~MqttManager() { }

    void connect();
    void sendSystemInfo();
    void turnOffAllLights();
    void startClean();
    void stopClean();
    void gotoDock();
};


#endif