#include <Arduino.h>

#define LED_LOW_LOW 32
#define LED_LOW_MID 33
#define LED_MID_LOW 25
#define LED_MID_HIGH 26
#define LED_HIGH_MID 27
#define LED_HIGH_HIGH 14
#define LED_DIST_OFF -100
#define LED_DIST_LOW_LOW -95
#define LED_DIST_LOW_MID -80
#define LED_DIST_MID_LOW -75
#define LED_DIST_MID_HIGH -70
#define LED_DIST_HIGH_MID -55
const int lightDuration = 50;
const int nbLed = 6;

const int ledPin[6] = {LED_LOW_LOW, LED_LOW_MID, LED_MID_LOW, LED_MID_HIGH, LED_HIGH_MID, LED_HIGH_HIGH};

/* Light down for all leds */
void ledCtrlRaz(){
    digitalWrite(LED_LOW_LOW, LOW);
    digitalWrite(LED_LOW_MID, LOW);
    digitalWrite(LED_MID_LOW, LOW);
    digitalWrite(LED_MID_HIGH, LOW);
    digitalWrite(LED_HIGH_MID, LOW);
    digitalWrite(LED_HIGH_HIGH, LOW);
}

/*  */
void ledCtrlLightUp(int ll,int lm,int ml,int mh,int hm,int hh){
    const int ledValue[6] = {ll,lm,ml,mh,hm,hh};
    for(int i = 0; i < nbLed; i++){
        digitalWrite(ledPin[i], ledValue[i]);
        delay(lightDuration/nbLed);
    }
}

/* Initialization led status */
void ledCtrlSetupPins(){
    pinMode(LED_LOW_LOW, OUTPUT);
    pinMode(LED_LOW_MID, OUTPUT);
    pinMode(LED_MID_LOW, OUTPUT);
    pinMode(LED_MID_HIGH, OUTPUT);
    pinMode(LED_HIGH_MID, OUTPUT);
    pinMode(LED_HIGH_HIGH, OUTPUT);
    ledCtrlRaz();
}