#include "ThingSpeak.h"
#include <ESP8266WiFi.h>

#define LED 2
#define LED2 0
#define LED3 4

const char* ssid     = "Siraj Randhawa@unifi";
const char* password = "Sahib128";
const char* host = "api.thingspeak.com";
char thingSpeakAddress[] = "api.thingspeak.com";
int status = WL_IDLE_STATUS;
WiFiClient  client;
int readValue;
int readValue2;
int readValue3;
int failedCounter = 0;
unsigned long myChannelNumber =  363550;
const char * myReadAPIKey = "R3DJVXNKEQ9ZGGZT";

void setup() {
  Serial.begin(9600); // for debugging reasons
  startEthernet();
  ThingSpeak.begin(client);
//  pinMode(LED, OUTPUT);
//  digitalWrite(LED, LOW);
//  pinMode(LED2, OUTPUT);
//  digitalWrite(LED2, LOW);
//  pinMode(LED3, OUTPUT);
//  digitalWrite(LED3, LOW);
//  delay(500);
//  for (int i = 0; i < 5; i++) {
//    digitalWrite(LED, HIGH);
//    digitalWrite(LED2, HIGH);
//    digitalWrite(LED3, HIGH);
//    delay(500);
//    digitalWrite(LED, LOW);
//    digitalWrite(LED2, LOW);
//    digitalWrite(LED3, LOW);
//    delay(500);
//  }
//  digitalWrite(LED, HIGH);
//  digitalWrite(LED2, HIGH);
//  digitalWrite(LED3, HIGH);
//  delay(500);
}

void loop() {
  readValue = ThingSpeak.readIntField(myChannelNumber, 1, myReadAPIKey);
  //readValue2 = ThingSpeak.readIntField(myChannelNumber, 2, myReadAPIKey);
  //readValue3 = ThingSpeak.readIntField(myChannelNumber, 3, myReadAPIKey);
  Serial.print("readValue = ");
  Serial.println(readValue);
//  Serial.print("readValue2 = ");
//  Serial.println(readValue2);
//  Serial.print("readValue3 = ");
//  Serial.println(readValue3);
  if (readValue == 30) {
    Serial.println("LED 1 OFF");
    //digitalWrite(LED, HIGH);
  }
  if (readValue == 0) {
    Serial.println("LED 1 ON");
    //digitalWrite(LED, LOW);
  }
//  if ( readValue2 == 30) {
//    Serial.println("LED 2 OFF");
//    digitalWrite(LED2, HIGH);
//  }
//  if ( readValue2 == 0) {
//    Serial.println("LED 2 ON");
//    digitalWrite(LED2, LOW);
//  }
//  if ( readValue3 == 30) {
//    Serial.println("LED 3 OFF");
//    digitalWrite(LED3, HIGH);
//  }
//  if ( readValue3 == 0) {
//    Serial.println("LED 3 ON");
//    digitalWrite(LED3, LOW);
//  }
}

void startEthernet()
{
  failedCounter = 0;

  client.stop();

  Serial.println("Connecting Arduino to network...");
  Serial.println();

  delay(1000);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  delay(1000);
}
