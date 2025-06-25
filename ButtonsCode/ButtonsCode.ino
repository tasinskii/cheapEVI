#include <BLEMIDI_Transport.h>
#include <hardware/BLEMIDI_ESP32.h>
BLEMIDI_CREATE_INSTANCE("AWESOME_INSTRUMENT", MIDI)

const int v1pin = 12;
const int v2pin = 13;
const int v3pin = 14;
const int partialPin = 15;
const int btnPin = 23;

int v1State;
int v2State;
int v3State;
int partialState;
int btnState;
int lastBtnState;
int note; //0-7
int partial = 0;


void setup() {
  // put your setup code here, to run once:
  pinMode(v1pin, INPUT_PULLUP);
  pinMode(v2pin, INPUT_PULLUP);
  pinMode(v3pin, INPUT_PULLUP);
  pinMode(partialPin, INPUT_PULLUP);
  pinMode(btnPin, INPUT_PULLUP);
  MIDI.begin();
  Serial.begin(9600);  // Initialize serial communication
  

}

void loop() {
  // put your main code here, to run repeatedly:
  //capture valve state
  v1State = digitalRead(v1pin);
  v2State = digitalRead(v2pin);
  v3State = digitalRead(v3pin);
  partialState = digitalRead(partialPin);
  lastBtnState = btnState;
  btnState = digitalRead(btnPin);
  
  if (v1State && v2State && v3State) note = 1; // 111
  if (v1State && !v2State && v3State) note = 2; // 101
  if (!v1State && v2State && v3State) note = 3; // 011
  if (v1State && v2State && !v3State) note = 4; // 110
  if (v1State && !v2State && !v3State) note = 5; // 100
  if (!v1State && v2State && !v3State) note = 6; // 010
  if (!v1State && !v2State && !v3State) note = 7; // 000
  
  if (partialState) partial = 7;
  if (!partialState) partial = 0;

  if(lastBtnState == 1 && btnState == 0) MIDI.sendNoteOn(partial+note+51, 127, 1);
  /*
  Serial.println("---------");
  Serial.println(note);
  Serial.println(v1State);
  Serial.println(v2State);
  Serial.println(v3State);
  Serial.println("---------");
  */
  delay(50);

}
