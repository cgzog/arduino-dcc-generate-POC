
/*
  DCC test packet generator
  
  This program will repeatedly send a hard-coded DCC packet containing the following information:
  
  Locomotive address:    55
  Direction:             Forward
  Speed Step:            20
  
  It will pause between packets (during which time no other packets will be sent in this version)
  
  This version attempts to generate a bi-polar "style" DCC output which normally is at approximately
  2.5v and drops to 0 or rises to 5v.  In order to support this, the DC output pin #8 needs to be tied
  to both pull up and pull down resistors; the output is placed in a input mode to effectively tristate
  it when no DCC signal is being generated.  While this is not technically "DCC", it makes packet capture
  on an oscilliscope much easier to see.
  
  In a production deployment, the DCC signal would likely drive an H-bridge to generate the appropriate
  DCC polarity but for testing, this is easy to work with and view when running.
  
 */

#define  DCC_PORT            8

#define  INTER_PACK_DELAY    25    // milliseconds


// DCC manifest constants - asl of these are hand tuned to hit the spec numbers based on using the
// digitalWrite() routines
//
// These routines also suffer from jitter that puts them outside of the DCC spec - direct port
// writes will probably be needed for more accurate control

#define  DCC_1_DELAY         54    // 58 microseconds for one phase of '1' bit - 6.5% long
#define  DCC_0_DELAY         96    // 100 microseconds for one phase of '0' bit

#define  PREAMBLE_BIT_COUNT  14    // number of 1's in packet preamble

#define  DCC_NEG             0
#define  DCC_POS             1

#define  DCC_ADDR_SHORT      0
#define  DCC_ADDR_LONG       1

#define  DCC_DIR_FOR         1        
#define  DCC_DIR_REV         0

//#define  DCC_BIT_1           { DIGITAL_WRITE(DCC_PORT, DCC_NEG); delayMicroseconds(DCC_1_DELAY); DIGITAL_WRITE(DCC_PORT, DCC_POS); delayMicroseconds(DCC_1_DELAY); }
//#define  DCC_BIT_0           { DIGITAL_WRITE(DCC_PORT, DCC_NEG); delayMicroseconds(DCC_0_DELAY); DIGITAL_WRITE(DCC_PORT, DCC_POS); delayMicroseconds(DCC_0_DELAY); }
#define  DCC_BIT_1           { DIGITAL_WRITE(DCC_PORT, DCC_NEG); delayMicroseconds(DCC_1_DELAY); DIGITAL_WRITE(DCC_PORT, DCC_POS); delayMicroseconds(DCC_1_DELAY); }
#define  DCC_BIT_0           { DIGITAL_WRITE(DCC_PORT, DCC_NEG); delayMicroseconds(DCC_0_DELAY); DIGITAL_WRITE(DCC_PORT, DCC_POS); delayMicroseconds(DCC_0_DELAY); }



#define  PIN_MODE(pin, mode)          pinMode(pin, mode)
#define  DIGITAL_WRITE(pin, level)    digitalWrite(pin, level)


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
  PIN_MODE(DCC_PORT, INPUT);               // effectively tristate it
  
  // setup test instruction packet
  testInst.bits.fixed  = 0x01;
  testInst.bits.dir    = DCC_DIR_FOR;
  testInst.bits.compat = 1;
  testInst.bits.speed  = 6;
  
}

//  dccBit
//
//  clock out the appropriate DCC bit by toggling the DCC port low and then high 
//
// assumes any need for tri-stating of the output is handled by the caller

void dccBit(int bit) {

int wait;

 if (bit == 1) {
    wait = DCC_1_DELAY;
  } else {
    wait = DCC_0_DELAY;
  }
  
  DIGITAL_WRITE(DCC_PORT, DCC_NEG);
  delayMicroseconds(wait);
  DIGITAL_WRITE(DCC_PORT, DCC_POS);
  delayMicroseconds(wait);
}



//  dccByte
//
//  clock out a DCC byte

void dccByte(unsigned char byte) {

int  i;

  PIN_MODE(DCC_PORT, OUTPUT);

  for (i = 0 ; i < 8 ; i++) {
    if (byte & 0x80) {          // check the high bit
      DCC_BIT_1;
    } else {
      DCC_BIT_0;
    }
    byte <<= 1;                 // shift for the next bit
  }
 
  PIN_MODE(DCC_PORT, INPUT);
}


//  dccBaselinePacket
//
//  send the DCC baseline packet as described in NMRA S9.2
//
//  supports both long and short addresses

void dccBaselinePacket(int addrType, unsigned char *addr, int dataCount, unsigned char *data) {

int            i;
unsigned char  local;

  for (i = 0 ; i < PREAMBLE_BIT_COUNT ; i++) {
    DCC_BIT_1;      // send the preamble bits
  }
  
  DCC_BIT_0;        // end of preamble

  if (addrType == DCC_ADDR_SHORT) {    // process it as a short address (implies number of address bytes)
    dccByte(*addr);
  } else {                             // it's a long address - 2 bytes to clock out
  }
  
  DCC_BIT_0;        // end of address
  
  for (i = 0 ; i < dataCount ; i++) {
    dccByte(*data);
    data++;        // next byte, if any...
  }
    
  DCC_BIT_0;        // end of instructions - start of error detection value
  
  dccByte((unsigned char) 0x43);    // hard coded for now
  
  DCC_BIT_1;        // end of error detection.
}


void loop() {
  
  delay(2000);
  
  dccBaselinePacket(DCC_ADDR_SHORT, (unsigned char *)&testAddr, 1, (unsigned char *)&testInst);

  dccByte(0);

}




