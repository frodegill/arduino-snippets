//This snippet is GPL (as it includes a GPL font found on Internet), and by default displays a scrolltext on 3 chained MAX7219 LED matrix displays.
//
//Connect VCC to 5V, GND to GND, and DIN, CS and CLK to GPIO pins (I used digital pin 2, 3 and 4)
//
//Chain the LED matrixes by connecting the 5 LED matrix output pins to the next LED matrix input pins.
//The first data sent will end up on the display farthest away from the Arduino.

const int DIN_PIN       = 2; //Pin used to connected Arduino to DIN on the MAX7219
const int CS_PIN        = 3; //Pin used to connected Arduino to CS on the MAX7219
const int CLK_PIN       = 4; //Pin used to connected Arduino to CLK on the MAX7219

const int BRIGHTNESS    = 8; // 0-15
const int SCROLLDELAY   = 75; //Higher value gives slower scrolling

const int MATRIX_COUNT  = 3; //The amount of LED matrixes you have chained..
byte g_screenbuffer[8][MATRIX_COUNT];

const byte g_text[] = "This is a scrolltext spanning 3 MAX7219 LED matrix displays!  ";
size_t g_text_length; //This variable is automatically set in setup();


/************************************************************************
* font.c
* Copyright (C) Lisa Milne 2014 <lisa@ltmnet.com>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
* See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>
************************************************************************/
/* the values in this array are a 8x8 bitmap font for ascii characters */

static byte g_font[] = {
4,        /* (space)  */
B00000000,
B00000000,
B00000000,
B00000000,

1,        /* ! */
B00101111,

3,        /* " */
B00000011,
B00000000,
B00000011,

5,        /* # */
B00010100,
B00111110,
B00010100,
B00111110,
B00010100,

5,        /* $ */
B00100100,
B00101010,
B01111111,
B00101010,
B00010010,

8,        /* % */
B00000010,
B00000101,
B00010101,
B00001010,
B00010100,
B00101010,
B00101000,
B00010000,

6,        /* & */
B00010110,
B00101001,
B00101001,
B00010001,
B00010000,
B00101000,

1,        /* ' */
B00000011,

3,        /* ( */
B00001100,
B00010010,
B00100001,

3,        /* ) */
B00100001,
B00010010,
B00001100,

5,        /* * */
B00010101,
B00001110,
B00011111,
B00001110,
B00010101,

5,        /* + */
B00001000,
B00001000,
B00111110,
B00001000,
B00001000,

2,        /* , */
B01000000,
B00100000,

4,        /* - */
B00001000,
B00001000,
B00001000,
B00001000,

1,        /* . */
B00100000,

6,        /* / */
B00100000,
B00010000,
B00001000,
B00000100,
B00000010,
B00000001,

6,        /* 0 */
B00001100,
B00010010,
B00100001,
B00100001,
B00010010,
B00001100,

3,        /* 1 */
B00100010,
B00111111,
B00100000,

6,        /* 2 */
B00100010,
B00110001,
B00101001,
B00101001,
B00100101,
B00100010,

6,        /* 3 */
B00010010,
B00100001,
B00101001,
B00101001,
B00100101,
B00010010,

5,        /* 4 */
B00011000,
B00010100,
B00010010,
B00111111,
B00010000,

6,        /* 5 */
B00010111,
B00100101,
B00100101,
B00100101,
B00100101,
B00011001,

6,        /* 6 */
B00011110,
B00100101,
B00100101,
B00100101,
B00100101,
B00011000,

6,        /* 7 */
B00100001,
B00010001,
B00001001,
B00000101,
B00000011,
B00000001,

6,        /* 8 */
B00011010,
B00100101,
B00100101,
B00100101,
B00100101,
B00011010,

6,        /* 9 */
B00000110,
B00101001,
B00101001,
B00101001,
B00101001,
B00011110,

1,        /* : */
B00100100,

2,        /* ; */
B01000000,
B00100100,

6,        /* < */
B00001000,
B00001000,
B00010100,
B00010100,
B00100010,
B00100010,

6,        /* = */
B00010100,
B00010100,
B00010100,
B00010100,
B00010100,
B00010100,

6,        /* > */
B00100010,
B00100010,
B00010100,
B00010100,
B00001000,
B00001000,

5,        /* ? */
B00000010,
B00000001,
B00101001,
B00001001,
B00000110,

6,        /* @ */
B00011000,
B00100100,
B01000010,
B10111010,
B10101010,
B10111110,

6,        /* A */
B00110000,
B00001100,
B00001011,
B00001011,
B00001100,
B00110000,

5,        /* B */
B00111111,
B00100101,
B00100101,
B00100101,
B00011010,

6,        /* C */
B00001100,
B00010010,
B00100001,
B00100001,
B00100001,
B00010010,

5,        /* D */
B00111111,
B00100001,
B00100001,
B00100001,
B00011110,

5,        /* E */
B00111111,
B00100101,
B00100101,
B00100101,
B00100001,

5,        /* F */
B00111111,
B00000101,
B00000101,
B00000101,
B00000001,

6,        /* G */
B00001100,
B00010010,
B00100001,
B00101001,
B00101001,
B00011010,

6,        /* H */
B00111111,
B00000100,
B00000100,
B00000100,
B00000100,
B00111111,

5,        /* I */
B00100001,
B00100001,
B00111111,
B00100001,
B00100001,

5,        /* J */
B00010000,
B00100000,
B00100001,
B00100001,
B00011111,

5,        /* K */
B00111111,
B00001000,
B00001100,
B00010010,
B00100001,

6,        /* L */
B00111111,
B00100000,
B00100000,
B00100000,
B00100000,
B00100000,

7,        /* M */
B00111111,
B00000010,
B00000100,
B00001000,
B00000100,
B00000010,
B00111111,

6,        /* N */
B00111111,
B00000010,
B00000100,
B00001000,
B00010000,
B00111111,

5,        /* O */
B00011110,
B00100001,
B00100001,
B00100001,
B00011110,

5,        /* P */
B00111111,
B00000101,
B00000101,
B00000101,
B00000010,

5,        /* Q */
B00011110,
B00100001,
B00100001,
B00100001,
B01011110,

5,        /* R */
B00111111,
B00000101,
B00001101,
B00010101,
B00100010,

5,        /* S */
B00010010,
B00100101,
B00101001,
B00101001,
B00010010,

7,        /* T */
B00000001,
B00000001,
B00000001,
B00111111,
B00000001,
B00000001,
B00000001,

6,        /* U */
B00011111,
B00100000,
B00100000,
B00100000,
B00100000,
B00011111,

8,        /* V */
B00000001,
B00000110,
B00011000,
B00100000,
B00100000,
B00011000,
B00000110,
B00000001,

7,        /* W */
B00111111,
B00010000,
B00001000,
B00000100,
B00001000,
B00010000,
B00111111,

6,        /* X */
B00100001,
B00010010,
B00001100,
B00001100,
B00010010,
B00100001,

7,        /* Y */
B00000001,
B00000010,
B00000100,
B00111000,
B00000100,
B00000010,
B00000001,

6,        /* Z */
B00100001,
B00110001,
B00101001,
B00100101,
B00100011,
B00100001,

7,        /* Æ */
B00110000,
B00001100,
B00001011,
B00001011,
B00001101,
B00111001,
B00101001,

5,        /* Ø */
B00111110,
B00110001,
B00101101,
B00100011,
B00011111,

6,        /* Å */
B00100000,
B00011000,
B00010101,
B00010101,
B00011000,
B00100000,

3,        /* [ */
B00111111,
B00100001,
B00100001,

6,        /* \ */
B00000001,
B00000010,
B00000100,
B00001000,
B00010000,
B00100000,

3,        /* ] */
B00100001,
B00100001,
B00111111,

3,        /* ^ */
B00000010,
B00000001,
B00000010,

6,        /* _ */
B00100000,
B00100000,
B00100000,
B00100000,
B00100000,
B00100000,

2,        /* ` */
B00000001,
B00000010,

6,        /* a */
B00010000,
B00101010,
B00101010,
B00101010,
B00011010,
B00111100,

6,        /* b */
B00111111,
B00010100,
B00100100,
B00100100,
B00100100,
B00011000,

4,        /* c */
B00011000,
B00100100,
B00100100,
B00100100,

6,        /* d */
B00011000,
B00100100,
B00100100,
B00100100,
B00010100,
B00111111,

6,        /* e */
B00011100,
B00101010,
B00101010,
B00101010,
B00101010,
B00001100,

3,        /* f */
B00001000,
B00111110,
B00001010,

5,        /* g */
B00011000,
B10100100,
B10100100,
B10001000,
B01111100,

4,        /* h */
B00111111,
B00000100,
B00000100,
B00111000,

1,        /* i */
B00111101,

4,        /* j */
B10000000,
B10000000,
B10000100,
B01111101,

4,        /* k */
B00111111,
B00010000,
B00011000,
B00100100,

2,        /* l */
B00111111,
B00100000,

6,        /* m */
B00111100,
B00000100,
B00001000,
B00001000,
B00000100,
B00111100,

5,        /* n */
B00111100,
B00001000,
B00000100,
B00000100,
B00111100,

6,        /* o */
B00011000,
B00100100,
B00100100,
B00100100,
B00100100,
B00011000,

6,        /* p */
B11111100,
B00101000,
B00100100,
B00100100,
B00100100,
B00011000,

6,        /* q */
B00011000,
B00100100,
B00100100,
B00100100,
B00101000,
B11111100,

5,        /* r */
B00111100,
B00001000,
B00000100,
B00000100,
B00001000,

4,        /* s */
B00100100,
B00101010,
B00101010,
B00010010,

4,        /* t */
B00000100,
B00111110,
B00100100,
B00000100,

5,        /* u */
B00011100,
B00100000,
B00100000,
B00010000,
B00111100,

6,        /* v */
B00001100,
B00010000,
B00100000,
B00100000,
B00010000,
B00001100,

8,        /* w */
B00001100,
B00110000,
B00100000,
B00010000,
B00010000,
B00100000,
B00110000,
B00001100,

6,        /* x */
B00100100,
B00101000,
B00010000,
B00010000,
B00101000,
B00100100,

6,        /* y */
B10000100,
B10001000,
B01010000,
B00100000,
B00010000,
B00001100,

4,        /* z */
B00100100,
B00110100,
B00101100,
B00100100,

7,        /* æ */
B00010000,
B00101010,
B00101010,
B00101010,
B00011110,
B00111010,
B00101110,

6,        /* ø */
B00111000,
B00100100,
B00110100,
B00101100,
B00100100,
B00011100,

6,        /* å */
B00010000,
B00101010,
B00101011,
B00101011,
B00011010,
B00111100,

4,        /* { */
B00001100,
B00111111,
B00100001,
B00100001,

1,        /* | */
B01111111,

4,        /* } */
B00100001,
B00100001,
B00111111,
B00001100,

6,        /* ~ */
B00010000,
B00001000,
B00001000,
B00010000,
B00010000,
B00001000,
};


//Displayable US-ASCII characters, and upper- and lowercase Norwegian special characters Æ, Ø and Å (for a very simple version of iso-8859-1)
const byte g_alphabeth[] = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ\xC6\xD8\xC5[\\]^_`abcdefghijklmnopqrstuvwxyz\xE6\xF8\xE5{|}~";
size_t g_alphabeth_length; //This variable is automatically set in setup();

size_t g_scroll_char; //The current first character to display
size_t g_scroll_char_index; //The index into g_font for the current character
size_t g_scroll_char_pos; //The current scroll position for the current character

//Helper-function returning index in g_alphabeth for a given character (or 0 (index for [SPACE]) if not found) 
size_t FindCharPosition(byte ch)
{
  size_t i;
  for (i=0; i<g_alphabeth_length; i++)
  {
    if (g_alphabeth[i] == ch)
    {
      return i;
    }
  }
  return 0;
}

//Helper-function returning index in g_font given character position
size_t FindFontIndex(byte ch)
{
  size_t index=0, i;
  size_t alphabeth_pos = FindCharPosition(ch);
  for (i=0; i<alphabeth_pos; i++)
  {
    index += (g_font[index] + 1); //Step over fontlength and fontdata for current position
  }
  return index;
}

void FindNextScrollPos(size_t& scroll_char, size_t& scroll_char_index, size_t& scroll_char_pos)
{
  scroll_char_pos++;
  if (scroll_char_pos > g_font[scroll_char_index])
  {
    scroll_char = (scroll_char + 1) % g_text_length;
    scroll_char_index = FindFontIndex(g_text[scroll_char]);
    scroll_char_pos = 0;
  }
}

void PaintText(size_t scroll_char, size_t scroll_char_index, size_t scroll_char_pos)
{
  int i;
  for (i=0; i<8*MATRIX_COUNT; i++)
  {
    if (scroll_char_pos < g_font[scroll_char_index])
    {
      g_screenbuffer[i%8][i/8] = g_font[scroll_char_index + 1 + scroll_char_pos];
    }
    else
    {
      g_screenbuffer[i%8][i/8] = 0;
    }

    FindNextScrollPos(scroll_char, scroll_char_index, scroll_char_pos);
  }
}

// == MAX7219 Commands =========================

const byte MAX7219_REG_DIGIT0       = 0x01;
//const byte MAX7219_REG_DIGIT1       = 0x02; //Not used, we just offset MAX7219_REG_DIGIT0
//const byte MAX7219_REG_DIGIT2       = 0x03;
//const byte MAX7219_REG_DIGIT3       = 0x04;
//const byte MAX7219_REG_DIGIT4       = 0x05;
//const byte MAX7219_REG_DIGIT5       = 0x06;
//const byte MAX7219_REG_DIGIT6       = 0x07;
//const byte MAX7219_REG_DIGIT7       = 0x08;
const byte MAX7219_REG_DECODE_MODE  = 0x09;
const byte MAX7219_REG_INTENSITY    = 0x0a;
const byte MAX7219_REG_SCAN_LIMIT   = 0x0b;
const byte MAX7219_REG_SHUTDOWN     = 0x0c;
const byte MAX7219_REG_DISPLAY_TEST = 0x0f;



void putByte(const byte data)
{
  byte i = 8;
  while (i-- > 0)
  {
    digitalWrite(CLK_PIN, LOW);
    digitalWrite(DIN_PIN, (data & (1<<i)) ? HIGH : LOW);
    digitalWrite(CLK_PIN, HIGH);
  }
}

void pushCommandToMatrixes(byte command, byte* data)
{
  int i;
  digitalWrite(CS_PIN, LOW);
  for (i=MATRIX_COUNT-1; i>=0; i--)
  {
    putByte(command);
    putByte(data[i]);
  }
  digitalWrite(CS_PIN, HIGH); 
}

void pushSingleCommandToAll(byte command, byte data)
{
  int i;
  digitalWrite(CS_PIN, LOW);     
  for (i=0; i<MATRIX_COUNT; i++)
  {
    putByte(command);
    putByte(data);
  }
  digitalWrite(CS_PIN, HIGH);
}

// == Arduino Setup ============================

void setup ()
{
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

  g_alphabeth_length = strlen((const char*)g_alphabeth);
  g_text_length = strlen((const char*)g_text);

  g_scroll_char = 0;
  g_scroll_char_index = FindFontIndex(g_text[g_scroll_char]);
  g_scroll_char_pos = 0;
}

// == Arduino Loop =============================

void loop ()
{
  PaintText(g_scroll_char, g_scroll_char_index, g_scroll_char_pos);

  FindNextScrollPos(g_scroll_char, g_scroll_char_index, g_scroll_char_pos);
  
  int i;
  for (i=0; i<8; i++)
  {
    pushCommandToMatrixes(MAX7219_REG_DIGIT0+i, g_screenbuffer[i]);
  }

  delay(SCROLLDELAY);
}
