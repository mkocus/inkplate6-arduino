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
    mqttClient.loop();
}

void MqttManager::publishControlMsg(const char* msg) {
    if (!mqttClient.connected()){
        connect();
    }

    auto state = "{\"action\": \"" + String(msg) + "\"}";
    Serial.println(state);

    if (mqttClient.publish("zigbee2mqtt/Inkplate-Control", state.c_str())) {
        Serial.println("Mqtt state published.");
    } else {
        Serial.println("Mqtt state publish failed.");
    }
}

void MqttManager::sendSystemInfo() {
    if (!mqttClient.connected()){
        connect();
    }

    int temperature = display.readTemperature();
    double voltage = display.readBattery();

    int tmp = (1-(4.35-voltage)/(4.35-3.3))*100;
    int perc = 100;
    if (tmp < 100) perc = tmp;

    auto msg = "Inkplate temperature: " + String(temperature) + ", Battery Voltage: " 
        + String(voltage) + ", Battery %: " + String(perc);
    Serial.println(msg);
    auto state = "{\"temperature\": " + String(temperature) + ",\"voltage\": " + String(voltage) + ",\"battery\": " + String(perc) + "}";

    if (mqttClient.publish("zigbee2mqtt/Inkplate", state.c_str())) {
        Serial.println("Mqtt state published.");
    } else {
        Serial.println("Mqtt state publish failed.");
    }
}

void MqttManager::turnOffAllLights() {
    this->publishControlMsg("turnOffAllLights");
}

void MqttManager::startClean() {
    this->publishControlMsg("startClean");
}

void MqttManager::stopClean() {
    this->publishControlMsg("stopClean");
}

void MqttManager::gotoDock() {
    this->publishControlMsg("gotoDock");
}
