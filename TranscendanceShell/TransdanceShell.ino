#include "FastLED.h" // FastLED library


#define NUM_LEDS_PER_STRIP_MAX 1260
#define NUM_STRIPS 8

int STRIP_LAYOUT[] = {140, 140, 140, 200, 250, 560, 630, 1260};

int SPEED_LAYOUT[] = {8, 8, 8, 6, 5, 2, 2, 1};

int SPEED_LAST[] = {0, 0, 0, 0, 0, 0, 0, 0};

struct CRGB leds[NUM_STRIPS * NUM_LEDS_PER_STRIP_MAX];
// Pattern Control
uint32_t pattern = 1;
uint32_t max_pattern = 8;
uint32_t patternCounter = 0;
int patternPeriod = 100;

void setup() {
  Serial.begin(57600); // Initialize serial port for debugging.
  delay(500);          // Soft startup to ease the flow of electrons.
  LEDS.addLeds<WS2811_PORTD, NUM_STRIPS, RGB>(leds, NUM_LEDS_PER_STRIP_MAX);
  FastLED.setBrightness(255);
  set_max_power_in_volts_and_milliamps(5, 50000);
 
}


//bulb =1
//strip = 2

CRGBPalette16 calibratePalette2 = CRGBPalette16(
    CRGB::Red, CRGB::Red, CRGB::Blue,CRGB::Blue,
    CRGB::Blue, CRGB::Blue, CRGB::Blue, CRGB::Blue,
    CRGB::Pink, CRGB::Pink, CRGB::Pink, CRGB::Pink,
    CRGB::Pink, CRGB::Pink, CRGB::Pink,  CRGB::Pink);
    
CRGBPalette16 calibratePalette = CRGBPalette16(
    CRGB::Red, CRGB::Red, CRGB::Orange,CRGB::Orange,
    CRGB::Yellow, CRGB::Yellow, CRGB::Green, CRGB::Green,
    CRGB::Blue, CRGB::Blue, CRGB::Violet, CRGB::Violet,
    CRGB::Pink, CRGB::Pink, CRGB::White,  CRGB::White);
    
void loop() {
  // put your main code here, to run repeatedly:

  EVERY_N_MILLISECONDS(100)
  {
    patternCounter += 1;
    Serial.print("patternCounter:");
    Serial.println(patternCounter);
    Serial.print("pattern:");
    Serial.println(pattern);
   // if(pattern > max_pattern){
     // pattern = 0;
    //  patternCounter = 0;
  //  }

  //  calibrate();  
    OscialateComplexWrapper(10);
  }
  if(pattern == 1){
   // run_patterns();
  }
}

void calibrate(){
 Serial.println("calibrate");
 for (uint8_t s = 0; s < NUM_STRIPS; s++){
  int ans =  STRIP_LAYOUT[s];
  Serial.println(ans);
  for (int i = 0; i < ans; i++){
     leds[i + (s * NUM_LEDS_PER_STRIP_MAX)] = FixColor(calibratePalette2[s],s);
  }
 }
  FastLED.show();
}

void run_patterns(){

 if(pattern == 1){
  ColorWipeRainRunner2();
 }
 
}

void checkPatternForComplete(int patternPeriod)
{
  if (patternCounter > patternPeriod)
  {
    pattern = pattern + 1;
    patternCounter = 0;
  }
}

uint8_t trailMax = 0;
uint8_t apart = 0;
uint8_t amountOfAparts = 0;

void ColorWipeRainRunner2(){
   EVERY_N_MILLISECONDS(100){
    switch (pattern)
    {
      case 1: 
      ColorWipeRainWrapper(ChangeColor(),false,10);
      checkPatternForComplete(patternPeriod);
      FastLED.show();
      break;
    default:
      break;
    }
  }
}

void ColorWipeRainWrapper(CRGB color, bool allowReverse, int wait)
{
  Serial.println("ColorWipeRainWrapper");
  ColorWipeRain(color, allowReverse, wait);
}



void ColorWipeRain(CRGB color, bool allowReverse, uint8_t wait)
{
  int reverse = 0;
  if (allowReverse)
  {
    int r = random8(16, 30);
    if (r % 2 == 0)
    {
      reverse = NUM_LEDS_PER_STRIP_MAX - 1;
    }
  }

  int colorWipeSpeed = wait;
  Serial.println("ColorWipe");
  random16_add_entropy(random());
  int trailMax = random8(8, 16);
  int apart = random8(8, 40);
  int amountOfAparts = random8(8, 16);
  for (int c = (0 - (trailMax)); c < NUM_LEDS_PER_STRIP_MAX + trailMax + (apart * amountOfAparts); c++)
  {
    Serial.println("c"+String(c));
    if(c >NUM_LEDS_PER_STRIP_MAX){
      continue;
    }
    for (int strip = 0; strip < NUM_STRIPS; strip++)
    {
      int speeds = SPEED_LAYOUT[strip];
      int last = SPEED_LAST[strip];
      int i = c / speeds;
      if(last != i || 1 == 1){
        for (int t = 0; t < trailMax; t++)
        {
  
          if (i - t > 0 && i - t < NUM_LEDS_PER_STRIP_MAX)
          {
            leds[(strip * NUM_LEDS_PER_STRIP_MAX) + abs(reverse - (i - t))] = color;
            leds[(strip * NUM_LEDS_PER_STRIP_MAX) + abs(reverse - (i - t))].fadeToBlackBy(t * (int)(256 / trailMax));
          }
          for (int a = 1; a < amountOfAparts; a++)
          {
            int iapart = ((i - t) - (apart * a));
            if (iapart > 0 && (iapart) < NUM_LEDS_PER_STRIP_MAX)
            {
              leds[(strip * NUM_LEDS_PER_STRIP_MAX) + abs(reverse - iapart)] = color;
              leds[(strip * NUM_LEDS_PER_STRIP_MAX) + abs(reverse - iapart)].fadeToBlackBy(t * (int)(256 / trailMax));
            }
          }
        }

      }
    }
    FastLED.show();
    delay(colorWipeSpeed);
    for (int strip = 0; strip < NUM_STRIPS; strip++)
    {
      int speeds = SPEED_LAYOUT[strip];
      int last = SPEED_LAST[strip];
      int i = c / speeds;
    //  if(last != i){
        for (int t = 0; t < trailMax; t++)
        {
          if (i - t > 0 && i - t < NUM_LEDS_PER_STRIP_MAX)
          {
            leds[(strip * NUM_LEDS_PER_STRIP_MAX) + abs(reverse - (i - t))].fadeToBlackBy(t * (int)(256 / trailMax));
          }
          for (int a = 1; a < amountOfAparts; a++)
          {
            int iapart = ((i - t) - (apart * a));
            if (iapart > 0 && (iapart) < NUM_LEDS_PER_STRIP_MAX)
            {
              leds[(strip * NUM_LEDS_PER_STRIP_MAX) + abs(reverse - 0)].fadeToBlackBy(t * (int)(256 / trailMax));
              leds[(strip * NUM_LEDS_PER_STRIP_MAX) + abs(reverse - iapart)].fadeToBlackBy(t * (int)(256 / trailMax));
            }
          }
        }
     // }
      SPEED_LAST[strip] = i;
    }
   
  }
  Serial.println("ColorWipeRain Complete");
}


CRGB ChangeColor()
{
  random16_add_entropy(random());
  return CRGB(random8(100, 255), random8(0, 80), random8(0, 200));
}

CRGB FixColor(CRGB color, int stripe){
  CRGB new_color = color;
 // if(stripe <= 5){
   //   new_color = CRGB(color.g,color.r,color.b);
//  }
  return new_color;
}




int rdms[NUM_STRIPS][3];
CHSV colorOC[NUM_STRIPS];
void OscialateComplexWrapper(int delay)
{
  if (patternCounter <= 1)
  {
    Serial.println("OscialateComplex");
    CRGB wcolor = CRGB::WhiteSmoke;
 
    for (uint8_t x = 0; x < NUM_STRIPS; x++)
    {
      for (int y = 0; y < NUM_LEDS_PER_STRIP_MAX; y++)
      {
        leds[(x * NUM_LEDS_PER_STRIP_MAX) + (y)] = CRGB::Black;
      }
    }
    FastLED.show(); // display this frame
    for (uint8_t x = 0; x < NUM_STRIPS; x++)
    {
   //   leds[(x * NUM_LEDS_PER_STRIP_MAX) + (middle)] = wcolor;
    }
    FastLED.show();

    Serial.print("OscialateComplex");

    for (uint8_t strip = 0; strip < NUM_STRIPS; strip++)
    {
      int middle = STRIP_LAYOUT[strip] / 2;
      rdms[strip][0] = random8(middle - 10, middle - 1); // Size
      rdms[strip][1] = 0;                                // COUNTER
      rdms[strip][2] = 0;                                // 0 = up, 1 =down;
    }

    // Got Out
    for (uint8_t s = 0; s < NUM_STRIPS; s++)
    {
      colorOC[s] = CHSV(random8(), random8(80, 150), 180);
    }
  }
  OscialateComplex();
}

void OscialateComplex()
{
  uint8_t colorSpeed = 10;
 
  CRGB wcolor = CRGB::WhiteSmoke;
  // 1 is position
  // 0 travel amount
  // 2 is direction 0 up, 1 down
  for (uint8_t strip = 0; strip < NUM_STRIPS; strip++)
  {
    int middle = STRIP_LAYOUT[strip] / 2;
    if (rdms[strip][1] == 0)
    {
      rdms[strip][2] = 0;
      rdms[strip][0] = random8(0, middle - 1);
      colorOC[strip] = CHSV(random8(), random8(80, 190), 255);
    }
    else if (rdms[strip][1] >= middle)
    {
      rdms[strip][2] = 1;
    }
    if (rdms[strip][2] == 0)
    {
      rdms[strip][1] = rdms[strip][1] + 1;
    }
    else
    {
      rdms[strip][1] = rdms[strip][1] - 1;
    }
    Serial.println(rdms[strip][1]);
    Serial.println("---------------");
    leds[(strip * NUM_LEDS_PER_STRIP_MAX) + (middle - rdms[strip][1])] = wcolor;
    leds[(strip * NUM_LEDS_PER_STRIP_MAX) + (middle + rdms[strip][1])] = wcolor;
  }
  FastLED.show();
  for (uint8_t strip = 0; strip < NUM_STRIPS; strip++)
  {
      int middle = STRIP_LAYOUT[strip] / 2;
    leds[(strip * NUM_LEDS_PER_STRIP_MAX) + (middle - rdms[strip][1])] = colorOC[strip];
    leds[(strip * NUM_LEDS_PER_STRIP_MAX) + (middle + rdms[strip][1])] = colorOC[strip];
  }
}
