#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <EEPROM.h>
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "SettingsServer.h"

extern "C" {
  #include "user_interface.h"
}

Adafruit_ILI9341 tft = Adafruit_ILI9341();

const char* ssid = "HackerHotel-pub";
const char* password = "";
void tftSetup(void){
  tft.begin();
  tft.setRotation(3);
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(0,0);
  tft.setTextColor(ILI9341_WHITE); tft.setTextSize(1);
  tft.println("ESP8266 PVOUTPUT monitor");
  delay(100);
}  
 
void setup(void)
{
  delay(100);
  Serial.begin(115200);
  delay(100);
  Serial.println("start"); 
 
  delay(100);
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
  
  Serial.println(system_get_free_heap_size());
}
 
void loop(void)
{
  settingsServerTask();

} 







void testFillScreen(void) {
  tft.fillScreen(ILI9341_BLACK);
  delay(100);
  tft.fillScreen(ILI9341_RED);
  delay(100);
  tft.fillScreen(ILI9341_GREEN);
  delay(100);
  tft.fillScreen(ILI9341_BLUE);
  delay(100);
  tft.fillScreen(ILI9341_BLACK);
}
