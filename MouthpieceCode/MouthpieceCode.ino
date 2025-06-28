#include <BLEMIDI_Transport.h>
#include <hardware/BLEMIDI_ESP32.h>

//BLEMIDI_CREATE_DEFAULT_INSTANCE()
 BLEMIDI_CREATE_INSTANCE("MOUTHPIECE_TEST", MIDI)


/*
 * MPXV7002DP Differential Pressure Sensor
 * The MPXV7002DP has the following characteristics:
 * - Pressure range: ±2 kPa (±0.29 psi)
 * - Sensitivity: 1 V/kPa (2.5V output at 0 kPa)
 * - Output voltage: 0.5V to 4.5V (for ±2 kPa range)
 */
const int mouthpiecePin = 35;

// The output can go up to 4.5V and the ESP32 has a 3.3V ADC limit:
// Need to use a voltage divider to scale the output down to 0-3.3V
// Vout_scaled = Vout_sensor * (R2/(R1+R2))
// 3.3V = 5V * (about 2/3), We can use R1 = 10k Ohms and R2 = 20k Ohms
const float R1 = 10000.0;
const float R2 = 20000.0;
const float VDIVIDER_RATIO = R2 / (R1 + R2);

// Calibration values:
const float VOUT_AT_ZERO_kPa = 2.63; // Sensor output at 0 kPa (MODIFY IF NEEDED)
const float SENSOR_MIN_V = 0.5;     // Sensor output at -2 kPa
const float SENSOR_MAX_V = 4.5;     // Sensor output at +2 kPa
const float PRESSURE_RANGE = 2.0;   // + or - 2 kPa

// MIDI configuration
const int MIDI_NOTE = 55;
const int MIDI_CHANNEL = 1;

void setup() {
  Serial.begin(115200);
  analogReadResolution(12); // Set ESP32 ADC to 12-bit (digital read 0-4095)
  BLEMIDI.begin();
}

void loop() {
  int rawADC = analogRead(mouthpiecePin); // read raw ADC value between 0-4095
  float vout_scaled = (rawADC / 4095.0) * 3.3; // convert ADC to scaled voltage (0-3.3V)
  // Reverse voltage divider to get original sensor voltage (0.5V-4.5V)
  float vout_sensor = vout_scaled / VDIVIDER_RATIO;
  // Convert voltage to pressure (kPa)
  float pressure_kPa = ((vout_sensor - VOUT_AT_ZERO_kPa) / (SENSOR_MAX_V - SENSOR_MIN_V)) * (2 * PRESSURE_RANGE);

  // Map pressure to MIDI volume (0-127)
  int volume = 0;
  if (pressure_kPa > 0.1) {
    volume = constrain(map(pressure_kPa * 100, 0, 200, 0, 127), 0, 127);  // 0-2 kPa → 0-127
  }

  MIDI.sendNoteOn(MIDI_NOTE, volume, MIDI_CHANNEL);
  delay(10);

  
  // // Print results
  // Serial.print("Raw ADC: ");
  // Serial.print(rawADC);
  // Serial.print(" | Voltage (scaled): ");
  // Serial.print(vout_scaled, 3);
  // Serial.print("V | Voltage (sensor): ");
  // Serial.print(vout_sensor, 3);
  // Serial.print("V | Pressure: ");
  // Serial.print(pressure_kPa, 3);
  // Serial.println(" kPa");
  // delay(500);  // Adjust delay as needed
  
}

