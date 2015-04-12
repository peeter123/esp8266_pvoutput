#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <EEPROM.h>
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "SettingsServer.h"
#include "pvoutput.h"
#include <Ticker.h>
#define GRAPHWIDTH 185
#define SCREENHEIGTH 240
#define SCREENWIDTH 320
#define INTERFACECOLOR ILI9341_RED
#define GRAPHBORDER 15
#define GRAPH_Y_START 20
#define GRAPHHEIGHT 200
#define REQUEST_SIZE 288
#define TEXT_START_X (GRAPHWIDTH+GRAPHBORDER)

extern "C" {
  #include "user_interface.h"
}

Adafruit_ILI9341 tft = Adafruit_ILI9341();

uint8_t graphLine[GRAPHWIDTH];

const char* ssid = "HackerHotel-pub";
const char* password = "";
const float pi = 3.14;
Ticker screenTaskTicker;
Ticker pvTaskTicker;
PvStats pvStats;
PvStatus pvStatus;
bool updateStats=1;
bool updateScreen=0;

void setup(void)
{
  delay(100);
  Serial.begin(115200);
  Serial.println("start"); 
  tftSetup();
 // Connect to WiFi network
  WiFi.printDiag(Serial);
  WiFi.begin(ssid, password);
  
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  utilStart();
  startSettingsServer();
  setupPvOutput();
  updateScreen = 1;
  pvTaskTicker.attach(300,pvStatsTask);
  Serial.println(system_get_free_heap_size());
}
 
void loop(void)
{
  settingsServerTask();
//  pvStatus.energyGeneration = random(0,20000);
  delay(1);
  
  if(updateStats)
  {
    updateStats = 0;
    if(!PVOutput.getStats(&pvStats)) {
      Serial.println("erroroororor, no stats");
    }
    
    pvStatus.energyGeneration = pvStats.instantaneousPower[pvStats.len-1];
    updateScreen = 1;
  }
  if (updateScreen)
  {
    updateScreen = 0;
    screenTask();
  }
} 

void screenUpdateTask(void)
{
  updateScreen = 1;
}

void pvStatsTask(void)
{
  updateStats = 1;
}

void setupPvOutput(void)
{
  char apiKey[41];
  char systemId[8];
  getApiKey(apiKey);
  getSystemID(systemId);
  Serial.print("Apikey: ");Serial.println(apiKey);
  Serial.print("Systemid: ");Serial.println(systemId);
  PVOutput.begin(apiKey, systemId);
}
