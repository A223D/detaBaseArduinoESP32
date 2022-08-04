#include <ArduinoJson.h>
#include <detaBaseArduinoESP32.h>
#include <WiFiClientSecure.h>

char* apiKey = "MY_KEY";
char* detaID = "MY_ID";
char* detaBaseName = "MY_BASE";

WiFiClientSecure client;
DetaBaseObject detaObj(client, detaID, detaBaseName, apiKey, true);
StaticJsonDocument<50> outer;

String jsonString;

int val = 1;

void setup() {
  Serial.begin(115200);
  StaticJsonDocument<20> internal;
  Serial.println("Let's begin initialization");
  JsonArray items = outer.createNestedArray("items");
  internal["age"] = val;
  items.add(internal);
  serializeJson(outer, jsonString);
  Serial.println(jsonString);
  Serial.println("Reached before WiFi init");
  WiFi.begin("0xCAFE", "0xC0FFEE");
  Serial.println("Waiting to connect to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  jsonString = "";

}

void loop() {
  StaticJsonDocument<120> receiving;
  outer["items"][0]["age"] = val * 2;
  serializeJson(outer, jsonString);
  Serial.println(jsonString);

  result myResult = detaObj.putObject(jsonString.c_str());
  Serial.println();
  Serial.println(myResult.statusCode);
  Serial.println(myResult.reply);
  deserializeJson(receiving, myResult.reply);
  const char* assignedKey = receiving["processed"]["items"][0]["key"];
  val = receiving["processed"]["items"][0]["age"];

  Serial.print("Key assigned by Deta:\t");
  Serial.println(assignedKey);
  Serial.print("Value received:\t");
  Serial.println(val);

  jsonString = "";
  delay(10000);
}
