// Returns true if more time than timeoutMs has passed since the last valid packet
bool checkDataTimeout(unsigned long now, unsigned long lastPacketTime, unsigned long timeoutMs) {
  return (now - lastPacketTime > timeoutMs);
}

// Attempts to reconnect to the sender, respecting a cooldown so we don't spam connect() calls
void attemptReconnect(unsigned long now) {
  if (now - lastReconnectAttempt > RECONNECT_COOLDOWN_MS) {
    Serial.println("Attempting to reconnect...");
    lastReconnectAttempt = now;

    if (pClient->connect(handAddress)) { //runs if connection has succeeded
      if (!bindCharacteristics()) {
        // Connected but can't find the characteristics: drop and retry next cooldown
        Serial.println("Bind failed, disconnecting to retry");
        pClient->disconnect();
      }
    }
  }
}