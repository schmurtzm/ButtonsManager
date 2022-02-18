// Button Manager by Schmurtz  (https://github.com/schmurtzm/ButtonsManager)
// This is my own button management, as there are probably thousands of them ;)
// Thinked to be simple and easily modifiable.
// It can manage :
//      - multiple buttons without interfering
//      - dual simultaneous press on 2 buttons
//      - double click event on one button
//      - short press, long press , extra long press
//      - detect event on press and on release button
//      - a RGB LED which help to understand which action has been executed
//          (for example if a long click has been taken into account)


// This example works with 2 buttons on GPIO 12 and 13
// It displays buttons events on serial :
//              - short press begins / short press released
//              - long press begins / long press released
//              - extra long press begins / extra long press released
//              - double press on one button
//              - 2 buttons pressed simultaneously


// It also use a RGB led to show what happens to the user :

//              - single press = 3 green blinks
//              - double press = 3 OrangeRed blinks
//              - long press = red increasing light until 3 yellow blinks when it is taken into account
//              - dual press of 2 buttons = 3 blue blinks

//    So it require Fastled library (easy to find in arduino IDE, Platformio or Github)
//    Led is connected on pin 2 by default

#include "Arduino.h"
#include "FastLED.h"

typedef struct
{
  byte ButtonGpio = 255;        // 255 means that this button is not initialized
  bool CurrentGPIOstate = 1;    // Digital state of the pin (High or LOW)
  bool PreviousGPIOState = 1;   // Previous digital state of the pin (High or LOW)
  unsigned long time = 0;       // Used to record the time since when the button has been pressed
  bool BoundarySwitcher = 0;    // this boolean is used to debounce the button and avoid repetitions
  byte CurrentButtonstate = 0;  // Current state of the button :
  //                                0 : nothing , 1 = short press begins , 2 = short press released , 3 = long press begins
  //                                4 = long press released , 5 = extra long press begins , 6 = extra long press released , 7 = currently pushed
} button;

button ButtonsDatas[2];             // here we create a structure to manage 2 buttons

const int DEBOUNCE_TIME = 150;      // debounce time = minimal time between 2 clicks (in ms)
const int DOUBLE_PRESS = 500;       // maximum time between 2 press to obtain a double press event
const int LONG_PRESS = 1500;        // time to press a button to trigger a long press action
const int EXTRA_LONG_PRESS = 3000;  // time to press a button to trigger an extra long press action

const int BTN_1 = 12;               // we set a GPIO for the first button
const int BTN_2 = 13;

bool DblePressActionDone = 0;       // used to debouche simulataneous dual press


// ============== declarations for LEDS : ==============
#define NUM_LEDS 1         // Number of leds
#define DATA_PIN 2         // data pin
CRGB leds[NUM_LEDS];       // led declaration

unsigned long LedTimer = 0;     // To avoid delay() function
byte  Globo = 0;                // To create some nice progressive effects
byte LedBlinkCount = 255;       // To manage de lengh duration of blinking function
CRGB BlinkColor = CRGB::Black;  // To create some nice progressive effects
// ============ End of declarations for LEDS :============

/* ###################################### Physical Buttons Function ########################################### */

void ButtonManager()
{
  byte MaxGloboValue = 60;

  for (int i = 0; i < sizeof ButtonsDatas / sizeof ButtonsDatas[0]; i++)   // for each button in ButtonsDatas
  {
    if (ButtonsDatas[i].ButtonGpio < 255)       // if a gpio has been defined
    {

      ButtonsDatas[i].CurrentGPIOstate = digitalRead(ButtonsDatas[i].ButtonGpio);               // we update the GPIO state


      // ==================================================== Case where we have just had a change of state : button pushed =====================================================


      if (ButtonsDatas[i].CurrentGPIOstate == LOW && ButtonsDatas[i].PreviousGPIOState == HIGH) //
      {

        if (millis() - ButtonsDatas[i].time < DEBOUNCE_TIME)  // if during the moment the button is pressed the time of pressing is less than ...
        {
          ButtonsDatas[i].CurrentGPIOstate = HIGH;  // we cancel the action to debounce
          ButtonsDatas[i].CurrentButtonstate = 0;

        }
        else if (millis() - ButtonsDatas[i].time < DOUBLE_PRESS )
        {
          ButtonsDatas[i].time = millis(); // We start counting the time from the moment the button is pressed
          ButtonsDatas[i].CurrentButtonstate = 8;  // like explained above: 1 = short press begins
          BlinkColor = CRGB::OrangeRed   ;
        }
        else
        {
          ButtonsDatas[i].time = millis(); // We start counting the time from the moment the button is pressed
          ButtonsDatas[i].CurrentButtonstate = 1;  // like explained above: 1 = short press begins
          BlinkColor = CRGB::Green;
        }
      }


      // ================================================================== Case where the button remains pressed ==================================================================


      if (ButtonsDatas[i].CurrentGPIOstate == LOW && ButtonsDatas[i].PreviousGPIOState == LOW)
      {
        if (millis() - ButtonsDatas[i].time > 200)  // We start led globo effect after 200ms
        {
          if (Globo < MaxGloboValue) {      // we create an effect that makes the led light get brighter (MaxGloboValue is = 60 here) before blinking 3 times to signal the action
            if (millis() - LedTimer >  LONG_PRESS / MaxGloboValue)  // if during the moment the button is pressed the time of pressing is less than ...
            {
              LedTimer =  millis();
              Globo++; // max value is 259, 260 return to low value ->  Globo effect
            }
            leds[0].r = Globo;  // it will be a red globo effect
            leds[0].g = 0;
            leds[0].b = 0;
            FastLED.show();   // we display the current brightess on the led

          }
        }

        if (Globo == MaxGloboValue) // if we are to the
        {
          BlinkColor = CRGB::Yellow;
          LedBlinkCount = 0;
        }

        if (millis() - ButtonsDatas[i].time <= LONG_PRESS)  // if during the moment the button is pressed the time of pressing is less than ...
        {
          ButtonsDatas[i].CurrentButtonstate = 7;   // necessary if we want to detect the button only one time when it is pressed
        }


        if (millis() - ButtonsDatas[i].time > LONG_PRESS && millis() - ButtonsDatas[i].time < EXTRA_LONG_PRESS) // if during the moment the button is pressed the time of pressing is more than ...
        {
          if (ButtonsDatas[i].BoundarySwitcher == 0) // on the next boundary we will check if it is = to 1
          {
            ButtonsDatas[i].BoundarySwitcher = 1;    // so it allow to execute that only one time...
            ButtonsDatas[i].CurrentButtonstate = 3;
          }
          else
          {
            ButtonsDatas[i].CurrentButtonstate = 7;   // necessary if we want to detect the button only one time when it is pressed
          }


        }



        if (millis() - ButtonsDatas[i].time >= EXTRA_LONG_PRESS) // si durant le moment ou le bouton est appuyé le temps d'appui est sup à ...
        {
          if (ButtonsDatas[i].BoundarySwitcher == 1) // on the next boundary we will check if it is = to 0
          {
            ButtonsDatas[i].BoundarySwitcher = 0;    // so it allow to execute that only one time...
            ButtonsDatas[i].CurrentButtonstate = 5;
          }
          else
          {
            ButtonsDatas[i].CurrentButtonstate = 7;   // necessary if we want to detect the button only one time when it is pressed
          }
        }

      }


      // ================================================================== In case the button has just been released ==================================================================


      if (ButtonsDatas[i].CurrentGPIOstate == HIGH && ButtonsDatas[i].PreviousGPIOState == LOW)
      {

        if (millis() - ButtonsDatas[i].time <= LONG_PRESS)
        {
          ButtonsDatas[i].CurrentButtonstate = 2;
          if ( leds[0]  !=  CRGB(0x0000FF))   // blue with dual press
          {
            LedBlinkCount = 0; // This will start a blinking
          }
        }


        if (millis() - ButtonsDatas[i].time > LONG_PRESS && millis() - ButtonsDatas[i].time < EXTRA_LONG_PRESS) // if during the moment the button is pressed the time of pressing is between LONG_PRESS and EXTRA_LONG_PRESS ...
        {
          ButtonsDatas[i].CurrentButtonstate = 4;
        }


        if (millis() - ButtonsDatas[i].time >= EXTRA_LONG_PRESS)            // if during the moment the button is pressed the time of pressing is more than EXTRA_LONG_PRESS ...
        {
          ButtonsDatas[i].CurrentButtonstate = 6;
        }

        ButtonsDatas[i].time = millis();   // To debounce the next push
        Globo = 0;   // the button has been released, so the globo effect is reinitialized.


      }


      // ============================================================================= In case nothing happens =============================================================================



      if (ButtonsDatas[i].CurrentGPIOstate == HIGH && ButtonsDatas[i].PreviousGPIOState == HIGH)
      {
        ButtonsDatas[i].BoundarySwitcher = 0;
        ButtonsDatas[i].CurrentButtonstate = 0;
      }
      // ===================================================================================================================================================================================


      ButtonsDatas[i].PreviousGPIOState = ButtonsDatas[i].CurrentGPIOstate;
    }
  }
}



void LedBlink()
{
  if (LedBlinkCount < 3)                  // there will be 3 blink
  {
    if (millis() - LedTimer >= 200) {     // one blink every 200 ms
      LedTimer = millis();
      if (leds[0] == CRGB(0x000000)) {    // if led color is black (we can't comopare with html color, must use hex code : http://fastled.io/docs/3.1/struct_c_r_g_b.html)
        fill_solid(leds, NUM_LEDS, BlinkColor);
        FastLED.show();
      } else {
        fill_solid(leds, NUM_LEDS, CRGB::Black);
        FastLED.show();
        LedBlinkCount++;
      }
    }
  }


}

/* ################################## Setup ############################################# */
void setup()
{
  Serial.begin(115200);
  Serial.println(F("Button Manager starts !"));

  //we initialize some buttons :
  pinMode(BTN_1, INPUT_PULLUP);
  pinMode(BTN_2, INPUT_PULLUP);
  ButtonsDatas[0].ButtonGpio = BTN_1;
  ButtonsDatas[1].ButtonGpio = BTN_2;

  // ============ déclaration pour les LEDS : ============

  //FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  leds[0] = CRGB::Black;
  FastLED.show();
  // =========== fin déclaration pour les LEDS ===========
}

/* ##################################### Loop ############################################# */

void loop()
{

  ButtonManager(); // we call the main function and refresh the button state
  LedBlink();      // Function for managing blinking of RGB led without the use of delay()
  
  for (int i = 0; i < sizeof ButtonsDatas / sizeof ButtonsDatas[0]; i++)   // for each button in ButtonsDatas
  {
    switch (ButtonsDatas[i].CurrentButtonstate) {
      case 0:
        // Nothing happens on the buttons... So we do nothing ;)
        break;
      case 1:
        Serial.print(F("Short press in progress on GPIO"));
        Serial.println(ButtonsDatas[i].ButtonGpio);
        // if (ButtonsDatas[i].ButtonGpio == 12) Serial.println("Button on GPIO12 pressed, let's do something !");  // exemple of action
        break;
      case 2:
        Serial.print(F("Short press released on GPIO : "));
        Serial.println(ButtonsDatas[i].ButtonGpio);
        break;
      case 3:
        Serial.print(F("Long press in progress on GPIO "));
        Serial.println(ButtonsDatas[i].ButtonGpio);
        break;
      case 4:
        Serial.print(F("Long press released on GPIO : "));
        Serial.println(ButtonsDatas[i].ButtonGpio);
        break;
      case 5:
        Serial.print(F("Extra long press in progress on GPIO "));
        Serial.println(ButtonsDatas[i].ButtonGpio);
        break;
      case 6:
        Serial.print(F("Extra long press released on GPIO : "));
        Serial.println(ButtonsDatas[i].ButtonGpio);
        break;
      case 7:
        //        Serial.print(F("Extra long press released on GPIO : "));
        //        Serial.println(ButtonsDatas[i].ButtonGpio);
        break;
      case 8:
        Serial.print(F("Double press detected on GPIO : "));
        Serial.println(ButtonsDatas[i].ButtonGpio);
        break;
      default:
        break;
    }
  }

  if (ButtonsDatas[0].CurrentButtonstate  > 0 && ButtonsDatas[1].CurrentButtonstate > 0)
  {
    if (DblePressActionDone == false)
    {
      Serial.print(F("2 buttons pressed simultaneously !"));
      BlinkColor = CRGB::Blue;   
      LedBlinkCount = 0;        // We start a blue blinking when a dual press is detected
      DblePressActionDone = true;
    }
  }
  else
  {
    DblePressActionDone = false;
  }

}
