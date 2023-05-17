#include <detaBaseArduinoESP32.h>
#include <WiFiClientSecure.h>
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#define LED 2

char* apiKey = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
char* detaID = "xxxxxxxxxxxxxxx";
char* detaBaseName = "xxxxxxxx";

WiFiClientSecure client;
DetaBaseObject detaObj(client, detaID, detaBaseName, apiKey, true);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
String formattedDate;
String dayStamp;
String timeStamp;
String macAddress;
char date[11];
char timeStr[9];


void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  WiFi.begin("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", "xxxxxxxxxxxxxxxxxxxxxxxxxx");
  Serial.println("Waiting to connect to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  digitalWrite(LED, HIGH);

  timeClient.begin();
  timeClient.setTimeOffset(10800); //GMT+3

  while(!timeClient.update()) {
    timeClient.forceUpdate();
  }
  if (timeClient.isTimeSet()) {
    unsigned long epochTime = timeClient.getEpochTime();
    struct tm *timeinfo;
    time_t time = (time_t)epochTime;
    timeinfo = gmtime(&time);

    int year = timeinfo->tm_year + 1900;
    int month = timeinfo->tm_mon + 1;
    int day = timeinfo->tm_mday;
    int hour = timeinfo->tm_hour;
    int minute = timeinfo->tm_min;
    int second = timeinfo->tm_sec;
    
    sprintf(date, "%02d-%02d-%04d", day, month, year);
    sprintf(timeStr, "%02d:%02d:%02d", hour, minute, second);

    printf("Formatted date: %s\n", date);
    printf("Formatted time: %s\n", time);

  }

  macAddress = WiFi.macAddress();
}

// PUT "{\"items\":[{\"age\":4}]}"

//INSERT "{\"item\":{\"key\":\"cba\",\"age\":4}}"

//INSERT "{\"item\":{\"key\":\"abc\",\"age\":4}}"

//UPDATE "{\"increment\":{\"age\":1}}", key:abc

//UPDATE "{\"increment\":{\"age\":1}}", key:cba

//QUERY "{\"query\":[{\"age?lt\": 10}]}"

void loop() {
  
  String putString = "{\"item\":{\"key\":\"" + macAddress + "\",\"date\":\"" + String(date) + "\"}}";
  printResult(detaObj.insertObject(putString.c_str()));
  Serial.println();
  String updateString = "{\"set\":{\"online\":\"" + String(date) +" "+ String(timeStr) + "\"}}";
  printResult(detaObj.updateObject(updateString.c_str(), macAddress.c_str()));

  while (true);
}
