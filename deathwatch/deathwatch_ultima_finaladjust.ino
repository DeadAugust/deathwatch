/* 
"MY DAYS ARE NUMBERED"
Deathwatch sketch for Adafruit Circuit Playground and 128x64 OLED screen
Frankensteined from several lovely Adafruit tutorials by August Luhrs [deadaugust]
Version 1.0 Jul. 1 2017

based on:
vu_meter example from Circuit Playground library
SSD1306 128x64 12C example from SSD1306 library
buttoncycler example from NeoPixel library

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
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>  //had to uncomment #define "SSD1306_128_64" and out "#define ...32"


//from SSD1306
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

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
#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16 
static const unsigned char PROGMEM logo16_glcd_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000 };



#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!"); //if you're getting this, check line 22
#endif


byte peak = 16;        // Peak level of column; used for falling dots
unsigned int sample;
byte dotCount = 0;     //Frame counter for peak dot
byte dotHangCount = 0; //Frame counter for holding peak dot

float fscale(float originalMin, float originalMax, float newBegin, float newEnd, float inputValue, float curve);

bool slideSwitch;
int days;             //number of days left to live
int firstWatch = 1;   //for the single run of watch() 
long rando;           //random number for oracle()


//had to put all the functions before setup because kept getting "not declared in scope" error

/*SSD1306 functions
.
.
.
.
. */


//draws the cool circle thing
void testdrawcircle(void) {
  for (int16_t i=0; i<display.height(); i+=2) {
    display.drawCircle(display.width()/2, display.height()/2, i, WHITE);
    display.display();
    delay(1);
  }
}

//watch display function
void watch(){ 
  if (firstWatch == 1){ //so it only loops once
    // draw cool circle thing
    testdrawcircle();
    display.display();
    delay(2000);
    display.clearDisplay();
    
    // days display
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(33,0);
    display.println("You have");
    display.setTextSize(3);
    display.println(days);
    display.setTextSize(2);
    display.println("days left to live");
    display.display(); 
    display.clearDisplay();
    //ends loop
    firstWatch = 0;
  }
}
    
    
/* my functions! yay
.
.
.
*/

void oracle(){ //runs RNG until new prophecy (this is the "timer")
  if (rando != 333) { //lucky number ;)
    rando = random(8888888);  //adjust this based on desired spacing of event
                            //1000 was less than a minute, 100k was a few minutes
    Serial.println(rando);
  }
  else {
    prophecy(); //spits out new day
  }
}

void prophecy(){ //reveals new life expectancy
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,34);
  display.println("YOU'RE GOING TO DIE");
  display.display();
  theaterChaseRainbow(55);
  lifeGen();
  display.clearDisplay();
  firstWatch = 1; //reset?
  rando = 0;
}

void lifeGen(){ //life expectancy generator, probability i'll die in various age blocks
  int years = random(100);  //also, fuck leap years, every year has 365 days
  Serial.println(years);
  if (years == 0) {  //1% probability I'll die in the next 9 years
    days = random(3286); //3,285 days in the next 9 years
    Serial.println(days);
  }
  else if (years == 1 || years == 2){ //2% chance I'll die in 10-19 years
    days = random(3286, 6936);
    Serial.println(days);
  }
  else if (years > 2 && years < 7){ //4% chance I'll die in 20-29 years
    days = random(6936, 10586);
    Serial.println(days);
  }
  else if (years > 6 && years < 15){ //8% chance I'll die in 30-39 years
    days = random(10586, 14236);
    Serial.println(days);
  }
  else if (years > 14 && years < 29){ //15% chance I'll die in 40-49 years
    days = random(14236, 17886);
    Serial.println(days);
  }
  else { //69-70% chance I'll die between age 50-88 (arbitrary end year, actual life expect is 81.5 years)
    days = random(17886, 32121);
    Serial.println(days);
  }
}





/*neopixel function
.
.
*/

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j= 210; j < 248; j++) {     // cycles through blue --> green
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


void setup() {
  CircuitPlayground.begin();
  Serial.begin(9600); //just to check the RNG day thing (i think)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
                  // had to change to 0X3C -- **IMPORTANT**
  display.clearDisplay();
  randomSeed(analogRead(0)); //initializes psuedo-random number generator with fairly random input
  prophecy(); //new prophecy every time it starts
}

void loop() {
  slideSwitch = CircuitPlayground.slideSwitch();
 
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
    // CircuitPlayground.redLED(LOW); //just use for debugging
    CircuitPlayground.clearPixels(); //keep this or else party mode will keep neopixels lit
    strip.begin(); //redundant?
    watch(); //runs the display animation once then displays days
    oracle(); //runs until new days
  }
}

