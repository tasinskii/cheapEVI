#include <BLEMIDI_Transport.h>
#include <hardware/BLEMIDI_ESP32.h>
BLEMIDI_CREATE_INSTANCE("MOUTHPIECE_TEST", MIDI)

const int mouthpiecePin = 35;
const float PRESSURE_THRESHOLD = 0.1;  // Minimum kPa to trigger note
const int MIDI_NOTE = 55;              // Note to play (e.g., G3)
const int MIDI_CHANNEL = 1;

// Swell parameters
const int SWELL_RATE = 5;    // How fast volume ramps (lower = smoother)
const int DEBOUNCE_MS = 200; // Delay before note-off after breath stops

// State tracking
bool isNoteActive = false;
int currentVolume = 0;
unsigned long lastBreathTime = 0;

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);
  BLEMIDI.begin();
}

void loop() {
  // Read and convert pressure
  float pressure_kPa = (((analogRead(mouthpiecePin) / 4095.0) * 3.3 / 0.666) - 2.63) * 2.0;
  int targetVolume = (pressure_kPa > PRESSURE_THRESHOLD) ? 
    constrain(map(pressure_kPa * 100, 10, 200, 0, 127), 0, 127) : 0;

  // Note triggering logic
  if (targetVolume > 0) {
    lastBreathTime = millis();
    if (!isNoteActive) {
      MIDI.sendNoteOn(MIDI_NOTE, targetVolume, MIDI_CHANNEL);
      isNoteActive = true;
    }
  }

  // Volume swell (while note is active)
  if (isNoteActive) {
    if (targetVolume > currentVolume) {
      currentVolume = min(currentVolume + SWELL_RATE, targetVolume);
    } else if (targetVolume < currentVolume) {
      currentVolume = max(currentVolume - SWELL_RATE, targetVolume);
    }
    MIDI.sendControlChange(11, currentVolume, MIDI_CHANNEL); // Expression (CC11)
  }

  // Note release (after debounce)
  if (isNoteActive && millis() - lastBreathTime > DEBOUNCE_MS) {
    MIDI.sendNoteOff(MIDI_NOTE, 0, MIDI_CHANNEL);
    isNoteActive = false;
    currentVolume = 0;
  }

  delay(10); // 10ms update rate (~100Hz)
}
