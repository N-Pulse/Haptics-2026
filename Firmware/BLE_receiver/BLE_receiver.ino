#include <BLEDevice.h>
#include <BLEClient.h>
#include <BLEUtils.h>
#include <ESP32Servo.h>

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

const int M1_PIN = 4; // vibration motor 1 (thumb)  - confirm 
const int M2_PIN = 5; // vibration motor 2 (index)  -confirm 
const int M3_PIN = 6; // vibration motor 3 (middle) - confirm 
const int M4_PIN = 7; // servo (flexion)  - confirm 
const int LED_HOT_PIN = 8;    // blue — lights when overheating - confirm
const int LED_NORMAL_PIN = 9; // for now ights when temperature is normal - to be confirmed

Servo flexServo;

BLERemoteCharacteristic* pRemoteCharacteristic;
bool connected = false;

class MyClientCallback : public BLEClientCallbacks {
  void onConnect(BLEClient* pclient) { connected = true; }
  void onDisconnect(BLEClient* pclient) { connected = false; }
};

void setup() {
  Serial.begin(115200);

  setupMotors(M1_PIN, M2_PIN, M3_PIN, M4_PIN, flexServo);

  BLEDevice::init("ESP32_Arm");

  BLEClient* pClient = BLEDevice::createClient();
  pClient->setClientCallbacks(new MyClientCallback());

  BLEAddress handAddress("14:c1:9f:38:aa:ed"); // placeholder - will be swithced out with real address of sender
  pClient->connect(handAddress);

  BLERemoteService* pRemoteService = pClient->getService(SERVICE_UUID);
  pRemoteCharacteristic = pRemoteService->getCharacteristic(CHARACTERISTIC_UUID);
}

void loop() {
  if (connected && pRemoteCharacteristic->canRead()) {
    String value = pRemoteCharacteristic->readValue();

    // Sender packs packet[5] (5 ints) as raw bytes via (uint8_t*)packet, sizeof(packet)
    if (value.length() == sizeof(int) * 5) {
      int packet[5];
      memcpy(packet, value.c_str(), sizeof(packet));  //sender sends 16 bytes, so here copies bytes into int[4] 

      int thumb  = packet[0];
      int index  = packet[1];
      int middle = packet[2];
      int flex   = packet[3];
      int temp = packet[4];

      Serial.printf("Thumb:%d Index:%d Middle:%d Flex:%d Temp:%d\n", thumb, index, middle, flex, temp/10.0);

      setVibrationMotor(M1_PIN, thumb);
      setVibrationMotor(M2_PIN, index);
      setVibrationMotor(M3_PIN, middle);
      setServoAngle(flexServo, flex);
      updateTempLEDs(LED_HOT_PIN, LED_NORMAL_PIN, temp);
    } else {
      Serial.printf("Unexpected packet size: %d bytes\n", value.length());
    }
  }
  delay(10); 
}