#ifndef __WIFIMANAGER_H__
#define __WIFIMANAGER_H__

#include "IPAddress.h"
#include "Secrets.h"

class WifiManager {
private:
    // these speed up wifi connection
    IPAddress ip = IPAddress(192, 168, 50, 211);
    IPAddress gateway = IPAddress(192, 168, 50, 1);
    IPAddress subnet = IPAddress(255, 255, 255, 0);
    IPAddress dns = IPAddress(8, 8, 8, 8);
    uint8_t router_mac[6] = WIFI_MAC;
    int channel = 3;
    const char* ssid = WIFI_SSID;
    const char* password = WIFI_PASS;

public:
    WifiManager() { }
    ~WifiManager() { }
    void fastConnect();
};


#endif
