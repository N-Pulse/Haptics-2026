# FSR Sensitivity Test Protocol

**Note:** Increasing pressure decreases both FSR resistance and measured
voltage.

## Setting up Arduino IDE for STM32

In Arduino IDE, go to **File \> Preferences** and add the copy URL
to the **Additional Boards Manager URLs** field, then click **OK**:

https://github.com/stm32duino/BoardManagerFiles/raw/main/package_stmicroelectronics_index.json

Then:

-   Go to **Tools \> Board \> Boards Manager**, select **Contributed**
    as the type, search for **STM32 MCU based boards**, and install it.
-   Go to **Tools \> Board Part Number** and select **Nucleo G431KB**.
-   Go to **Tools \> Ports** and select the port the STM32 is connected to.

## 1. Test preparation

1.  Connect the STM32 to the computer via USB.
2.  Open Arduino IDE and upload the FSR test code.
3.  Open the Serial Monitor and set the baud rate to `9600`.
4.  Set the line ending to **No line ending**.
5.  Keep the sensor unloaded before starting each trial.

## 2. Measurement procedure

Each trial lasts approximately 6 seconds, including the initial waiting
period.

1.  **Start the trial:** Type `L`, `M`, or `H` in the Serial Monitor and
    press Enter.
2.  **Preparation period (1 second):** The code displays
    `Starting test L/M/H` and waits 1 second before collecting
    measurements. Use this time to position the prosthesis finger parallel
    to the scale and calibrate the applied force.
3.  **Measurement period (5 seconds):** The code collects 250 samples,
    one every 20 ms. Maintain the selected pressure as consistently as
    possible throughout this 5s.
4.  **Results:** After the measurements are complete, the code prints
    the voltage and resistance statistics.
5.  **Release:** Remove the finger and allow the sensor to return to
    its unloaded state before starting the next trial.

**Important:** The code does not automatically verify that pressure is
applied during the measurement period. Apply pressure during the
1-second preparation period and maintain it for the full 5 seconds of
measurement.

## 3. Pressure levels

Perform five trials for each level.

-   **Light (`L`):** ~200 g
-   **Medium (`M`):** ~400 g
-   **Heavy (`H`):** ~1000 g

## 4. Test configurations

Repeat the complete procedure for each configuration, for example.
For each configuration, perform five light, five medium, and five heavy
trials, giving 15 trials per configuration.

## 5. Data to record

For every trial, record:

-   Mean voltage (V) 
-   Voltage standard deviation (V) 
-   Mean resistance (Ω)
-   Resistance standard deviation (Ω)
-   Voltage and resistance ranges

## 6. Evaluation

Compare the average readings across the five trials for each pressure
level and configuration.

A configuration is preferable when:

-   The mean readings for light, medium, and heavy pressure are clearly
    separated.
-   The measurements are repeatable across trials.
-   The sensor does not saturate at the ADC limits.

Because the applied force is not precise, the results characterize
electrical sensitivity under manually applied pressure, not sensitivity
in V/N or Ω/N.

# 8. Exploratory Test Results

## 8.1 Experimental conditions

Exploratory measurements were performed using different configurations. In all configurations, the finger was kept parallel to the plane of the scale/balance, which was used to monitor the applied load.

The following configurations were investigated:

1. Bare FSR on Ana's anatomical finger.
2. FSR with silicone coating v1 on Ana's anatomical finger.
3. FSR with silicone coating v1 on prosthesis finger v1.
4. Bare FSR on prosthesis finger v2, featuring a bump to improve sensor attachment.
5. FSR with silicone coating v1 on prosthesis finger v2, featuring a bump to improve sensor attachment.
6. FSR with an aluminium platform and silicone coating v1 on prosthesis finger v2, featuring a bump to improve sensor attachment.

**Important experimental limitations:**

- These measurements are exploratory and were not repeated five times per pressure level.
- Results should therefore be interpreted as preliminary and used to identify promising configurations for subsequent systematic testing.
- The applied load was monitored using a scale/balance.

## 8.2 Summary of results

The following tables report the mean voltage and resistance for each pressure level. Resistance values are expressed in kΩ for readability.

### Configuration 1 – Bare FSR on anatomical finger

| Parameter | Light | Medium | Heavy |
|---|---:|---:|---:|
| Mean voltage (V) | 0.075 | 0.299 | 0.901 |
| Voltage SD (V) | 0.028 | 0.024 | 0.065 |
| Voltage range (V) | 0.103 | 0.132 | 0.288 |
| Mean resistance (kΩ) | 76.69 | 15.82 | 4.58 |
| Resistance SD (kΩ) | 76.13 | 1.45 | 0.42 |
| Resistance range (kΩ) | 974.29 | 7.94 | 1.84 |

### Configuration 2 – FSR with silicone coating v1 on anatomical finger

| Parameter | Light | Medium | Heavy |
|---|---:|---:|---:|
| Mean voltage (V) | 0.004 | 0.190 | 1.377 |
| Voltage SD (V) | 0.015 | 0.020 | 0.027 |
| Voltage range (V) | 0.088 | 0.078 | 0.176 |
| Mean resistance (kΩ) | 105.03 | 25.57 | 2.63 |
| Resistance SD (kΩ) | 77.71 | 2.96 | 0.074 |
| Resistance range (kΩ) | 284.17 | 11.86 | 0.48 |

### Configuration 3 – FSR with silicone coating v1 on prosthesis finger v1

| Parameter | Light | Medium | Heavy |
|---|---:|---:|---:|
| Mean voltage (V) | 0.000 | 0.254 | 1.138 |
| Voltage SD (V) | 0.000 | 0.041 | 0.057 |
| Voltage range (V) | 0.000 | 0.200 | 0.303 |
| Mean resistance (kΩ) | Invalid | 19.14 | 3.41 |
| Resistance SD (kΩ) | Invalid | 3.01 | 0.24 |
| Resistance range (kΩ) | Invalid | 14.53 | 1.31 |

### Configuration 4 – Bare FSR on prosthesis finger v2 (with attachment bump)

| Parameter | Light | Medium | Heavy |
|---|---:|---:|---:|
| Mean voltage (V) | 0.000 | 0.185 | 0.378 |
| Voltage SD (V) | 0.000 | 0.020 | 0.031 |
| Voltage range (V) | 0.000 | 0.083 | 0.108 |
| Mean resistance (kΩ) | Invalid | 26.28 | 12.31 |
| Resistance SD (kΩ) | Invalid | 2.90 | 1.15 |
| Resistance range (kΩ) | Invalid | 12.33 | 4.20 |

### Configuration 5 – FSR with silicone coating v1 on prosthesis finger v2 (with attachment bump)

| Parameter | Light | Medium | Heavy |
|---|---:|---:|---:|
| Mean voltage (V) | 0.242 | 0.488 | 1.199 |
| Voltage SD (V) | 0.012 | 0.066 | 0.029 |
| Voltage range (V) | 0.044 | 0.205 | 0.147 |
| Mean resistance (kΩ) | 19.75 | 9.43 | 3.17 |
| Resistance SD (kΩ) | 1.07 | 1.33 | 0.10 |
| Resistance range (kΩ) | 3.79 | 4.14 | 0.53 |

**Observations:**

- Unlike the previous prosthesis configurations, this configuration successfully registers light pressure.
- The mean voltage increases progressively from light to medium and heavy pressure.
- The three pressure levels produce clearly separated mean voltage readings.


### Configuration 6 – FSR with aluminium platform and silicone coating v1 on prosthesis finger v2 (with attachment bump)

| Parameter | Light | Medium | Heavy |
|---|---:|---:|---:|
| Mean voltage (V) | 0.045 | 0.288 | 0.474 |
| Voltage SD (V) | 0.024 | 0.015 | 0.080 |
| Voltage range (V) | 0.093 | 0.059 | 0.279 |
| Mean resistance (kΩ) | 133.52 | 16.39 | 9.87 |
| Resistance SD (kΩ) | 137.56 | 0.88 | 1.89 |
| Resistance range (kΩ) | 969.16 | 3.44 | 7.02 |

**Observations:**

- The sensor successfully registers all three pressure levels.
- The voltage difference between medium and heavy pressure is relatively small compared with Configuration 5.
- The aluminium platform may influence the mechanical transmission and distribution of force to the FSR. Its effect should be investigated further through repeated measurements.

## 8.3 Comparison and preliminary evaluation

The main findings are:

- **Prosthesis finger v2 with silicone-coated FSR:** This configuration provides the most promising preliminary results. It successfully detects all three pressure levels, with clearly distinguishable mean voltage readings.

- **Prosthesis finger v2 with aluminium platform and silicone-coated FSR:** This configuration also detects all three pressure levels. However, the voltage separation between medium and heavy pressure is smaller, and the light-pressure measurements exhibit substantial variability.

Based on these exploratory measurements, **the silicone-coated FSR mounted on prosthesis finger v2, using the attachment bump without the aluminium platform (Configuration 5), is the preferred configuration for further testing.**

It provides the clearest separation between the three pressure levels among the tested prosthesis configurations, while maintaining relatively low voltage variability at light and heavy pressure.

## 8.4 Next steps

The following tests should be performed on the preferred configuration:

- Perform five independent trials for each pressure level (light, medium, heavy).
- Maintain the target loads of approximately 200 g, 400 g and 1 kg using the scale.
- Record the complete voltage and resistance statistics for every trial.
- Verify that light pressure consistently produces a non-zero ADC reading.
- Evaluate the separation between pressure levels and the repeatability of measurements across trials.
- Investigate whether the attachment bump and silicone coating provide consistent mechanical contact across repeated loading cycles.
- If relevant, repeat the measurements with the aluminium platform to determine whether its effect on force distribution is consistent.

The final configuration should be selected based on repeatability and distinguishability of pressure levels, rather than on a single exploratory measurement.

**Note:** The SD values in the Arduino output are calculated over the 250 samples collected during each 5-second measurement. They do not represent trial-to-trial repeatability. Five independent repetitions are therefore still necessary before drawing conclusions about sensor reliability.