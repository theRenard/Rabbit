#include <Arduino.h>
#include <DRV8833.h>
#include <NeoPixelBus.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "avdweb_Switch.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include "Ticker.h"


/** PINS ASSIGNEMENTS **/
const uint8_t
  PIN_ANALOG_INPUT = A0,
  PIN_HEAD_BUTTON = D0,
  PIN_DFPLAYER_RX = D3,
  PIN_DFPLAYER_TX = D4,
  PIN_MOTOR_RIGHT_OPTO = D1,
  PIN_MOTOR_RIGHT_1 = D5,
  PIN_MOTOR_RIGHT_2 = D6,
  PIN_MOTOR_LEFT_OPTO = D2,
  PIN_MOTOR_LEFT_2 = D7,
  PIN_MOTOR_LEFT_1 = D8;

/** SOME CONFING CONSTANTS **/
const uint8_t
  PIXEL_COUNT = 15,
  COLOR_SATURATION = 128,
  DFPLAYER_VOLUME = 20; // 0 => 30

// MILLIS
unsigned long initial_millis;
unsigned long current_millis;
const unsigned long MQTT_DELAY = 5000;

// WIFI INSTANCE
WiFiManager wifiManager;

// MQTT INSTANCE
WiFiClient espClient;
PubSubClient MQTTclient(espClient);
#define MQTT_SERVER "homebridge"

// ARDUINO JSON INSTANCE
const size_t capacity = JSON_ARRAY_SIZE(5) + 3*JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(4) + 70;
DynamicJsonDocument MQTTSubPayload(capacity);

// SWITCH INSTANCES
// Switch headButton = Switch(PIN_HEAD_BUTTON, INPUT_PULLUP, LOW); // not working (inverted GND VCC on board)
// Switch leftMotorOpto = Switch(PIN_MOTOR_LEFT_OPTO, INPUT, HIGH); // now working (broken lcd)
// Switch rightMotorOpto = Switch(PIN_MOTOR_RIGHT_OPTO, INPUT, HIGH);

// NEOPIXEL INSTANCE
NeoPixelBus<NeoGrbFeature, NeoEsp8266Dma800KbpsMethod> strip(PIXEL_COUNT);

RgbColor red(COLOR_SATURATION, 0, 0);
RgbColor green(0, COLOR_SATURATION, 0);
RgbColor blue(0, 0, COLOR_SATURATION);
RgbColor white(COLOR_SATURATION);
RgbColor black(0);

// DRV8833 INSTANCE
DRV8833 driver = DRV8833();

// SERIAL INSTANCE
SoftwareSerial mySoftwareSerial(PIN_DFPLAYER_RX, PIN_DFPLAYER_TX); // RX, TX

// MP3 INSTANCE
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);

//* FUNCTIONS *//
//* FUNCTIONS *//
//* FUNCTIONS *//

boolean reconnect() {
  if (MQTTclient.connect("Rabbit")) {
    Serial.println("MQTT is connected...");
    MQTTclient.subscribe("iot/rabbit");
  }
  return MQTTclient.connected();
}

void MQTTcallback(char* topic, byte* payload, unsigned int length) {

  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  deserializeJson(MQTTSubPayload, payload, length);

  driver.motorAForward();
  driver.motorBForward();

  if (MQTTSubPayload.containsKey("sound")) {

    RgbColor color;

    JsonArray sound = MQTTSubPayload["sound"].as<JsonArray>();
    for(JsonVariant v : sound) {

      for (uint16_t pixel = 0; pixel < PIXEL_COUNT; pixel++) {
        if (MQTTSubPayload.containsKey("colors")) {
          JsonArray colors = MQTTSubPayload["colors"].as<JsonArray>();
          color = RgbColor(colors[0], colors[1], colors[2]);
        } else {
          color = RgbColor(random(255), random(255), random(255));
        }
        strip.SetPixelColor(pixel, color);
      }
      strip.Show();

      Serial.println(v.as<int>());
      myDFPlayer.play(v.as<int>());
      int stt = myDFPlayer.readState();
      int type = myDFPlayer.readType();
      while (stt == 529 || stt == 513 || stt == 512) {
        stt = myDFPlayer.readState();
        type = myDFPlayer.readType();
        Serial.println( stt );
        Serial.println( type );
        delay(300);
        if (type == 11) break;
      }
    }
  }

  driver.motorAStop();
  driver.motorBStop();

  for (uint16_t pixel = 0; pixel < PIXEL_COUNT; pixel++) {
      RgbColor color = RgbColor(0, 0, 0);
      strip.SetPixelColor(pixel, color);
  }
  strip.Show();

}

//* SETUP *//
//* SETUP *//
//* SETUP *//

void setup() {
  mySoftwareSerial.begin(9600);

  Serial.begin(115200);
  while (!Serial); // wait for serial attach

  Serial.println();
  Serial.println("Initializing...");
  Serial.flush();

  // this resets all the neopixels to an off state
  strip.Begin();
  strip.Show();

  Serial.println();
  Serial.println("Running...");

  // Attach a motor to the input pins:
  driver.attachMotorA(PIN_MOTOR_RIGHT_1, PIN_MOTOR_RIGHT_2);  // RIGHT_MOTOR
  driver.attachMotorB(PIN_MOTOR_LEFT_1, PIN_MOTOR_LEFT_2);    // LEFT_MOTOR

  //reset saved settings
  //wifiManager.resetSettings();

  wifiManager.autoConnect("Rabbit");
  Serial.println("Rabbit connected to WiFi :)");

  // this sets the MQTT client and its callback
  Serial.println("Setting MQTT client...");
  MQTTclient.setServer(MQTT_SERVER, 1883);
  MQTTclient.setCallback(MQTTcallback);
  initial_millis = 0;

  if (!myDFPlayer.begin(mySoftwareSerial, true, false)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true){
      delay(0); // Code to make it compatible with ESP8266 watch dog.
    }
  }
  Serial.println(F("DFPlayer Mini online."));

  myDFPlayer.volume(DFPLAYER_VOLUME);

  Serial.println("Ready!");
}

//* LOOP *//
//* LOOP *//
//* LOOP *//

void loop() {
  if (!MQTTclient.connected()) {

    current_millis = millis();
    if (current_millis - initial_millis > MQTT_DELAY) {

      Serial.println("MQTT not connected...");
      Serial.println("reconnecting...");
      initial_millis = current_millis;

      // Attempt to reconnect
      if (reconnect()) {
        initial_millis = 0;
      }
    }
  } else {
    // Client connected
    MQTTclient.loop();
  }
}
