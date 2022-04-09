#include "WifiManager.h"
#include "Inkplate.h"

// faster than default wifi connection procedure
void WifiManager::fastConnect() {
    if (WiFi.status() == WL_CONNECTED) {
        return;
    }

    Serial.print("Connecting to Wifi.");
    WiFi.mode(WIFI_MODE_STA);
    WiFi.config(ip, gateway, subnet, dns);
    WiFi.setAutoConnect(true);    // autoconnect from saved credentials
    WiFi.persistent(true);        // save the wifi credentials to flash

    int counter = 0;
    WiFi.begin(ssid, password, channel, router_mac, true); 
    while (WiFi.status() != WL_CONNECTED) {
        delay(100);
        Serial.print(".");
        if (++counter > 100) {        // allow up to 10-sec to connect to wifi
            Serial.println("");
            Serial.println(" Wifi timed-out. Rebooting..");
            delay(10);  // so the serial message has time to get sent
            ESP.restart();
        }
    }
    Serial.println("");
    Serial.println("Connected!");
}