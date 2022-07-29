#ifndef DETABASE_LIBRARY_H
#define DETABASE_LIBRARY_H

#include <Arduino.h>
#include <WiFiClientSecure.h>


#define DETABASE_TIMEOUT -1
#define DETABASE_SUCCESS 1
#define DETABASE_NOTFOUND -2
#define DETABASE_PAYLOAD_ISSUE -3
#define DETABASE_UNAUTHORIZED -4
#define DETABASE_WIFI_NOT_CONNECTED -5

typedef struct {
  int statusCode;
  String reply;
} result;

void printResult(result resultObject);

class DetaBaseObject {
  public:
    int initialize(WiFiClientSecure wifiObject, char* detaID, char* detaBaseName, char* apiKey, char* ca, bool debugOption);
    int initialize(WiFiClientSecure wifiObject, char* detaID, char* detaBaseName, char* apiKey, char* ca);
    ~DetaBaseObject();
    int putObject(char* jsonObject);
    int getObject(char* key);
    int insertObject(char* jsonObject);
    int updateObject(char* jsonObject, char* key);
    int query(char* queryObject);
    char* getDetaID();
    char* getBaseURI();
    result testResult();
    result getObjectResult(char* key);
    


  private:
    char* _apiKey;
    char* _detaID;
    char* _detaBaseName;
    char* _baseURI;
    WiFiClientSecure _wifiObject;
    bool _debugOn;
    void checkTimeout();
    void parseReply(result* returnObject);
};

#endif
