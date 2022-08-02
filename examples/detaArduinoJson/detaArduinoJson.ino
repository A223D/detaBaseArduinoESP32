#include <ArduinoJson.h>
#include "detaBaseArduinoESP32.h"
#include <WiFiClientSecure.h>

#define LED 2

char* apiKey = "MY_KEY";
char* detaID = "MY_ID";
char* detaBaseName = "MY_BASE";

WiFiClientSecure client;
DetaBaseObject detaObj(client, detaID, detaBaseName, apiKey, true);
StaticJsonDocument<50> outer;

String jsonString;

int val = -1;

void setup() {
  Serial.begin(115200);
  StaticJsonDocument<10> internal;
  Serial.println("Let's begin initialization");
  JsonArray items = outer.createNestedArray("items");
  internal["value"] = val;
  items.add(internal);
  serializeJson(outer, jsonString);
  Serial.println(jsonString);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  Serial.println("Reached before WiFi init");
  WiFi.begin("0xCAFE", "0xC0FFEE");
  Serial.println("Waiting to connect to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  digitalWrite(LED, HIGH);
  jsonString = "";

}

void loop() {
  outer["items"][0]["value"] = val;
  serializeJson(outer, jsonString);
  Serial.println(jsonString);
  
    printResult(detaObj.putObject(jsonString.c_str()));
    Serial.println();
    printResult(detaObj.putObject("{\"items\":[{\"value\":-1}]}"));
    Serial.println();
  //  printResult(detaObj.getObject("cba"));
  //  Serial.println();
  //  printResult(detaObj.deleteObject("abc"));
  //  Serial.println();
  //  printResult(detaObj.insertObject("{\"item\":{\"key\":\"cba\",\"age\":4}}"));
  //  Serial.println();
  //  printResult(detaObj.insertObject("{\"item\":{\"key\":\"abc\",\"age\":4}}"));
  //  Serial.println();
  //  printResult(detaObj.updateObject("{\"increment\":{\"age\":1}}", "abc"));
  //  Serial.println();
  //  printResult(detaObj.updateObject("{\"increment\":{\"age\":1}}", "bcs"));
  //  Serial.println();
  //  printResult(detaObj.query("{\"query\":[{\"age?lt\": 10}]}"));
  //  Serial.println();
  jsonString = "";
  val++;
  delay(1000);
}
