
// will be replaced with real min/max once you've tested actual sensors on hardware
const int PRESSURE_MIN = 0;
const int PRESSURE_MAX = 4000;
const int FLEX_MIN = 200;
const int FLEX_MAX = 3800;

// Maps a value to a vibration motor PWM duty cycle (0-255)
int mapToVibrationDuty(int rawValue) {
  int duty = map(rawValue, PRESSURE_MIN, PRESSURE_MAX, 0, 255);
  return constrain(duty, 0, 255); //to guarantee value is between the correct range
}

// Maps a flexion value to a servo angle (0-180 degrees)
int mapToServoAngle(int rawValue) {
  int angle = map(rawValue, FLEX_MIN, FLEX_MAX, 0, 180);
  return constrain(angle, 0, 180);
}