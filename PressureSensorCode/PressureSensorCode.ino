/*
 * MPXV7002DP Differential Pressure Sensor Test Code
 * 
 * This code reads the analog output from the MPXV7002DP sensor and converts it
 * to a pressure value in kPa, then prints the results to the Serial Monitor.
 * 
 * Connections:
 * - Sensor Pin 1 (Vout): Connect to Arduino analog input (e.g., A0)
 * - Sensor Pin 2 (GND): Connect to Arduino GND
 * - Sensor Pin 3 (Vs): Connect to Arduino 5V
 * 
 * The MPXV7002DP has the following characteristics:
 * - Pressure range: ±2 kPa (±0.29 psi)
 * - Sensitivity: 1 V/kPa (2.5V output at 0 kPa)
 * - Output voltage: 0.5V to 4.5V (for ±2 kPa range)
 */

const int pressurePin = 35;  // Analog input pin for sensor
const float VREF = 5.0;      // Arduino reference voltage (change to 3.3 for 3.3V boards)
const float ZERO_PRESSURE_VOLTAGE = 2.5;  // Output voltage at 0 kPa
const float SENSITIVITY = 0.5;  // 0.5V/kPa (from datasheet)

void setup() {
  Serial.begin(9600);  // Initialize serial communication
  Serial.println("MPXV7002DP Differential Pressure Sensor Test");
  Serial.println("Pressure (kPa)\tVoltage (V)\tRaw ADC");
}

void loop() {
  // Read the analog value (0-1023)
  int rawValue = analogRead(pressurePin);
  
  // Convert to voltage (0-VREF)
  float voltage = rawValue * (VREF / 1023.0);
  
  // Convert voltage to pressure in kPa
  // Pressure = (Vout - Voffset) / Sensitivity
  float pressure = (voltage - ZERO_PRESSURE_VOLTAGE) / SENSITIVITY;
  
  // Print all values to serial monitor
  Serial.print(pressure, 4);    // 4 decimal places for pressure
  Serial.print("\t\t");
  Serial.print(voltage, 3);     // 3 decimal places for voltage
  Serial.print("\t\t");
  Serial.println(rawValue);     // Raw ADC value
  
  delay(500);  // Wait 0.5 seconds between readings
}