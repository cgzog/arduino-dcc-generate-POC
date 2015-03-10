/*
  DCC test packet generator
  
  This program will repeatedly send a hard-coded DCC packet containing the following information:
  
  Locomotive address:    55
  Direction:             Forward
  Speed Step:            20
  
  It will pause between packets (during which time no other packets will be sent in this version
 */

#define  DCC_PORT            13

#define  INTER_PACK_DELAY    25    // milliseconds


// DCC manifest constants - asl of these are hand tuned to hit the spec numbers based on using the
// digitalWrite() routines
//
// These routines also suffer from jitter that puts them outside of the DCC spec - direct port
// writes will probably be needed for more accurate control

#define  DCC_1_DELAY         54    // 58 microseconds for one phase of '1' bit - 6.5% long
#define  DCC_0_DELAY         96    // 100 microseconds for one phase of '0' bit

#define  PREAMBLE_BIT_COUNT  14    // number of 1's in packet preamble

#define  DCC_ADDR_SHORT      0
#define  DCC_ADDR_LONG       1

#define  DCC_DIR_FOR         1        
#define  DCC_DIR_REV         0

typedef struct {
  unsigned char  sAddr;
} DCC_SHORT_ADDR;

typedef struct {
  unsigned short lAddr;
} DCC_LONG_ADDR;

typedef union {
  unsigned int  val;
  
  struct {
    unsigned int  fixed:2;
    unsigned int  dir:1;
    unsigned int  compat:1;
    unsigned int  speed:4;
  } bits;
} DCC_BASIC_PACKET;

DCC_SHORT_ADDR    testAddr = { 55 };
DCC_BASIC_PACKET  testInst;


// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 13 as an output.
  pinMode(13, OUTPUT);
  digitalWrite(DCC_PORT, LOW);  // steady state condition
  
  // setup test instruction packet
  testInst.bits.fixed  = 0x01;
  testInst.bits.dir    = DCC_DIR_FOR;
  testInst.bits.compat = 1;
  testInst.bits.speed  = 6;
  
}

//  clock out the appropriate DCC bit by toggling the DCC port high
void dccBit(int bit)

{
  switch (bit) {
    
    case 1:
      digitalWrite(DCC_PORT, HIGH);
      delayMicroseconds(DCC_1_DELAY);
      digitalWrite(DCC_PORT, LOW);
      delayMicroseconds(DCC_1_DELAY);
      break;

    case 0:
      digitalWrite(DCC_PORT, HIGH);
      delayMicroseconds(DCC_0_DELAY);
      digitalWrite(DCC_PORT, LOW);
      delayMicroseconds(DCC_0_DELAY);
      break;
  }
}



//  dccByte
//
//  clock out a DCC byte

void dccByte(unsigned char byte) {

int  i;

    for (i = 0 ; i < 8 ; i++) {
      if (byte & 0x80) {          // check the high bit
        dccBit(1);
      } else {
        dccBit(0);
      }
      byte <<= 1;                 // shift for the ext bit
    }  
}


//  dccPacket
//
//  send the DCC packet
//
//  supports both long and short addresses

void dccPacket(int addrType, unsigned char *addr, int dataCount, unsigned char *data) {

int            i;
unsigned char  local;

  for (i = 0 ; i < PREAMBLE_BIT_COUNT ; i++) {
    dccBit(1);      // send the preamble bits
  }
  
  dccBit(0);        // end of preamble
  
  if (addrType == DCC_ADDR_SHORT) {    // process it as a short address (implies number of address bytes)
    dccByte(*addr);
  } else {                             // it's a long address - 2 bytes to clock out
  }
  
  dccBit(0);        // end of address
  
  for (i = 0 ; i < dataCount ; i++) {
    dccByte(*data);
    data++;        // next byte, if any...
  }
    
  dccBit(0);        // end of instructions - start of error detection value
  
  dccByte((unsigned char) 0x43);    // hard coded for now
  
  dccBit(1);        // end of error detection
}


void loop() {
  
  dccPacket(DCC_ADDR_SHORT, (unsigned char *)&testAddr, 1, (unsigned char *)&testInst);

  dccByte(0);
  
  delay(1000);
}



#if 0
// the loop function runs over and over again forever
void loop() {
  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(INTER_PACK_DELAY);
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  delay(INTER_PACK_DELAY*2);
  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(INTER_PACK_DELAY);
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  delay(INTER_PACK_DELAY*3);
  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(INTER_PACK_DELAY);
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  delay(INTER_PACK_DELAY*50);
}
#endif



