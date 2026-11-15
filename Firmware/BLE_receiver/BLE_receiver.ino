#include <BLEDevice.h>
#include <BLEClient.h>
#include <BLEUtils.h>
#include <ESP32Servo.h>

#define TEST_MODE
#ifdef TEST_MODE
  #define NO_BLE
#endif

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

// manual forward declarations (functions called from inside the callback class) 
void updateBluetoothLED(int bluetoothPin, bool isConnected);
void resetComms();
void resetPacketTracking();
enum PacketResult { PKT_OK, PKT_LEGACY, PKT_BAD_SIZE, PKT_BAD_START, PKT_BAD_LENGTHS };

const int M1_PIN = 4;
const int M2_PIN = 5;
const int M3_PIN = 6;
const int M4_PIN = 7;
const int LED_HOT_PIN = 8;
const int LED_BLUETOOTH_PIN = 9;

const unsigned long DATA_TIMEOUT_MS = 150;        // from meeting: 100-150ms window
const unsigned long RECONNECT_COOLDOWN_MS = 2000; // don't spam connect attempts

Servo flexServo;

BLERemoteCharacteristic* pRemoteCharacteristic = nullptr;
BLEClient* pClient; // global so safety.ino's attemptReconnect() can use it
BLEAddress handAddress("14:c1:9f:38:aa:ed");

bool connected = false;
unsigned long lastPacketTime = 0;
unsigned long lastReconnectAttempt = 0;
bool actuatorsShutDown = false;

class MyClientCallback : public BLEClientCallbacks {
  void onConnect(BLEClient* pclient) {
    connected = true;
    lastPacketTime = millis(); //returns nb of miliseconds that passed from start 
    updateBluetoothLED(LED_BLUETOOTH_PIN, true); //turns LED on
  }
  void onDisconnect(BLEClient* pclient) {
    connected = false;
    resetComms();                       // NEW: forget the ack characteristic
    resetPacketTracking();
    pRemoteCharacteristic = nullptr;    // NEW: pointers are stale after a disconnect
    updateBluetoothLED(LED_BLUETOOTH_PIN, false); //turns LED off
  }
};

// NEW: finds the data characteristic AND the ack characteristic.
// Call it after every successful connect (setup AND reconnect).
bool bindCharacteristics() { //after connecting finds sender's service & data caracteristic & ack characterisitc
  BLERemoteService* pRemoteService = pClient->getService(SERVICE_UUID);
  if (pRemoteService == nullptr) {
    Serial.println("Service not found");
    return false; //if can't find service or data caracteristic
  }
  pRemoteCharacteristic = pRemoteService->getCharacteristic(CHARACTERISTIC_UUID);
  if (pRemoteCharacteristic == nullptr) {
    Serial.println("Data characteristic not found");
    return false;
  }
  setupComms(pRemoteService);  // ack characteristic; ok if it fails, data still works
  return true;
}

void setup() {
  Serial.begin(115200);
  unsigned long t0 = millis(); //if need S3
  while (!Serial && millis() - t0 < 3000) delay(10);   // S3 native USB: wait up to 3 s for the monitor
  setupMotors(M1_PIN, M2_PIN, M3_PIN, M4_PIN, flexServo);
  setupTempLEDs(LED_HOT_PIN, LED_BLUETOOTH_PIN);

#ifdef TEST_MODE
  connected = true;                 // pretend we are paired
  lastPacketTime = millis();
  Serial.println("TEST MODE: no BLE. Commands: t/i/m/f<value>, a<value>, z, r, x");
#else
  BLEDevice::init("ESP32_Arm");
  pClient = BLEDevice::createClient();
  pClient->setClientCallbacks(new MyClientCallback());
  if (pClient->connect(handAddress)) {
    bindCharacteristics();
  } else {
    Serial.println("First connect failed, loop() will retry");
  }
#endif
}

void loop() {
  unsigned long now = millis();

  int packet[4];
  bool gotPacket = false;
  uint16_t packetNumber = 0;

#ifdef TEST_MODE
  handleTestCommands();
  gotPacket = getTestPacket(packet);

#else
  if (connected && pRemoteCharacteristic != nullptr && pRemoteCharacteristic->canRead()) {
    String value = pRemoteCharacteristic->readValue();
    static uint16_t legacyCounter = 0;
    PacketResult result = parsePacket(value, packet, packetNumber);

    if (result == PKT_OK) {
      if (trackPacket(packetNumber)) gotPacket = true;   // false = repeat of an old packet -- nothing new
    } else if (result == PKT_LEGACY) {
      packetNumber = ++legacyCounter;                    
      gotPacket = true;
    } else {
      countBadPacket();
      Serial.printf("Bad packet (code %d, %d bytes)\n", result, value.length());
    }
  }
#endif

#ifdef NO_BLE
  static uint16_t fakeCounter = 0;
  if (gotPacket) packetNumber = ++fakeCounter;
#endif

  if (gotPacket) {
    float filtered_thumb  = updateFilter(0, packet[0]);
    float filtered_index  = updateFilter(1, packet[1]);
    float filtered_middle = updateFilter(2, packet[2]);
    float filtered_flex   = updateFilter(3, packet[3]);

    int thumb  = pressureThreshold(filtered_thumb);
    int index  = pressureThreshold(filtered_index);
    int middle = pressureThreshold(filtered_middle);
    int flex   = flexionThreshold(filtered_flex);

    Serial.printf("Thumb:%d Index:%d Middle:%d Flex:%d\n", thumb, index, middle, flex);

    setVibrationMotor(M1_PIN, thumb);
    setVibrationMotor(M2_PIN, index);
    setVibrationMotor(M3_PIN, middle);
    setServoAngle(flexServo, flex);

    lastPacketTime = now;
    actuatorsShutDown = false;
    onDataReceived(actuatorsShutDown, packetNumber);
  }

  //to print the stats
  updatePulses();
  #ifndef NO_BLE
  printPacketStats();
  #endif


//Read temp even if bluetooth is down
  static unsigned long lastTempMs = 0;
  if (now - lastTempMs >= 500) { //every 500ms reads temp
    lastTempMs = now;
    float temp = readTemperatureC();
    //Serial.printf("Temp:%.1f C\n", temp);
    updateHotLED(LED_HOT_PIN, temp);
  }

//safety shut off 
  if ((not connected or checkDataTimeout(now, lastPacketTime, DATA_TIMEOUT_MS)) and not actuatorsShutDown) { //if no link or no package for 150ms
    Serial.println("No data / disconnected - triggering safety shutoff");
    shutdownActuators(M1_PIN, M2_PIN, M3_PIN, flexServo);
    actuatorsShutDown = true;
  }

  if (connected) updateHeartbeat(actuatorsShutDown); //sends heartbeat every 100ms

#ifndef TEST_MODE
  if (!connected) attemptReconnect(now);
#endif

  delay(10); //pauses loop for 10 miliseconds
}