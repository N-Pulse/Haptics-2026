// Barrido continuo de servo SG90: 0° -> 180° -> 0°
// Placa: Nucleo-G431KB (core STM32duino)
// Conexión: señal (naranja) -> A0, VCC (rojo) -> 5V, GND (marrón) -> GND

#include <Servo.h>

Servo servo;

const int SERVO_PIN = A0;

void setup() {
  // 600-2400 us es el rango seguro del SG90
  servo.attach(SERVO_PIN, 600, 2400);
}

void loop() {
  // Hacia adelante: 0° -> 180°
  for (int angle = 0; angle <= 180; angle++) {
    servo.write(angle);
    delay(10);   // menor delay = más rápido
  }

  // Hacia atrás: 180° -> 0°
  for (int angle = 180; angle >= 0; angle--) {
    servo.write(angle);
    delay(10);
  }
}
