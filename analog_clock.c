//This snippet displays an analog clock (not real-time) on a single MAX7219 LED matrix display.
//Datasheet: <URL: http://datasheets.maximintegrated.com/en/ds/MAX7219-MAX7221.pdf >
//
//Connect VCC to 5V, GND to GND, and DIN, CS and CLK to GPIO pins (I used digital pin 2, 3 and 4)

const int DIN_PIN       = 2; //Pin used to connected Arduino to DIN on the MAX7219
const int CS_PIN        = 3; //Pin used to connected Arduino to CS on the MAX7219
const int CLK_PIN       = 4; //Pin used to connected Arduino to CLK on the MAX7219

const int BRIGHTNESS    = 8; // 0-15

static const int DELAY = 10; //milliseconds to wait before redrawing

double hour_angle;
double minute_angle;
static const double DELTA_ANGLE = 5.0; //Amount to increase x every frame

byte screenbuffer[8];


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

void ClearBuffer()
{
  int i;
  for (i=0; i<8; i++)
  {
    screenbuffer[i] = 0;
  }
}

void ShowBuffer()
{
  int i;
  for (i=0; i<8; i++)
  {
    pushCommand(MAX7219_REG_DIGIT0+i, screenbuffer[i]);
  }
}

void PutPixel(int x, int y)
{
  if (x>=0 && x<8 && y>=0 && y<8)
  {
    screenbuffer[y] |= 1<<(7-x);
  }
}

double DegreesToRadians(double deg)
{
  return deg/180.0*3.14159265;
}

void Draw(double angle, int length)
{
  double cx = cos(DegreesToRadians(angle));
  double sy = sin(DegreesToRadians(angle));
  int i;
  for (i=1; i<=length; i++)
  {
    PutPixel(4.0 + i*cx, 4.0 + i*sy);
  }
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

  hour_angle = 0.0;
  minute_angle = 0.0;
}  

// == Arduino Loop =============================

void loop () {
  minute_angle += DELTA_ANGLE;
  hour_angle += DELTA_ANGLE/12.0;

  ClearBuffer();
  Draw(minute_angle, 4);
  Draw(hour_angle, 3);
  ShowBuffer();

  delay(DELAY);
}
