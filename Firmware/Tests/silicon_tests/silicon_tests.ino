
#include <math.h>

// FSR connected to A1 in a voltage divider:
// 5V -> FSR -> A1 -> fixed resistor (1 kOhm) -> GND

const int pinSensor = A1;          // Analog pin connected to the FSR
const float voltajeRef = 3.3;      // ADC reference voltage in volts
const int resolucionADC = 4095;    // Maximum ADC value for a 10-bit Arduino
const float resistenciaFija = 1000.0; // Fixed resistor value in ohms

const int nSamples = 250;          // Number of measurements per test
const int sampleDelayMs = 20;      // Delay between measurements (ms)
                                   // Total measurement time: approximately 5 s

void setup() {
  Serial.begin(9600);              // Start serial communication

  Serial.println("FSR sensitivity test");
  Serial.println("Instructions:");
  Serial.println("1. Enter L for light, M for medium, or H for heavy.");
  Serial.println("2. Apply the selected pressure when the test starts.");
  Serial.println("3. Maintain the pressure as consistently as possible.");
  Serial.println("4. Release the sensor after the test finishes.");
  Serial.println("5. Repeat each level at least 5 times.");
  Serial.println("6. Repeat the procedure for each sensor configuration.");
}

void loop() {

  // Wait until a character is received through the Serial Monitor.
  if (Serial.available() == 0) return;

  // Read the requested pressure level:
  // L = light, M = medium, H = heavy.
  char testLevel = Serial.read();

  // Ignore any character other than L, M, or H.
  if (testLevel != 'L' && testLevel != 'M' && testLevel != 'H' &&
      testLevel != 'l' && testLevel != 'm' && testLevel != 'h') {
    return;
  }

  // Convert lowercase input to uppercase.
  testLevel = toupper(testLevel);

  // Initialize accumulators for voltage statistics.
  float sumV = 0, sumV2 = 0;
  float minV = 1e6, maxV = -1e6;

  // Initialize accumulators for resistance statistics.
  float sumR = 0, sumR2 = 0;
  float minR = 1e12, maxR = -1;
  int validR = 0;

  // Display the selected test level.
  Serial.print("Starting test ");
  Serial.println(testLevel);

  // Wait 1 second to give you time to prepare your finger.
  // Apply the selected pressure during the measurement period.
  delay(1000);

  // Acquire 250 measurements over approximately 5 seconds.
  for (int i = 0; i < nSamples; i++) {

    // Read the analog voltage as a digital value (0-1023).
    int adc = analogRead(pinSensor);

    // Convert the ADC reading to voltage.
    float voltage = adc * voltajeRef / resolucionADC;

    // Update voltage statistics.
    sumV += voltage;                 // Sum for calculating the mean
    sumV2 += voltage * voltage;      // Sum of squared values
    if (voltage < minV) minV = voltage;
    if (voltage > maxV) maxV = voltage;

    // Estimate FSR resistance using the voltage-divider equation.
    // For this circuit:
    // R_FSR = R_fixed * (V_ref - V_measured) / V_measured
    //
    // Using the ADC counts directly avoids an extra voltage conversion.
    // If adc is zero, the resistance cannot be calculated reliably.
    if (adc > 0) {
      float resistance = resistenciaFija *
                         (resolucionADC - adc) / (float)adc;

      // Update resistance statistics.
      sumR += resistance;
      sumR2 += resistance * resistance;
      if (resistance < minR) minR = resistance;
      if (resistance > maxR) maxR = resistance;
      validR++;
    }

    // Wait 20 ms before the next measurement.
    delay(sampleDelayMs);
  }

  // Calculate the mean voltage and standard deviation.
  float meanV = sumV / nSamples;
  float varianceV = sumV2 / nSamples - meanV * meanV;
  float stdV = sqrt(max(0.0, varianceV));

  // Print voltage results.
  Serial.println("--- RESULTS ---");
  Serial.print("Level: ");
  Serial.println(testLevel);

  Serial.print("Voltage mean (V): ");
  Serial.println(meanV, 3);

  Serial.print("Voltage min (V): ");
  Serial.println(minV, 3);

  Serial.print("Voltage max (V): ");
  Serial.println(maxV, 3);

  // Variation within the 5-second measurement period.
  Serial.print("Voltage range (V): ");
  Serial.println(maxV - minV, 3);

  // Lower SD means less fluctuation during the test.
  Serial.print("Voltage SD (V): ");
  Serial.println(stdV, 3);

  // Calculate and print resistance statistics if valid readings exist.
  if (validR > 0) {

    float meanR = sumR / validR;
    float varianceR = sumR2 / validR - meanR * meanR;
    float stdR = sqrt(max(0.0, varianceR));

    Serial.print("Resistance mean (ohm): ");
    Serial.println(meanR, 1);

    Serial.print("Resistance min (ohm): ");
    Serial.println(minR, 1);

    Serial.print("Resistance max (ohm): ");
    Serial.println(maxR, 1);

    // Variation in resistance during the measurement period.
    Serial.print("Resistance range (ohm): ");
    Serial.println(maxR - minR, 1);

    Serial.print("Resistance SD (ohm): ");
    Serial.println(stdR, 1);

  } else {
    // ADC = 0 means the voltage is too low for this calculation.
    Serial.println("Resistance: invalid (ADC = 0)");
  }

  // Test completed. Release the sensor and prepare for the next test.
  Serial.println("Release the sensor.");
  Serial.println("Enter L, M, or H for the next test.");
}
