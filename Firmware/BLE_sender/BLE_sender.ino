#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

BLECharacteristic *pCharacteristic;

int PIN_THUMB  = 1;
int PIN_INDEX  = 2;
int PIN_MIDDLE = 3;
int PIN_FLEX   = 4;

void setup() {
  Serial.begin(115200); //turns on comm channel between chip & computer 

  BLEDevice::init("ESP32_Hand");  //turns bluetooth on and gives name 
  //Serial.println(BLEDevice::getAddress().toString().c_str()); //prints board's address - to know to put in receiver
  BLEServer *pServer = BLEDevice::createServer(); //creates the station - what broadcasts and host data
  BLEService *pService = pServer->createService(SERVICE_UUID); //creates a ''folder'' insiide the station 

  pCharacteristic = pService->createCharacteristic(  //creates file in that folder with UUId
    CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY //"other devices can read this value, and I can also push updates out automatically (notify) whenever it changes."
  );
  pCharacteristic->addDescriptor(new BLE2902());

  pService->start(); // swithces folder on - makes it active 
  pServer->getAdvertising()->start(); //starts broadcasting 
}

void loop() {
  int raw_thumb  = readSensor(PIN_THUMB);
  int raw_index  = readSensor(PIN_INDEX);
  int raw_middle = readSensor(PIN_MIDDLE);
  int raw_flex   = readSensor(PIN_FLEX);

  float filtered_thumb  = updateFilter(0, raw_thumb);
  float filtered_index  = updateFilter(1, raw_index);
  float filtered_middle = updateFilter(2, raw_middle);
  float filtered_flex   = updateFilter(3, raw_flex);

  //Serial.printf("raw: %d,%d,%d,%d  filtered: %.1f,%.1f,%.1f,%.1f\n",
 // raw_thumb, raw_index, raw_middle, raw_flex,
  //filtered_thumb, filtered_index, filtered_middle, filtered_flex); //to see the values, to have an actual range for mapping function 

  int packet[4];
  packet[0] = pressureThreshold(filtered_thumb);
  packet[1]  = pressureThreshold(filtered_index);
  packet[2] = pressureThreshold(filtered_middle);
  packet[3] = flexionThreshold(filtered_flex);

  pCharacteristic->setValue((uint8_t*)packet, sizeof(packet));
  pCharacteristic->notify();


  delay(10);
}