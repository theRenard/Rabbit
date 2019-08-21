#include <Arduino.h>

#include <DRV8833.h>
#include <NeoPixelBus.h>
#include "avdweb_Switch.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

const uint16_t PixelCount = 15; // this example assumes 4 pixels, making it smaller will cause a failure

// Switch multiresponseButton = Switch(multiresponseButtonpin);
// Other examples of constructors
Switch multiresponseButton = Switch(D1, INPUT, LOW); // button to VCC, 10k pull-down resistor, no internal pull-up resistor, HIGH polarity
Switch multiresponseButton2 = Switch(D2, INPUT, LOW); // button to VCC, 10k pull-down resistor, no internal pull-up resistor, HIGH polarity
Switch multiresponseButton3 = Switch(D0, INPUT_PULLUP, HIGH); // button to VCC, 10k pull-down resistor, no internal pull-up resistor, HIGH polarity
// Switch multiresponseButton = Switch(multiresponseButtonpin, INPUT_PULLUP, HIGH); // debounceTime 1ms
// Switch multiresponseButton = Switch(multiresponseButtonpin, INPUT_PULLUP, LOW); // debounceTime 1ms

#define colorSaturation 128

NeoPixelBus<NeoGrbFeature, NeoEsp8266Dma800KbpsMethod> strip(PixelCount);

RgbColor red(colorSaturation, 0, 0);
RgbColor green(0, colorSaturation, 0);
RgbColor blue(0, 0, colorSaturation);
RgbColor white(colorSaturation);
RgbColor black(0);

HslColor hslRed(red);
HslColor hslGreen(green);
HslColor hslBlue(blue);
HslColor hslWhite(white);
HslColor hslBlack(black);

// Create an instance of the DRV8833:
DRV8833 driver = DRV8833();

// Pin numbers. Replace with your own!
// Attach the Arduino's pin numbers below to the
// Ain1 and Ain2 DRV8833 pins.
const uint8_t inputA1 = D5, inputA2 = D6, inputB1 = D7, inputB2 = D8;


SoftwareSerial mySoftwareSerial(D3, D4); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);


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
  driver.attachMotorA(inputA1, inputA2);
  driver.attachMotorB(inputB1, inputB2);

  Serial.println("Ready!");


  if (!myDFPlayer.begin(mySoftwareSerial, false)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true){
      delay(0); // Code to compatible with ESP8266 watch dog.
    }
  }
  Serial.println(F("DFPlayer Mini online."));

  myDFPlayer.volume(10);  //Set volume value. From 0 to 30
  myDFPlayer.play(1);  //Play the first mp3

}

void loop() {
  // put your main code here, to run repeatedly:

  multiresponseButton.poll();
  multiresponseButton2.poll();
  multiresponseButton3.poll();
  // if(multiresponseButton.longPress()) Serial.println("multiresponseButton longPress");
  // if(multiresponseButton.doubleClick()) Serial.println("multiresponseButton doubleClick a");
  // if(multiresponseButton.singleClick()) Serial.println("multiresponseButton singleClick");
  // if(multiresponseButton2.longPress()) Serial.println("multiresponseButton longPress");
  // if(multiresponseButton2.doubleClick()) Serial.println("multiresponseButton doubleClick b");
  // if(multiresponseButton2.singleClick()) Serial.println("multiresponseButton singleClick");
  if(multiresponseButton3.longPress()) Serial.println("multiresponseButton longPress c");
  if(multiresponseButton3.doubleClick()) Serial.println("multiresponseButton doubleClick c");
  if(multiresponseButton3.singleClick()) Serial.println("multiresponseButton singleClick c");

  static unsigned long timer = millis();

  if (millis() - timer > 3000) {
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

    int adcvalue = 0;
    adcvalue = analogRead(A0);
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
    // //strip.SetPixelColor(3, RgbwColor(colorSaturation));
    // strip.Show();


  // Wait to see the effect:
  // delay(5000);

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

