#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <Wire.h>
#include "RTClib.h"

RTC_DS3231 rtc;
DHT dht(2, DHT11);
RF24 rf24(7, 8);

struct data
{
  int tandanode;
  float temperature;
  float humidity;
  uint8_t jam;
  uint8_t menit;
  uint8_t detik;
};

struct data sentData;

const unsigned long interval = 30000;  // 15 seconds interval
const unsigned long node11TimeStart = 0;
const unsigned long node11TimeEnd = 6000;

void setup()
{
  Serial.begin(9600);
  dht.begin();
  rf24.begin();
  rf24.setDataRate(RF24_2MBPS);
  rf24.openWritingPipe(0xF0F0F0F0E1LL);
  rf24.setPALevel(RF24_PA_MIN);
  rf24.stopListening();
  rtc.begin();
}

void loop()
{
  DateTime now = rtc.now();
  unsigned long currentMillis = millis();

  if (now.second() % 30 >= 0 && now.second() % 30 <= 6)
  {
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    if (isnan(humidity) || isnan(temperature))
    {
      Serial.println("Failed to read DHT11 sensor!");
      return;
    }

    sentData.tandanode = 11;
    sentData.temperature = temperature;
    sentData.humidity = humidity;
    //sentData.jam = now.hour();
    //sentData.menit = now.minute();
    //sentData.detik = now.second();

    rf24.write(&sentData, sizeof(sentData));

    Serial.println("Data sent from Node 11:");
    Serial.print("Tandanode: ");
    Serial.println(sentData.tandanode);
    Serial.print("Temperature: ");
    Serial.print(sentData.temperature);
    Serial.println(" °C");
    Serial.print("Humidity: ");
    Serial.print(sentData.humidity);
    Serial.println(" %");
    Serial.print("Jam: ");
    Serial.println(sentData.jam);
    Serial.print("Menit: ");
    Serial.println(sentData.menit);
    Serial.print("Detik: ");
    Serial.println(sentData.detik);
    Serial.println();
  }

  delay(1000);
}
