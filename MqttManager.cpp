#include "MqttManager.h"
#include "Inkplate.h"

void MqttManager::connect() {
    Serial.println("Connecting to MQTT broker...");
    while (!mqttClient.connected()) {
        if (mqttClient.connect(mqtt_id, mqtt_user, mqtt_password)){
            Serial.println("Connected!");
        } else {
            Serial.print("Failed, rc: ");
            Serial.println(mqttClient.state());
            Serial.println("Trying again in 5s.");
            delay(5000);
        }
    }
}

void MqttManager::sendSystemInfo() {
    if (!mqttClient.connected()){
        connect();
    }

    mqttClient.loop();

    int temperature = display.readTemperature();
    float voltage = display.readBattery();

    int tmp = (1-(4.1-voltage)/(4.1-3.3))*100;
    int perc = 100;
    if (tmp < 100) perc = tmp;

    auto msg = "Inkplate temperature: " + String(temperature) + ", Battery Voltage: " 
        + String(voltage) + ", Battery %: " + String(perc);
    Serial.println(msg);
    auto state = "{\"temperature\": " + String(temperature) + ",\"voltage\": " + String(voltage) + ",\"battery\": " + String(perc) + "}";

    char* tab = new char[msg.length() + 1];
    strcpy(tab, state.c_str());
    if (mqttClient.publish("zigbee2mqtt/Inkplate", tab)) {
        Serial.println("Mqtt state published.");
    } else {
        Serial.println("Mqtt state publish failed.");
    }
}