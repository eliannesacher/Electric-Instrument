/*
  GenEd1080 Lab 7 (EE Lab 2)
  13-button MIDI buttonboard
*/

// Include the MIDI library
#include <MIDI.h>

// Create and bind the MIDI interface to the default hardware Serial port
MIDI_CREATE_DEFAULT_INSTANCE();

// Define some fixed variables re: the buttonboard
const int numButtons = 1; // This is a 1-button buttonboard
const int buttons[numButtons] = {A2}; // The buttons will be connected to these pins on the Arduino4

// Define some variables that have fixed value (const)
const int modulator = 0; // Acceptable values: -4 to +4
const int volume = 127; // Acceptable values: 0 to 127

// Initialize the buttonPlaying array to be all zeros. This array describes the status of all the buttons - whether it is currently being pressed or not ('YES being pressed' or 'NOT being pressed').
bool buttonPlaying[numButtons] = {0};  // bool has two states: [0 and 1] = [FALSE and TRUE] = [LOW and HIGH]. 0 = this button is not being pressed (the button is not pressed); 1 = this button is already being pressed (the button is pressed) 

void setup() {
  // define pinMode for all the buttons as INPUT
  for (int i = 0; i < numButtons; i++) {
    pinMode(buttons[i], INPUT);
  }

  // set up the built-in LED as an OUTPUT (If we don't do this, the Arduino does not know "how to talk to" the built-inLED and we wouldn't be able to use it)
  pinMode(LED_BUILTIN, OUTPUT);

  MIDI.begin();
}


void loop() {
  for (int i = 0; i < numButtons; i++) {  // Loop through all 13 buttons...
    if (readButton(buttons[i], 10)) { // Is the i-th button pressed? The function readButton(buttonPin,buttonDelay) (see below) is used to determine the state of the button. If the i-th button is YES pressed, then continue...
      if (!buttonPlaying[i]) {  // ...Is the status of the i-th button currently 'NOT being pressed'? (!buttonPlaying[i] = 1 if the button is NOT being pressed; !buttonPlaying[i] = 0 if the button is not NOT being pressed aka if the button is being pressed). If the i-th button is 'NOT being pressed', then continue...
        buttonPlaying[i] = true;  // So if the i-th button is 'NOT being pressed', then update the status of i-th button to be 'YES being pressed'
        MIDI.sendNoteOn(60 + i + 12*modulator, volume, 1); // and press the i-th button
        digitalWrite(LED_BUILTIN, HIGH);  // and also flash the LED on for 100ms
        delay(100);
        digitalWrite(LED_BUILTIN, LOW);
      }
    } 
    else {  // So if the i-th button was NOT pressed, then...
      if (buttonPlaying[i]) { // Is the status of the i-th button currently 'YES being pressed'? If it is...
        buttonPlaying[i] = false; // then update the status of the i-th button to be 'NOT being pressed'
        MIDI.sendNoteOff(60 + i + 12*modulator, volume, 1);  // and stop being pressed the button
      }
    }
  }
}

// readButton -- A helper function for reading bouncy buttons. 
// All mechanical buttons are "bouncy" - every time you press a button, the metal contacts inside will bounce against each other rapidly, making and breaking electrical contact repeatedly. The Arduino will thus interpret the single button press as many sequential "button presses".
// All buttons are pull-downs, so we'll just debounce when the button goes high. 
bool readButton(int buttonPin, int buttonDelay) {
  bool buttonstate = digitalRead(buttonPin);  // read the buttonstate
  if (buttonstate == HIGH) {  
    delay(buttonDelay); // if it's HIGH, then stop reading the button for some amount of time (buttonDelay = some number of milliseconds)
    buttonstate = digitalRead(buttonPin); // read the buttonstate again 
    if (buttonstate == HIGH) {
      return true;  // if it's still HIGH, then yes the button really was pressed. The function will return a value of TRUE
    }
  }
  else {
    return false; // otherwise, then no the button was not pressed. The function will return a value of FALSE
  }
}
