# Buttons Manager by Schmurtz
 ----
  Buttons Manager is an arduino function to manage buttons.
  This is my own button management, as there are probably thousands of them ;)
  I've created it because I didn't find a simple library with all the events that I needed...
  
  Thinked to be simple and easily modifiable, it is compatible with arduino, ESP8266 and ESP32.
  It will natively compatible with arduino IDE and PLatformio.
  
  It can manage :
  
       - multiple buttons without interfering
       - dual simultaneous press on 2 buttons
       - double click event on one button
       - short press, long press , extra long press
       - detect event on press and on release button
       - a RGB LED which help to understand which action has been executed
           (for example if a long click has been taken into account)

 Examples :
 ----
  3 examples are available : with one button, with 2 buttons and 2 buttons with an RGB light.
  
  These examples display buttons events on serial :
  
               - short press begins / short press released
               - long press begins / long press released
               - extra long press begins / extra long press released
               - double press on one button
               - 2 buttons pressed simultaneously


  The LED example use an RGB led to show what happens to the user :

               - single press = 3 green blinks
               - double press = 3 OrangeRed blinks
               - long press = red increasing light until 3 yellow blinks when it is taken into account
               - dual press of 2 buttons = 3 blue blinks


			   
			   
So it require [Fastled library](https://github.com/FastLED/FastLED) and at least one RGB led compatible with Fastled (Neopixel, WS2801, WS2811, WS2812B...)
Fastled is a library easy to find in arduino IDE, Platformio or Github.
Led is connected on GPIO 2 by default.
	 
By default examples use GPIO 13 for the one button example and 12 & 13 for the two buttons example.

Release Notes (yyyy/mm/dd):
 ----
```
/*  V1.0 - 2022/02/18 :                                                                     */
/*    - Inital release                                                                      */
```

 How to use :
 ----
 Download this repository and run one example.
 If you want to use it in your project copy the function and add the declarations of your buttons in setup.
 After that you just need to run ButtonManager() in your loop to refresh the state of all your buttons.
 
 GPIO are pulled up so you don't need a resistor : just plug your button between GPIO 13 and Ground.
 By default examples use GPIO 13 for the one button example and 12 & 13 for the two buttons example


 Some Remarks :
 ----
This function probably deserve to be transformed as library 😅 !
I will use it like this but if youneed feel free to fork it and transform it as a library it will be probably more handful like this.
 

 

## Thanks
Do you want to improve this project? Don't hesitate, **Participate** : edit and publish your modifications :)<br/>
You like this project ?
You can also buy me a coffee to keep my motivation up !<br/><br/>
[![Buy me a coffee][buymeacoffee-shield]][buymeacoffee]
<br/><br/>


[buymeacoffee-shield]: https://www.buymeacoffee.com/assets/img/guidelines/download-assets-sm-2.svg
[buymeacoffee]: https://www.buymeacoffee.com/schmurtz
