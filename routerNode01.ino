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
  int jam;
  int menit;
  int detik;
};

struct data1{
  int tandanode;
  float temperature;
  float humidity;
  int jam;
  int menit;
  int detik;
};

struct data receivedData;
struct data sentData;
struct data1 sentDataR;

sentData.jam = now.hour();
sentData.menit = now.minute();
sentData.detik = now.second();


const unsigned long interval = 15000;
/*const unsigned long node01ReadStart = 4000;
const unsigned long node01ReadEnd = 6000;
const unsigned long node11Start = 0;
const unsigned long node11End = 3000;
const unsigned long node12Start = 9000;
const unsigned long node12End = 12000;*/


void setup()
{
  Serial.begin(9600);
  dht.begin();
  rf24.begin();
  rf24.setDataRate(RF24_2MBPS);
  rf24.openReadingPipe(1, 0xF0F0F0F0E1LL);
  rf24.openWritingPipe(0xF0F0F0F0E2LL)
  rf24.setPALevel(RF24_PA_MIN);
  rtc.begin();
}

void loop()
{
  DateTime now = rtc.now();
  unsigned long currentMillis = millis();

 
  if (currentMillis % interval >= 6000 && currentMillis % interval <= 9000)
  {
    rf24.stopListening();

    sentDataR.tandanode = 1;
    sentDataR.temperature = dht.readTemperature();
    sentDataR.humidity = dht.readHumidity();
    sentDataR.jam = now.hour();
    sentDataR.menit = now.minute();
    sentDataR.detik = now.second();

    if (isnan(humidity) || isnan(temperature))
    {
      Serial.println("Failed to read DHT11 sensor!");
      return;
    }
    
    rf24.write(&sentDataR, sizeof(sentDataR));
    Serial.print("Sent data node 1 to Gateway at");
    Serial.println(sentDataR.jam);
    Serial.print(":");
    Serial.print(sentDataR.menit);
    Serial.print(":");
    Serial.print(sentDataR.detik);
    Serial.println();
    delay(1000);
  }
  
  if (currentMillis % interval >= 0 && currentMillis % interval <= 3000){
    rf24.startListening();
    
    if (rf24.available()){
      rf24.read(&receivedData, sizeof(receivedData));
      
      if (receivedData.tandanode == 11){
        Serial.print("Received Data from Node: ");
        Serial.println(receivedData.tandanode);
        Serial.print("Temperature: ");
        Serial.print(receivedData.temperature);
        Serial.println(" °C");
        Serial.print("Humidity: ");
        Serial.print(receivedData.humidity);
        Serial.println(" %");
        Serial.print(": ");
        Serial.println(now.hour());
        Serial.print(": ");
        Serial.println(now.minute());
        Serial.print(": ");
        Serial.println(now.second());
        Serial.println();
        
        if (currentMillis % interval >= 3000 && currentMillis % interval <= 6000){
          sentData.tandanode = receivedData.tandanode;
          sentData.temperature = receivedData.temperature;
          sentData.humidity = receivedData.humidity;
          sentData.jam = now.hour();
          sentData.menit = now.minute();
          sentData.detik = now.second();
          
          rf24.stopListening();
          rf24.write(&sentData, sizeof(sentData));
          Serial.print("Sent data node 11 to Gateway at");
          Serial.println(sentData.jam);
          Serial.print(":");
          Serial.print(sentData.menit);
          Serial.print(":");
          Serial.print(sentData.detik);
          Serial.println();
          delay(1000);
        }
      }
    }
  }
  
  if (currentMillis % interval >= 9000 && currentMillis % interval <= 12000){
    rf24.startListening();
    
    if (rf24.available()){
      rf24.read(&receivedData, sizeof(receivedData));
      if (receivedData.tandanode == 12){
        Serial.print("Received Data from Node: ");
        Serial.println(receivedData.tandanode);
        Serial.print("Temperature: ");
        Serial.print(receivedData.temperature);
        Serial.println(" °C");
        Serial.print("Humidity: ");
        Serial.print(receivedData.humidity);
        Serial.println(" %");
        Serial.print(": ");
        Serial.println(now.hour());
        Serial.print(": ");
        Serial.println(now.minute());
        Serial.print(": ");
        Serial.println(now.second());
        Serial.println();
        
        if (currentMillis % interval >= 12000 && currentMillis % interval <= 15000){
        
          sentData.tandanode = receivedData.tandanode;
          sentData.temperature = receivedData.temperature;
          sentData.humidity = receivedData.humidity;
          sentData.jam = now.hour();
          sentData.menit = now.minute();
          sentData.detik = now.second();

          rf24.stopListening();
          rf24.write(&sentData, sizeof(sentData));
          Serial.print("Sent data node 12 to Gateway at");
          Serial.println(sentData.jam);
          Serial.print(":");
          Serial.print(sentData.menit);
          Serial.print(":");
          Serial.print(sentData.detik);
          Serial.println();
          delay(1000);
        }
      }
    }
  }
}
