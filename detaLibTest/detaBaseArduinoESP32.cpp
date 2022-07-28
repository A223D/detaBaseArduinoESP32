#include "detaBaseArduinoESP32.h"

int DetaBaseObject::initialize(WiFiClientSecure wifiObject, char* detaID, char* detaBaseName, char* apiKey, char* ca) {
  _detaID = detaID;
  _apiKey = apiKey;
  _detaBaseName = detaBaseName;
  _baseURI = (char *)malloc((strlen("/v1/") + strlen(detaID) + strlen("/") + strlen(detaBaseName) + 1 ) * sizeof(char));
  _wifiObject = wifiObject;
  _wifiObject.setCACert(ca);
  strcpy(_baseURI, "/v1/");
  strncat(_baseURI, detaID, strlen(detaID));
  strncat(_baseURI, "/", strlen("/"));
  strncat(_baseURI, detaBaseName, strlen(detaBaseName));
  Serial.println("But actually here");
  return 5;
}

int DetaBaseObject::initialize(WiFiClientSecure wifiObject, char* detaID, char* detaBaseName, char* apiKey, char* ca, bool debugOption) {
  _debugOn = debugOption;
  _detaID = detaID;
  _apiKey = apiKey;
  _detaBaseName = detaBaseName;
  _baseURI = (char *)malloc((strlen("/v1/") + strlen(detaID) + strlen("/") + strlen(detaBaseName) + 1 ) * sizeof(char));
  _wifiObject = wifiObject;
  _wifiObject.setCACert(ca);
  strcpy(_baseURI, "/v1/");
  strncat(_baseURI, detaID, strlen(detaID));
  strncat(_baseURI, "/", strlen("/"));
  strncat(_baseURI, detaBaseName, strlen(detaBaseName));
  Serial.println("But actually here");
  return 5;
}


DetaBaseObject::~DetaBaseObject() {
  //probably won't need these. You only free what you malloc
  //    free(_detaID);
  //    free(_apiKey);
  //    free(_detaBaseName);
  free(_baseURI);
}

char* DetaBaseObject::getDetaID() {
  return _detaID;
}

char* DetaBaseObject::getBaseURI() {
  return _baseURI;
}

int DetaBaseObject::putObject(char* jsonObject) {

  if(_debugOn){
    Serial.println("This is putObject");
    Serial.print("json: \t");
    Serial.println(jsonObject);
    Serial.print("Length: \t");
    Serial.println(strlen(jsonObject));
  }

  //have to dynamically get JSON data
  if (_wifiObject.connect("database.deta.sh", 443)) {
    Serial.println("Connected to server");
    _wifiObject.print("PUT ");
    _wifiObject.print(_baseURI);
    _wifiObject.print("/items");
    _wifiObject.println(" HTTP/1.1");
    _wifiObject.println("Host: database.deta.sh");
    _wifiObject.println("User-Agent: Arduino/1.0");
    _wifiObject.println("Accept-Encoding: gzip, deflate");
    _wifiObject.println("Accept: */*");
    _wifiObject.println("Connection: keep-alive");
    _wifiObject.println("Content-Type: application/json");
    _wifiObject.print("x-api-key: ");
    _wifiObject.println(_apiKey);
    _wifiObject.print("Content-Length: ");
    _wifiObject.println(strlen(jsonObject));
    _wifiObject.println();
    _wifiObject.println(jsonObject);
  } else {
    Serial.println("Could not connect to server");
    while (true);
  }


  unsigned long timeout = millis();
  while (_wifiObject.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout  !");
      _wifiObject.stop();
      while (true); //figure out what to do here
    }
  }

  if (_debugOn) {
    // Read all the lines of the reply from server and print them to Serial
    while (_wifiObject.available()) {
      String line = _wifiObject.readStringUntil('\r');
      Serial.print(line);
    }
  }
  _wifiObject.stop();
  Serial.println();
  Serial.println("closed connection");
  return -35;

}

int DetaBaseObject::getObject(char* key) {
  Serial.println("Not implemented get Object yet");
  return -234;


}
