#include <Conceptinetics.h>


DMX_Master dmx_master ( 100 , 2 );

void setup() {
  // put your setup code here, to run once:
  dmx_master.enable ();
  dmx_master.setChannelRange ( 1,8,0 );
  dmx_master.setChannelValue ( 5,254 );
}

uint8_t x = 0;
uint8_t r = 0;
uint8_t g = 0;
uint8_t b = 0;
uint8_t a = 127;


//new dmx
//1 is red
//2 is green
//3 is blue
//4 is orange/amber


//5 is bright
// 6 is red
// 7 is green
// 8 is blue

uint8_t rd = 0;
uint8_t gd = 0;
uint8_t bd = 0;
uint8_t ad = 107;

uint8_t dm = 0;
uint8_t rr = 0;
uint8_t gg = 0;
uint8_t bb = 0;
uint8_t ggd = 0;
uint8_t rrd = 0;
uint8_t bbd = 0;
uint8_t dmd = 0;
// the loop routine runs over and over again forever:
void loop() 
{
  
  // Keep fading channel 1 in from 0 to 100%

  r = increment(rd, r);
  rd = change_direction(40,254,rd,r);

  g = increment(gd, g);
  gd = change_direction(0,15,gd,g);

  b = increment(bd, b);
  bd = change_direction(0,30,bd,b);
  
  a = increment(ad, b);
  ad = change_direction(40,254,ad,a);

  dm = increment(dmd, dm);
  dmd = change_direction(40,255,dmd,dm);
  
  rr = increment(rrd, rr);
  rrd = change_direction(60,255,rrd,rr);
  
  gg = increment(ggd, gg);
  ggd = change_direction(0,45,ggd,gg);

  bb = increment(bbd, bb);
  bbd = change_direction(0,60,bbd,bb);
  
  dmx_master.setChannelValue ( 1, r ); 
  dmx_master.setChannelValue ( 2, g ); 
  dmx_master.setChannelValue ( 3, b ); 
  dmx_master.setChannelValue ( 4, a ); 
  dmx_master.setChannelValue ( 5, dm );    
  dmx_master.setChannelValue ( 6, rr );  
  dmx_master.setChannelValue ( 7, gg );
  dmx_master.setChannelValue ( 8, bb );
  //old
  delay ( 400 );
}  //

uint8_t increment(uint8_t dir, uint8_t value){
  if(dir == 0){
    return value + 1;
  }else{
    return value - 1;
  }
}

uint8_t change_direction(uint8_t min_value, uint8_t max_value, uint8_t current_direction, uint8_t value){
  if(value <= min_value){
    return 0;
  }else if(value >= max_value){
     return 1;
  }else{
    return current_direction;
  }
}



//0,42,85,128,171,213,0
//red,yellow,green,aqua,blue,purple,red
