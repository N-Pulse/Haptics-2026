//config for the vibration motor drivers
const int PWM_FREQ = 5000;
const int PWM_RES  = 8; // 8-bit -> duty 0-255

void setupMotors(int m1Pin, int m2Pin, int m3Pin, int m4Pin, Servo &servo) {
  ledcAttach(m1Pin, PWM_FREQ, PWM_RES);
  ledcAttach(m2Pin, PWM_FREQ, PWM_RES);
  ledcAttach(m3Pin, PWM_FREQ, PWM_RES);

  servo.setPeriodHertz(50);
  servo.attach(m4Pin, 500, 2400); // double check pulse range if servo is diffferent 
}

// Drives one vibration motor given its pin and a raw pressure value
void setVibrationMotor(int pin, int rawValue) {
  int duty = mapToVibrationDuty(rawValue);
  ledcWrite(pin, duty);
}

// Drives the flexion/extension servo given a raw flexion value
void setServoAngle(Servo &servo, int rawValue) {
  int angle = mapToServoAngle(rawValue);
  servo.write(angle);
}