#include "Utils.h"

bool setApiKey(String Key){
  Serial.println("Write key:"+Key);
  if(Key.length() < APIKEY_LENGTH)
 {
   int i; 
   for(i=0;i<Key.length();i++)
   {
     EEPROM.write(PERSISTENT_STORAGE_APIKEY_START+i,Key[i]);
   }
   EEPROM.write(PERSISTENT_STORAGE_APIKEY_START+i,0);
   EEPROM.commit();
   delay(1);
   Serial.print("Written:");
   Serial.println(i);
   return true;
 }
 return false;
}

void utilStart(void)
{
  EEPROM.begin(PERSISTENT_STORAGE_LENGTH);
}

bool getApiKey(char * apiKey)
{
 Serial.println("Getting key");
 uint8_t i =  0;
 uint8_t aChar;
 do{
   aChar = EEPROM.read(PERSISTENT_STORAGE_APIKEY_START+i);
   if( aChar)
   {
     apiKey[i] = aChar;
     Serial.write(aChar);
   }
   else
   {
     apiKey[i] = '\0';
     Serial.println("0");
     break;
   }
 }
 while((i++)<APIKEY_LENGTH);
 return true;
}

bool setSystemID(String ID)
{
  Serial.println("Write system ID:" + ID);
  if(ID.length() < SYSTEMID_LENGTH)
  {
   int i; 
   for(i=0;i<ID.length();i++)
   {
     EEPROM.write(PERSISTENT_STORAGE_SYSTEMID_START + i, ID[i]);
   }
   EEPROM.write(PERSISTENT_STORAGE_SYSTEMID_START + i, 0);
   EEPROM.commit();
   delay(1);
   Serial.print("Written:");
   Serial.println(i);
   return true;
 }
 return false;  
}

bool getSystemID(char * systemId)
{
 Serial.println("Getting system id");
 uint8_t i =  0;
 uint8_t aChar;
 do{
   aChar = EEPROM.read(PERSISTENT_STORAGE_SYSTEMID_START+i);
   if( aChar)
   {
     systemId[i] = aChar;
     Serial.write(aChar);
   }
   else
   {
     systemId[i] = '\0';
     Serial.println("0");
     break;
   }
 }
 while((i++) < SYSTEMID_LENGTH);
 return true;
}
