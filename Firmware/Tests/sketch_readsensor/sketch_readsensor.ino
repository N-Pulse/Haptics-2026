// Lectura de sensor de presión resistivo (FSR) mediante divisor de voltaje
// Conexión: 
//   5V (o 3.3V en STM32) -> Sensor FSR -> Pin analógico -> Resistencia fija (10kΩ) -> GND

const int pinSensor = A1;      // Pin analógico de lectura
const int pinSensor2 = A0;      // Pin analógico de lectura

const float voltajeRef = 5;  // 3.3 si usas STM32 (ajusta según tu placa)
const int resolucionADC = 1023; // 1023 para Arduino (10 bits), 4095 para STM32 (12 bits)
const float resistenciaFija = 1000.0; // Valor de la resistencia del divisor (ohms)s

void setup() {
  Serial.begin(9600);
}

void loop() {
  int lecturaADC = analogRead(pinSensor);
  int lecturaADCraw = analogRead(pinSensor2);

  
  // Convertir lectura a voltaje
  float voltaje = lecturaADC * (voltajeRef / resolucionADC);
  
  // Evitar división por cero cuando no hay presión (voltaje ~0)
  float resistenciaFSR;
  if (voltaje > 0.01) {
    resistenciaFSR = resistenciaFija * (voltajeRef - voltaje) / voltaje;
  } else {
    resistenciaFSR = -1; // Sin contacto / resistencia muy alta
  }
  
  //Serial.print("ADC: ");
  //Serial.print(lecturaADC);
 // Serial.print("ADC raw: ");
  //Serial.print(lecturaADCraw);
  //Serial.print("  |  Voltaje: ");
  Serial.println(voltaje, 3);
  //Serial.print(" V  |  Resistencia FSR: ");
  //if (resistenciaFSR > 0) {
  //  Serial.print(resistenciaFSR, 0);
  //  Serial.println(" ohms");
 // } else {
  //  Serial.println("N/A (sin presión)");
  //}

  delay(100);
}