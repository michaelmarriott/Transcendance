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
uint32_t pattern = 1;
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
int delayOnColorWipeRain = 50;
///

//Fire
CRGBPalette16 firePalette = HeatColors_p;
///


CRGBPalette16  blackWhitePalette = CRGBPalette16(
  CRGB(10, 10, 20),  CRGB::Black, CRGB(10, 10, 20), CRGB::Black,
  CRGB::White,  CRGB::Black,   CRGB::Black,  CRGB(80, 80, 100),
  CRGB::Black,  CRGB(80, 80, 100),  CRGB(40, 40, 60), CRGB::Black,
  CRGB::White,  CRGB::Black,   CRGB::Black, CRGB(10, 10, 20)
);

void setup() {
  Serial.begin(57600);                                        // Initialize serial port for debugging.
  delay(500);                                                // Soft startup to ease the flow of electrons.
  LEDS.addLeds<WS2811_PORTD, NUM_STRIPS>(leds, NUM_LEDS_PER_STRIP);
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

  EVERY_N_MILLISECONDS(100) {
    patternCounter +=1;
    Serial.print("patternCounter:");
    Serial.println(patternCounter);
    Serial.print("pattern:");
    Serial.println(pattern);
    
    if(pattern > 6){
      pattern = 1;
      patternCounter = 0;
    }
  }

 EVERY_N_MILLISECONDS(200) {
  switch (pattern) {

      case 3:
        spottedFlickersFire(); 
        if (patternCounter > patternPeriod) {
          pattern = pattern + 1;
          patternCounter = 0;
        }
        FastLED.show();
      case 6:
        spottedFlickersFire(); 
        if (patternCounter > patternPeriod) {
          pattern = pattern + 1;
          patternCounter = 0;
        }
        FastLED.show();
  
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
       case 1:
          RippleWrapper(10); 
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


 EVERY_N_MILLISECONDS(500) {
    switch (pattern) {
      case 37:
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
    
    CRGB pixelColor = CHSV(190, 150, index);
    CRGB pixelColor3 = CHSV(190, 150, index2);
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
          leds[(strip * NUM_LEDS_PER_STRIP) + abs(reverse - (i - t))].fadeToBlackBy(210);
        }
        for (int a = 1; a < amountOfAparts; a++) {
          int iapart = ((i - t) - (apart * a));
          if (iapart > 0 && (iapart) < NUM_LEDS_PER_STRIP) {
            leds[(strip * NUM_LEDS_PER_STRIP) + abs(reverse - 0)].fadeToBlackBy(250);
            leds[(strip * NUM_LEDS_PER_STRIP) + abs(reverse - iapart)].fadeToBlackBy(210);
          }
        }
      }

    }

  }
  Serial.println("ColorWipeRain Complete");
}


int ripple_color;
int ripple_center = 0;
int ripple_step = -1;
int ripple_maxSteps = 74;
float ripple_fadeRate = 0.95;
int ripple_diff;

//background color
uint32_t ripple_currentBg = random(256);
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
      leds[(x * NUM_LEDS_PER_STRIP) + i] = CHSV(32, ripple_currentBg, 15);         // strip.setPixelColor(l, Wheel(currentBg, 0.1));
    }
  }

  if (ripple_step == -1) {
    ripple_center = random(NUM_LEDS_PER_STRIP);
    ripple_color = 32;
    ripple_step = 0;
  }

  if (ripple_step == 0) {
    leds[ripple_center] = CHSV(ripple_color, 60, 250);         // strip.setPixelColor(center, Wheel(color, 1));
    ripple_step ++;
  }
  else {
    if (ripple_step < ripple_maxSteps) {
      Serial.println(pow(ripple_fadeRate, ripple_step));
      for (int x = 0; x < NUM_STRIPS; x++) {
        leds[wrap((x * NUM_LEDS_PER_STRIP) + ripple_center + ripple_step)] = CHSV(ripple_color, 120, pow(ripple_fadeRate, ripple_step) * 255);     //   strip.setPixelColor(wrap(center + step), Wheel(color, pow(fadeRate, step)));
        leds[wrap(ripple_center - ripple_step)] = CHSV(ripple_color, 255, pow(ripple_fadeRate, ripple_step) * 255);     //   strip.setPixelColor(wrap(center - step), Wheel(color, pow(fadeRate, step)));
        if (ripple_step > 3) {
          leds[wrap((x * NUM_LEDS_PER_STRIP) + ripple_center + ripple_step - 3)] = CHSV(ripple_color, 120, pow(ripple_fadeRate, ripple_step - 2) * 255);   //   strip.setPixelColor(wrap(center + step - 3), Wheel(color, pow(fadeRate, step - 2)));
          leds[wrap((x * NUM_LEDS_PER_STRIP) + ripple_center - ripple_step + 3)] = CHSV(ripple_color, 120, pow(ripple_fadeRate, ripple_step - 2) * 255);   //   strip.setPixelColor(wrap(center - step + 3), Wheel(color, pow(fadeRate, step - 2)));
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
  return CRGB(random8(20,200), random8(0,0), random8(0,10));
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
    uint8_t math = abs8(i - (NUM_LEDS-1)) * 255 / (NUM_LEDS-1);

    // Take the noise function from earlier and subtract math from it,
    // so the further along the strip you are, the higher palette index you
    // are likely to get. This results in the base of the fire (white, yellow)
    // being at the far end of the strip
    uint8_t index = qsub8 (noise, math);

    // Set the LED color from the palette
    leds[i] = ColorFromPalette(blackWhitePalette, index, 2);   
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
