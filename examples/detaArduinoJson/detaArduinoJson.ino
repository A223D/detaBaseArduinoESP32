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
  outer["items"][0]["value"] = val;
  serializeJson(outer, jsonString);
  Serial.println(jsonString);

  printResult(detaObj.putObject(jsonString.c_str()));
  Serial.println();
  jsonString = "";
  val++;
  delay(1000);
}
