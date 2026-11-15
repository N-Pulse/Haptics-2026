int noise_floor = 100;   //modify when calibrated with real sensors
int max_pressure = 4000;  //to modify
int min_flexion = 200; //to modify
int max_flexion = 3800; //to modify 

int pressureThreshold(float value) {
  if (value < noise_floor) return 0;
  if (value > max_pressure) return max_pressure;
  return (int)value;
}

int flexionThreshold(float value) {
  if (value < min_flexion) return min_flexion;
  if (value > max_flexion) return max_flexion;
  return (int)value;
}