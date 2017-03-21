static const int RED_PIN   = D1;
static const int GREEN_PIN = D2;
static const int BLUE_PIN  = D3;

static const int DELAY = 1; //ms

static const int MAX_COUNTER = 1023;
int counter = MAX_COUNTER;

int red_value;
int green_value;
int blue_value;

int red_direction;
int green_direction;
int blue_direction;


void setup()
{
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
}

void loop()
{
  counter++;
  if (counter > MAX_COUNTER)
  {
    counter = 0;
    red_direction = (random(2)==0) ? 0 : ((red_value==0) ? 1 : -1); //-1=down, 0=stay, 1=up
    green_direction = (random(2)==0) ? 0 : ((green_value==0) ? 1 : -1);
    blue_direction = (random(2)==0) ? 0 : ((blue_value==0) ? 1 : -1);
  }

  red_value += red_direction;
  green_value += green_direction;
  blue_value += blue_direction;
  
  analogWrite(RED_PIN, red_value);
  analogWrite(GREEN_PIN, green_value);
  analogWrite(BLUE_PIN, blue_value);

  delay(DELAY);
}

