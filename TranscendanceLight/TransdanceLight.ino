
#define NUM_LEDS 800                                           // Number of LED's.
#define NUM_LEDS_PER_STRIP 100
#define NUM_STRIPS 8

void setup() {
  Serial.begin(57600);                                        // Initialize serial port for debugging.
  delay(500);                                                // Soft startup to ease the flow of electrons.
  LEDS.addLeds<WS2811_PORTD, NUM_STRIPS>(leds, NUM_LEDS_PER_STRIP);
  FastLED.setBrightness(220);
  //set_max_power_in_volts_and_milliamps(5, 20000);

}

void loop() {
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
}

void breath() {


  
}
