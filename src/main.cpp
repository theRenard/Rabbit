#include <Arduino.h>
#include <DRV8833.h>
#include <NeoPixelBus.h>
#include "avdweb_Switch.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

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

// SWITCH INSTANCES
Switch headButton = Switch(PIN_HEAD_BUTTON, INPUT_PULLUP, LOW); // not working (inverted GND VCC on board)
Switch leftMotorOpto = Switch(PIN_MOTOR_LEFT_OPTO, INPUT, HIGH); // now working (broken lcd)
Switch rightMotorOpto = Switch(PIN_MOTOR_RIGHT_OPTO, INPUT, HIGH);

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

// ** SETUP
// ** SETUP
// ** SETUP

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


  if (!myDFPlayer.begin(mySoftwareSerial, false)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true){
      delay(0); // Code to compatible with ESP8266 watch dog.
    }
  }
  Serial.println(F("DFPlayer Mini online."));

  myDFPlayer.volume(DFPLAYER_VOLUME);
  myDFPlayer.play(1);  //Play the first mp3

  Serial.println("Ready!");

}

void loop() {
  // put your main code here, to run repeatedly:

  headButton.poll();
  leftMotorOpto.poll();
  rightMotorOpto.poll();
  if(headButton.longPress()) Serial.println("headButton longPress");
  if(headButton.doubleClick()) Serial.println("headButton doubleClick a");
  if(headButton.singleClick()) Serial.println("headButton singleClick");
  if(leftMotorOpto.longPress()) Serial.println("leftMotorOpto longPress");
  if(leftMotorOpto.doubleClick()) Serial.println("leftMotorOpto doubleClick b");
  if(leftMotorOpto.singleClick()) Serial.println("leftMotorOpto singleClick");
  if(rightMotorOpto.longPress()) Serial.println("rightMotorOpto longPress c");
  if(rightMotorOpto.doubleClick()) Serial.println("rightMotorOpto doubleClick c");
  if(rightMotorOpto.singleClick()) Serial.println("rightMotorOpto singleClick c");

  static unsigned long timer = millis();

  if (millis() - timer > 500) {
    timer = millis();
    myDFPlayer.next();  //Play next mp3 every 3 second.
  }

  if (myDFPlayer.available()) {
    printDetail(myDFPlayer.readType(), myDFPlayer.read()); //Print the detail message from DFPlayer to handle different errors and states.
  }

  // Serial.println("Forward:");
  // // Put the motor in forward:
    driver.motorAForward();
    driver.motorBForward();

    // Serial.println("Colors R, G, B, W...");

    // int adcvalue = 0;
    // adcvalue = analogRead(PIN_ANALOG_INPUT);
    // Serial.print("VALUE: "+String(adcvalue));

    // // set the colors,
    // // if they don't match in order, you need to use NeoGrbFeature feature
    // strip.SetPixelColor(0, red);
    // strip.SetPixelColor(1, green);
    // strip.SetPixelColor(2, blue);
    // strip.SetPixelColor(3, white);
    // strip.SetPixelColor(4, red);
    // strip.SetPixelColor(5, green);
    // strip.SetPixelColor(6, blue);
    // strip.SetPixelColor(7, white);
    // strip.SetPixelColor(8, red);
    // strip.SetPixelColor(9, green);
    // strip.SetPixelColor(10, blue);
    // strip.SetPixelColor(11, white);
    // strip.SetPixelColor(12, green);
    // strip.SetPixelColor(13, blue);
    // strip.SetPixelColor(14, white);
    // // the following line demonstrates rgbw color support
    // // if the NeoPixels are rgbw types the following line will compile
    // // if the NeoPixels are anything else, the following line will give an error
    // //strip.SetPixelColor(3, RgbwColor(COLOR_SATURATION));
    // strip.Show();


  // Wait to see the effect:
  // delay(1000);

  // // Pause the motor for stability:
  // driver.motorAStop();
  // driver.motorBStop();

  // Serial.println("Reverse:");
  // // Put the motor in reverse:
  // driver.motorAReverse();
  // driver.motorBReverse();


  //   Serial.println("Off ...");

  //   // turn off the pixels
  //   strip.SetPixelColor(0, black);
  //   strip.SetPixelColor(1, black);
  //   strip.SetPixelColor(2, black);
  //   strip.SetPixelColor(3, black);
  //   strip.SetPixelColor(4, black);
  //   strip.SetPixelColor(5, black);
  //   strip.SetPixelColor(6, black);
  //   strip.SetPixelColor(7, black);
  //   strip.SetPixelColor(8, black);
  //   strip.SetPixelColor(9, black);
  //   strip.SetPixelColor(10, black);
  //   strip.SetPixelColor(11, black);
  //   strip.SetPixelColor(12, black);
  //   strip.SetPixelColor(13, black);
  //   strip.SetPixelColor(14, black);
  //   strip.Show();



  // // Wait to see the effect:
  // delay(5000);


  //   Serial.println("HSL Colors R, G, B, W...");

  //   // set the colors,
  //   // if they don't match in order, you may need to use NeoGrbFeature feature
  //   strip.SetPixelColor(0, hslRed);
  //   strip.SetPixelColor(1, hslGreen);
  //   strip.SetPixelColor(2, hslBlue);
  //   strip.SetPixelColor(3, hslWhite);
  //   strip.Show();


  // Serial.println("Stop:");
  // // Stop the motor:
  // driver.motorAStop();
  // driver.motorBStop();
  // // Wait to see the effect:
  // delay(5000);

}


void printDetail(uint8_t type, int value){
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerUSBInserted:
      Serial.println("USB Inserted!");
      break;
    case DFPlayerUSBRemoved:
      Serial.println("USB Removed!");
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }

}

