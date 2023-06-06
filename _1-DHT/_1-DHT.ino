#include "DHT.h"
#define DHTPIN1 22
#define DHTPIN2 23
#define DHTPIN3 24
#define DHTPIN4 25
#define DHTTYPE DHT22
DHT dht1(DHTPIN1, DHTTYPE);
DHT dht2(DHTPIN2, DHTTYPE);
DHT dht3(DHTPIN3, DHTTYPE);
DHT dht4(DHTPIN4, DHTTYPE);
void setup() {
  Serial.begin(115200);
  Serial.println("DHT22 test!");
  dht1.begin();
  dht2.begin();
  dht3.begin();
  dht4.begin();
}
void loop() {
  delay(500);
  float humidity_1 = dht1.readHumidity();
  float humidity_2 = dht2.readHumidity();
  float humidity_3 = dht3.readHumidity();
  float humidity_4 = dht4.readHumidity();
  float temperature_1 = dht1.readTemperature();
  float temperature_2 = dht2.readTemperature();
  float temperature_3 = dht3.readTemperature();
  float temperature_4 = dht4.readTemperature();
  if (isnan(humidity_1) || isnan(temperature_1)) {
    Serial.println("Failed to read from DHT 1 sensor!");
    return;
  }
  if (isnan(humidity_2) || isnan(temperature_2)) {
    Serial.println("Failed to read from DHT 2 sensor!");
    return;
  }
  if (isnan(humidity_3) || isnan(temperature_3)) {
    Serial.println("Failed to read from DHT 1 sensor!");
    return;
  }
  if (isnan(humidity_4) || isnan(temperature_4)) {
    Serial.println("Failed to read from DHT 4 sensor!");
    return;
  }
  Serial.print("Humidity 1: ");
  Serial.print(humidity_1);
  Serial.print(" %\t");
  Serial.print("Temperature 1: ");
  Serial.print(temperature_1);
  Serial.print(" *C ");
  Serial.print(" %\t");
  Serial.print("Humidity 2: ");
  Serial.print(humidity_2);
  Serial.print(" %\t");
  Serial.print("Temperature 2: ");
  Serial.print(temperature_2);
  Serial.print(" *C ");
  Serial.print(" %\t");
  Serial.print("Humidity 3: ");
  Serial.print(humidity_3);
  Serial.print(" %\t");
  Serial.print("Temperature 3: ");
  Serial.print(temperature_3);
  Serial.print(" *C ");
  Serial.print(" %\t");
  Serial.print("Humidity 4: ");
  Serial.print(humidity_4);
  Serial.print(" %\t");
  Serial.print("Temperature 4: ");
  Serial.print(temperature_4);
  Serial.println(" *C ");
}
