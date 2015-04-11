#ifndef UTILS_H
#define UTILS_H
#include <Arduino.h>
#include <EEPROM.h>

extern void utilStart(void);
extern bool setApiKey(String);
extern bool getApiKey(char *);
extern bool setSystemID(String);
extern bool getSystemID(char *);

#define APIKEY_LENGTH (40 + 1)
#define SYSTEMID_LENGTH (7 + 1)
#define PERSISTENT_STORAGE_START 0
#define PERSISTENT_STORAGE_VERSION 1
#define PERSISTENT_STORAGE_APIKEY_START (PERSISTENT_STORAGE_VERSION + 1)
#define PERSISTENT_STORAGE_SYSTEMID_START (PERSISTENT_STORAGE_APIKEY_START + APIKEY_LENGTH + 1)
#define PERSISTENT_STORAGE_LENGTH (PERSISTENT_STORAGE_SYSTEMID_START+SYSTEMID_LENGTH + 1 - PERSISTENT_STORAGE_START)
#endif //UTILS_H
