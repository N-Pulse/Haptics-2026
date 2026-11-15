// testMode.ino - fake sender for bench tests (no BLE sender needed)
int testPacket[4] = {0, 0, 0, 2000};   // thumb, index, middle, flex
bool testDataFlowing = true;
bool testRampOn = false;

// Serial commands (set the Serial Monitor line ending to "Newline"):
//   t2000 / i1500 / m800 / f2500 -> set one value (raw, 0-4095)
//   a3000 -> set thumb, index and middle together
//   z     -> all pressures to 0
//   r     -> toggle automatic ramp (0 -> 4000 -> 0 every 4 s, all three fingers)
//   x     -> toggle data flow (stop data to test the safety shutoff)
void handleTestCommands() {
  while (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    if (cmd.length() == 0) continue;
    char c = cmd.charAt(0);
    int v = cmd.substring(1).toInt();
    switch (c) {
      case 't': testPacket[0] = v; testRampOn = false; break;
      case 'i': testPacket[1] = v; testRampOn = false; break;
      case 'm': testPacket[2] = v; testRampOn = false; break;
      case 'f': testPacket[3] = v; break;
      case 'a': testPacket[0] = testPacket[1] = testPacket[2] = v; testRampOn = false; break;
      case 'z': testPacket[0] = testPacket[1] = testPacket[2] = 0; testRampOn = false; break;
      case 'r': testRampOn = !testRampOn; break;
      case 'x': testDataFlowing = !testDataFlowing;
                Serial.printf("Data flowing: %d\n", testDataFlowing); break;
    }
  }
}

// Fills packet[4] like the real sender would. Returns false when data is "stopped".
bool getTestPacket(int packet[4]) {
  if (!testDataFlowing) return false;
  if (testRampOn) {
    unsigned long t = millis() % 4000;
    int p = (t < 2000) ? t * 2 : (4000 - t) * 2;   // triangle wave 0..4000
    testPacket[0] = testPacket[1] = testPacket[2] = p;
  }
  for (int i = 0; i < 4; i++) packet[i] = testPacket[i];
  return true;
}