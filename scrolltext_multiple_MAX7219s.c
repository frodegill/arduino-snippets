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

const byte g_text[] = "This is a scrolltext spanning 3 MAX7219 LED matrix displays! ";
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
B00000000, /* (space) */
B00000000,
B00000000,
B00000000,
B00000000,
B00000000,
B00000000,
B00000000,

B00001000, /* ! */
B00001000,
B00001000,
B00001000,
B00000000,
B00001000,
B00000000,
B00000000,

B00101000, /* " */
B00101000,
B00000000,
B00000000,
B00000000,
B00000000,
B00000000,
B00000000,

B00000000, /* # */
B00101000,
B01111100,
B00101000,
B01111100,
B00101000,
B00000000,
B00000000,

B00001000, /* $ */
B00011110,
B00101000,
B00011100,
B00001010,
B00111100,
B00001000,
B00000000,

B01100000, /* % */
B10010100,
B01101000,
B00010110,
B00101001,
B00000110,
B00000000,
B00000000,

B00011100, /* & */
B00100000,
B00100000,
B00011001,
B00100110,
B00011001,
B00000000,
B00000000,

B00001000, /* ' */
B00001000,
B00000000,
B00000000,
B00000000,
B00000000,
B00000000,
B00000000,

B00001000, /* ( */
B00010000,
B00100000,
B00100000,
B00010000,
B00001000,
B00000000,
B00000000,

B00010000, /* ) */
B00001000,
B00000100,
B00000100,
B00001000,
B00010000,
B00000000,
B00000000,

B00101010, /* * */
B00011100,
B00111110,
B00011100,
B00101010,
B00000000,
B00000000,
B00000000,

B00000000, /* + */
B00001000,
B00001000,
B00111110,
B00001000,
B00001000,
B00000000,
B00000000,

B00000000, /* , */
B00000000,
B00000000,
B00000000,
B00000000,
B00001000,
B00010000,
B00000000,

B00000000, /* - */
B00000000,
B00000000,
B00111100,
B00000000,
B00000000,
B00000000,
B00000000,

B00000000, /* . */
B00000000,
B00000000,
B00000000,
B00000000,
B00001000,
B00000000,
B00000000,

B00000010, /* / */
B00000100,
B00001000,
B00010000,
B00100000,
B01000000,
B00000000,
B00000000,

B00011000, /* 0 */
B00100100,
B01000010,
B01000010,
B00100100,
B00011000,
B00000000,
B00000000,

B00001000, /* 1 */
B00011000,
B00001000,
B00001000,
B00001000,
B00011100,
B00000000,
B00000000,

B00111100, /* 2 */
B01000010,
B00000100,
B00011000,
B00100000,
B01111110,
B00000000,
B00000000,

B00111100, /* 3 */
B01000010,
B00000100,
B00011000,
B01000010,
B00111100,
B00000000,
B00000000,

B00001000, /* 4 */
B00011000,
B00101000,
B01001000,
B01111100,
B00001000,
B00000000,
B00000000,

B01111110, /* 5 */
B01000000,
B01111100,
B00000010,
B01000010,
B00111100,
B00000000,
B00000000,

B00111100, /* 6 */
B01000000,
B01111100,
B01000010,
B01000010,
B00111100,
B00000000,
B00000000,

B01111110, /* 7 */
B00000100,
B00001000,
B00010000,
B00100000,
B01000000,
B00000000,
B00000000,

B00111100, /* 8 */
B01000010,
B00111100,
B01000010,
B01000010,
B00111100,
B00000000,
B00000000,

B00111100, /* 9 */
B01000010,
B01000010,
B00111110,
B00000010,
B00111100,
B00000000,
B00000000,

B00000000, /* : */
B00000000,
B00001000,
B00000000,
B00000000,
B00001000,
B00000000,
B00000000,

B00000000, /* ; */
B00000000,
B00001000,
B00000000,
B00000000,
B00001000,
B00010000,
B00000000,

B00000000, /* < */
B00000110,
B00011000,
B01100000,
B00011000,
B00000110,
B00000000,
B00000000,

B00000000, /* = */
B00000000,
B01111110,
B00000000,
B01111110,
B00000000,
B00000000,
B00000000,

B00000000, /* > */
B01100000,
B00011000,
B00000110,
B00011000,
B01100000,
B00000000,
B00000000,

B00111000, /* ? */
B01000100,
B00000100,
B00011000,
B00000000,
B00010000,
B00000000,
B00000000,

B00000000, /* @ */
B00111100,
B01000100,
B10011100,
B10010100,
B01011100,
B00100000,
B00011100,

B00011000, /* A */
B00011000,
B00100100,
B00111100,
B01000010,
B01000010,
B00000000,
B00000000,

B01111000, /* B */
B01000100,
B01111000,
B01000100,
B01000100,
B01111000,
B00000000,
B00000000,

B00111000, /* C */
B01000100,
B10000000,
B10000000,
B01000100,
B00111000,
B00000000,
B00000000,

B01111000, /* D */
B01000100,
B01000100,
B01000100,
B01000100,
B01111000,
B00000000,
B00000000,

B01111100, /* E */
B01000000,
B01111000,
B01000000,
B01000000,
B01111100,
B00000000,
B00000000,

B01111100, /* F */
B01000000,
B01111000,
B01000000,
B01000000,
B01000000,
B00000000,
B00000000,

B00111000, /* G */
B01000100,
B10000000,
B10011100,
B01000100,
B00111000,
B00000000,
B00000000,

B01000010, /* H */
B01000010,
B01111110,
B01000010,
B01000010,
B01000010,
B00000000,
B00000000,

B00111110, /* I */
B00001000,
B00001000,
B00001000,
B00001000,
B00111110,
B00000000,
B00000000,

B00011100, /* J */
B00000100,
B00000100,
B00000100,
B01000100,
B00111000,
B00000000,
B00000000,

B01000100, /* K */
B01001000,
B01010000,
B01110000,
B01001000,
B01000100,
B00000000,
B00000000,

B01000000, /* L */
B01000000,
B01000000,
B01000000,
B01000000,
B01111110,
B00000000,
B00000000,

B01000001, /* M */
B01100011,
B01010101,
B01001001,
B01000001,
B01000001,
B00000000,
B00000000,

B01000010, /* N */
B01100010,
B01010010,
B01001010,
B01000110,
B01000010,
B00000000,
B00000000,

B00011100, /* O */
B00100010,
B00100010,
B00100010,
B00100010,
B00011100,
B00000000,
B00000000,

B01111000, /* P */
B01000100,
B01111000,
B01000000,
B01000000,
B01000000,
B00000000,
B00000000,

B00011100, /* Q */
B00100010,
B00100010,
B00100010,
B00100010,
B00011100,
B00000010,
B00000000,

B01111000, /* R */
B01000100,
B01111000,
B01010000,
B01001000,
B01000100,
B00000000,
B00000000,

B00011100, /* S */
B00100010,
B00010000,
B00001100,
B00100010,
B00011100,
B00000000,
B00000000,

B01111111, /* T */
B00001000,
B00001000,
B00001000,
B00001000,
B00001000,
B00000000,
B00000000,

B01000010, /* U */
B01000010,
B01000010,
B01000010,
B01000010,
B00111100,
B00000000,
B00000000,

B10000001, /* V */
B01000010,
B01000010,
B00100100,
B00100100,
B00011000,
B00000000,
B00000000,

B01000001, /* W */
B01000001,
B01001001,
B01010101,
B01100011,
B01000001,
B00000000,
B00000000,

B01000010, /* X */
B00100100,
B00011000,
B00011000,
B00100100,
B01000010,
B00000000,
B00000000,

B01000001, /* Y */
B00100010,
B00010100,
B00001000,
B00001000,
B00001000,
B00000000,
B00000000,

B01111110, /* Z */
B00000100,
B00001000,
B00010000,
B00100000,
B01111110,
B00000000,
B00000000,

B00011111, /* Æ */
B00011000,
B00100100,
B00111111,
B01000010,
B01000011,
B00000000,
B00000000,

B00011110, /* Ø */
B00100110,
B00101010,
B00101010,
B00110010,
B00111100,
B00000000,
B00000000,

B00011000, /* Å */
B00000000,
B00011000,
B00100100,
B00111100,
B01000010,
B00000000,
B00000000,

B00111000, /* [ */
B00100000,
B00100000,
B00100000,
B00100000,
B00111000,
B00000000,
B00000000,

B01000000, /* \ */
B00100000,
B00010000,
B00001000,
B00000100,
B00000010,
B00000000,
B00000000,

B00111000, /* ] */
B00001000,
B00001000,
B00001000,
B00001000,
B00111000,
B00000000,
B00000000,

B00010000, /* ^ */
B00101000,
B00000000,
B00000000,
B00000000,
B00000000,
B00000000,
B00000000,

B00000000, /* _ */
B00000000,
B00000000,
B00000000,
B00000000,
B01111110,
B00000000,
B00000000,

B00010000, /* ` */
B00001000,
B00000000,
B00000000,
B00000000,
B00000000,
B00000000,
B00000000,

B00000000, /* a */
B00111100,
B00000010,
B00111110,
B01000110,
B00111010,
B00000000,
B00000000,

B01000000, /* b */
B01000000,
B01111100,
B01000010,
B01100010,
B01011100,
B00000000,
B00000000,

B00000000, /* c */
B00000000,
B00011100,
B00100000,
B00100000,
B00011100,
B00000000,
B00000000,

B00000010, /* d */
B00000010,
B00111110,
B01000010,
B01000110,
B00111010,
B00000000,
B00000000,

B00000000, /* e */
B00111100,
B01000010,
B01111110,
B01000000,
B00111100,
B00000000,
B00000000,

B00000000, /* f */
B00011000,
B00010000,
B00111000,
B00010000,
B00010000,
B00000000,
B00000000,

B00000000, /* g */
B00000000,
B00110100,
B01001100,
B01000100,
B00110100,
B00000100,
B00111000,

B00100000, /* h */
B00100000,
B00111000,
B00100100,
B00100100,
B00100100,
B00000000,
B00000000,

B00001000, /* i */
B00000000,
B00001000,
B00001000,
B00001000,
B00001000,
B00000000,
B00000000,

B00001000, /* j */
B00000000,
B00011000,
B00001000,
B00001000,
B00001000,
B00001000,
B01110000,

B00100000, /* k */
B00100000,
B00100100,
B00101000,
B00111000,
B00100100,
B00000000,
B00000000,

B00010000, /* l */
B00010000,
B00010000,
B00010000,
B00010000,
B00011000,
B00000000,
B00000000,

B00000000, /* m */
B00000000,
B01100110,
B01011010,
B01000010,
B01000010,
B00000000,
B00000000,

B00000000, /* n */
B00000000,
B00101110,
B00110010,
B00100010,
B00100010,
B00000000,
B00000000,

B00000000, /* o */
B00000000,
B00111100,
B01000010,
B01000010,
B00111100,
B00000000,
B00000000,

B00000000, /* p */
B00000000,
B01011100,
B01100010,
B01000010,
B01111100,
B01000000,
B01000000,

B00000000, /* q */
B00000000,
B00111010,
B01000110,
B01000010,
B00111110,
B00000010,
B00000010,

B00000000, /* r */
B00000000,
B00101100,
B00110010,
B00100000,
B00100000,
B00000000,
B00000000,

B00000000, /* s */
B00011100,
B00100000,
B00011000,
B00000100,
B00111000,
B00000000,
B00000000,

B00000000, /* t */
B00010000,
B00111100,
B00010000,
B00010000,
B00011000,
B00000000,
B00000000,

B00000000, /* u */
B00000000,
B00100010,
B00100010,
B00100110,
B00011010,
B00000000,
B00000000,

B00000000, /* v */
B00000000,
B01000010,
B01000010,
B00100100,
B00011000,
B00000000,
B00000000,

B00000000, /* w */
B00000000,
B10000001,
B10000001,
B01011010,
B01100110,
B00000000,
B00000000,

B00000000, /* x */
B00000000,
B01000010,
B00100100,
B00011000,
B01100110,
B00000000,
B00000000,

B00000000, /* y */
B00000000,
B01000010,
B00100010,
B00010100,
B00001000,
B00010000,
B01100000,

B00000000, /* z */
B00000000,
B00111100,
B00001000,
B00010000,
B00111100,
B00000000,
B00000000,

B00000000, /* æ */
B00111111,
B00000101,
B00111111,
B01000110,
B00111011,
B00000000,
B00000000,

B00000000, /* ø */
B00000000,
B00111110,
B01001010,
B01010010,
B01111100,
B00000000,
B00000000,

B00011000, /* å */
B00111100,
B00000010,
B00111110,
B01000110,
B00111010,
B00000000,
B00000000,

B00011100, /* { */
B00010000,
B00110000,
B00110000,
B00010000,
B00011100,
B00000000,
B00000000,

B00001000, /* | */
B00001000,
B00001000,
B00001000,
B00001000,
B00001000,
B00001000,
B00000000,

B00111000, /* } */
B00001000,
B00001100,
B00001100,
B00001000,
B00111000,
B00000000,
B00000000,

B00000000, /* ~ */
B00000000,
B00000000,
B00110010,
B01001100,
B00000000,
B00000000,
B00000000
};


//Displayable US-ASCII characters, and upper- and lowercase Norwegian special characters Æ, Ø and Å (for a very simple version of iso-8859-1)
const byte g_alphabeth[] = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ\xC6\xD8\xC5[\\]^_`abcdefghijklmnopqrstuvwxyz\xE6\xF8\xE5{|}~";
size_t g_alphabeth_length; //This variable is automatically set in setup();

size_t g_scroll_pos;//This variable is automatically set in setup();
size_t g_scroll_char_pos;//This variable is automatically set in setup();

//Helper-function returning index in g_alphabeth for a given character (or 0 (index for [SPACE]) if not found) 
size_t findCharPosition(byte ch)
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

  g_scroll_pos = 0;
  g_scroll_char_pos = 0;
}  

// == Arduino Loop =============================

void loop ()
{
  int i;
  size_t c_index[MATRIX_COUNT+1]; //On a 8x8 display and a 8x8 fixed-width font, while scrolling you can see maximum (MATRIX_COUNT+1) characters
  for (i=0; i<(MATRIX_COUNT+1); i++)
  {
    c_index[i] = findCharPosition(g_text[(g_scroll_pos + i) % g_text_length]); //Find index for the (MATRIX+1) visible characters from g_scroll_pos (with wrapping at end of text)
  }

  byte display_data[MATRIX_COUNT]; //Displaydata for all displays for a single row
  byte font_data1, font_data2; //Data for the two characters (one that scrolls out and one that scrolls into the display)  that can be visible in a given display 
  int row;
  
  //Lets calculate!
  for (row=0; row<8; row++) //Row by row..
  {
    for (i=0; i<MATRIX_COUNT; i++) //Display by display..
    {
      font_data1 = g_font[c_index[i]*8 + row]; //The actual font bitmap for this row and the character scrolling out of the display
      font_data2 = g_font[c_index[i+1]*8 + row]; //The actual font bitmap for this row and the character scrolling into the display
      display_data[i] = ((font_data1<< g_scroll_char_pos) | (font_data2>>(8-g_scroll_char_pos))) & 0xFF; //Calculate display_data according to the scroll-position
    }
    pushCommandToMatrixes(MAX7219_REG_DIGIT0+row, display_data); //Send this row to the displays
  }

  g_scroll_char_pos = (g_scroll_char_pos + 1) % 8; //Scroll one pixel
  if (g_scroll_char_pos == 0) //If we have scrolled a complete character, jump one position in the text and keep on scrolling
  {
    g_scroll_pos = (g_scroll_pos + 1) % g_text_length;
  }
 
  delay(SCROLLDELAY);
}
