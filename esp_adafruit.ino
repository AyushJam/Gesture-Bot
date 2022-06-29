/*------------------------------------------------------
The Gesture Controlled Bot Project
iBot Club, CFI IIT Madras
  - the below code runs on ESP32
  - it first connects to WiFi
  - then connects to Adafruit IO using MQTT
  - receives data
-------------------------------------------------------*/

#include <WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#define WLAN_SSID "ayush"
#define WLAN_PASS "00000000"

#define AIO_SERVER "io.adafruit.com"
#define AIO_SERVERPORT 1883
#define AIO_USERNAME "AyushJam"
#define AIO_KEY "<your aio key here>"

int output = 2;
WiFiClient client; // Create an ESP8266 WiFiClient class to connect to the MQTT server.

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY); // Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.

Adafruit_MQTT_Subscribe ges_Control = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/gesturebot");

void MQTT_connect();

void setup()
{
  Serial.begin(115200);
  delay(10);
  pinMode(2, OUTPUT);
  // Connect to WiFi access point.
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);
  WiFi.begin(WLAN_SSID, WLAN_PASS);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println(".");
  }

  Serial.println();
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  mqtt.subscribe(&ges_Control);
}

uint32_t x = 0;

void loop()
{
  MQTT_connect();
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000)))
  {
    if (subscription == &ges_Control)
    {
      Serial.print(F("Finger count: "));
      Serial.println((char *)ges_Control.lastread);

      /*
      switch ((int)ges_Control.lastread)
      ​{
          case 1:
            // straight
            break;

          case 2:
            // turn r
            break;

          case 3:
            // turn l
            break;

          case 4:
            // reverse

          default:
            // stop
            continue;
      }
      */
    }
  }
}

void MQTT_connect()
{
  int8_t ret; // byte datatype
  // Stop if already connected.
  if (mqtt.connected())
  {
    return;
  }

  Serial.print("Connecting to MQTT... ");
  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0)
  { // connect will return 0 for connected
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000); // wait 5 seconds
    retries--;

    if (retries == 0)
    {
      // basically die and wait for WDT to reset me
      while (1)
        ;
    }
  }

  Serial.println("MQTT Connected!");
}
