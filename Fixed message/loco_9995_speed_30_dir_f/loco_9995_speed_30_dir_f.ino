/*
  DCC test packet generator
  
  This program will repeatedly send a hard-coded DCC packet containing the following information:
  
  Locomotive address:    9995
  Direction:             Forward
  Speed Step:            30
  
  It will pause between packets (during which time no other packets will be sent in this version
 */

#define  DCC_PORT            13

#define  INTER_PACK_DELAY    500   // milliseconds

#define  DCC_1_DELAY         58    // microseconds for one phase of '1' bit
#define  DCC_0_DELAY         100   // microseconds for one phase of '0' bit

#define  PREAMBLE_BIT_COUNT  14    // number of 1's in packet preamble

#define  DCC_ADDR_SHORT      0
#define  DC_ADDR_LONG        1



// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 13 as an output.
  pinMode(13, OUTPUT);
  digitalWrite(DCC_PORT, HIGH);  // steady state condition
}


void dcc1bit() {
  digitalWrite(DCC_PORT, LOW);
  delayMicroseconds(DCC_1_DELAY);
  digitalWrite(DCC_PORT, HIGH);
  delayMicroseconds(DCC_1_DELAY);
}


void dcc0bit() {
  digitalWrite(DCC_PORT, LOW);
  delayMicroseconds(DCC_0_DELAY);
  digitalWrite(DCC_PORT, HIGH);
  delayMicroseconds(DCC_0_DELAY);
}


//  dccPacket
//
//  send the DCC packet
//
//  supports both long and short addresses

void dccPacket(int addrType, unsigned char *addr, int dataCount, unsigned char *data)

{
int  i;

  for (i = 0 ; i < PREAMBLE_BIT_COUNT ; i++) {
    dcc1bit();      // send the preamble bits
  }
  
  dcc0bit();        // end of preamble
  
  
  dcc0bit();        // end of address
  
  dcc1bit();        // end of error detection
}




// the loop function runs over and over again forever
void loop() {
  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(INTER_PACK_DELAY);
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  delay(INTER_PACK_DELAY);
}




