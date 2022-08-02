#include "detaBaseArduinoESP32.h"

DetaBaseObject::DetaBaseObject(WiFiClientSecure wifiObject, const char* detaID, const char* detaBaseName, const char* apiKey, bool debugOption) {
  _debugOn = debugOption;
  _detaID = detaID;
  _apiKey = apiKey;
  _detaBaseName = detaBaseName;
  _baseURI = (char *)malloc((strlen("/v1/") + strlen(detaID) + strlen("/") + strlen(detaBaseName) + 1 ) * sizeof(char));
  _wifiObject = wifiObject;
  _wifiObject.setCACert(_detaRootCa);
  _host = "database.deta.sh";
  strcpy(_baseURI, "/v1/");
  strncat(_baseURI, detaID, strlen(detaID));
  strncat(_baseURI, "/", strlen("/"));
  strncat(_baseURI, detaBaseName, strlen(detaBaseName));
}

DetaBaseObject::DetaBaseObject(WiFiClientSecure wifiObject, const char* detaID, const char* detaBaseName, const char* apiKey) {
  _detaID = detaID;
  _apiKey = apiKey;
  _detaBaseName = detaBaseName;
  _baseURI = (char *)malloc((strlen("/v1/") + strlen(detaID) + strlen("/") + strlen(detaBaseName) + 1 ) * sizeof(char));
  _wifiObject = wifiObject;
  _wifiObject.setCACert(_detaRootCa);
  _host = "database.deta.sh";
  strcpy(_baseURI, "/v1/");
  strncat(_baseURI, detaID, strlen(detaID));
  strncat(_baseURI, "/", strlen("/"));
  strncat(_baseURI, detaBaseName, strlen(detaBaseName));
}

DetaBaseObject::~DetaBaseObject() {
  free(_baseURI);
}

const char* DetaBaseObject::getDetaID() {
  return _detaID;
}

const char* DetaBaseObject::getBaseURI() {
  return _baseURI;
}

result DetaBaseObject::putObject(const char* jsonObject) {

  if (_debugOn) {
    Serial.println("This is putObject");
    Serial.print("json: \t");
    Serial.println(jsonObject);
    Serial.print("Length: \t");
    Serial.println(strlen(jsonObject));
  }
  result returnObject;
  initResult(&returnObject);

  if (_wifiObject.connect(_host, HTTPS_PORT)) {
    _wifiObject.print("PUT ");
    _wifiObject.print(_baseURI);
    _wifiObject.print("/items");
    writePayloadHeaders(jsonObject);
  } else {
    returnObject.reply = "Could not connect to server";
    return returnObject;
  }
  if (checkTimeout()) {
    returnObject.reply = "Client Timeout";
    return returnObject;
  }
  parseReply(&returnObject);
  _wifiObject.stop();
  if (_debugOn) {
    Serial.println("closed connection");
  }
  return returnObject;
}

result DetaBaseObject::insertObject(const char* jsonObject) {
  if (_debugOn) {
    Serial.println("This is insertObject");
    Serial.print("jsonObject: \t");
    Serial.println(jsonObject);
    Serial.print("Length: \t");
    Serial.println(strlen(jsonObject));
  }
  result returnObject;
  initResult(&returnObject);
  if (_wifiObject.connect(_host, HTTPS_PORT)) {
    _wifiObject.print("POST ");
    _wifiObject.print(_baseURI);
    _wifiObject.print("/items");
    writePayloadHeaders(jsonObject);
  } else {
    returnObject.reply = "Could not connect to server";
    return returnObject;
  }
  if (checkTimeout()) {
    returnObject.reply = "Client Timeout";
    return returnObject;
  }
  parseReply(&returnObject);
  _wifiObject.stop();
  if (_debugOn) {
    Serial.println("closed connection");
  }
  return returnObject;
}

result DetaBaseObject::updateObject(const char* jsonObject, const char* key) {
  if (_debugOn) {
    Serial.println("This is updateObject");
    Serial.print("key: \t");
    Serial.println(key);
    Serial.print("Length: \t");
    Serial.println(strlen(key));
    Serial.print("json: \t");
    Serial.println(jsonObject);
    Serial.print("Length: \t");
    Serial.println(strlen(jsonObject));
  }
  result returnObject;
  initResult(&returnObject);

  if (_wifiObject.connect(_host, HTTPS_PORT)) {
    _wifiObject.print("PATCH ");
    _wifiObject.print(_baseURI);
    _wifiObject.print("/items/");
    _wifiObject.print(key);
    writePayloadHeaders(jsonObject);
  } else {
    returnObject.reply = "Could not connect to server";
    return returnObject;
  }
  if (checkTimeout()) {
    returnObject.reply = "Client Timeout";
    return returnObject;
  }
  parseReply(&returnObject);
  _wifiObject.stop();
  if (_debugOn) {
    Serial.println("closed connection");
  }
  return returnObject;
}

result DetaBaseObject::query(const char* queryObject) {
  if (_debugOn) {
    Serial.println("This is query");
    Serial.print("key: \t");
    Serial.println(queryObject);
    Serial.print("Length: \t");
    Serial.println(strlen(queryObject));
  }
  result returnObject;
  initResult(&returnObject);
  if (_wifiObject.connect(_host, HTTPS_PORT)) {
    _wifiObject.print("POST ");
    _wifiObject.print(_baseURI);
    _wifiObject.print("/query");
    writePayloadHeaders(queryObject);
  } else {
    returnObject.reply = "Could not connect to server";
    return returnObject;
  }
  if (checkTimeout()) {
    returnObject.reply = "Client Timeout";
    return returnObject;
  }
  parseReply(&returnObject);
  _wifiObject.stop();
  if (_debugOn) {
    Serial.println("closed connection");
  }
  return returnObject;
}

void printResult(result resultObject) {
  Serial.print("Status:\t");
  Serial.println(resultObject.statusCode);
  Serial.print("Reply:\t");
  Serial.println(resultObject.reply);  //should be println
}

result DetaBaseObject::getObject(const char* key) {
  if (_debugOn) {
    Serial.println("This is getObject");
    Serial.print("key: \t");
    Serial.println(key);
    Serial.print("Length: \t");
    Serial.println(strlen(key));
  }
  result returnObject;
  initResult(&returnObject);
  if (_wifiObject.connect(_host, HTTPS_PORT)) {
    _wifiObject.print("GET ");
    _wifiObject.print(_baseURI);
    _wifiObject.print("/items/");
    _wifiObject.print(key);
    writeNonPayloadHeaders();
    _wifiObject.println();
  } else {
    returnObject.reply = "Could not connect to server";
    return returnObject;
  }

  if (checkTimeout()) {
    returnObject.reply = "Client Timeout";
    return returnObject;
  }
  parseReply(&returnObject);

  _wifiObject.stop();
  if (_debugOn) {
    Serial.println("closed connection");
  }
  return returnObject;
}

result DetaBaseObject::deleteObject(const char* key) {
  if (_debugOn) {
    Serial.println("This is delete Object");
    Serial.print("key: \t");
    Serial.println(key);
    Serial.print("Length: \t");
    Serial.println(strlen(key));
  }
  result returnObject;
  initResult(&returnObject);
  if (_wifiObject.connect(_host, HTTPS_PORT)) {
    _wifiObject.print("DELETE ");
    _wifiObject.print(_baseURI);
    _wifiObject.print("/items/");
    _wifiObject.print(key);
    writeNonPayloadHeaders();
    _wifiObject.println();
  } else {
    returnObject.reply = "Could not connect to server";
    return returnObject;
  }

  if (checkTimeout()) {
    returnObject.reply = "Client Timeout";
    return returnObject;
  }
  parseReply(&returnObject);

  _wifiObject.stop();
  if (_debugOn) {
    Serial.println("closed connection");
  }
  return returnObject;

}

bool DetaBaseObject::checkTimeout() {
  unsigned long timeout = millis();
  while (_wifiObject.available() == 0) {
    if (millis() - timeout > TIMEOUT_THRESH) {
      _wifiObject.stop();
      return true;
    }
  }
  return false;
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

void DetaBaseObject::writePayloadHeaders(const char* object) {
  writeNonPayloadHeaders();
  _wifiObject.println("Content-Type: application/json");
  _wifiObject.print("Content-Length: ");
  _wifiObject.println(strlen(object));
  _wifiObject.println();
  _wifiObject.println(object);
}

void DetaBaseObject::initResult(result* resultObject) {
  resultObject->statusCode = -1;
  resultObject->reply = "If you see this, something went wrong and error was not assigned here. Contact developer.";
}
