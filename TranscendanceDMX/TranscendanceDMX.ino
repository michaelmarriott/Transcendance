/*
  Read the 5 Channels of DMX Data coming from an ESP32 shield running Example 1
  By: Andy England
  SparkFun Electronics
  Date: , 2018
  License: GNU. See license file for more information but you can
  basically do whatever you want with this code.
  This example runs two servos and a number of LED's off of 5 DMX channels
  
  Feel like supporting open source hardware?
  Buy a board from SparkFun! https://www.sparkfun.com/products/15110
  Hardware Connections:
  Connect pan/tilt servos to pins DATA1 and DATA2, connect LEDs to CLOCK and DATA0. Connect a DMX XLR-3 Cable in between the Output and Input shields
*/

#include <SparkFunDMX.h>

SparkFunDMX dmx;

//Channel Definitions
#define TOTAL_CHANNELS 8


//Pin Definitions for ESP32 WROOM DMX to LED Shield
//#define CLOCK = 5;
//#define DATA0 = 19;
////#define DATA1 = 18;
//#define DATA2 = 27;

uint8_t x = 1;

void setup() {
  Serial.begin(115200);
  delay(3000);
  Serial.println("starting...");
 // dmx.initRead(TOTAL_CHANNELS); 
  dmx.initWrite(TOTAL_CHANNELS);           // initialization for complete bus
  Serial.println("initialized...");
  delay(200);               // wait a while (not necessary)
}

void loop() {

 
  if(x == 1){
     dmx.write(1, 200);
  }else if(x == 2){
     dmx.write(2, 80);
  }else if(x == 3){
    dmx.write(3, 50);
  }else{
    dmx.write(4, 240);
  }

 if(x > TOTAL_CHANNELS){
   x = 1;
 }
  Serial.print(x++);//Print and increment x. Since x is an unsigned 8 bit integer it will loop back to 0 if we try to increment it past 255
  dmx.update(); // update the DMX bus witht he values that we have written
  Serial.println(": updated!");
  delay(1000);
}
