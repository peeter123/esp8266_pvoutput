/* 
  PVOutput.cpp - esp8266 ESP request class
*/

#include "pvoutput.h"

int read(WiFiClient &client, char *buff, int len) {
  int readBytes = 0;
  int b;
  while((client.connected() || client.available() > 0) && readBytes < len) {
    if ((b = client.read()) != -1) {
      buff[readBytes++] = (char)b;
    }
    delay(0);
  }
  delay(0);
  return readBytes;
}


PVOutputClass::PVOutputClass() 
{
}

void PVOutputClass::begin(String apiKey, String systemId) {
  _apiKey = apiKey;
  _systemId = systemId;
}

bool PVOutputClass::readCSV(WiFiClient &client, char *out, int size) {
  int i = 0;
  char c;
  while (read(client, &c, 1) > 0) {
    if (c == ',' || c == ';') {
      break;
    } else if (i < size) {
      out[i++] = c;
    } else {
      return false;
    }
  }
  if (i == 0) {
    // We started reading at end of stream.
    return false;
  }
  out[i++] = '\0';
  return true;
}

bool PVOutputClass::parseInt(const char *in, int *out) {
  if (strcmp(in, "NaN") == 0) {
    *out = 0;
    return true;
  }
  *out = atoi(in);
  return true;
}

bool PVOutputClass::parseFloat(const char *in, float *out) {
  if (strcmp(in, "NaN") == 0) {
    *out = 0.0f;
    return true;
  }
  String s(in);
  *out = s.toFloat();
  return true;
}

bool PVOutputClass::readPvStatus(WiFiClient &client, PvStatus *status) {
  char buff[32];
  return
       readCSV(client, status->date, sizeof(status->date)) // date
    && readCSV(client, status->time, sizeof(status->time)) // time
    && readCSV(client, buff, sizeof(buff)) && parseInt(buff, &status->energyGeneration) // energyGeneration
    && readCSV(client, buff, sizeof(buff)) && parseFloat(buff, &status->energyEfficiency) // energyEfficiency
    && readCSV(client, buff, sizeof(buff)) && parseInt(buff, &status->instantaneousPower) // instantaneousPower
    && readCSV(client, buff, sizeof(buff)) && parseInt(buff, &status->averagePower) // averagePower
    && readCSV(client, buff, sizeof(buff)) && parseFloat(buff, &status->normalisedOutput) // normalisedOutput
    && readCSV(client, buff, sizeof(buff)) && parseInt(buff, &status->energyConsumption) // energyConsumption
    && readCSV(client, buff, sizeof(buff)) && parseInt(buff, &status->powerConsumption) // powerConsumption
    && readCSV(client, buff, sizeof(buff)) && parseFloat(buff, &status->temperature) // temperature
    && readCSV(client, buff, sizeof(buff)) && parseFloat(buff, &status->voltage); // voltage
}

bool PVOutputClass::getStats(PvStats *out)
{
  String host = "pvoutput.org";
  WiFiClient client;
  if (!client.connect(host.c_str(), 80)) {
    Serial.println("connection failed");
    return false;
  }
  
  String url = "/service/r2/getstatus.jsp?sid=" + _systemId + "&key=" + _apiKey + "&h=1&limit=288";
  String query = String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n";
  
  client.print(query);    
  
  // Skipping HTTP headers
  const char *searchStr = "\r\n\r\n";
  int searchIndex = 0;
  int searchLen = 4;
  char c;
  while(read(client, &c, 1) > 0) {
    if (searchStr[searchIndex] == c) {
      searchIndex++;
      if (searchIndex >= searchLen) {
        break;
      }
    } else {
      searchIndex = 0;
    }
  }


  // Reading stats
  PvStatus status;
  out->len = 0;
  while(readPvStatus(client, &status)) {
    if (out->len == 0) {
      memcpy(out->date, status.date, sizeof(out->date));
      memcpy(out->time, status.time, sizeof(out->time));
    }
    out->instantaneousPower[out->len] = status.instantaneousPower;
    out->len++;
  }
  
  // Swapping
  for(int i=0;i<out->len/2;i++) {
    int swp = out->instantaneousPower[i];
    out->instantaneousPower[i] = out->instantaneousPower[out->len-i-1];
    out->instantaneousPower[out->len-i-1] = swp;
  }
  client.stop();
  return true;
}


PVOutputClass PVOutput;
