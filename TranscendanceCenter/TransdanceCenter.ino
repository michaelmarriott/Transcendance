#include "FastLED.h"                                          // FastLED library.

//https://wokwi.com/projects/358569580274920449 FlashFireLike
//https://wokwi.com/projects/358568650122972161 
#define NUM_LEDS 800                                           // Number of LED's.
#define NUM_LEDS_PER_STRIP 100
#define NUM_STRIPS 8
#define NUM_NON_STATIC 4
#define NUM_STATIC 4

struct CRGB leds[NUM_STRIPS*NUM_LEDS_PER_STRIP];
//Pattern Control
uint32_t pattern = 3;
uint32_t patternCounter = 0;
int patternPeriod = 2000;
///

///Piano split
uint8_t   noiseData[NUM_LEDS];
uint8_t   noiseData2[NUM_LEDS];
uint8_t   noiseData3[NUM_LEDS];
uint8_t   noiseData4[NUM_LEDS];
uint8_t   octaveVal   = 1;
uint16_t  xVal        = 0;
int       scaleVal    = 20;
uint16_t  timeVal     = 0;


uint8_t   octaveVal2   = 1;
uint16_t  xVal2        = 90;
int       scaleVal2   = 100;
uint16_t  timeVal2     = 0;


uint8_t   octaveVal3   = 2;
uint16_t  xVal3        = 0;
int       scaleVal3    = 20;
uint16_t  timeVal3     = 0;


uint8_t   octaveVal4   = 1;
uint16_t  xVal4        = 0;
int       scaleVal4   = 1000;
uint16_t  timeVal4     = 0;
///

//ColorWipeRain
int delayOnColorWipeRain = 100;
///

//Fire
CRGBPalette16 firePalette = HeatColors_p;
///


CRGBPalette16  blackWhitePalette = CRGBPalette16(
  CRGB(200, 100, 10), CRGB(random(90,200), 40, 0),   CRGB(random(80,200), 10, 20),  CRGB(random(60,200), 40, 30),
  CRGB(140, 70, 0),  CRGB(random(50,200), 40, 10),  CRGB(random(50,200), 40, 30),  CRGB(random(80,200), 10, 0),
  CRGB(100, 50, 10), CRGB(random(80,200), 40, 0),   CRGB(random(80,200), 10, 10),  CRGB(random(60,200), 40, 30),
  CRGB(140, 70, 0),  CRGB(random(140,200), 70, 10), CRGB(random(80,200), 60, 30),   CRGB(random(80,200), 40, 20)
);


///


#define CYCLE_LENGTH 3

#define TWINKLE_SPEED 4

// Overall twinkle density.
// 0 (NONE lit) to 8 (ALL lit at once).  
// Default is 5.
#define TWINKLE_DENSITY 5

// How often to change color palettes.
#define SECONDS_PER_PALETTE  20
// Also: toward the bottom of the file is an array 
// called "ActivePaletteList" which controls which color
// palettes are used; you can add or remove color palettes
// from there freely.

// Background color for 'unlit' pixels
// Can be set to CRGB::Black if desired.
CRGB gBackgroundColor = CRGB(CRGB::FairyLight).nscale8_video(16);
// Example of dim incandescent fairy light background color
// CRGB gBackgroundColor = CRGB(CRGB::FairyLight).nscale8_video(16);

// If AUTO_SELECT_BACKGROUND_COLOR is set to 1,
// then for any palette where the first two entries 
// are the same, a dimmed version of that color will
// automatically be used as the background color.
#define AUTO_SELECT_BACKGROUND_COLOR 1

// If COOL_LIKE_INCANDESCENT is set to 1, colors will 
// fade out slighted 'reddened', similar to how
// incandescent bulbs change color as they get dim down.
#define COOL_LIKE_INCANDESCENT 0


const TProgmemRGBPalette16 Ice2_p FL_PROGMEM =
{
  CRGB(100,0,0), CRGB(100,0,0), CRGB(100,0,0), CRGB(100,0,0),
  CRGB(100,0,0), CRGB(100,0,0), CRGB(100,0,0), CRGB(100,0,0),
  CRGB(100,0,0), CRGB(100,0,0), CRGB(100,0,0), CRGB(100,0,0),
  CRGB(130,0,0), CRGB(130,0,0), CRGB(160,0,0), CRGB(160,0,0)
};


CRGBPalette16 gCurrentPalette = blackWhitePalette;
CRGBPalette16 gTargetPalette = blackWhitePalette;
///
void setup() {
  Serial.begin(57600);                                        // Initialize serial port for debugging.
  delay(500);                                                // Soft startup to ease the flow of electrons.
  LEDS.addLeds<WS2811_PORTD, NUM_STRIPS, RGB>(leds, NUM_LEDS_PER_STRIP);
  FastLED.setBrightness(220);
  //set_max_power_in_volts_and_milliamps(5, 20000);

}

void loop() {

  timeVal = millis() / 2;

  timeVal2 = millis() / 2;
  
  memset(noiseData, 0, 250);
  memset(noiseData2, 0, 250);
  fill_raw_noise8(noiseData, 250, octaveVal, xVal, scaleVal, timeVal);
  fill_raw_noise8(noiseData2, 250, octaveVal2, xVal2, scaleVal2, timeVal2);
  memset(noiseData3, 0, NUM_LEDS);
  memset(noiseData4, 0, NUM_LEDS);
  fill_raw_noise8(noiseData3, NUM_LEDS, octaveVal, xVal, scaleVal, timeVal);
  fill_raw_noise8(noiseData4, NUM_LEDS, octaveVal2, xVal2, scaleVal2, timeVal2);
  // put your main code here, to run repeatedly:


  EVERY_N_SECONDS( SECONDS_PER_PALETTE ) { 
    chooseNextColorPalette( gTargetPalette ); 
  }
  
  EVERY_N_MILLISECONDS( 10 ) {
    nblendPaletteTowardPalette( gCurrentPalette, gTargetPalette, 12);
  }

  EVERY_N_MILLISECONDS(100) {
    patternCounter +=1;
    Serial.print("patternCounter:");
    Serial.println(patternCounter);
    Serial.print("pattern:");
    Serial.println(pattern);
    
    if(pattern > 8){
      pattern = 1;
      patternCounter = 0;
    }
  }
//breath(1); 
//FastLED.show();



  EVERY_N_MILLISECONDS(20) {
  switch (pattern) {

      case 3:
        //breath(1);
       drawTwinkles(leds);
        if (patternCounter > patternPeriod) {
          pattern = pattern + 1;
          patternCounter = 0;
        }
        FastLED.show();
        break;
      
      default:
        break;
    }

 }


 EVERY_N_MILLISECONDS(60) {
  switch (pattern) {

      case 3:
        //breath(1);
       //drawTwinkles(leds);
        if (patternCounter > patternPeriod) {
          pattern = pattern + 1;
          patternCounter = 0;
        }
        FastLED.show();
        break;
      
      default:
        break;
    }

 }


 EVERY_N_MILLISECONDS(100) {
  switch (pattern) {
        FastLED.show();
        break;
       
      case 6:
        spottedFlickersFire(); 
        if (patternCounter > patternPeriod) {
          pattern = pattern + 1;
          patternCounter = 0;
        }
        FastLED.show();
        break;
      default:
        break;
    }

 }


 EVERY_N_MILLISECONDS(100) {
    switch (pattern) {
      case 4:
        RippleWrapper(10); 
        if (patternCounter > patternPeriod) {
          pattern = pattern + 1;
          patternCounter = 0;
        }
        FastLED.show();
        break;
       case 1:
          RippleWrapper(10); 
          if (patternCounter > patternPeriod) {
            pattern = pattern + 1;
            patternCounter = 0;
          }
          FastLED.show();
          break;
        break;    
      default:
        break;
    }
 }



 EVERY_N_MILLISECONDS(500) {
    switch (pattern) {
      case 7:
        runWhiteFireSingle(); 
        if (patternCounter > patternPeriod) {
          pattern = pattern + 1;
          patternCounter = 0;
        }
        FastLED.show();
        
        break; 

      default:
        break;
    }
  } 

  EVERY_N_MILLISECONDS(5000) {
    switch (pattern) {
        case 2:
        ColorWipeRainWrapper(false, delayOnColorWipeRain); 
        if (patternCounter >  patternPeriod/40) {
           pattern = pattern + 1;
          patternCounter = 0;
        }
        FastLED.show();
       case 5:
        ColorWipeRainWrapper(false, delayOnColorWipeRain); 
        if (patternCounter > patternPeriod/40) {
          pattern = pattern + 1;
          patternCounter = 0;
        }
        FastLED.show();
        
      default:
        break;
    }
  } 
}


static float pulseSpeed = 1;  // Larger value gives faster pulse.

uint8_t hueA = 15;  // Start hue at valueMin.
uint8_t satA = 230;  // Start saturation at valueMin.
float valueMin = 60.0;  // Pulse minimum value (Should be less then valueMax).

uint8_t hueB = 95;  // End hue at valueMax.
uint8_t satB = 255;  // End saturation at valueMax.
float valueMax = 255.0;  // Pulse maximum value (Should be larger then valueMin).

uint8_t hue1 = hueA;  // Do Not Edit
uint8_t sat1 = satA;  // Do Not Edit
float val1 = valueMin;  // Do Not Edit
uint8_t hueDelta = hueA - hueB;  // Do Not Edit
static float delta = (valueMax - valueMin) / 2.35040238;  // Do Not Edit
void breath1(){

 uint8_t brightness = (exp(sin(millis() / 2000.0 * PI)) - 0.368) * 42.546;
 if(brightness <  40){
  brightness = 40;
 }
  for (int i = 0; i < NUM_LEDS; i++) {
   
    leds[i] = CHSV(30, 150, brightness);

    // You can experiment with commenting out these dim8_video lines
    // to get a different sort of look.
    //leds[i].r = dim8_video(leds[i].r);
//leds[i].g = dim8_video(leds[i].g);
   // leds[i].b = dim8_video(leds[i].b);
  }
}


void breath(uint16_t interval) {
    const uint8_t min_brightness = 130;
    static uint8_t delta = 0; // goes up to 255 then overflows back to 0
 
    static uint32_t pm = 0; // previous millis
    if ( (millis() - pm) > interval ) {
        pm = millis();
 
        // for the LEDs in the current state setting the brightness higher than max_brightness will not actually increase the brightness displayed
        uint8_t max_brightness = 210;
        uint8_t b = scale8(triwave8(delta), max_brightness-min_brightness)+min_brightness;

        for (int i = 0; i < NUM_LEDS; i++) {
          leds[i] = CHSV(30, 200, b);
          
        }
       // FastLED.setBrightness(b);
 
        delta++;
    }
}


void pianoSplit() {
   
  for(int l = 0; l < NUM_LEDS_PER_STRIP; l++){
  //  if(noiseData[l] < 75){
   //  noiseData[l] = 75;
   // }
    CRGB pixelColor = CHSV(90, 200, noiseData[l]);
    if(noiseData2[l] < 150){
     noiseData2[l] = noiseData2[l]/2;
    }
    CRGB pixelColor2 =  CHSV(90, 200, noiseData2[l]);
    for(int n = 0; n < NUM_NON_STATIC; n++){
        leds[l+(n*NUM_LEDS_PER_STRIP)] = pixelColor;
    }
    for(int s = NUM_NON_STATIC; s < NUM_STATIC + NUM_NON_STATIC; s++){
        leds[l + (s * NUM_LEDS_PER_STRIP)] = pixelColor2;
    }
  }
}

void pianoSingleLineSplit() {

  if(noiseData[0] < 75){
     noiseData[0] = 175;
  }
  CRGB pixelColor =  CHSV(150, 200, noiseData3[0]);
  if(noiseData2[0] > 175){
     noiseData2[0] = 175;
  }
  CRGB pixelColor2 =  CHSV(150, 200, noiseData4[0]);
  for(int l = 0; l < NUM_LEDS_PER_STRIP; l++){
    for(int n = 0; n < NUM_NON_STATIC; n++){
        leds[l+(n*NUM_LEDS_PER_STRIP)] = pixelColor;
    }
    for(int s = NUM_NON_STATIC; s < NUM_STATIC + NUM_NON_STATIC; s++){
        leds[l+(s*NUM_LEDS_PER_STRIP)] = pixelColor2;
    }
  }
}

void runWhiteFireSingle() {
  int  a = millis();
  
  for (int i = 0; i < NUM_LEDS_PER_STRIP; i++) {

    // 3D noise, x is constant, we move through time on y and z axis
    // The 60 here will change the scale of the effect, lower is smoother
    // higher is more flickery. The time value for z was in the original code
    // as that was a 2D matrix version. I've left it in here as it looks 
    // nice in 1D too!
    uint8_t noise = inoise8 (0 , i * 60 + a , a / 3);

    // Divides 255 by (NUM_LEDS_PER_STRIP - 1) and subtracts that amount away from 255 to return 
    // a decreasing number each time e.g. for NUM_LEDS = 18, difference between
    // each point is 15, so:
    // i = 0, math = 255
    // i = 1, math = 240
    // i = 2, math = 225
    // ...
    // i = NUM_LEDS, math =  0
    uint8_t math = abs8(i - (NUM_LEDS_PER_STRIP-1)) * 255 / (NUM_LEDS_PER_STRIP-1);

    // Take the noise function from earlier and subtract math from it,
    // so the further along the strip you are, the higher palette index you
    // are likely to get. This results in the base of the fire (white, yellow)
    // being at the far end of the strip
    uint8_t index = qsub8 (noise, math);

    // Set the LED color from the palette
    leds[i] = ColorFromPalette (firePalette, index, 255);   
  }  

  uint8_t flash1 = NUM_LEDS_PER_STRIP/2;
  uint8_t noise = inoise8 (0 , flash1 * 60 + a , a / 5);
  uint8_t math = abs8(flash1 - (NUM_LEDS_PER_STRIP-1)) * 255 / (NUM_LEDS_PER_STRIP-1);
  uint8_t index = qsub8 (noise, math);
  
  uint8_t flash2 = NUM_LEDS_PER_STRIP;

   uint8_t noise2 = inoise8 (0 , flash2 * 60 + a , a / 5);
  uint8_t math2 = abs8(flash2 - (NUM_LEDS_PER_STRIP-1)) * 255 / (NUM_LEDS_PER_STRIP-1);
  uint8_t index2 = qsub8 (noise2, math2);

    Serial.print("index:");
    Serial.print(index);
    Serial.print("index2:");
    Serial.println(index2);

    if (index > 20){
      index = 175;
    }else if (index2 <= 20){
      index = 0;
    }
    if (index2 > 100){
      index2 = 175;
    }else if (index2 <= 100){
      index2 = 0;
    }
  for(int l = 0; l < NUM_LEDS_PER_STRIP; l++){
    
    CRGB pixelColor = CHSV(30, 150, index);
    CRGB pixelColor3 = CHSV(30, 150, index2);
    for(int n = 0; n < NUM_NON_STATIC; n++){
        leds[l+(n*NUM_LEDS_PER_STRIP)] = pixelColor;
    }
    for(int s = NUM_NON_STATIC; s < NUM_STATIC + NUM_NON_STATIC; s++){
        leds[l+(s*NUM_LEDS_PER_STRIP)] = pixelColor3;
    }
  }

}



void runWhiteFireSplit() {
  int  a = millis();
  
  for (int i = 0; i < NUM_LEDS; i++) {

    // 3D noise, x is constant, we move through time on y and z axis
    // The 60 here will change the scale of the effect, lower is smoother
    // higher is more flickery. The time value for z was in the original code
    // as that was a 2D matrix version. I've left it in here as it looks 
    // nice in 1D too!
    uint8_t noise = inoise8 (0 , i * 60 + a , a / 3);

    // Divides 255 by (NUM_LEDS - 1) and subtracts that amount away from 255 to return 
    // a decreasing number each time e.g. for NUM_LEDS = 18, difference between
    // each point is 15, so:
    // i = 0, math = 255
    // i = 1, math = 240
    // i = 2, math = 225
    // ...
    // i = NUM_LEDS, math =  0
    uint8_t math = abs8(i - (NUM_LEDS-1)) * 255 / (NUM_LEDS-1);

    // Take the noise function from earlier and subtract math from it,
    // so the further along the strip you are, the higher palette index you
    // are likely to get. This results in the base of the fire (white, yellow)
    // being at the far end of the strip
    uint8_t index = qsub8 (noise, math);

    // Set the LED color from the palette
    leds[i] = ColorFromPalette (firePalette, index, 255);   
  }  


}



void ColorWipeRainWrapper(bool allowReverse, int wait) {
  Serial.println("ColorWipeRainWrapper");
  ColorWipeRain(ChangeColor(), allowReverse, wait);
}

void ColorWipeRain(CRGB color, bool allowReverse, uint8_t wait) {

  int reverse = 0;
  if (allowReverse) {
    int r = random8(16, 30);
    if (r % 2 == 0) {
      reverse = NUM_LEDS_PER_STRIP - 1;
    }
  }

  int colorWipeSpeed = wait;
  Serial.println("ColorWipe");
  random16_add_entropy(random());
  int trailMax = random8(4, 8);
  int apart = random8(8, 20);
  int amountOfAparts = random8(6, 20);
  for (int i = (0 - (trailMax)); i < NUM_LEDS_PER_STRIP + trailMax + (apart * amountOfAparts); i++) {

    for (int strip = 0; strip < NUM_STRIPS; strip++) {

      for (int t = 0; t < trailMax; t++) {

        if (i - t > 0 && i - t < NUM_LEDS_PER_STRIP) {
          leds[(strip * NUM_LEDS_PER_STRIP) + abs(reverse - (i - t))] = color;
          leds[(strip * NUM_LEDS_PER_STRIP) + abs(reverse - (i - t))].fadeToBlackBy(t * (int)(256 / trailMax));
        }
        for (int a = 1; a < amountOfAparts; a++) {
          int iapart = ((i - t) - (apart * a));
          if (iapart > 0 && (iapart) < NUM_LEDS_PER_STRIP) {
            leds[(strip * NUM_LEDS_PER_STRIP) + abs(reverse - iapart)] = color;
            leds[(strip * NUM_LEDS_PER_STRIP) + abs(reverse - iapart)].fadeToBlackBy(t * (int)(256 / trailMax));
          }
        }
      }
    }
    FastLED.show();
    delay(colorWipeSpeed);
    for (int strip = 0; strip < NUM_STRIPS; strip++) {
      for (int t = 0; t < trailMax; t++) {
        if (i - t > 0 && i - t < NUM_LEDS_PER_STRIP) {
          leds[(strip * NUM_LEDS_PER_STRIP) + abs(reverse - (i - t))].fadeToBlackBy(0);
        }
        for (int a = 1; a < amountOfAparts; a++) {
          int iapart = ((i - t) - (apart * a));
          if (iapart > 0 && (iapart) < NUM_LEDS_PER_STRIP) {
            leds[(strip * NUM_LEDS_PER_STRIP) + abs(reverse - 0)].fadeToBlackBy(0);
            leds[(strip * NUM_LEDS_PER_STRIP) + abs(reverse - iapart)].fadeToBlackBy(0);
          }
        }
      }

    }

  }
  Serial.println("ColorWipeRain Complete");
}


int ripple_color = 25;
int ripple_center = 0;
int ripple_step = -1;
int ripple_maxSteps = 74;
float ripple_fadeRate = 0.95;
int ripple_diff;

//background color
uint32_t ripple_currentBg = random(180,200);
uint32_t ripple_nextBg = ripple_currentBg;

void RippleWrapper(int speed) {
  Ripple(speed);
}

void Ripple(int speed) {


  if (ripple_currentBg == ripple_nextBg) {
    ripple_nextBg = random(256);
  }
  else if (ripple_nextBg > ripple_currentBg) {
    ripple_currentBg++;
  } else {
    ripple_currentBg--;
  }
  for (int x = 0; x < NUM_STRIPS; x++) {
    for (uint16_t i = 0; i < NUM_LEDS_PER_STRIP; i++) {
      leds[(x * NUM_LEDS_PER_STRIP) + i] = CHSV(ripple_color , ripple_currentBg, 45);         // strip.setPixelColor(l, Wheel(currentBg, 0.1));
    }
  }

  if (ripple_step == -1) {
    ripple_center = random(NUM_LEDS_PER_STRIP);
    ripple_color = 25;
    ripple_step = 0;
  }

  if (ripple_step == 0) {
    leds[ripple_center] = CHSV(ripple_color, 190, 250);         // strip.setPixelColor(center, Wheel(color, 1));
    ripple_step ++;
  }
  else {
    if (ripple_step < ripple_maxSteps) {
      Serial.println(pow(ripple_fadeRate, ripple_step));
      for (int x = 0; x < NUM_STRIPS; x++) {
        leds[wrap((x * NUM_LEDS_PER_STRIP) + ripple_center + ripple_step)] = CHSV(ripple_color, ripple_currentBg, pow(ripple_fadeRate, ripple_step) * 255);     //   strip.setPixelColor(wrap(center + step), Wheel(color, pow(fadeRate, step)));
        leds[wrap(ripple_center - ripple_step)] = CHSV(ripple_color, 255, pow(ripple_fadeRate, ripple_step) * 255);     //   strip.setPixelColor(wrap(center - step), Wheel(color, pow(fadeRate, step)));
        if (ripple_step > 3) {
          leds[wrap((x * NUM_LEDS_PER_STRIP) + ripple_center + ripple_step - 3)] = CHSV(ripple_color, ripple_currentBg, pow(ripple_fadeRate, ripple_step - 2) * 255);   //   strip.setPixelColor(wrap(center + step - 3), Wheel(color, pow(fadeRate, step - 2)));
          leds[wrap((x * NUM_LEDS_PER_STRIP) + ripple_center - ripple_step + 3)] = CHSV(ripple_color, ripple_currentBg, pow(ripple_fadeRate, ripple_step - 2) * 255);   //   strip.setPixelColor(wrap(center - step + 3), Wheel(color, pow(fadeRate, step - 2)));
        }
      }
      ripple_step ++;
    }
    else {
      ripple_step = -1;
    }
  }
}

int wrap(int step) {
  if (step < 0) return NUM_LEDS_PER_STRIP + step;
  if (step > NUM_LEDS - 1) return step - NUM_LEDS_PER_STRIP;
  return step;
}


CRGB ChangeColor() {
  random16_add_entropy(random());
  return CRGB(random8(90,210), random8(10,90), random8(0,0));
}


void spottedFlickersFire() {
  int  a = millis();
  
  for (int i = 0; i < NUM_LEDS; i++) {

    // 3D noise, x is constant, we move through time on y and z axis
    // The 60 here will change the scale of the effect, lower is smoother
    // higher is more flickery. The time value for z was in the original code
    // as that was a 2D matrix version. I've left it in here as it looks 
    // nice in 1D too!
    uint8_t noise = inoise8 (0 , i * 60 + a , a / 3);

    // Divides 255 by (NUM_LEDS - 1) and subtracts that amount away from 255 to return 
    // a decreasing number each time e.g. for NUM_LEDS = 18, difference between
    // each point is 15, so:
    // i = 0, math = 255
    // i = 1, math = 240
    // i = 2, math = 225
    // ...
    // i = NUM_LEDS, math =  0
    uint8_t math = abs8(i - (NUM_LEDS_PER_STRIP-1)) * 255 / (NUM_LEDS_PER_STRIP-1);

    // Take the noise function from earlier and subtract math from it,
    // so the further along the strip you are, the higher palette index you
    // are likely to get. This results in the base of the fire (white, yellow)
    // being at the far end of the strip
    uint8_t index = qsub8 (noise, math);

    // Set the LED color from the palette
    leds[i] = ColorFromPalette(blackWhitePalette, index, 20);   
  }  
}



int rdms[NUM_STRIPS][3];
CHSV colorOC[NUM_STRIPS];
void OscialateComplexWrapper(int delay) {
  if (patternCounter <= 1 ) {
    Serial.println("OscialateComplex");
    CRGB wcolor = CRGB::WhiteSmoke;
    int middle = NUM_LEDS_PER_STRIP / 2;
    for (uint8_t x = 0; x < NUM_STRIPS; x++) {
      for (int y = 0; y < NUM_LEDS_PER_STRIP; y++) {
        leds[(x * NUM_LEDS_PER_STRIP) + (y)] = CRGB::Black;
      }
    }
    FastLED.show(); // display this frame
    for (uint8_t x = 0; x < NUM_STRIPS; x++) {
      leds[(x * NUM_LEDS_PER_STRIP) + (middle)] = wcolor;
    }
    FastLED.show();

    Serial.print("OscialateComplex");

    for (uint8_t s = 0; s < NUM_STRIPS; s++) {
      rdms[s][0] = random8(middle - 10, middle - 1); //Size
      rdms[s][1] = 0;//COUNTER
      rdms[s][2] = 0;//0 = up, 1 =down;
    }

    //Got Out
    for (uint8_t s = 0; s < NUM_STRIPS; s++) {
      colorOC[s] = CHSV(random8(0,64), random8(0,80), 180);
    }
  }
  OscialateComplex();

}


void OscialateComplex() {
  uint8_t colorSpeed = 10;
  int middle = NUM_LEDS_PER_STRIP / 2;
  CRGB wcolor = CRGB::WhiteSmoke;
  // 1 is position
  // 0 travel amount
  // 2 is direction 0 up, 1 down
  for (uint8_t strip = 0; strip < NUM_STRIPS; strip++) {
    if (rdms[strip][1] == 0) {
      rdms[strip][2] = 0;
      rdms[strip][0] = random8(0, middle - 1);
      colorOC[strip] = CHSV(random8(), random8(80,150), 180);
    }
    else if (rdms[strip][1] >= middle) {
      rdms[strip][2] = 1;
    }
    if (rdms[strip][2] == 0) {
      rdms[strip][1] = rdms[strip][1] + 1;
    }
    else {
      rdms[strip][1] = rdms[strip][1] - 1;
    }
 Serial.println(rdms[strip][1]);
 Serial.println("---------------");
    leds[(strip * NUM_LEDS_PER_STRIP) + (middle - rdms[strip][1])] = wcolor;
    leds[(strip * NUM_LEDS_PER_STRIP) + (middle + rdms[strip][1])] = wcolor;

  }
  FastLED.show();
  for (uint8_t strip = 0; strip < NUM_STRIPS; strip++) {
    leds[(strip * NUM_LEDS_PER_STRIP) + (middle - rdms[strip][1])] = colorOC[strip];
    leds[(strip * NUM_LEDS_PER_STRIP) + (middle + rdms[strip][1])] = colorOC[strip];
  }

}

CRGB FixColor(CRGB color, int stripe){
  CRGB new_color = color;
 // if(stripe <= 5){
   //   new_color = CRGB(color.g,color.r,color.b);
//  }
  return new_color;
}




//  This function loops over each pixel, calculates the 
//  adjusted 'clock' that this pixel should use, and calls 
//  "CalculateOneTwinkle" on each pixel.  It then displays
//  either the twinkle color of the background color, 
//  whichever is brighter.
void drawTwinkles( CRGB L)
{
  // "PRNG16" is the pseudorandom number generator
  // It MUST be reset to the same starting value each time
  // this function is called, so that the sequence of 'random'
  // numbers that it generates is (paradoxically) stable.
  uint16_t PRNG16 = 11337;
  
  uint32_t clock32 = millis();

  // Set up the background color, "bg".
  // if AUTO_SELECT_BACKGROUND_COLOR == 1, and the first two colors of
  // the current palette are identical, then a deeply faded version of
  // that color is used for the background color
  CRGB bg;
  if( (AUTO_SELECT_BACKGROUND_COLOR == 1) &&
      (gCurrentPalette[0] == gCurrentPalette[1] )) {
    bg = gCurrentPalette[0];
    uint8_t bglight = bg.getAverageLight();
    if( bglight > 64) {
      bg.nscale8_video( 16); // very bright, so scale to 1/16th
    } else if( bglight > 16) {
      bg.nscale8_video( 64); // not that bright, so scale to 1/4th
    } else {
      bg.nscale8_video( 86); // dim, scale to 1/3rd.
    }
  } else {
    bg = gBackgroundColor; // just use the explicitly defined background color
  }

  uint8_t backgroundBrightness = bg.getAverageLight();

  for (int a = 0; a < NUM_LEDS; a++)
    {
    PRNG16 = (uint16_t)(PRNG16 * 2053) + 1384; // next 'random' number
    uint16_t myclockoffset16= PRNG16; // use that number as clock offset
    PRNG16 = (uint16_t)(PRNG16 * 2053) + 1384; // next 'random' number
    // use that number as clock speed adjustment factor (in 8ths, from 8/8ths to 23/8ths)
    uint8_t myspeedmultiplierQ5_3 =  ((((PRNG16 & 0xFF)>>4) + (PRNG16 & 0x0F)) & 0x0F) + 0x08;
    uint32_t myclock30 = (uint32_t)((clock32 * myspeedmultiplierQ5_3) >> 3) + myclockoffset16;
    uint8_t  myunique8 = PRNG16 >> 8; // get 'salt' value for this pixel

    // We now have the adjusted 'clock' for this pixel, now we call
    // the function that computes what color the pixel should be based
    // on the "brightness = f( time )" idea.
    CRGB c = computeOneTwinkle( myclock30, myunique8);

    uint8_t cbright = c.getAverageLight();
    int16_t deltabright = cbright - backgroundBrightness;
    if( deltabright >= 32 || (!bg)) {
      // If the new pixel is significantly brighter than the background color, 
      // use the new color.
      leds[a] = c;
    } else if( deltabright > 0 ) {
      // If the new pixel is just slightly brighter than the background color,
      // mix a blend of the new color and the background color
      leds[a] = blend( bg, c, deltabright * 8);
    } else { 
      // if the new pixel is not at all brighter than the background color,
      // just use the background color.
      leds[a] = bg;
    }
  }
}


//  This function takes a time in pseudo-milliseconds,
//  figures out brightness = f( time ), and also hue = f( time )
//  The 'low digits' of the millisecond time are used as 
//  input to the brightness wave function.  
//  The 'high digits' are used to select a color, so that the color
//  does not change over the course of the fade-in, fade-out
//  of one cycle of the brightness wave function.
//  The 'high digits' are also used to determine whether this pixel
//  should light at all during this cycle, based on the TWINKLE_DENSITY.
CRGB computeOneTwinkle( uint32_t ms, uint8_t salt)
{
  uint16_t ticks = ms >> (8-TWINKLE_SPEED);
  uint8_t fastcycle8 = ticks;
  uint16_t slowcycle16 = (ticks >> 8) + salt;
  slowcycle16 += sin8( slowcycle16);
  slowcycle16 =  (slowcycle16 * 2053) + 1384;
  uint8_t slowcycle8 = (slowcycle16 & 0xFF) + (slowcycle16 >> 8);
  
  uint8_t bright = 0;
  if( ((slowcycle8 & 0x0E)/2) < TWINKLE_DENSITY) {
    bright = attackDecayWave8( fastcycle8);
  }

  uint8_t hue = slowcycle8 - salt;
  CRGB c;
  if( bright > 0) {
    c = ColorFromPalette( gCurrentPalette, hue, bright, NOBLEND);
    if( COOL_LIKE_INCANDESCENT == 1 ) {
      coolLikeIncandescent( c, fastcycle8);
    }
  } else {
    c = CRGB::Black;
  }
  return c;
}


// This function is like 'triwave8', which produces a 
// symmetrical up-and-down triangle sawtooth waveform, except that this
// function produces a triangle wave with a faster attack and a slower decay:
//
//     / \ 
//    /     \ 
//   /         \ 
//  /             \ 
//

uint8_t attackDecayWave8( uint8_t i)
{
  if( i < 86) {
    return i * 3;
  } else {
    i -= 86;
    return 255 - (i + (i/2));
  }
}

// This function takes a pixel, and if its in the 'fading down'
// part of the cycle, it adjusts the color a little bit like the 
// way that incandescent bulbs fade toward 'red' as they dim.
void coolLikeIncandescent( CRGB& c, uint8_t phase)
{
  if( phase < 128) return;

  uint8_t cooling = (phase - 128) >> 4;
  c.g = qsub8( c.g, cooling);
  c.b = qsub8( c.b, cooling * 2);
}

// A mostly red palette with green accents and white trim.
// "CRGB::Gray" is used as white to keep the brightness more uniform.
const TProgmemRGBPalette16 RedGreenWhite_p FL_PROGMEM =
{  CRGB::Red, CRGB::Red, CRGB::Red, CRGB::Red, 
   CRGB::Red, CRGB::Red, CRGB::Red, CRGB::Red, 
   CRGB::Red, CRGB::Red, CRGB::Gray, CRGB::Gray, 
   CRGB::Green, CRGB::Green, CRGB::Green, CRGB::Green };

// A mostly (dark) green palette with red berries.
#define Holly_Green 0x00580c
#define Holly_Red   0xB00402
const TProgmemRGBPalette16 Holly_p FL_PROGMEM =
{  Holly_Green, Holly_Green, Holly_Green, Holly_Green, 
   Holly_Green, Holly_Green, Holly_Green, Holly_Green, 
   Holly_Green, Holly_Green, Holly_Green, Holly_Green, 
   Holly_Green, Holly_Green, Holly_Green, Holly_Red 
};

// A red and white striped palette
// "CRGB::Gray" is used as white to keep the brightness more uniform.
const TProgmemRGBPalette16 RedWhite_p FL_PROGMEM =
{  CRGB::Red,  CRGB::Red,  CRGB::Red,  CRGB::Red, 
   CRGB::Gray, CRGB::Gray, CRGB::Gray, CRGB::Gray,
   CRGB::Red,  CRGB::Red,  CRGB::Red,  CRGB::Red, 
   CRGB::Gray, CRGB::Gray, CRGB::Gray, CRGB::Gray };

// A mostly blue palette with white accents.
// "CRGB::Gray" is used as white to keep the brightness more uniform.
const TProgmemRGBPalette16 BlueWhite_p FL_PROGMEM =
{  CRGB::Blue, CRGB::Blue, CRGB::Blue, CRGB::Blue, 
   CRGB::Blue, CRGB::Blue, CRGB::Blue, CRGB::Blue, 
   CRGB::Blue, CRGB::Blue, CRGB::Blue, CRGB::Blue, 
   CRGB::Blue, CRGB::Gray, CRGB::Gray, CRGB::Gray };

// A pure "fairy light" palette with some brightness variations
#define HALFFAIRY ((CRGB::FairyLight & 0xFEFEFE) / 2)
#define QUARTERFAIRY ((CRGB::FairyLight & 0xFCFCFC) / 4)
const TProgmemRGBPalette16 FairyLight_p FL_PROGMEM =
{  CRGB::FairyLight, CRGB::FairyLight, CRGB::FairyLight, CRGB::FairyLight, 
   HALFFAIRY,        HALFFAIRY,        CRGB::FairyLight, CRGB::FairyLight, 
   QUARTERFAIRY,     QUARTERFAIRY,     CRGB::FairyLight, CRGB::FairyLight, 
   CRGB::FairyLight, CRGB::FairyLight, CRGB::FairyLight, CRGB::FairyLight };

// A palette of soft snowflakes with the occasional bright one
const TProgmemRGBPalette16 Snow_p FL_PROGMEM =
{  0x304048, 0x304048, 0x304048, 0x304048,
   0x304048, 0x304048, 0x304048, 0x304048,
   0x304048, 0x304048, 0x304048, 0x304048,
   0x304048, 0x304048, 0x304048, 0xE0F0FF };

// A palette reminiscent of large 'old-school' C9-size tree lights
// in the five classic colors: red, orange, green, blue, and white.
#define C9_Red    0xB80400
#define C9_Orange 0x902C02
#define C9_Green  0x046002
#define C9_Blue   0x070758
#define C9_White  0x606820
const TProgmemRGBPalette16 RetroC9_p FL_PROGMEM =
{  C9_Red,    C9_Orange, C9_Red,    C9_Orange,
   C9_Orange, C9_Red,    C9_Orange, C9_Red,
   C9_Green,  C9_Green,  C9_Green,  C9_Green,
   C9_Blue,   C9_Blue,   C9_Blue,
   C9_White
};

// A cold, icy pale blue palette
#define Ice_Blue1 0x0C1040
#define Ice_Blue2 0x182080
#define Ice_Blue3 0x5080C0
const TProgmemRGBPalette16 Ice_p FL_PROGMEM =
{
  Ice_Blue1, Ice_Blue1, Ice_Blue1, Ice_Blue1,
  Ice_Blue1, Ice_Blue1, Ice_Blue1, Ice_Blue1,
  Ice_Blue1, Ice_Blue1, Ice_Blue1, Ice_Blue1,
  Ice_Blue2, Ice_Blue2, Ice_Blue2, Ice_Blue3
};


// Add or remove palette names from this list to control which color
// palettes are used, and in what order.
const TProgmemRGBPalette16* ActivePaletteList[] = {
  &FairyLight_p,
  &Snow_p,
  &Ice_p  
};


// Advance to the next color palette in the list (above).
void chooseNextColorPalette( CRGBPalette16& pal)
{
  const uint8_t numberOfPalettes = sizeof(ActivePaletteList) / sizeof(ActivePaletteList[0]);
  static uint8_t whichPalette = -1; 
  whichPalette = addmod8( whichPalette, 1, numberOfPalettes);

  pal = *(ActivePaletteList[whichPalette]);
}

///////////////////
