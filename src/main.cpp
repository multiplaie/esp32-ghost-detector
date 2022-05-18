
/*
 * 
 * This detects advertising messages of BLE devices and compares it with stored MAC addresses. 
 * If one matches, it sends an MQTT message to swithc something
   Copyright <2017> <Andreas Spiess>
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
  to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
  and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
  DEALINGS IN THE SOFTWARE.
   
   Based on Neil Kolban's example file: https://github.com/nkolban/ESP32_BLE_Arduino
 */

#include "BLEDevice.h"
#include <math.h>
#include <stdio.h>
#include <Arduino.h>

#define LED_LOW_LOW 32
#define LED_LOW_MID 33
#define LED_MID_LOW 25
#define LED_MID_HIGH 26
#define LED_HIGH_MID 27
#define LED_HIGH_HIGH 14
#define LED_DIST_OFF -100
#define LED_DIST_LOW_LOW -95
#define LED_DIST_LOW_MID -85
#define LED_DIST_MID_LOW -75
#define LED_DIST_MID_HIGH -65
#define LED_DIST_HIGH_MID -55

BLEScan* pBLEScan;
BLEClient*  pClient;
bool deviceFound = false;
String knownAddresses[] = { "fb:97:03:d2:41:5e", "fa:5d:de:60:af:3a"};
String deviceAdd;
int deviceRssi = -200;
const short int limitResearch = 7;
int currentIndexResearch = 0;
const short int limitFailed = 3;
short int currentFailed = 0;


class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    /**
        Called for each advertising BLE server.
    */
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      bool known = false;
      for (int i = 0; i < (sizeof(knownAddresses) / sizeof(knownAddresses[0])); i++) {
        if (strcmp(advertisedDevice.getAddress().toString().c_str(), knownAddresses[i].c_str()) == 0) known = true;
      }
      if (known) {
        printf("Device found: %d - pos: %d ",advertisedDevice.getRSSI(), currentIndexResearch);
        Serial.println(advertisedDevice.getRSSI());
        deviceFound = true;
        if(deviceRssi < advertisedDevice.getRSSI()){
          deviceAdd = advertisedDevice.getAddress().toString().c_str();
          deviceRssi = advertisedDevice.getRSSI();
        }
      }
      if(currentIndexResearch >= limitResearch )
        advertisedDevice.getScan()->stop();
      else currentIndexResearch = currentIndexResearch+1;
   }
}; 

void distanceLED()
{
    if(deviceRssi < LED_DIST_OFF && currentFailed >= limitFailed){
      digitalWrite(LED_LOW_LOW, LOW);
      digitalWrite(LED_LOW_MID, LOW);
      digitalWrite(LED_MID_LOW, LOW);
      digitalWrite(LED_MID_HIGH, LOW);
      digitalWrite(LED_HIGH_MID, LOW);
      digitalWrite(LED_HIGH_HIGH, LOW);
    }else if(deviceRssi > LED_DIST_OFF && deviceRssi < LED_DIST_LOW_LOW){
      digitalWrite(LED_LOW_LOW, HIGH);
      digitalWrite(LED_LOW_MID, LOW);
      digitalWrite(LED_MID_LOW, LOW);
      digitalWrite(LED_MID_HIGH, LOW);
      digitalWrite(LED_HIGH_MID, LOW);
      digitalWrite(LED_HIGH_HIGH, LOW);
    }else if(deviceRssi > LED_DIST_LOW_LOW && deviceRssi < LED_DIST_LOW_MID){
      digitalWrite(LED_LOW_LOW, HIGH);
      digitalWrite(LED_LOW_MID, HIGH);
      digitalWrite(LED_MID_LOW, LOW);
      digitalWrite(LED_MID_HIGH, LOW);
      digitalWrite(LED_HIGH_MID, LOW);
      digitalWrite(LED_HIGH_HIGH, LOW);
    }else if(deviceRssi > LED_DIST_LOW_MID && deviceRssi < LED_DIST_MID_LOW){
      digitalWrite(LED_LOW_LOW, HIGH);
      digitalWrite(LED_LOW_MID, HIGH);
      digitalWrite(LED_MID_LOW, HIGH);
      digitalWrite(LED_MID_HIGH, LOW);
      digitalWrite(LED_HIGH_MID, LOW);
      digitalWrite(LED_HIGH_HIGH, LOW);
    }else if(deviceRssi > LED_DIST_MID_LOW && deviceRssi < LED_DIST_MID_HIGH){
      digitalWrite(LED_LOW_LOW, HIGH);
      digitalWrite(LED_LOW_MID, HIGH);
      digitalWrite(LED_MID_LOW, HIGH);
      digitalWrite(LED_MID_HIGH, HIGH);
      digitalWrite(LED_HIGH_MID, LOW);
      digitalWrite(LED_HIGH_HIGH, LOW);
    }else if(deviceRssi > LED_DIST_MID_HIGH && deviceRssi < LED_DIST_HIGH_MID){
      digitalWrite(LED_LOW_LOW, HIGH);
      digitalWrite(LED_LOW_MID, HIGH);
      digitalWrite(LED_MID_LOW, HIGH);
      digitalWrite(LED_MID_HIGH, HIGH);
      digitalWrite(LED_HIGH_MID, HIGH);
      digitalWrite(LED_HIGH_HIGH, LOW);
    }else if(deviceRssi > LED_DIST_HIGH_MID){
      digitalWrite(LED_LOW_LOW, HIGH);
      digitalWrite(LED_LOW_MID, HIGH);
      digitalWrite(LED_MID_LOW, HIGH);
      digitalWrite(LED_MID_HIGH, HIGH);
      digitalWrite(LED_HIGH_MID, HIGH);
      digitalWrite(LED_HIGH_HIGH, HIGH);
    }
  }


void setup() {
  Serial.begin(115200);
  Serial.println("Starting Arduino BLE Client application...");
  pinMode(LED_LOW_LOW, OUTPUT);
  pinMode(LED_LOW_MID, OUTPUT);
  pinMode(LED_MID_LOW, OUTPUT);
  pinMode(LED_MID_HIGH, OUTPUT);
  pinMode(LED_HIGH_MID, OUTPUT);
  pinMode(LED_HIGH_HIGH, OUTPUT);
  digitalWrite(LED_LOW_LOW, LOW);
  digitalWrite(LED_LOW_MID, LOW);
  digitalWrite(LED_MID_LOW, LOW);
  digitalWrite(LED_MID_HIGH, LOW);
  digitalWrite(LED_HIGH_MID, LOW);
  digitalWrite(LED_HIGH_HIGH, LOW);

  BLEDevice::init("");
  pClient  = BLEDevice::createClient();
  Serial.println(" - Created client");
  pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);
}

void loop() {
  Serial.println("BLE Scan restarted.....");
  deviceFound = false;
  deviceAdd = "";
  deviceRssi = -200;
  currentIndexResearch = 0;
  BLEScanResults scanResults = pBLEScan->start(2.5);
  if (deviceFound) {
    Serial.println("------");
    Serial.println(deviceAdd);
    Serial.println(deviceRssi);
    printf("distance: %f\n", (float)pow((float)10, (((float)-70 - (float)deviceRssi) / (float)(10*4))));
    Serial.println("------");
    currentFailed = 0;
  }
  else {
    currentFailed = currentFailed +1;
  }
  distanceLED();
} 
