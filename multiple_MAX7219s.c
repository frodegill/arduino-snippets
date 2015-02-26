//This snippet displays the numbers "1", "2" and "3" on 3 chained MAX7219 LED matrix displays.
//
//Connect VCC to 5V, GND to GND, and DIN, CS and CLK to GPIO pins (I used digital pin 2, 3 and 4)
//
//Chain the LED matrixes by connecting the 5 LED matrix output pins to the next LED matrix input pins.
//The first data sent will end up on the display farthest away from the Arduino.

const int DIN_PIN       = 2; //Pin used to connected Arduino to DIN on the MAX7219
const int CS_PIN        = 3; //Pin used to connected Arduino to CS on the MAX7219
const int CLK_PIN       = 4; //Pin used to connected Arduino to CLK on the MAX7219

const int BRIGHTNESS    = 8; // 0-15

const int MATRIX_COUNT  = 3; //The amount of LED matrixes you have chained..

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

void pushCommandToMatrixes(byte command, byte* data) {
  int i;
  digitalWrite(CS_PIN, LOW);
  for (i=0; i<MATRIX_COUNT; i++) {
    putByte(command);
    putByte(data[i]);
  }
  digitalWrite(CS_PIN, HIGH); 
}

void pushSingleCommandToAll(byte command, byte data) {
  int i;
  digitalWrite(CS_PIN, LOW);     
  for (i=0; i<MATRIX_COUNT; i++) {
    putByte(command);
    putByte(data);
  }
  digitalWrite(CS_PIN, HIGH);
}

// == Arduino Setup ============================

void setup () {
  pinMode(DIN_PIN, OUTPUT);
  pinMode(CS_PIN, OUTPUT);
  pinMode(CLK_PIN, OUTPUT);

  //initialize of the MAX7219
  digitalWrite(CLK_PIN, HIGH);  
  pushSingleCommandToAll(MAX7219_REG_SCAN_LIMIT, 0x07);      
  pushSingleCommandToAll(MAX7219_REG_DECODE_MODE, 0x00);  // using an led matrix (not digits)
  pushSingleCommandToAll(MAX7219_REG_SHUTDOWN, 0x01);     // not in shutdown mode
  pushSingleCommandToAll(MAX7219_REG_DISPLAY_TEST, 0x00); // no display test
  pushSingleCommandToAll(MAX7219_REG_INTENSITY, BRIGHTNESS & 0x0f);
}  

// == Arduino Loop =============================

void loop () {
  byte display_data[8][MATRIX_COUNT];
  display_data[0][0] = B00011000; //"1"
  display_data[1][0] = B00011000;
  display_data[2][0] = B01111000;
  display_data[3][0] = B00011000;
  display_data[4][0] = B00011000;
  display_data[5][0] = B00011000;
  display_data[6][0] = B00011000;
  display_data[7][0] = B00111100;

  display_data[0][1] = B00111100; //"2"
  display_data[1][1] = B01100110;
  display_data[2][1] = B00000110;
  display_data[3][1] = B00001100;
  display_data[4][1] = B00001000;
  display_data[5][1] = B00010010;
  display_data[6][1] = B00111110;
  display_data[7][1] = B01111110;

  display_data[0][2] = B00111100; //"3"
  display_data[1][2] = B01100110;
  display_data[2][2] = B00000110;
  display_data[3][2] = B00011100;
  display_data[4][2] = B00000100;
  display_data[5][2] = B01100110;
  display_data[6][2] = B01100110;
  display_data[7][2] = B00111100;

  pushCommandToMatrixes(MAX7219_REG_DIGIT0, display_data[0]); //Array of first lines for the 3 displays
  pushCommandToMatrixes(MAX7219_REG_DIGIT1, display_data[1]); //Array of second lines for the 3 displays
  pushCommandToMatrixes(MAX7219_REG_DIGIT2, display_data[2]);
  pushCommandToMatrixes(MAX7219_REG_DIGIT3, display_data[3]);
  pushCommandToMatrixes(MAX7219_REG_DIGIT4, display_data[4]);
  pushCommandToMatrixes(MAX7219_REG_DIGIT5, display_data[5]);
  pushCommandToMatrixes(MAX7219_REG_DIGIT6, display_data[6]);
  pushCommandToMatrixes(MAX7219_REG_DIGIT7, display_data[7]);  //Array of last lines for the 3 displays
}
