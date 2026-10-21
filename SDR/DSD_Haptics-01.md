# Design Solution Definition: `Haptics`

> **Project:** `Haptics`
> **Document ID:** `DSD-01-Haptics`
> **Version:** `0.1.0` · **Status:** `Draft`
> **Project phase:** `Proof of concept`
> **Last modified:** `2026-10-17`
> **Repository path:** `Haptics-2026/SDR/DSD_Haptics-01.md`


HOW TO USE THIS TEMPLATE
- Replace every `<placeholder>`. Delete guidance comments before review.
- One DSD per subsystem or module
- The DSD must stand alone: a reviewer should be able to judge the design
  from this document, the linked requirements, and the linked evidence.
- Normative language: "shall" = requirement, "should" = recommendation,
  "may" = optional.

---

## Document control

### Roles and sign-off

| Role | Name | Affiliation / Handle | Date | Signature / Review link |
|---|---|---|---|---|
| Author(s) | Ana Benavente | Team Leader / anabenaventez-hub | | |
| Reviewer (technical) | | | | |
| Concurrence (SE Reviewer) | | | | |
| Concurrence (Quality management reviewer) | | | | |
| Approval (Project Manager) | | | | |

> Note : Link approval to any relevant PR (Pull Request)
> Affiliation/Handle : Role/ Username on Github

### Modification log

| Version | Date | Author(s) | Main changes | PR / Commit |
|---|---|---|---|---|
| 0.1.0 | 2026-10-17 | Ana Benavente | First version | |

---

## Table of contents

1. [Introduction](#1-introduction)
2. [Requirements and traceability](#2-requirements-and-traceability)
3. [Concept justification](#3-concept-justification)
4. [Traded-off concepts](#4-traded-off-concepts)
5. [Quality management and compliance](#5-quality-management-and-compliance) /!\ THIS SECTION IS NOT FINALISED /!\
6. [Licensing](#6-open-source-considerations) /!\ THIS SECTION IS NOT FINALISED /!\
7. [Open issues and future work](#7-open-issues-and-future-work)
8. [References](#8-references)
9. [Appendices](#9-appendices)

---

## 1. Introduction

### 1.1 Purpose and scope

<!-- Why does this document exist, at which review/milestone is it produced,
and what decision does it support? State what is IN and OUT of scope. 

DO NOT CHANGE ANYTHING IN THIS SECTION EXCEPT THE HIGHLIGHTED SECTIONS -->

This Design Solution Definition (DSD) is at a `Proof of concept` stage
validated on `<date>`. It justifies the concept selected for `Haptics`.

It covers:

- the problem the subsystem must solve
- the retained concept: architecture, components, and design choices
- the concepts that were considered and not retained
- risks, quality, and compliance implications.

**Out of scope:** `Not relevant.`

### 1.2 Responsibility and change authority

<!-- Who owns this document? Who may approve changes? How are changes
proposed (issue, RFC, pull request)? -->

- **Document owner:** `Ana Benavente / Haptics` <!-- Who is responsible to keep this document up to date ? --> 
- **Change authority:** `Team Leader / N-Pulse` <!-- What is the role/responsibility of the person -->


### 1.3 Abbreviated terms and definitions

| Term | Definition |
|---|---|
| `BLE` | `Bluetooth Low Energy` |
| `GND` | `Ground` |
| `LED` | `Light-Emitting Diode` |
| `MCU` | `Microcontroller Unit` |
| `NTC` | `Negative Temperature Coefficient` |
| `PCB` | `Printed Circuit Board` |
| `PWM` | `Pulse Width Modulation` |

## 2. Requirements and traceability

<!-- Do not duplicate the full requirements document. Link to it and list only
the requirements this design directly addresses. -->

| Requirement ID | Verification method | Status |
|---|---|---|
| HA-001  | D  | In-progress  |
| HA-002  | I  | In-progress |
| HA-003  | T  | To do  |
| HA-004  | D  | In-progress   |
| HA-005  | T  |  In-progress  |
| HA-006  | T  | In-progress |
| HA-007  | D  | In-progress   |
| HA-008  | T  | To-do  |
| HA-009  | I  | In-progress   |
| HA-010  | T  | In-progress   |
| HA-011  | T  | To do  |
| HA-012  | T | In-progress |
| HA-013  | T | In-progress |
| SYS-041  | D  | To do |
| SYS-040  | D  | To do  |
| SYS-039  | D  | To do  |
| SYS-034 | D  | In-progress  |
| SYS-035  |  M | To do  |
| SYS-031  | T  | To do  |
| SYS-029  | T  | In-progress  |
| SYS-030 | D |  In-progress |
| SYS-028  | T  | In-progress  |
| SYS-027  | T  | To do  |
| SYS-021  | D  | To do  |
| SYS-022 | I  | To do  |
| SYS-019  |  T | In-progress  |
| SYS-020  | T  | In-progress  |
| SYS-018  | T  | To do |
| SYS-016 | T  | To do  |
| SYS-011  | D  | In-progress  |
| SYS-010  | T  | To do  |
| SYS-017  | T  | To do  |
| SAF-005 | I  | In-progress  |
| SAF-008  | T  | In-progress  |
| SAF-009 | T  | To do  |


> **Verification methods:** (D)emonstration, (T)est, (M)easurement, (I)nspection, (A)nalysis
 

---

## 3. Concept justification

`<Explain HOW the design addresses the technical problem it's trying to solve. Every major element should trace back to a need or requirement>`

<span style="color: red;">The following chapters and sub-chapters are an example. Tailor them to be the most relevant to your own system.</span>


### 3.1 Solution overview

`The retained concept is a wearable bracelet that turns the contact pressure measured at the prosthesis into vibration on the user’s skin, addressing the problem of sensation feedback. Pressure readings are sent wirelessly to the bracelet, where an MCU converts them into vibration of different intensities, so the user can feel contact without looking at the hand. Additionally, the prosthesis’s wrist extension and flexion, given by the motor position, are also sent to the bracelet, which reproduces this movement with a servomotor. The architecture was selected to meet all the requirements while keeping power consumption low, the device compact, and operation safe, comfortable and reliable for the user.`


<!-- Mermaid renders natively on GitHub. Alternatively link an image from
`docs/figures/`. Include interfaces: data, mechanical, electrical,
biological/sample flow, and user interaction. -->


```mermaid
flowchart LR
    subgraph PROS["Prosthesis"]
        S["Pressure sensors"]
        W["Wrist position"]
    end

    subgraph BRC["Haptic bracelet"]
        MCU["MCU"]
        VIB["Vibration motors"]
        SRV["Servomotor mechanism"]
        PWR["Battery and power management"]
    end

    USER["User"]

    S -->|"Pressure sensor data (BLE)"| MCU
    W -->|"Wrist motor position (BLE)"| MCU
    MCU -->|"Vibration motor control"| VIB
    MCU -->|"Servomotor control"| SRV
    VIB -->|"Vibration"| USER
    SRV -->|"Movement"| USER
    PWR -->|"Power (3.3 V)"| MCU
    PWR -->|"Power (3.3 V)"| VIB
    PWR -->|"Power (5 V)"| SRV
    USER -->|"User interaction (on/off)"| PWR

```

*Figure 3.1: High-level architecture of `Haptics`.*


### 3.2 Main design choices

<!-- One subsection per significant choice. Use this pattern:
Decision · Rationale · Requirements addressed · Consequences. -->

#### 3.2.1 `Battery and power system`

- **Decision:** `Power the bracelet using a 7.4 V 3400 mAh 2S Li-ion pack through an on/off switch with 2 buck converters (3.3 and 5 V) as well as temperature control via the thermistor.`
- **Rationale:** `Prioritize battery and user safety, as well as efficient buck conversion for stable rails and have enough headroom for the 5V rail, with a usage of minimum 4 hours.`
- **Requirements addressed:** `HA-001, HA-011, SYS-034, SYS-019, SYS-020, SYS-018, SYS-010, SAF-008.`
- **Consequences / limitations:** `Need of 2S protection board and charger, and visible warning of temperature as well as thermometer calibration.`

#### 3.2.2 `Wireless communication`

- **Decision:** `Use an ESP32-C3 Super Mini as the bracelet's MCU, receiving pressure and wrist-position data from the prosthesis via BLE.>`
- **Rationale:** `BLE is built onto the chip and is low power, removing any wires between the prosthesis and bracelet, allowing the user to manipulate the bracelet separately.`
- **Requirements addressed:** `HA-009, SYS-029, SYS-016, SYS-017.`
- **Consequences / limitations:** `Latency and range depend on BLE.`

#### 3.2.3 `Pressure sensors and circuit`

- **Decision:** `Use resistive force-sensing sensors (FSR 400) to transduce contact pressure into an analog signal, each read through a voltage divider with a fixed resistor (1 kΩ), chosen experimentally to maximize sensitivity over the force range expected at the prosthesis.`
- **Rationale:** `Thin, low-cost, responsive to light contact, and low-power.`
- **Requirements addressed:** `HA-004, HA-005, HA-006, HA-007, HA-008, HA-009, HA-010, SYS-028, SYS-021, SYS-011.`
- **Consequences / limitations:** `Need of calibration, has a non-linear response, and a sensitivity trade-off for ranges.`

#### 3.2.4 `Vibration feedback`

- **Decision:** `Render the pressure measured at the prosthesis as vibration on the skin, with the intensity increasing with the pressure.`
- **Rationale:** `Direct way of reproducing sensation by stimulating tactile receptors intuitively and perceptively, using 3V vibration motors which are low-cost, low-power, and light.`
- **Requirements addressed:** `HA-002, HA-003, HA-004, HA-005, HA-006, HA-007, HA-008, SYS-031, SYS-028, SYS-021, SYS-011.`
- **Consequences / limitations:** `Skin could adapt to continuous vibration, so sustained contact could become less noticeable over time, and intensity levels have a resolution trade-off, limited by latency.`

#### 3.2.5 `Servomotor feedback`

- **Decision:** `Render the prosthesis's wrist position (flexion/extension), given by the wrist motor position, as a movement of a servomotor mechanism on the bracelet.`
- **Rationale:** `Motion following the natural direction and position of the wrist intuitively.`
- **Requirements addressed:** `HA-002, HA-003, SYS-031.`
- **Consequences / limitations:** `Adds to power draw, size and mechanical & electrical complexity, must be comfortable and safe, and range must be matched to wrist movement also limited by latency.`


#### 3.2.6 Operating modes

<!-- For instruments: operating modes. For protocols and pipelines: workflow
stages. Include entry and exit conditions and what is active in each. -->

| Mode / Stage | Purpose | Entry condition | Exit condition | Active components |
|---|---|---|---|---|
| Idle | Wait for the prosthesis to connect with minimal power use and no output to the user.|Power switch turned on and boot finished, or BLE link lost while no fault is present.  | BLE connection established with the prosthesis (goes to Nominal) or a fault is detected (goes to Safe / fault).| MCU with BLE advertising, power management (buck converters), BLE status LED. Vibration motors, servomotor and drivers are off.|
| Nominal operation |Convert incoming pressure and wrist position data into vibration and servomotor feedback. |BLE connected and valid data being received.|BLE link lost or no data (goes to Idle), fault detected (goes to Safe / fault), or power switch off. | MCU and BLE, vibration motors with their drivers (PWM), servomotor, battery and power rails, NTC temperature monitoring. |
| Calibration | Move the servomotor to position 0 so it always starts from the same reference, matching the zero of the prosthesis’s wrist position.| Power switch turned on (runs automatically after boot), or re-homing requested by the user or over BLE.| Servomotor reaches position 0 (goes to Idle), or homing fails (goes to Safe / fault).|MCU, servomotor and its 5 V rail. Vibration motors off, BLE can start in parallel. |
| Safe / fault state | Protect the user and the hardware by stopping the outputs when battery's temperature rises or the switch is turned off.|Battery temperature above 40 °C or turned off manually. | Condition cleared (goes to Idle), or a latched fault requires the user to switch off and on. | MCU, temperature monitoring (NTC), over-temperature LED. Vibration motors and servomotor are disabled. BLE can stay on to report the fault.|

### 3.3 Functionally linked systems

#### 3.3.1 `Electronics circuits`
add schematics, pcb boards

#### 3.3.2 `Firmware`
add code diagram

#### 3.3.3 `Mechanics`
add CAD designs 

### 3.4 Performance budgets and design margins

Mass/Power/Compute/Delay budgets. Either include them here directly or link the relevant excel sheet

---

## 4. Traded-off concepts

<!-- Show your work. This section protects the project from re-litigating
decisions and records why alternatives were rejected. -->

`<This is the most important part of this document. Please fill it in with great attention. >`

`<The goal of this section is to go over every other solution you found relevant but that you chose not to pursue.>`

### 4.1 Trade-off method

The trade-off method used was based on the following criteria : 
- `<criteria nmbr 1>`
- `<criteria nmbr 2>`
- ...


`<Describe criteria, weights, and scoring scale. This is a standardized way to explicit your priorities in the design.>`

<span style='color: red;'> The table given below is an example. Adapt it with the above mentionned criterias</span>

`<example for an ADC chip for a proof of concept board>`


| Criterion | Weight (1 - 5) | Rationale for weight |
|---|---|---|
| Meets performance requirements | 5 | This ADC was chosen to be part of a first proof of concept. As such, performance is the priority. |
| Safety and biosafety | 3 | ADC is not in direct contact with the patient. While a medical approved chip is favourable, it is not mandatory |
| Cost | 3 | This is a proof of concept. The ADC is ordered in small batch and as such prices are higher. Additionally, budget is a constraint for this board  |
| Reproducibility and accessibility | 4 | Proof of concept. By definition needs to work reliabily and predictably |
| Supply chain robustness  | 2 | This ADC has high performance requirements that cannot be fulfilled with common chips. |
| Maintainability and community support | 1 | This is not expected to be replaced, changed or maintained. |

Score scale : 
> [1] : Does not meet any requirements
> [2] : Meets some requirements 
> [3] : Meets all minimum requirements 'Shall' requirements
> [4] : Meets all minimum requirements and 'should'
> [5] : Meets all requirements, 'Shall, should & may'

### 4.2 Traded-off design choices

#### 4.2.1 `<Alternative design choice A>`

- **Description:**
- **Pros**:
- **Cons**:

#### 4.2.2 `<Alternative design choice A>` score

 

| Criterion | Weight (1-5) | score (1-5) |
|---|---|---|
| Meets performance requirements | 5 | 3 |
| Safety and biosafety | 3 | 5 |
| Cost | 3 | 1 |
| Reproducibility and accessibility | 4 | 4 |
| Supply chain robustness* | 2 | 2 |
| Maintainability and community support | 1 | 1 |
| ---|---|---|
|Total Score| 54 | max score : 90 | 

#### 4.2 `<Retained design choice>` score

- **Pros**:
- **Cons**:

| Criterion | Weight (1-5) | score (1-5) |
|---|---|---|
| Meets performance requirements | 5 | 4 |
| Safety and biosafety | 3 | 3 |
| Cost | 3 | 3 |
| Reproducibility and accessibility | 4 | 4 |
| Supply chain robustness* | 2 | 3 |
| Maintainability and community support | 1 | 2 |
| ---|---|---|
|Total Score| 62 | max score : 90 | 

### 4.3 Trade-off summary

| Option | Score | Retained | Key reason |
|---|---|---|---|
| Retained concept | 62 | Yes | Better performance, lower price |
| Alternative A | 54 | No | Too expensive, lower performance|

## 5. Quality management and compliance

**WORK IN PROGRESS**
<!--
<!-- Adapt to your regulatory context. If the project is RUO-only, say so and
explain which parts apply by choice (good practice) versus by obligation. 

### 5.1 Regulatory classification and pathway

| Item | Statement |
|---|---|
| Intended use classification | `<RUO / IVD / medical device / bioprocess / environmental / other>` |
| Target jurisdictions | `<EU / US / CH / UK / other>` |
| Applicable regulation | `<e.g. EU IVDR 2017/746, EU MDR 2017/745, FDA 21 CFR 820 (QMSR), 21 CFR Part 11>` |
| Device / product class | `<class and rationale>` |
| Regulatory strategy | `<summary, notified body / agency contact status>` |

### 5.2 Quality management system

- **QMS framework:** `<ISO 13485 / ISO 9001 / GLP / GMP / GCP / lightweight in-house QMS>`
- **QMS scope and document hierarchy:** `<link>`
- **Design controls followed:** `<design inputs → outputs → review → verification → validation → transfer → changes>`
- **This document's place in the design history file (DHF):** `<link>`

### 5.3 Applicable standards and guidelines

| Standard / Guideline | Title | Applicability | Compliance status |
|---|---|---|---|
| ISO 13485 | Medical devices: Quality management systems | | Planned / Partial / Compliant / N/A |
| ISO 14971 | Application of risk management to medical devices | | |
| IEC 62304 | Medical device software life cycle | | |
| IEC 61010-1 | Safety of electrical equipment for measurement, control, laboratory use | | |
| IEC 62366-1 | Usability engineering | | |
| ISO 20387 | Biobanking: general requirements | | |
| `<other>` | | | |


### 5.4 Ethics, access, and biosecurity compliance

| Topic | Applicability | Status / reference |
|---|---|---|
| Ethics committee / IRB approval | | |
| Informed consent | | |
| Nagoya Protocol / access and benefit sharing | | |
| Biosafety committee approval | | |
| Import / export / dual-use controls | | |
| Animal research regulations | | |

-->

## 6. Licensing

**WORK IN PROGRESS**

<!--
| Topic | Decision |
|---|---|
| Hardware license | `<CERN-OHL-S / CERN-OHL-W / TAPR / other>` |
| Software license | `<Apache-2.0 / GPL-3.0 / MIT / other>` |
| Documentation and data license | `<CC-BY-4.0 / CC0 / other>` |
| Open Source Hardware certification | `<OSHWA status>` |
| Licence compatibility of all dependencies | `<checked on YYYY-MM-DD, link to SBOM>` |
| Patents / IP / prior art | `<statement, defensive publication?>` |
| Contribution and governance model | `<link to CONTRIBUTING.md / GOVERNANCE.md>` |
| Liability and warranty disclaimer | `<statement, especially for any clinical use>` |
| Sourcing for builders | `<BOM with alternates, availability of parts worldwide>` |
| Quality of community-contributed changes | `<review gates, required tests, who can merge>` |

-->

## 7. Open issues and future work

| ID | Issue / TBD / TBC | Owner | Due | Impact if unresolved |
|---|---|---|---|---|
| OI-01 | | | | |

---

## 8. References

1. `<Applicable documents (binding): requirements spec, standards, regulations>`
2. `<Reference documents (informative): papers, datasheets, prior DSDs>`

---

## 9. Appendices

### Appendix A: Bill of materials

`<link to BOM file>`

<!-- FILL With relevant items>
