// ACK_CHAR_UUID must match what the sender team creates. Placeholder for now.
#define ACK_CHAR_UUID "PUT-SAME-UUID-AS-SENDER-HERE" //so receiver can also send things 

const unsigned long HEARTBEAT_INTERVAL_MS = 100;
const unsigned long ACK_MIN_INTERVAL_MS   = 20;   // don't flood BLE

enum MsgType : uint8_t { MSG_ACK = 0x01, MSG_HEARTBEAT = 0x02 }; //each sotred in 1 byte 

// 8 bytes. The sender must use the exact same struct.
struct __attribute__((packed)) StatusMsg {
  uint8_t  type;        // ACK or hearbeat
  uint8_t  flags;       // bit0 = actuators shut off by safety timeout //so only bite 0 counts - others are if add any other status later
  uint16_t ackedPacket;     // packets received so far 
  uint32_t timestampMs; // receiver millis()
};

BLERemoteCharacteristic* pAckChar = nullptr;
static unsigned long lastHeartbeatMs = 0;
static unsigned long lastAckMs = 0;
static uint16_t lastAckedPacket = 0;  

// Call once after connecting, with the same service you read the data from.
bool setupComms(BLERemoteService* service) { //looks for ack mailbox of sender & checks if has permission to modify it
  pAckChar = service->getCharacteristic(BLEUUID(ACK_CHAR_UUID));
  if (pAckChar == nullptr || !pAckChar->canWriteNoResponse()) {
    Serial.println("Ack characteristic missing or not writable");
    pAckChar = nullptr;
    return false;
  }
  return true;
}

static void sendStatus(uint8_t type, bool shutOff) {
  if (pAckChar == nullptr) return;
  StatusMsg msg;
  msg.type = type;
  msg.flags = shutOff ? 0x01 : 0x00;
  msg.ackedPacket = lastAckedPacket;
  msg.timestampMs = millis();
  pAckChar->writeValue((uint8_t*)&msg, sizeof(msg), false);  // false = no response
}

// Call each time a valid data packet has been read.
void onDataReceived(bool shutOff, uint16_t packetNumber) {
  lastAckedPacket = packetNumber;
  unsigned long now = millis();
  if (now - lastAckMs >= ACK_MIN_INTERVAL_MS) {
    sendStatus(MSG_ACK, shutOff);
    lastAckMs = now;
  }
}

// Call every loop() while connected.
void updateHeartbeat(bool shutOff) {
  unsigned long now = millis();
  if (now - lastHeartbeatMs >= HEARTBEAT_INTERVAL_MS) {
    sendStatus(MSG_HEARTBEAT, shutOff);
    lastHeartbeatMs = now;
  }
}
void resetComms() { pAckChar = nullptr; lastAckedPacket = 0; }