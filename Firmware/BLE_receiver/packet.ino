//reads and validates data packets from the sender

const uint8_t PACKET_START_BYTE = 0xA5;

struct __attribute__((packed)) PacketHeader {
  uint8_t  start;          // always 0xA5, marks the beginning of a packet //1 byte
  uint16_t packetNumber;   // +1 for every packet sent, wraps at 65535 --> for packet counting //2 bytes
  uint8_t  pressureBytes;  // bytes of pressure data after the header (3 sensors x 2 = 6) //1 byte (for the header aka 6)
  uint8_t  wristBytes;     // bytes of wrist data after that (1 sensor x 2 = 2) //1 byte (for the header)
};  // 5 bytes, then 6 + 2 data bytes = 13 bytes total


// Fills packet[4] = thumb, index, middle, flex. Sets packetNumber for PKT_OK.
PacketResult parsePacket(const String &value, int packet[4], uint16_t &packetNumber) {
  size_t len = value.length();

  // old sender: 4 raw ints (16 bytes), no header
  if (len == sizeof(int) * 4) {
    memcpy(packet, value.c_str(), sizeof(int) * 4);
    return PKT_LEGACY;
  }

  if (len < sizeof(PacketHeader)) return PKT_BAD_SIZE;

  PacketHeader hdr;
  memcpy(&hdr, value.c_str(), sizeof(hdr));

  if (hdr.start != PACKET_START_BYTE) return PKT_BAD_START;
  if (hdr.pressureBytes != 6 || hdr.wristBytes != 2) return PKT_BAD_LENGTHS;
  if (len != sizeof(PacketHeader) + hdr.pressureBytes + hdr.wristBytes) return PKT_BAD_SIZE;

  const uint8_t *data = (const uint8_t *)value.c_str() + sizeof(PacketHeader);
  for (int i = 0; i < 4; i++) {
    uint16_t v;
    memcpy(&v, data + i * 2, 2);
    packet[i] = v;
  }
  packetNumber = hdr.packetNumber;
  return PKT_OK;
}

// ---------- packet counting ----------
uint32_t totalPacketsReceived = 0;
uint32_t totalPacketsLost = 0;
uint32_t totalBadPackets = 0;
static bool     havePrevPacket = false;
static uint16_t lastPacketNumber = 0;
static uint8_t  oldPacketStreak = 0;

// Returns true if this is a NEW packet, false if it is a repeat or an old one.
bool trackPacket(uint16_t n) {
  if (!havePrevPacket) {
    havePrevPacket = true;
    lastPacketNumber = n;
    totalPacketsReceived++;
    return true;
  }
  uint16_t diff = (uint16_t)(n - lastPacketNumber);   // wraps around correctly at 65535
  if (diff == 0) return false;                         // same packet read again (we poll faster than the sender sends)
  if (diff >= 0x8000) {                                // looks older than the last one
    if (++oldPacketStreak < 5) return false;           // probably a late or repeated packet: ignore
    oldPacketStreak = 0;                               // 5 in a row: the sender restarted, resync
    lastPacketNumber = n;
    totalPacketsReceived++;
    return true;
  }
  oldPacketStreak = 0;
  if (diff > 1) totalPacketsLost += diff - 1;          // skipped numbers = lost packets
  lastPacketNumber = n;
  totalPacketsReceived++;
  return true;
}

void resetPacketTracking() { havePrevPacket = false; oldPacketStreak = 0; }  // call on disconnect

void countBadPacket() { totalBadPackets++; }

// Prints the counters every 5 seconds
void printPacketStats() {
  static unsigned long lastPrintMs = 0;
  unsigned long now = millis();
  if (now - lastPrintMs < 5000) return;
  lastPrintMs = now;
  uint32_t total = totalPacketsReceived + totalPacketsLost;
  float lossPct = total > 0 ? 100.0 * totalPacketsLost / total : 0;
  Serial.printf("Packets rx:%u lost:%u bad:%u loss:%.1f%%\n",
                totalPacketsReceived, totalPacketsLost, totalBadPackets, lossPct);
}