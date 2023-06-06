#include <ESP8266WiFi.h>
#include "DHT.h"
#include "ThingSpeak.h"
#include <Wire.h>
#include <Adafruit_ADS1015.h>
#define DHTPIN 16
#define DHTTYPE DHT11
#define RELAY_1 2
#define RELAY_2 14

const int pirPin = 13;
const char* ssid     = "Nadi Eleczone Solution";
const char* password = "nadi1991";

const char* host = "api.thingspeak.com";
char thingSpeakAddress[] = "api.thingspeak.com";

String writeAPIKey = "8XYQUUPCCV7MEMVH";
unsigned long myChannelNumber = 404133;
const char * myReadAPIKey = "KX5SGXZLH5NCOK1J";

// Variable Setup
long lastConnectionTime = 0;
boolean lastConnected = false;
int failedCounter = 0;
String strTemp = "0";
String strHumi = "0";
String strLdr = "0";
String strGas = "0";
String strMag = "0";
String strPir = "0";
int readValue_1, readValue_2;
float h, t;
int16_t ldr_val, gas_val, mag_val;
int pirState = 0;
int intruderState = 0;

// Initialize Client
WiFiClient client;
DHT dht(DHTPIN, DHTTYPE);
Adafruit_ADS1115 ads;

void setup()
{
  // Start Serial for debugging on the Serial Monitor
  Serial.begin(9600);
  dht.begin();
  // Start Ethernet on Arduino
  startEthernet();
  ThingSpeak.begin(client);
  ads.setGain(GAIN_ONE);
  ads.begin();
  pinMode(RELAY_1, OUTPUT);
  pinMode(RELAY_2, OUTPUT);
  pinMode(pirPin, INPUT);
  digitalWrite(RELAY_1, LOW);
  digitalWrite(RELAY_2, LOW);
}

void loop()
{
  pirState = digitalRead(pirPin);
  if (pirState == HIGH) {
    intruderState = 50;
  } 
  if (pirState == LOW){
    intruderState = 10;
  }
  h = dht.readHumidity();
  t = dht.readTemperature();
  ldr_val = ads.readADC_SingleEnded(0);
  gas_val = ads.readADC_SingleEnded(1);
  mag_val = ads.readADC_SingleEnded(2);
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
  Serial.print("MAGNETIC SENSOR: "); Serial.println(mag_val);
  Serial.print("GAS SENSOR: "); Serial.println(gas_val);
  Serial.print("LDR SENSOR: "); Serial.println(ldr_val);
  Serial.println(" ");
  readValue_1 = ThingSpeak.readIntField(myChannelNumber, 1, myReadAPIKey);
  readValue_2 = ThingSpeak.readIntField(myChannelNumber, 2, myReadAPIKey);
  Serial.print("readValue_1 = ");
  Serial.println(readValue_1);
  Serial.print("readValue_2 = ");
  Serial.println(readValue_2);
  if ( readValue_1 == 30) {
    digitalWrite(RELAY_1, HIGH);
  }
  if ( readValue_2 == 30) {
    digitalWrite(RELAY_2, HIGH);
  }
  if ( readValue_1 != 30) {
    digitalWrite(RELAY_1, LOW);
  }
  if ( readValue_2 != 30) {
    digitalWrite(RELAY_2, LOW);
  }
  strHumi = String(h);
  strTemp = String(t);
  strGas = String(gas_val);
  strMag = String(mag_val);
  strLdr = String(ldr_val);
  strPir = String(intruderState);
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
  updateThingSpeak("field1=" + strPir +"field2=" + strGas +"field3=" + strMag +"field4=" + strLdr +"field5=" + strTemp + "&field6=" + strHumi);

  lastConnected = client.connected();
  delay(500);
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
