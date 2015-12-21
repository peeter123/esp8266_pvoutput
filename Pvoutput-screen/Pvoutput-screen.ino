#include <Time.h>
#include <TimeLib.h>
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


#define GRAPHWIDTH 175
#define SCREENHEIGTH 240
#define SCREENWIDTH 320
#define INTERFACECOLOR ILI9341_RED
#define GRAPHBORDER 16
#define GRAPH_Y_START 20
#define GRAPHHEIGHT 200
#define REQUEST_SIZE 288
#define SPACE_BETWEEN_STUFF 3
#define TEXT_START_X (GRAPHWIDTH+GRAPHBORDER+5)
#define CURRENTWATT_START_Y (SYSTEMNAME_DIVIDER_Y + SPACE_BETWEEN_STUFF)
#define TOTALLYGENERATED_START_Y (TOTALLYGENERATED_DIVIDER_Y+SPACE_BETWEEN_STUFF)
#define SYSTEMNAME_START_Y (1+SPACE_BETWEEN_STUFF)
#define SYSTEMNAME_TEXTSIZE 2
#define SYSTEMNAME_DIVIDER_Y (SYSTEMNAME_START_Y+SYSTEMNAME_TEXTSIZE*8+SPACE_BETWEEN_STUFF*2)
#define TOTALLYGENERATED_DIVIDER_Y (CURRENTWATT_START_Y+2*8+2*8+4*8+SPACE_BETWEEN_STUFF*4)
#define TIME_TEXTSIZE 2

extern "C" {
  #include "user_interface.h"
}
//Adafruit_ILI9341(cs,dc,rst)
//Adafruit_ILI9341 tft = Adafruit_ILI9341(15,2,16);
//Adafruit_ILI9341::Adafruit_ILI9341(int8_t cs, int8_t dc, int8_t mosi,int8_t sclk, int8_t rst, int8_t miso)
Adafruit_ILI9341 tft = Adafruit_ILI9341(15,2,0);

void tftSetup(void);
void setupPvOutput(void);
void pvStatsTask(void);
void screenTask(void);
int scaleForGraph(uint16_t graphWidth, uint16_t graphHeight,const int*,uint8_t*,int);
void tftDrawGraph(uint16_t,uint16_t,uint16_t,uint16_t,uint16_t,uint8_t*,uint16_t); //returns maximum y (from old data)
void tftDrawGraphTimeLegend(uint16_t, uint16_t, uint16_t,uint16_t, uint16_t, uint8_t, uint8_t, uint16_t, uint16_t);
void fakeDataForGraph(int samples);

uint8_t graphLine[GRAPHWIDTH];
uint8_t graphLine_old[GRAPHWIDTH];
uint8_t hasFirstData=0;
int graphLineOldLength=0;


const char* ssid = "DieBie";
const char* password = "delammevleugel";
const float pi = 3.14;
Ticker screenTaskTicker;
Ticker pvTaskTicker;
PvStats pvStats;
PvStatus pvStatus;
PvSystemService pvSystemService;
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
    if(PVOutput.getStats(&pvStats,GRAPHWIDTH)) {
      hasFirstData=1;
    }
    delay(100);
    PVOutput.getStatus(&pvStatus);
    delay(100);
    PVOutput.getPvSystemService(&pvSystemService);
    updateScreen = 1;
  }
  if (updateScreen)
  {
    updateScreen = 0;
    screenTask();
  }
  if(hasFirstData==0)
  {
        if(PVOutput.getStats(&pvStats,GRAPHWIDTH)) {
      hasFirstData=1;
    }
    delay(5000);
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
  if(!getApiKey(apiKey))
  {
    setApiKey("EAZWindData");
    getApiKey(apiKey);
  }
  if(!getSystemID(systemId))
  {
    setSystemID("21479");
    getSystemID(systemId);
  }
  
  Serial.print("Apikey: ");Serial.println(apiKey);
  Serial.print("Systemid: ");Serial.println(systemId);
  PVOutput.begin(apiKey, systemId);
}

//--------------------------------------------

void plotLines()
{
  Serial.print("stat Amount:");
  Serial.println(pvStats.len);
  if (graphLineOldLength>0)
  {
    tftDrawGraph(GRAPHBORDER,GRAPH_Y_START,GRAPHBORDER+GRAPHWIDTH,GRAPHHEIGHT+GRAPH_Y_START,ILI9341_BLACK,graphLine_old,graphLineOldLength);
  }
  scaleForGraph(GRAPHHEIGHT,GRAPHWIDTH,pvStats.instantaneousPower,graphLine,pvStats.len);
  tftDrawGraph(GRAPHBORDER,GRAPH_Y_START,GRAPHBORDER+GRAPHWIDTH,GRAPHHEIGHT+GRAPH_Y_START,ILI9341_GREEN,graphLine,pvStats.len);  
  graphLineOldLength=pvStats.len;
  memcpy(graphLine_old,graphLine,graphLineOldLength);

} 



void fakeDataForGraph(int samples)
{
  int i;
  pvStats.len=random(0,samples);
  for(i=0;i<REQUEST_SIZE;i++)
  {
    pvStats.instantaneousPower[i] = random(0,20000);
  } 
  
}

void screenTask(void)
{
  //Clear dataplaces
  //tft.fillRect(GRAPHBORDER,GRAPH_Y_START,GRAPHWIDTH,200-GRAPH_Y_START,ILI9341_BLACK);
  plotLines();
  char shortSystemName[9];

  tft.setTextSize(TIME_TEXTSIZE);
  tft.setCursor(2,SCREENHEIGTH-TIME_TEXTSIZE*8);
  tft.print(pvStats.startTime);

  tft.setCursor(TEXT_START_X-5*11-5,SCREENHEIGTH-TIME_TEXTSIZE*8);
  tft.print(pvStats.endTime);
  
  tft.setCursor(TEXT_START_X+12,SYSTEMNAME_START_Y);
  tft.setTextSize(SYSTEMNAME_TEXTSIZE);
  memcpy(shortSystemName,pvSystemService.systemName,sizeof(shortSystemName)-1);
  shortSystemName[sizeof(shortSystemName)-1]=0;
  tft.print(shortSystemName);
  


  
  if (pvStatus.instantaneousPower<10000)
    tft.setTextSize(4);
  else
    tft.setTextSize(3);
 
  tft.setCursor(TEXT_START_X+12,CURRENTWATT_START_Y+2*8+2*SPACE_BETWEEN_STUFF);
  tft.print(pvStatus.instantaneousPower);


  
  tft.setCursor(TEXT_START_X+12,TOTALLYGENERATED_START_Y+2*8+2*SPACE_BETWEEN_STUFF);
  tft.setTextSize(4);
  if (pvStatus.energyGeneration<1000)
  {
    tft.print(pvStatus.energyGeneration);
    tft.setCursor(TEXT_START_X+12,TOTALLYGENERATED_START_Y+6*8+3*SPACE_BETWEEN_STUFF);
    tft.setTextSize(2);
    tft.print("Wh  ");
  }
  else
  {

     tft.print(pvStatus.energyGeneration/1000);
     if (pvStatus.energyGeneration<100000)
     {
        tft.print(".");
        tft.print((pvStatus.energyGeneration%1000)/100); 
     }
     tft.setCursor(TEXT_START_X+12,TOTALLYGENERATED_START_Y+6*8+3*SPACE_BETWEEN_STUFF);
     tft.setTextSize(2);
     tft.print("kWh");  
   }
}   
  
  


void tftSetup(void){
  tft.begin();
  tft.setRotation(3);
  tft.fillScreen(ILI9341_BLACK);

  tftDrawGraphTimeLegend(GRAPHBORDER,GRAPH_Y_START,GRAPHBORDER+GRAPHWIDTH,220,ILI9341_WHITE,0,24,0,1200); 
  
  tft.drawRect(TEXT_START_X+1,0,SCREENWIDTH-1-TEXT_START_X,SCREENHEIGTH-1,INTERFACECOLOR); //all encompassing rectangle for text.
  
  tft.drawFastHLine(TEXT_START_X+1,SYSTEMNAME_DIVIDER_Y,SCREENWIDTH-1-TEXT_START_X,INTERFACECOLOR); //first divider in text (under name)
  tft.drawFastHLine(TEXT_START_X+1,TOTALLYGENERATED_DIVIDER_Y,SCREENWIDTH-1-TEXT_START_X,INTERFACECOLOR); //second divider in text (current generated)
  
  tft.setTextSize(2);
  tft.setCursor(TEXT_START_X+12,CURRENTWATT_START_Y);
  tft.print("Cur. gen");
  tft.setCursor(TEXT_START_X+12,(CURRENTWATT_START_Y+3*SPACE_BETWEEN_STUFF+6*8));
  tft.setTextSize(2);
  tft.print("Watt");
  
  tft.setCursor(TEXT_START_X+12,TOTALLYGENERATED_START_Y);
  tft.print("Tot. gen");

  delay(100);
}  

 
void tftDrawGraph(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,uint16_t aColor,uint8_t data[],uint16_t datasize)
{
   int x;
   int xlength=x1-x0-1;
   if (datasize<xlength)
   {
     for (x=0;x<(datasize-1);x++)
     {
      Serial.println(x);
      Serial.println(map(x,0,datasize,0,xlength));
      tft.drawLine(x0+map(x,0,datasize,0,xlength),y1-data[x],x0+map(x+1,0,datasize,0,xlength),y1-data[x+1],aColor);
     }
  }
}

void tftDrawGraphTimeLegend(uint16_t x0, uint16_t y0, uint16_t x1,uint16_t y1, uint16_t aColor, uint8_t minhour, uint8_t maxhour, uint16_t minY, uint16_t maxY)
{
  uint8_t i;
  //X line
  tft.drawLine(x0-2,y0,x0-2,y1,aColor);
  tft.drawLine(x0-1,y0,x0-1,y1,aColor);
  
  //Y Line
  tft.drawLine(x0-2,y1+1,x1,y1+1,aColor);
  tft.drawLine(x0-2,y1+2,x1,y1+2,aColor);

  String text = "Watt";
  tft.setTextSize(2);
  tft.setTextColor(ILI9341_WHITE,ILI9341_BLACK);  
  for(i=0;i<4;i++)
  {
    tft.setCursor(x0-14, y0+i*16);
    tft.print(text[i]);
  }
  text = "Time";
  tft.setCursor(x1-5*11,y1+6);
//  tft.print(text);
}

int scaleForGraph(uint16_t graphHeight,uint16_t graphWidth, const int* data_in,uint8_t* data_out,int aLength)
{
    int maxy=data_in[0];
    int miny=data_in[0];
    int i;
    for(i=0;i<aLength;i++)
    {
     if(data_in[i]>maxy)
     {
        maxy=data_in[i];
     }
     if(data_in[i]<miny)
     {
        miny=data_in[i];
     }
    }
    
    Serial.print("maxy: ");
    Serial.println(maxy);
    Serial.print("miny: ");
    Serial.println(miny);
    
    if (aLength>graphWidth)
    {
      aLength=graphWidth;
    }
      for(i=0;i<aLength;i++)
      {
        data_out[i] = map(data_in[i],0,maxy,0,graphHeight); 
        Serial.print(data_out[i]);
        Serial.print(",");             
      }
      Serial.println("");
/*
    }
    else
    {
      for(i=0;i<aLength;i++)
      {
        data_out[i+(GRAPHWIDTH-aLength)] = map(data_in[i],0,maxy,0,GRAPHHEIGHT);      
      }
      for(i=0;i<(GRAPHWIDTH-aLength);i++)
      {
        data_out[i] = 0;
      }
    }   
*/
    
    return maxy; 
}

