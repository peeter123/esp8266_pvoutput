#ifndef SETTINGSSERVER_H
#define SETTINGSSERVER_H

#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "Utils.h"

extern void startSettingsServer(void);
extern void settingsServerTask(void);
extern const char* pvhostname;
extern bool updateStats;

#endif //SETTINGSSERVER_H
