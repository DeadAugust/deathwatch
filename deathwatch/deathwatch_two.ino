/* 
"MY DAYS ARE NUMBERED"
Deathwatch sketch for Adafruit Circuit Playground and 128x64 OLED screen
Frankensteined from several lovely Adafruit tutorials by August Luhrs [Github @deadaugust]
Version 1.0 Jul. 1 2017
Version 2.0 Feb. 23 2019

based on:
vu_meter example from Circuit Playground library
buttoncycler example from NeoPixel library

u8g2 library for all the OLED display stuff!

life expectancy based on stats for 24 y.o male:
https://flowingdata.com/2015/09/23/years-you-have-left-to-live-probably/

Adafruit invests time and resources providing open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit! (seriously, they're the best y'all)

Written by Adafruit Industries.  Distributed under the BSD license.
This paragraph must be included in any redistribution.
*/
#include <Adafruit_NeoPixel.h>  //i know this is kind of redundant, but i didn't want to mess with the neopixel effect i got from buttoncycler
#include <Adafruit_CircuitPlayground.h>
#include <math.h>
#include <SPI.h>
#include <Wire.h>

//for new OLED library
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

//the skull logo XBM
#define skull_width 128
#define skull_height 64
static const unsigned char skull_bits[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x00, 0x00, 
  0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x78, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x01, 0x00, 0x00, 0x00, 0x78, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x07, 0x00, 
  0x00, 0x00, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0xC0, 0x3F, 0x20, 0x04, 0xC0, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x07, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0xFF, 
  0xFF, 0xFF, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0xF8, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xFF, 0xFF, 0x1F, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xFF, 
  0xFF, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x80, 0xFF, 0xFF, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xFF, 0xFF, 0x0F, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xFC, 
  0xFF, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x80, 0xF1, 0x3F, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xF1, 0x3F, 0x02, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF9, 
  0x7F, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0xFE, 0xFF, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0xFF, 0x01, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 
  0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0xFC, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0xFF, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 
  0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0xF8, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x3F, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 
  0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0xE0, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x12, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 
  0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x30, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x30, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 
  0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x70, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x1C, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 
  0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x12, 0x04, 0x00, 0xC0, 0x1D, 0x00, 0xE0, 
  0x00, 0x00, 0xFC, 0x01, 0x1C, 0x00, 0x00, 0x07, 0x00, 0x7E, 0x06, 0x00, 
  0xC0, 0xCF, 0xC7, 0x6C, 0x3E, 0x0E, 0xCE, 0xF9, 0xCC, 0xC7, 0x07, 0x8F, 
  0x0F, 0xEE, 0xCC, 0x03, 0x80, 0xC7, 0xEE, 0x0E, 0xDE, 0x1F, 0xCE, 0xFD, 
  0xDD, 0xEF, 0x06, 0xC7, 0x0D, 0xEE, 0xEE, 0x07, 0x00, 0xC7, 0xEF, 0x0F, 
  0x8E, 0x0B, 0xFC, 0xD9, 0xFF, 0xEE, 0x0F, 0xDF, 0x0E, 0xFF, 0xEE, 0x07, 
  0x00, 0x83, 0xC7, 0x0D, 0x06, 0x0F, 0xF8, 0xF0, 0xCC, 0xC6, 0x07, 0x8E, 
  0x07, 0x3E, 0xCC, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0xA0, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, };


U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

//for vibrating motor
const int vibPin = 9;

//from neopixel
#define PIXEL_COUNT 10
#define PIXEL_PIN  17
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

//from vumeter
#define NUMFLAKES 10      
#define XPOS 0
#define YPOS 1
#define DELTAY 2
#define MIC_PIN         A4  // Microphone is attached to this analog pin (A4 for circuit playground)
#define SAMPLE_WINDOW   10  // Sample window for average level
#define PEAK_HANG       24  // Time of pause before peak dot falls
#define PEAK_FALL        4  // Rate of falling peak dot
#define INPUT_FLOOR     10  // Lower range of analogRead input
#define INPUT_CEILING  888  // Max range of analogRead input, the lower the value the more sensitive (1023 = max)
                            // i changed mine to 888, for louder music, and also b/c good number


byte peak = 16;        // Peak level of column; used for falling dots
unsigned int sample;
byte dotCount = 0;     //Frame counter for peak dot
byte dotHangCount = 0; //Frame counter for holding peak dot

float fscale(float originalMin, float originalMax, float newBegin, float newEnd, float inputValue, float curve);

//bool slideSwitch;
int days;             //number of days left to live
bool startProphecy; //for starting prophecy
int firstWatch = 1;   //for the single run of watch() 
long rando;           //random number for oracle()
int yrs; //for expDate
int mths;
int dys;


//had to put all the functions before setup because kept getting "not declared in scope" error



/*neopixel function
.
.
*/

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

/*vumeter functions
.
.
.
.
. */

//Used to draw a line between two points of a given color
void drawLine(uint8_t from, uint8_t to, uint32_t c) {
  uint8_t fromTemp;
  if (from > to) {
    fromTemp = from;
    from = to;
    to = fromTemp;
  }
  for(int i=from; i<=to; i++){
    CircuitPlayground.strip.setPixelColor(i, c);
  }
}


float fscale( float originalMin, float originalMax, float newBegin, float
newEnd, float inputValue, float curve){

  float OriginalRange = 0;
  float NewRange = 0;
  float zeroRefCurVal = 0;
  float normalizedCurVal = 0;
  float rangedValue = 0;
  boolean invFlag = 0;


  // condition curve parameter
  // limit range

  if (curve > 10) curve = 10;
  if (curve < -10) curve = -10;

  curve = (curve * -.1) ; // - invert and scale - this seems more intuitive - postive numbers give more weight to high end on output 
  curve = pow(10, curve); // convert linear scale into lograthimic exponent for other pow function

  /*
   Serial.println(curve * 100, DEC);   // multply by 100 to preserve resolution  
   Serial.println(); 
   */

  // Check for out of range inputValues
  if (inputValue < originalMin) {
    inputValue = originalMin;
  }
  if (inputValue > originalMax) {
    inputValue = originalMax;
  }

  // Zero Refference the values
  OriginalRange = originalMax - originalMin;

  if (newEnd > newBegin){ 
    NewRange = newEnd - newBegin;
  }
  else
  {
    NewRange = newBegin - newEnd; 
    invFlag = 1;
  }

  zeroRefCurVal = inputValue - originalMin;
  normalizedCurVal  =  zeroRefCurVal / OriginalRange;   // normalize to 0 - 1 float

  // Check for originalMin > originalMax  - the math for all other cases i.e. negative numbers seems to work out fine 
  if (originalMin > originalMax ) {
    return 0;
  }

  if (invFlag == 0){
    rangedValue =  (pow(normalizedCurVal, curve) * NewRange) + newBegin;

  }
  else     // invert the ranges
  {   
    rangedValue =  newBegin - (pow(normalizedCurVal, curve) * NewRange); 
  }

  return rangedValue;
}


// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
    return CircuitPlayground.strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } 
  else if(WheelPos < 170) {
    WheelPos -= 85;
    return CircuitPlayground.strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } 
  else {
    WheelPos -= 170;
    return CircuitPlayground.strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

/* my functions! yay
.
.
.
*/

//watch display function
void watch(){ 
  if (firstWatch == 1){ //so it only loops once
    u8g2.clearBuffer();
    u8g2.setCursor(30, 10);
    u8g2.print("You Have");
    u8g2.setCursor(40, 23);
    u8g2.print(yrs);
    u8g2.setCursor(70,23);
    u8g2.print("years");
    u8g2.setCursor(40, 36);
    u8g2.print(mths);
    u8g2.setCursor(70,36);
    u8g2.print("months");
    u8g2.setCursor(40, 49);
    u8g2.print(dys);
    u8g2.setCursor(70,49);
    u8g2.print("days");
    u8g2.setCursor(25, 62);
    u8g2.print("Left to Live");
    u8g2.sendBuffer();
    //ends loop
    firstWatch = 0;
  }
}

void oracle(){ //runs RNG until new prophecy (this is the "timer")
  if (rando != 333) { //lucky number ;)
    rando = random(10000000);  //adjust this based on desired spacing of event
                            //1000 was less than a minute, 100k was a few minutes
    if(rando == 8 || rando == 33 || rando == 42 || rando == 888 || rando == 69420){
      remindful();
    }
//    Serial.println(rando);
  }
  else {
    prophecy(); //spits out new day
  }
}

void remindful(){ //buzzes vibration motor
  digitalWrite(vibPin, HIGH);
  delay(2000);
  digitalWrite(vibPin, LOW);
}

void prophecy(){ //reveals new life expectancy
  u8g2.clearBuffer();          // clear the internal memory
  u8g2.drawXBMP( 0, 0, skull_width, skull_height, skull_bits);
  u8g2.sendBuffer();          // transfer internal memory to the display (needed?)
  theaterChaseRainbow(8);
  lifeGen();
  firstWatch = 1; //reset?
  rando = 0;
}

void lifeGen(){ //life expectancy generator, probability i'll die in various age blocks
  int years = random(100);  //also, fuck leap years, every year has 365 days
  if (years == 0) {  //1% probability I'll die in the next 5 years
    days = random(1825); //3,285 days in the next 9 years
  }
   else if (years == 1){ //1% chance I'll die in 5-10 years
    days = random(1825, 3650);
  }
  else if (years == 2 || years == 3){ //2% chance I'll die in 10-20 years
    days = random(3650, 7300);
  }
  else if (3<years<7){ //4% chance I'll die in 20-30 years
    days = random(7300, 10950);
  }
  else if (6<years<15){ //8% chance I'll die in 30-40 years
    days = random(10950, 14600);
  }
  else if (14<years<29){ //15% chance I'll die in 40-49 years
    days = random(14600, 18250);
  }
  else { //69-70% chance I'll die between age 50-105 (arbitrary end year, actual life expect is 81.5 years)
    days = random(18250, 29200);
  }
  //for displaying
  yrs = days/365;
  days = days % 365;
  mths = days/31;
  days = days % 31;
  dys = days;
}


void setup() {
  CircuitPlayground.begin();
  Serial.begin(9600); //just to check the RNG day thing (i think)
  u8g2.begin();
  u8g2.setFont(u8g2_font_oldwizard_tf); //thanks Angel!
  randomSeed(analogRead(0)); //initializes psuedo-random number generator with fairly random input
  pinMode(vibPin, OUTPUT);
  startProphecy = true;
}

void loop() {
 
  if (CircuitPlayground.slideSwitch()){ //if switch is to left (+): PARTY MODE!!!!1!!!11
    // CircuitPlayground.redLED(HIGH);   // just use for debugging
    
    //vu_meter code
    int numPixels = CircuitPlayground.strip.numPixels();
    unsigned long startMillis= millis();  // Start of sample window
    float peakToPeak = 0;   // peak-to-peak level

    unsigned int signalMax = 0;
    unsigned int signalMin = 1023;
    unsigned int c, y;

    // collect data for length of sample window (in mS)
    while (millis() - startMillis < SAMPLE_WINDOW)
    {
      sample = analogRead(MIC_PIN);
      if (sample < 1024)  // toss out spurious readings
      {
        if (sample > signalMax)
        {
          signalMax = sample;  // save just the max levels
        }
        else if (sample < signalMin)
        {
          signalMin = sample;  // save just the min levels
        }
      }
    }
    peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
   
    // Serial.println(peakToPeak);
  
    //Fill the strip with rainbow gradient
    for (int i=0;i<=numPixels-1;i++){
      CircuitPlayground.strip.setPixelColor(i,Wheel(map(i,0,numPixels-1,30,150)));
    }
  
    //Scale the input logarithmically instead of linearly
    c = fscale(INPUT_FLOOR, INPUT_CEILING, numPixels, 0, peakToPeak, 2);
  
    // Turn off pixels that are below volume threshold.
    if(c < peak) {
      peak = c;        // Keep dot on top
      dotHangCount = 0;    // make the dot hang before falling
    }
    if (c <= numPixels) { // Fill partial column with off pixels
      drawLine(numPixels, numPixels-c, CircuitPlayground.strip.Color(0, 0, 0));
    }
  
    // Set the peak dot to match the rainbow gradient
    y = numPixels - peak;
    CircuitPlayground.strip.setPixelColor(y-1,Wheel(map(y,0,numPixels-1,30,150)));
    CircuitPlayground.strip.show();
  
    // Frame based peak dot animation
    if(dotHangCount > PEAK_HANG) { //Peak pause length
      if(++dotCount >= PEAK_FALL) { //Fall rate 
        peak++;
        dotCount = 0;
      }
    } 
    else {
      dotHangCount++; 
    }
  }
  else {    //if switch is to right (-): Oracle Mode
//    CircuitPlayground.redLED(LOW); //just use for debugging
    CircuitPlayground.clearPixels(); //keep this or else party mode will keep neopixels lit
    strip.begin(); //redundant?
    if (startProphecy){
      prophecy();
      startProphecy = false;
    }
    watch(); //runs the display animation once then displays days
    oracle(); //runs until new days
    if (CircuitPlayground.rightButton()){ // to generate new days without lights
      Serial.println("right");
      lifeGen();
      firstWatch = 1;
//      watch();
      delay(1000);
    }
    if (CircuitPlayground.leftButton()){
      Serial.println("left");
      remindful();
    }
  }
}
