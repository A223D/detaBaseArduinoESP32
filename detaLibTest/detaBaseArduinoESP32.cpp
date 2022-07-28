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
  return 5;
}

//int DetaBaseObject::initialize(char* detaID, char* detaBaseName, char* apiKey) {
//  _detaID = detaID;
//  _apiKey = apiKey;
//  _detaBaseName = detaBaseName;
//  Serial.println("Reached before malloc");
//  _baseURI = (char *)malloc((strlen("/v1/") + strlen(detaID) + strlen("/") + strlen(detaBaseName) + 1 ) * sizeof(char));
//  Serial.println("Reached after malloc");
//  strcpy(_baseURI, "/v1/");
//  Serial.println("Copied v1 into URI space");
//  strncat(_baseURI, detaID, strlen(detaID));
//  Serial.println("Catted detaID into URI space");
//  strncat(_baseURI, "/", strlen("/"));
//  Serial.println("Catted / into URI space");
//  strncat(_baseURI, detaBaseName, strlen(detaBaseName));
//  Serial.println("Catted basename into URI space");
//  Serial.println(_baseURI);
//  return 5;
//}


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

char* DetaBaseObject::getBaseURI() {
  return _baseURI;
}

int DetaBaseObject::putObject(char* jsonObject) {
  //connect it to server
  Serial.print(F("PUT "));
  Serial.print(_baseURI);
  Serial.print("/items");
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

int DetaBaseObject::getObject(char* key) {

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
    _wifiObject.println(2 + 22);
    _wifiObject.println();
    _wifiObject.print("{\"items\": [{\"age\": ");
    _wifiObject.print(String(40));
    _wifiObject.println("}]}");
  } else {
    Serial.println("Could not connect to server");
    while (true);
  }
}
