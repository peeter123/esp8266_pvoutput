#ifndef pvoutput_h
#define pvoutput_h

#include <stddef.h>
#include <stdint.h>
#include <ESP8266WiFi.h>
#include "Arduino.h"
#include "pvstatus.h"
#include "pvstats.h"

class PVOutputClass
{
  public:
    PVOutputClass();
    void begin(String apiKey, String systemId);
    //bool getStatus(PvStatus *out);
    bool getStats(PvStats *out);
    bool parseInt(const char *in, int *out);
    bool parseFloat(const char *in, float *out);
    bool readCSV(WiFiClient &client, char *out, int size);
    bool readPvStatus(WiFiClient &client, PvStatus *status);
    
  protected:
    String _apiKey;
    String _systemId;
};

extern PVOutputClass PVOutput;

#endif
