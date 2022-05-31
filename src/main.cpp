#include "BLEDevice.h"
#include <math.h>
#include <stdio.h>
#include <Arduino.h>
#include "LedCtrl.h"
#include "BuzzerConfig.h"

/*BLE*/
BLEScan* pBLEScan;
BLEClient*  pClient;
bool deviceFound = false;
String knownAddresses[] = {/* "fb:97:03:d2:41:5e", */"fa:5d:de:60:af:3a"};
String deviceAdd;
int deviceRssi = -200;
const short int limitResearch = 20;
int currentIndexResearch = 0;
const short int limitFailed = 3;
short int currentFailed = 0;

/*Buzzer var*/
int currentFreqBuzzer = freqBuzzerInit;

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    /**
        Called for each advertising BLE server.
    */
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      bool known = false;
      //printf("Device found: %d - pos: %d - address: %s \n",advertisedDevice.getRSSI(), currentIndexResearch, advertisedDevice.getAddress().toString().c_str());
      if (strcmp(advertisedDevice.getAddress().toString().c_str(), knownAddresses[0].c_str()) == 0) {
        Serial.println(advertisedDevice.getRSSI());
        deviceFound = true;
        deviceAdd = advertisedDevice.getAddress().toString().c_str();
        deviceRssi = advertisedDevice.getRSSI();
        advertisedDevice.getScan()->stop();
      }
      if(currentIndexResearch >= limitResearch )
        advertisedDevice.getScan()->stop();
      else currentIndexResearch = currentIndexResearch+1;
   }
}; 

void IHMProximity()
{
  if(deviceRssi < LED_DIST_OFF && currentFailed >= limitFailed){
    ledcWriteTone(channelBuzzer, freqBuzzerNone);
    currentFreqBuzzer = freqBuzzerNone;
    ledCtrlRaz();
  }
  else if(deviceRssi > LED_DIST_OFF && deviceRssi < LED_DIST_LOW_LOW) {
    ledcWriteTone(channelBuzzer, freqBuzzerNone);
    ledCtrlLightUp(1,0,0,0,0,0);
    delay(100);
    ledCtrlRaz();
    currentFreqBuzzer = freqBuzzerLow;
  }

  else if(deviceRssi > LED_DIST_LOW_LOW && deviceRssi < LED_DIST_LOW_MID) {
    ledcWriteTone(channelBuzzer, freqBuzzerNone);
    ledCtrlLightUp(1,1,0,0,0,0);
    delay(100);
    ledCtrlRaz();
    currentFreqBuzzer = freqBuzzerLow;
  }
  else if(deviceRssi > LED_DIST_LOW_MID && deviceRssi < LED_DIST_MID_LOW) {
    ledcWriteTone(channelBuzzer, freqBuzzerLow);
    ledCtrlLightUp(1,1,1,0,0,0);
    delay(150);
    ledcWriteTone(channelBuzzer, freqBuzzerNone);
    ledCtrlRaz();
    currentFreqBuzzer = freqBuzzerLow;
  }
  else if(deviceRssi > LED_DIST_MID_LOW && deviceRssi < LED_DIST_MID_HIGH){
    ledcWriteTone(channelBuzzer, freqBuzzerLow);
    ledCtrlLightUp(1,1,1,1,0,0);
    delay(150);
    ledcWriteTone(channelBuzzer, freqBuzzerNone);
    ledCtrlRaz();
    currentFreqBuzzer = freqBuzzerLow;
  } 
  else if(deviceRssi > LED_DIST_MID_HIGH && deviceRssi < LED_DIST_HIGH_MID){
    ledcWriteTone(channelBuzzer, freqBuzzerHigh);
    ledCtrlLightUp(1,1,1,1,1,0);
    delay(150);
    currentFreqBuzzer = freqBuzzerHigh;
  } 
  else if(deviceRssi > LED_DIST_HIGH_MID){
    ledcWriteTone(channelBuzzer, freqBuzzerHigh);
    ledCtrlLightUp(1,1,1,1,1,1);
    delay(150);
    currentFreqBuzzer = freqBuzzerHigh;
  } 
}


void setup() {
  
  //console
  Serial.begin(115200);

  //Ble
  Serial.println("Starting Arduino BLE Client application...");
  BLEDevice::init("");
  pClient  = BLEDevice::createClient();
  pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);

  //LED
  ledCtrlSetupPins();
  
  //Buzzer
  ledcSetup(channelBuzzer, freqBuzzerInit, resolutionBuzzer);
  ledcAttachPin(pinBuzzer, channelBuzzer);
}

float distance(int deviceRssi){
  /*
    calc distance in meter
    10^((rssi_for_1_meter - device_rssi)/(10*pertubation [2-4]))

    rssi_for_1_meter : rssi of target device at 1 meter proximity
    device_rssi: rssi value get by ble
    perturbation [2-4] : coeff perturbation 2 => low, 4 => high
  */
  return (float)pow((float)10, (((float)-70 - (float)deviceRssi) / (float)(40)));
}

void loop() {
  ledcWriteTone(channelBuzzer, currentFreqBuzzer);
  Serial.println("BLE Scan restarted.....");
  deviceFound = false;
  deviceAdd = "";
  deviceRssi = -200;
  currentIndexResearch = 0;
  BLEScanResults scanResults = pBLEScan->start(2);
  

  if (deviceFound) {
    Serial.println("------");
    Serial.println(deviceAdd);
    Serial.println(deviceRssi);
    Serial.println("------");
    currentFailed = 0;
  }
  else {
    currentFailed = currentFailed +1;
  }
    IHMProximity();
Serial.println(currentFailed);
  
} 
