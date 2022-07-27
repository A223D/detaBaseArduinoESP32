#include "detaBaseArduinoESP32.h"

//int DetaBaseObject::initialize(WiFiSerialSecure wifiObject, const char* detaID, const char* detaBaseName, const char* apiKey){
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

int DetaBaseObject::putObject(char* jsonObject){
  //connect it to server
  Serial.print(F("PUT "));
  Serial.print(_baseURI);
  Serial.println(F(" HTTP/1.1"));
  Serial.println(F("Host: database.deta.sh"));
  Serial.println(F("User-Agent: Arduino/1.0"));
  Serial.println(F("Accept-Encoding: gzip, deflate"));
  Serial.println(F("Accept: */*"));
  Serial.println(F("Connection: keep-alive"));
  Serial.println(F("Content-Type: application/json"));
  Serial.print(F("x-api-key: "));
  Serial.println(_apiKey);
  Serial.print(F("Content-Length: "));
  Serial.println(strlen(jsonObject));
  Serial.println();
  Serial.println(jsonObject);

  /*
    unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout  !");
      client.stop();
      return;
    }
  }

  // Read all the lines of the reply from server and print them to Serial
  while (client.available()) {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  client.stop();
  Serial.println();
  Serial.println("closed connection");
  */

  return -231;

}
