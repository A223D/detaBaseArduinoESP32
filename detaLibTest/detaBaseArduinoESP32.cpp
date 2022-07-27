#include "detaBaseArduinoESP32.h"

//int DetaBaseObject::initialize(WiFiClientSecure wifiObject, const char* detaID, const char* detaBaseName, const char* apiKey){
//    _detaID = detaID
//    _apiKey = apiKey
//    _detaBaseName = detaBaseName
//    _baseURI = (char *)malloc((strlen("/v1/") + strlen(detaID) + strlen("/") + strlen(detaBaseName) + 1 ) * sizeof(char));
//    -wifiObject = wifiObject;
//    strcpy(_baseURI, "/v1/");
//    strncat(_baseURI, detaID, strlen(detaID));
//    strncat(_baseURI, "/", strlen("/"));
//    strncat(_baseURI, detaBaseName, strlen(detaBaseName));
//
//}

int DetaBaseObject::initialize(char* detaID, char* detaBaseName, char* apiKey) {
  _detaID = detaID;
  _apiKey = apiKey;
  _detaBaseName = detaBaseName;
  _baseURI = (char *)malloc((strlen("/v1/") + strlen(detaID) + strlen("/") + strlen(detaBaseName) + 1 ) * sizeof(char));
  strcpy(_baseURI, "/v1/");
  strncat(_baseURI, detaID, strlen(detaID));
  strncat(_baseURI, "/", strlen("/"));
  strncat(_baseURI, detaBaseName, strlen(detaBaseName));
  return 1;

}


DetaBaseObject::~DetaBaseObject() {
  //probably won't need these
  //    free(_detaID);
  //    free(_apiKey);
  //    free(_detaBaseName);
  free(_baseURI);
}

char* DetaBaseObject::getDetaID() {
  return _detaID;
}

char* DetaBaseObject::getBaseURI(){
  return _baseURI;
}
