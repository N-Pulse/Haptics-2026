
// 400 = 40.0°C
const int TEMP_THRESHOLD_TENTHS_C = 400; //threshold to be determined 

void setupTempLEDs(int hotPin, int normalPin) {
  pinMode(hotPin, OUTPUT);
  pinMode(normalPin, OUTPUT);
}

// rawTempTenths is the value received over BLE (°C × 10)
void updateTempLEDs(int hotPin, int normalPin, int rawTempTenths) {
  bool overheating = rawTempTenths > TEMP_THRESHOLD_TENTHS_C;
  digitalWrite(hotPin, overheating ? HIGH : LOW);
  digitalWrite(normalPin, overheating ? LOW : HIGH);
}