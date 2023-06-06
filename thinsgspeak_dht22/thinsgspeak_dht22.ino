#include <ESP8266WiFi.h>
#include "DHT.h"
#define DHTPIN 5
#define DHTTYPE DHT11
const char* ssid     = "Siraj Randhawa@unifi";
const char* password = "Sahib128";

char thingSpeakAddress[] = "api.thingspeak.com";

String writeAPIKey = "7FGJ329QQXERRA3P";

// Variable Setup
long lastConnectionTime = 0;
boolean lastConnected = false;
int failedCounter = 0;
String strTemp = "0";
String strHumi = "0";

// Initialize Arduino Ethernet Client
WiFiClient client;
DHT dht(DHTPIN, DHTTYPE);

void setup()
{
  // Start Serial for debugging on the Serial Monitor
  Serial.begin(9600);
  dht.begin();
  // Start Ethernet on Arduino
  startEthernet();
}

void loop()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println(" *C ");

  strHumi = String(h);
  strTemp = String(t);
  // Print Update Response to Serial Monitor
  if (client.available())
  {
    char c = client.read();
    Serial.print(c);
  }

  // Disconnect from ThingSpeak
  if (!client.connected() && lastConnected)
  {
    Serial.println("...disconnected");
    Serial.println();

    client.stop();
  }

  // Update ThingSpeak
  updateThingSpeak("&field1=" + strHumi + "&field2=" + strTemp); //htps://api.thisngspeak.com/TDJHFGHVKJ/$field1=20&field2=20
  //  data.sparkfun.com

  lastConnected = client.connected();
  if (failedCounter > 5) {
    WiFi.disconnect();
    startEthernet();
  }
  delay(15000);
}

void updateThingSpeak(String tsData)
{
  if (client.connect(thingSpeakAddress, 80))
  {
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + writeAPIKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(tsData.length());
    client.print("\n\n");

    client.print(tsData);

    lastConnectionTime = millis();

    if (client.connected())
    {
      Serial.println("Connecting to ThingSpeak...");
      Serial.println();

      failedCounter = 0;
    }
    else
    {
      failedCounter++;

      Serial.println("Connection to ThingSpeak failed (" + String(failedCounter, DEC) + ")");
      Serial.println();
    }
  }
  else
  {
    failedCounter++;

    Serial.println("Connection to ThingSpeak Failed (" + String(failedCounter, DEC) + ")");
    Serial.println();

    lastConnectionTime = millis();
  }
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
