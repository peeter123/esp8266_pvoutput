#define TESTER

#ifdef TESTER
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "pvoutput.h"

const char* ssid = "HackerHotel-pub";
const char* password = "";

void setup() {
  delay(100);
  Serial.begin(115200);
  delay(100);
  Serial.println("Start"); 

  // Connect to WiFi network
  WiFi.begin(ssid, password);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  PvStats stats;
  PVOutput.begin("5cfebf9e2e47b7fd69cebd5993f1a7f10effe510", "20235");
  

  if(!PVOutput.getStats(&stats)) {
    Serial.println("erroroororor");
    return;
  }
  
  Serial.println(stats.date);
  Serial.println(stats.time);
  Serial.println(stats.len);  
}

void loop() {
    
}

#endif
