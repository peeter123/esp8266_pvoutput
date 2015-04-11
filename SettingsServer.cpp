#include "SettingsServer.h"
const char* pvhostname = "pvmon";

ESP8266WebServer server(80);
MDNSResponder mdns;

void handle_root(){
  server.send(200, "text/plain", "ESP8266 Pvoutput monitor");
  delay(100);
}

void handle_apikey(void){
  String apiKey = server.arg("apikey");
  if (apiKey.length()>0){
    if(!setApiKey(apiKey)){Serial.println("Writing failed");};
  }
  char persistentApiKey[APIKEY_LENGTH + 1];  
  getApiKey(persistentApiKey);
  server.send(200, "text/plain", persistentApiKey);
    
    
}

void handle_refresh(void){
  
}

void handle_sysid(void){
    String sysid = server.arg("systemid");
  if (sysid.length()>0){
    if(!setSystemID(sysid)){Serial.println("Writing failed");};
  }
  char persistentsysid[SYSTEMID_LENGTH + 1];  
  getSystemID(persistentsysid);
  server.send(200, "text/plain", persistentsysid);
 
}

void startSettingsServer(void){
  server.on("/", handle_root);
  server.on("/apikey", handle_apikey);
  server.on("/refresh", handle_refresh);
  server.on("/systemid",handle_sysid);
  server.begin();
  Serial.println("HTTP server started");
  
  if(!mdns.begin(pvhostname, WiFi.localIP())) {
    Serial.println("Error setting up MDNS responder!");
  }
  Serial.println("mDNS responder started");
} 
   
void settingsServerTask(void){
  server.handleClient();
  mdns.update();
}





