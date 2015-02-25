//This snippet displays a single "picture" on a single MAX7219 LED matrix display.
//Datasheet: <URL: http://datasheets.maximintegrated.com/en/ds/MAX7219-MAX7221.pdf >
//
//Connect VCC to 5V, GND to GND, and DIN, CS and CLK to GPIO pins (I used digital pin 2, 3 and 4)

const int DIN_PIN       = 2; //Pin used to connected Arduino to DIN on the MAX7219
const int CS_PIN        = 3; //Pin used to connected Arduino to CS on the MAX7219
const int CLK_PIN       = 4; //Pin used to connected Arduino to CLK on the MAX7219

const int BRIGHTNESS    = 8; // 0-15

// == MAX7219 Commands =========================

const byte MAX7219_REG_DIGIT0       = 0x01;
const byte MAX7219_REG_DIGIT1       = 0x02;
const byte MAX7219_REG_DIGIT2       = 0x03;
const byte MAX7219_REG_DIGIT3       = 0x04;
const byte MAX7219_REG_DIGIT4       = 0x05;
const byte MAX7219_REG_DIGIT5       = 0x06;
const byte MAX7219_REG_DIGIT6       = 0x07;
const byte MAX7219_REG_DIGIT7       = 0x08;
const byte MAX7219_REG_DECODE_MODE  = 0x09;
const byte MAX7219_REG_INTENSITY    = 0x0a;
const byte MAX7219_REG_SCAN_LIMIT   = 0x0b;
const byte MAX7219_REG_SHUTDOWN     = 0x0c;
const byte MAX7219_REG_DISPLAY_TEST = 0x0f;



void putByte(const byte data) {
  byte i = 8;
  while (i-- > 0) {
    digitalWrite(CLK_PIN, LOW);
    digitalWrite(DIN_PIN, (data & (1<<i)) ? HIGH : LOW);
    digitalWrite(CLK_PIN, HIGH);
  }
}

void pushCommand(byte command, byte data) {    
  digitalWrite(CS_PIN, LOW);     
  putByte(command);
  putByte(data);
  digitalWrite(CS_PIN, HIGH); 
}

// == Arduino Setup ============================

void setup () {
  pinMode(DIN_PIN, OUTPUT);
  pinMode(CS_PIN, OUTPUT);
  pinMode(CLK_PIN, OUTPUT);

  //initialize of the MAX7219
  digitalWrite(CLK_PIN, HIGH);  
  pushCommand(MAX7219_REG_SCAN_LIMIT, 0x07);   // display all 8 digits   
  pushCommand(MAX7219_REG_DECODE_MODE, 0x00);  // using a led matrix (not digits)
  pushCommand(MAX7219_REG_SHUTDOWN, 0x01);     // not in shutdown mode
  pushCommand(MAX7219_REG_DISPLAY_TEST, 0x00); // no display test
  pushCommand(MAX7219_REG_INTENSITY, BRIGHTNESS & 0x0f);
}  

// == Arduino Loop =============================

void loop () {
  pushCommand(MAX7219_REG_DIGIT0, 1*1<<7 | 0*1<<6 | 0*1<<5 | 0*1<<4 | 0*1<<3 | 0*1<<2 | 0*1<<1 | 0*1<<0); //Light upper left LED..
  pushCommand(MAX7219_REG_DIGIT1, 0*1<<7 | 0*1<<6 | 0*1<<5 | 0*1<<4 | 0*1<<3 | 0*1<<2 | 0*1<<1 | 0*1<<0);
  pushCommand(MAX7219_REG_DIGIT2, 0*1<<7 | 0*1<<6 | 0*1<<5 | 0*1<<4 | 0*1<<3 | 0*1<<2 | 0*1<<1 | 0*1<<0);
  pushCommand(MAX7219_REG_DIGIT3, 0*1<<7 | 0*1<<6 | 0*1<<5 | 0*1<<4 | 0*1<<3 | 0*1<<2 | 0*1<<1 | 0*1<<0);
  pushCommand(MAX7219_REG_DIGIT4, 0*1<<7 | 0*1<<6 | 0*1<<5 | 0*1<<4 | 0*1<<3 | 0*1<<2 | 0*1<<1 | 0*1<<0);
  pushCommand(MAX7219_REG_DIGIT5, 0*1<<7 | 0*1<<6 | 0*1<<5 | 0*1<<4 | 0*1<<3 | 0*1<<2 | 0*1<<1 | 0*1<<0);
  pushCommand(MAX7219_REG_DIGIT6, 0*1<<7 | 0*1<<6 | 0*1<<5 | 0*1<<4 | 0*1<<3 | 0*1<<2 | 0*1<<1 | 0*1<<0);
  pushCommand(MAX7219_REG_DIGIT7, 0*1<<7 | 0*1<<6 | 0*1<<5 | 0*1<<4 | 0*1<<3 | 0*1<<2 | 0*1<<1 | 1*1<<0); //..and lower right LED
}
