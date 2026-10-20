const float THERM_R0 = 10000.0;   //to confirm
const float THERM_B = 3950.0;     //to confirm
const float THERM_T0 = 298.15;
const float THERM_FIXED_R = 10000.0; //to confirm actual divider resistor
const float ADC_VREF = 3.3;
const int ADC_RES = 4095;

const int PIN_THERMISTOR = 10; //to confirm

float readTemperatureC() {
  int adcValue = analogRead(PIN_THERMISTOR);
  float voltage = adcValue * (ADC_VREF / ADC_RES);
  if (voltage < 0.01) return -273.15; // sensor disconnected / invalid reading

  float resistance = THERM_FIXED_R * (ADC_VREF - voltage) / voltage;
  float tempK = 1.0 / ((1.0 / THERM_T0) + (1.0 / THERM_B) * log(resistance / THERM_R0));
  return tempK - 273.15;
}