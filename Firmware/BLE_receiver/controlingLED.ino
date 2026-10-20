const int TEMP_THRESHOLD_C = 40.0; //threshold to be determined 

void setupTempLEDs(int hotPin, int bluetoothPin) {
  pinMode(hotPin, OUTPUT);
  pinMode(bluetoothPin, OUTPUT);
}

void updateHotLED(int hotPin, float temp) {
  digitalWrite(hotPin, temp > TEMP_THRESHOLD_C ? HIGH : LOW);
}

void updateBluetoothLED(int bluetoothPin, bool isConnected) {
  digitalWrite(bluetoothPin, isConnected ? HIGH : LOW);
}