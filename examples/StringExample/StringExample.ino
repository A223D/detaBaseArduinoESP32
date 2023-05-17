#include <detaBaseArduinoESP32.h>
#include <WiFiClientSecure.h>
#include <WiFi.h>
#define LED 2

char* apiKey = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";  //deta project key
char* detaID = "xxxxxxxxx"; //deta project ID
char* detaBaseName = "xxx"; //deta base name

int num = 0;

WiFiClientSecure client;
DetaBaseObject detaObj(client, detaID, detaBaseName, apiKey, true);

String macAddress;


void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  WiFi.begin("xxxxxxxxxxx_2.4GHz", "xxxxxxyx"); //wifi credentials
  Serial.println("Waiting to connect to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  digitalWrite(LED, HIGH);

  macAddress = WiFi.macAddress();

  String putString = "{\"item\":{\"key\":\"" + macAddress + "\",\"date\":\"2023\"}}";
  printResult(detaObj.insertObject(putString.c_str()));
  Serial.println();


}


void loop() {
  num++;
  delay(10000);
  String updateString = "{\"set\":{\"update\":\"2023 + "+String(num)+"\"}}";
  printResult(detaObj.updateObject(updateString.c_str(), macAddress.c_str()));
}
