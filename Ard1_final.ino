/*
  GenEd1080 Lab 7 (EE Lab 2)
  13-key MIDI keyboard
*/

// Define some variables that have fixed value (const)
int modulation = 0; // Acceptable values: -4 to +4
int volume = 127; // Acceptable values: 0 to 127

// Include the MIDI library
#include <MIDI.h>

// Create and bind the MIDI interface to the default hardware Serial port
MIDI_CREATE_DEFAULT_INSTANCE();

// Define some fixed variables re: the keyboard
const int numKeys = 13; // This is a 13-key keyboard
const int buttons[numKeys] = {A1, A2, A3, A4, A5, A6, 0, 1, 2, 3, 4, 5, 6}; // The keys will be connected to these pins on the Arduino

// Initialize the notePlaying array to be all zeros. This array describes the status of all the notes - whether it is currently playing or not ('YES playing' or 'NOT playing').
bool notePlaying[numKeys] = {0};  // bool has two states: [0 and 1] = [FALSE and TRUE] = [LOW and HIGH]. 0 = this note is not playing (the key is not pressed); 1 = this note is already playing (the key is pressed) 

void setup() {
  // define pinMode for all the buttons as INPUT
  for (int i = 0; i < numKeys; i++) {
    pinMode(buttons[i], INPUT);
  }

  // set up the built-in LED as an OUTPUT (If we don't do this, the Arduino does not know "how to talk to" the built-inLED and we wouldn't be able to use it)
  pinMode(LED_BUILTIN, OUTPUT);

  MIDI.begin();
}


void loop() {

//pressing the pin 8 button increases the octave by 1 
//and pressing the pin 7 button decreases the octave by 1
 if (readButton(8, 10) == true) {
  if (modulation < 4) {
    modulation = modulation + 1;
    delay(200);
  }
 }

 else if (readButton(7, 10) == true) {
  if (modulation > -4) {
    modulation = modulation - 1;
    delay(200);
  }
 }

  else if (readButton(9, 10) == true) {
  if (volume > 10) {
    volume = volume - 10;
    delay(200);
  }
 }

  else if (readButton(10, 10) == true) {
  if (volume < 117) {
    volume = volume + 10;
    delay(200);
  }
 }

//turning the velocity knob adjusts velocity being sent by keyboard
 //volume = map(analogRead(A0), 0, 1023, 127, 0);

  for (int i = 0; i < numKeys; i++) {  // Loop through all 13 keys...
    if (readButton(buttons[i], 10)) { // Is the i-th key pressed? The function readButton(buttonPin,buttonDelay) (see below) is used to determine the state of the key. If the i-th key is YES pressed, then continue...
      if (!notePlaying[i]) {  // ...Is the status of the i-th note currently 'NOT playing'? (!notePlaying[i] = 1 if the note is NOT playing; !notePlaying[i] = 0 if the note is not NOT playing aka if the note is playing). If the i-th note is 'NOT playing', then continue...
        notePlaying[i] = true;  // So if the i-th note is 'NOT playing', then update the status of i-th note to be 'YES playing'
        MIDI.sendNoteOn(60 + i + 12*modulation, volume, 1); // and play the i-th note
        digitalWrite(LED_BUILTIN, HIGH);  // and also flash the LED on for 100ms
        delay(100);
        digitalWrite(LED_BUILTIN, LOW);
      }
    } 
    else {  // So if the i-th key was NOT pressed, then...
      if (notePlaying[i]) { // Is the status of the i-th note currently 'YES playing'? If it is...
        notePlaying[i] = false; // then update the status of the i-th note to be 'NOT playing'
        MIDI.sendNoteOff(60 + i + 12*modulation, volume, 1);  // and stop playing the note
      }
    }
  }
}

// readButton -- A helper function for reading bouncy buttons. 
// All mechanical buttons are "bouncy" - every time you press a button, the metal contacts inside will bounce against each other rapidly, making and breaking electrical contact repeatedly. The Arduino will thus interpret the single button press as many sequential "button presses".
// All buttons are pull-downs, so we'll just debounce when the button goes high. 
bool readButton(int buttonPin, int buttonDelay) {
  bool buttonState = digitalRead(buttonPin);  // read the buttonState
  if (buttonState == HIGH) {  
    delay(buttonDelay); // if it's HIGH, then stop reading the button for some amount of time (buttonDelay = some number of milliseconds)
    buttonState = digitalRead(buttonPin); // read the buttonState again 
    if (buttonState == HIGH) {
      return true;  // if it's still HIGH, then yes the button really was pressed. The function will return a value of TRUE
    }
  }
  else {
    return false; // otherwise, then no the button was not pressed. The function will return a value of FALSE
  }
}
