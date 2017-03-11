#include <ESP8266WiFi.h>

static const char* SSID     = "gamejam_arduino";
static const char* PASSWORD = "SecurityByObscurity";

static const int LED_PIN = D0;

static const int PORT = 8080;
WiFiServer server(PORT);

bool led_on = false;


void toggleLed()
{
  led_on = !led_on;
  digitalWrite(LED_PIN, led_on ? HIGH : LOW);
}

void setup()
{
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  { //Blink while connecting
    toggleLed();
    delay(100);
    toggleLed();
    delay(100);
  }

  server.begin();
}

void loop()
{
  WiFiClient client = server.available(); // wait for a client (web browser) to connect

  if (client && client.connected())
  {
    toggleLed();
    client.print("Led is ");
    client.println(led_on ? "on " : "off");
  }

}

