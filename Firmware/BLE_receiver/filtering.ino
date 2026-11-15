#define FILTER_WINDOW 5
#define NUM_SENSORS 4 // thumb, index, middle, flex

float filterBuffers[NUM_SENSORS][FILTER_WINDOW] = {{0}};
int filterIndex[NUM_SENSORS] = {0};
int filterCount[NUM_SENSORS] = {0}; // tracks how many real samples we've seen so far, per sensor

// sensorIndex: 0=thumb, 1=index, 2=middle, 3=flex
float updateFilter(int sensorIndex, int rawValue) {
  filterBuffers[sensorIndex][filterIndex[sensorIndex]] = rawValue;
  filterIndex[sensorIndex] = (filterIndex[sensorIndex] + 1) % FILTER_WINDOW;
  if (filterCount[sensorIndex] < FILTER_WINDOW) filterCount[sensorIndex]++;

  float sum = 0;
  for (int i = 0; i < filterCount[sensorIndex]; i++) {
    sum += filterBuffers[sensorIndex][i];
  }
  return sum / filterCount[sensorIndex]; // average of however many samples we have (up to 5)
}