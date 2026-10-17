// to confirm actual divider resistor value and get R0/B from thermistor datasheet
const float THERM_R0 = 10000.0;   // resistance at 25°C
const float THERM_B = 3950.0;     // Beta coefficient
const float THERM_T0 = 298.15;    // 25°C in Kelvin — fixed
const float THERM_FIXED_R = 10000.0; // fixed divider resistor
const float ADC_VREF = 3.3;       // ESP32 ADC reference voltage
const int ADC_RES = 4095;         // ESP32 12-bit ADC

const int PIN_THERMISTOR = 5; // to confirm 

// Reads the thermistor and returns temperature in tenths of a degree Celsius
// (e.g. 365 means 36.5°C) so it fits in the int packet alongside the other values
int readTemperatureTenthsC() {
  int adcValue = analogRead(PIN_THERMISTOR);
  float voltage = adcValue * (ADC_VREF / ADC_RES); //converting raw ADC number into actual voltage

  if (voltage < 0.01) return -1; // avoid dividing by 0

  float resistance = THERM_FIXED_R * (ADC_VREF - voltage) / voltage; //have voltage, must find resistor to use for formula
  float tempK = 1.0 / ((1.0 / THERM_T0) + (1.0 / THERM_B) * log(resistance / THERM_R0));
  float tempC = tempK - 273.15;

  return (int)(tempC * 10);
}