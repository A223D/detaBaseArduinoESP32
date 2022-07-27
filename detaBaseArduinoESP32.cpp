#include "detaBaseArduinoESP32.h"

int DetaBaseObject::initialize(WiFiClientSecure wifiObject, const char* detaID, const char* detaBaseName, const char* apiKey){
    if(!wifiObject.connected()){
        return DETABASE_WIFI_NOT_CONNECTED;
    }
    _detaID = strdup(detaID);
    _apiKey = strdup(apiKey);
    _detaBaseName = strdup(detaBaseName);
    _baseURI = (char *)malloc((strlen("/v1/") + strlen(detaID) + strlen("/") + strlen(detaBaseName) + 1 ) * sizeof(char));
    strcpy(_baseURI, "/v1/");
    strncat(_baseURI, detaID, strlen(detaID));
    strncat(_baseURI, "/", strlen("/"));
    strncat(_baseURI, detaBaseName, strlen(detaBaseName));
  
}

DetaBaseObject::~DetaBaseObject(){
    free(_detaID);
    free(_apiKey);
    free(_detaBaseName);
    free(_baseURI);
}
