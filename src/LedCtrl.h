/**
 *  Management of Leds with lib FastLed
 *  => https://fastled.io
 */ 

#include <Arduino.h>
#include <FastLED.h>

/***
 * 
 * limit rssi for each lights (in Db)
 */
#define LED_LOW_LOW 32          //very far
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
#define LED_DIST_HIGH_MID -55   //very close

#define NUM_LEDS 6              // total leds of scanner
#define LED_PIN 32 //

// init led color
CRGB leds[NUM_LEDS];
int ledColors[NUM_LEDS] = {CRGB::Red,CRGB::Red,CRGB::Yellow,CRGB::Yellow,CRGB::Green,CRGB::Green};

/* Light down for all leds */
void ledCtrlRaz(){
    leds[0] = CRGB::Black;
    leds[1] = CRGB::Black;
    leds[2] = CRGB::Black;
    leds[3] = CRGB::Black;
    leds[4] = CRGB::Black;
    leds[5] = CRGB::Black;
    FastLED.show();
}

/* switch leds */
void ledCtrlLightUp(int ll,int lm,int ml,int mh,int hm,int hh){
    const int ledValue[6] = {ll,lm,ml,mh,hm,hh};
    for(int i = 0; i < NUM_LEDS; i++){
        leds[i] = (ledValue[i]) ? ledColors[i] : CRGB::Black;
        //delay(lightDuration/nbLed);
    }
    FastLED.show();
}

/* Initialization led status */
void ledCtrlSetupPins(){
    FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
    FastLED.setBrightness(20);
    ledCtrlRaz();
}