//config for the vibration motor drivers
const int PWM_FREQ = 5000;
const int PWM_RES  = 8; // 8-bit -> duty 0-255

// ---------- Pulsing config ----------
// Vibration motors: ON at the pressure-mapped intensity, then OFF, repeating
const unsigned long VIB_PULSE_ON_MS  = 400;  //duration of vibration
const unsigned long VIB_PULSE_OFF_MS = 150; //entire cycle is 550ms //duration of pause 

// ---------- Servo smoothing config ----------
// Servo: no pulsing, it moves toward the target angle in small steps //max servo speed: 0.3 deg/ms - 3ms/deg 
const int           SERVO_STEP_DEG     = 1;   // degrees moved at a time  //larger steps are faster but less smooth
const unsigned long SERVO_STEP_MS      = 10;  // test for 6 // time between steps (1 deg / 15 ms ~ 180 deg in 2.7 s).    //larger = slower , smaller = faster
const int           SERVO_DEADBAND_DEG = 3;   // ignore target changes smaller than this while still //if twitches , raise 

// ---------- Internal state (targets are applied in updatePulses) ----------
const int NUM_VIB = 3;
int vibPins[NUM_VIB]       = {-1, -1, -1};
int vibTargetDuty[NUM_VIB] = {0, 0, 0};   // 0 = motor off, no pulsing

Servo* pulseServo         = nullptr;
int    servoTargetAngle   = 90;
int    servoCurrentAngle  = 90;
bool   servoPulseActive   = false;   // false after a safety shutoff
bool   servoMoving        = false;
unsigned long lastServoStepMs = 0;

// true during the ON part of a pulse cycle (all motors pulse in sync)
static bool pulsePhaseOn(unsigned long now, unsigned long onMs, unsigned long offMs) {
  return (now % (onMs + offMs)) < onMs; //the position insde the current cycle 
}

//finds which motor has the given pin nb returns -1 if not
static int vibIndexForPin(int pin) { 
  for (int i = 0; i < NUM_VIB; i++) if (vibPins[i] == pin) return i;
  return -1;
}

void setupMotors(int m1Pin, int m2Pin, int m3Pin, int m4Pin, Servo &servo) {
  servo.setPeriodHertz(50);
  servo.attach(m4Pin, 500, 2400); // double check pulse range if servo is diffferent
  pulseServo = &servo;
  servo.write(90);                // start at the neutral angle
  servoCurrentAngle = 90;
  servoTargetAngle  = 90;

  vibPins[0] = m1Pin;
  vibPins[1] = m2Pin;
  vibPins[2] = m3Pin;
  for (int i = 0; i < NUM_VIB; i++) {
    ledcAttach(vibPins[i], PWM_FREQ, PWM_RES);
    ledcWrite(vibPins[i], 0);
  }
}

// Moves the servo toward its target in small steps (no pulsing)
void updateServo() {
  if (pulseServo == nullptr || !servoPulseActive) return;

  int diff = servoTargetAngle - servoCurrentAngle;

  // start moving only when the target is far enough away (ignores noise)
  if (!servoMoving && abs(diff) >= SERVO_DEADBAND_DEG) servoMoving = true;
  if (!servoMoving) return;

  if (diff == 0) { servoMoving = false; return; }   // arrived: hold still

  unsigned long now = millis();
  if (now - lastServoStepMs < SERVO_STEP_MS) return;
  lastServoStepMs = now;

  int step = min(abs(diff), SERVO_STEP_DEG);
  servoCurrentAngle += (diff > 0) ? step : -step;
  pulseServo->write(servoCurrentAngle);
}

// Call EVERY loop() so the pulses keep their timing even between BLE packets
void updatePulses() {
  unsigned long now = millis(); //the current time

  // Vibration motors
  bool vibOn = pulsePhaseOn(now, VIB_PULSE_ON_MS, VIB_PULSE_OFF_MS); //if in ON part of cycle 
  for (int i = 0; i < NUM_VIB; i++) {
    if (vibPins[i] < 0) continue;
    ledcWrite(vibPins[i], vibOn ? vibTargetDuty[i] : 0); //writes target when ON and 0 when OFF
  }

  // Servo (moves gradually, no pulsing)
  updateServo();
}

// Sets the pulse intensity of one vibration motor from a raw pressure value
void setVibrationMotor(int pin, int rawValue) { //looks up motor for that pin - does nothing if unkown
  int i = vibIndexForPin(pin);
  if (i < 0) return;
  vibTargetDuty[i] = mapToVibrationDuty(rawValue);
  updatePulses(); // apply right away, timing continues in loop()
}

// Sets the target angle from a raw flexion value (the servo moves there gradually)
void setServoAngle(Servo &servo, int rawValue) {
  pulseServo = &servo;
  servoTargetAngle = mapToServoAngle(rawValue);
  servoPulseActive = true;
}

// Immediately stops all actuators — called when the safety timeout triggers
// Also clears the targets so updatePulses() does not restart anything
// (the servo goes to 90 at once, not gradually: safety comes first)
void shutdownActuators(int m1Pin, int m2Pin, int m3Pin, Servo &servo) {
  for (int i = 0; i < NUM_VIB; i++) vibTargetDuty[i] = 0;
  ledcWrite(m1Pin, 0);
  ledcWrite(m2Pin, 0);
  ledcWrite(m3Pin, 0);

  servoPulseActive  = false;
  servoMoving       = false;
  servoTargetAngle  = 90;
  servoCurrentAngle = 90;
  servo.write(90); // double check if that's the neutral angle actually 
}