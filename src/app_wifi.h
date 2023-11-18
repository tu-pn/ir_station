#pragma once

#include "Arduino.h"


typedef enum {
    WIFI_WAIT_COMMAND,
    WIFI_SCAN,
    WIFI_CHECK,
    WIFI_CONNECT,
    WIFI_CONNECT_DEFAULT_SSID,
    WIFI_IDLE,
    WIFI_EXIT,
    WIFI_CONNECT_FAILED,
    WIFI_CONNECTION_LOST,
    WIFI_DISCONNECTED
} wifi_state_t;


bool wifiIsConnected();
void wifiStart();