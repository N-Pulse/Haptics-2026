////maybe not needed here just in case
#define WINDOW_SIZE 5

int readings[4][WINDOW_SIZE];
int nextSlot[4] = {0, 0, 0, 0};
int count[4] = {0, 0, 0, 0};

float updateFilter(int sensorIndex, int sensorValue) {
  readings[sensorIndex][nextSlot[sensorIndex]] = sensorValue;
  nextSlot[sensorIndex] = (nextSlot[sensorIndex] + 1) % WINDOW_SIZE;
  if (count[sensorIndex] < WINDOW_SIZE) count[sensorIndex]++;

  long sum = 0;
  for (int i = 0; i < count[sensorIndex]; i++) {
    sum += readings[sensorIndex][i];
  }
  return (float)sum / count[sensorIndex];
}