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
  free(_baseURI);
}

char* DetaBaseObject::getDetaID() {
  return _detaID;
}

char* DetaBaseObject::getBaseURI() {
  return _baseURI;
}

result DetaBaseObject::putObject(char* jsonObject) {
  result returnObject;
  initResult(returnObject);
  if (_debugOn) {
    Serial.println("This is putObject");
    Serial.print("json: \t");
    Serial.println(jsonObject);
    Serial.print("Length: \t");
    Serial.println(strlen(jsonObject));
  }

  if (_wifiObject.connect("database.deta.sh", 443)) {
    Serial.println("Connected to server");
    _wifiObject.print("PUT ");
    _wifiObject.print(_baseURI);
    _wifiObject.print("/items");
    writePayloadHeaders(jsonObject);
  } else {
    Serial.println("Could not connect to server");
    while (true);
  }
  checkTimeout();
  parseReply(&returnObject);
  _wifiObject.stop();
  if (_debugOn) {
    Serial.println();
    Serial.println("closed connection");
  }
  return returnObject;
}

int DetaBaseObject::insertObject(char* jsonObject) {
  if (_debugOn) {
    Serial.println("This is insertObject");
    Serial.print("jsonObject: \t");
    Serial.println(jsonObject);
    Serial.print("Length: \t");
    Serial.println(strlen(jsonObject));
  }

  if (_wifiObject.connect("database.deta.sh", 443)) {
    Serial.println("Connected to server");
    _wifiObject.print("POST ");
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


  checkTimeout();

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

int DetaBaseObject::updateObject(char* jsonObject, char* key) {
  if (_debugOn) {
    Serial.println("This is updateObject");
    Serial.print("key: \t");
    Serial.println(key);
    Serial.print("Length: \t");
    Serial.println(strlen(key));
  }

  //  _wifiObject.println("Host: database.deta.sh");
  //  _wifiObject.println("User-Agent: Arduino/1.0");
  //  _wifiObject.println("Accept-Encoding: gzip, deflate");
  //  _wifiObject.println("Accept: */*");
  //  _wifiObject.println("Connection: keep-alive");
  //  _wifiObject.print("x-api-key: ");
  //  _wifiObject.println(_apiKey);

  if (_wifiObject.connect("database.deta.sh", 443)) {
    Serial.println("Connected to server");
    _wifiObject.print("PATCH ");
    _wifiObject.print(_baseURI);
    _wifiObject.print("/items/");
    _wifiObject.print(key);
    _wifiObject.println(" HTTP/1.1");
    writeNonPayloadHeaders();
    _wifiObject.println("Content-Type: application/json");
    _wifiObject.print("Content-Length: ");
    _wifiObject.println(strlen(jsonObject));
    _wifiObject.println();
    _wifiObject.println(jsonObject);
  } else {
    Serial.println("Could not connect to server");
    while (true);
  }


  checkTimeout();

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

result DetaBaseObject::query(char* queryObject) {
  if (_debugOn) {
    Serial.println("This is query");
    Serial.print("key: \t");
    Serial.println(queryObject);
    Serial.print("Length: \t");
    Serial.println(strlen(queryObject));
  }
  result returnObject;
  initResult(returnObject);
  if (_wifiObject.connect("database.deta.sh", 443)) {
    Serial.println("Connected to server");
    _wifiObject.print("POST ");
    _wifiObject.print(_baseURI);
    _wifiObject.print("/query");
    writePayloadHeaders(queryObject);
  } else {
    Serial.println("Could not connect to server");
    while (true);
  }
  checkTimeout();
  parseReply(&returnObject);
  _wifiObject.stop();
  if (_debugOn) {
    Serial.println();
    Serial.println("closed connection");
  }
  return returnObject;
}

result DetaBaseObject::testResult() {
  result returnObject;
  returnObject.statusCode = 1000;
  returnObject.reply = "Let's see if this works";
  return returnObject;
}

void printResult(result resultObject) {
  Serial.print("Status:\t");
  Serial.println(resultObject.statusCode);
  Serial.print("Reply:\t");
  Serial.println(resultObject.reply);  //should be println
}

result DetaBaseObject::getObject(char* key) {
  if (_debugOn) {
    Serial.println("This is getObject");
    Serial.print("key: \t");
    Serial.println(key);
    Serial.print("Length: \t");
    Serial.println(strlen(key));
  }
  result returnObject;
  initResult(returnObject);
  if (_wifiObject.connect("database.deta.sh", 443)) {
    Serial.println("Connected to server");
    _wifiObject.print("GET ");
    _wifiObject.print(_baseURI);
    _wifiObject.print("/items/");
    _wifiObject.print(key);
    writeNonPayloadHeaders();
    _wifiObject.println();
  } else {
    Serial.println("Could not connect to server");
    while (true);
  }

  checkTimeout();
  parseReply(&returnObject);

  _wifiObject.stop();
  if (_debugOn) {
    Serial.println();
    Serial.println("closed connection");
  }
  return returnObject;
}

void DetaBaseObject::checkTimeout() {
  unsigned long timeout = millis();
  while (_wifiObject.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout  !");
      _wifiObject.stop();
      while (true); //figure out what to do here
    }
  }
}

void DetaBaseObject::parseReply(result* returnObject) {
  returnObject->statusCode = _wifiObject.readStringUntil('\r').substring(9, 12).toInt();
  for (int i = 0; i < 8; i++) {
    returnObject->reply = _wifiObject.readStringUntil('\r');
  }
  returnObject->reply = _wifiObject.readStringUntil('\r').substring(1);
  returnObject->reply = returnObject->reply.substring(0, returnObject->reply.length() - 1);
}

void DetaBaseObject::writeNonPayloadHeaders() {
  _wifiObject.println(" HTTP/1.1");
  _wifiObject.println("Host: database.deta.sh");
  _wifiObject.println("User-Agent: Arduino/1.0");
  _wifiObject.println("Accept-Encoding: gzip, deflate");
  _wifiObject.println("Accept: */*");
  _wifiObject.println("Connection: keep-alive");
  _wifiObject.print("x-api-key: ");
  _wifiObject.println(_apiKey);
}

void DetaBaseObject::writePayloadHeaders(char* object) {
  writeNonPayloadHeaders();
  _wifiObject.println("Content-Type: application/json");
  _wifiObject.print("Content-Length: ");
  _wifiObject.println(strlen(object));
  _wifiObject.println();
  _wifiObject.println(object);
}

void DetaBaseObject::initResult(result resultObject) {
  resultObject.statusCode = -1;
  resultObject.reply = "If you see this, something went wrong";
}
