## Overview
This repository contains a prototype heating and closed-loop temperature control system designed to keep a mechanical bushing at a stable target temperature. The focus is reliable setpoint tracking and improved temperature uniformity across the component.

## Hardware
- 3D-printed bushing with integrated heating cavities/channels filled with conductive silver paste for resistive (Joule) heating
- DC power supply for heating power
- PWM power stage using a logic-level MOSFET (e.g., IRLZ44N) driven directly from an Arduino (5 V)
- Temperature sensing with an NTC thermistor (GA2K7MCD1) in a voltage-divider measurement chain
<img width="535" height="421" alt="grafik" src="https://github.com/user-attachments/assets/4c7cb4f1-28a0-44ab-bd33-aaa3cfd3669b" />


## Wiring
<img width="971" height="427" alt="grafik" src="https://github.com/user-attachments/assets/30a0f34d-0683-450e-8673-bfcfab30dec0" />
<img width="388" height="378" alt="grafik" src="https://github.com/user-attachments/assets/d8351435-de2a-4b66-a796-7f1531516db0" />


## Control Strategy
- Step-response experiments were used to characterize the thermal behavior (PT1-like / first-order)
- Classical PID tuning approaches (Ziegler–Nichols, Cohen–Coon) were tested but produced excessive overshoot
- A PI controller was chosen as a simpler and more robust solution for this thermal system
- Anti-windup: integral action is enabled only above a defined temperature threshold to reduce accumulation during warm-up

## Results
- Stable regulation to a defined temperature setpoint was achieved in most test cases
- The system can maintain temperature reliably using PWM power control and sensor feedback
- Limitations and improvement potential are documented (sensor placement, test stand consistency, temperature uniformity)
<img width="784" height="467" alt="grafik" src="https://github.com/user-attachments/assets/1f3b29f4-6ba0-48ab-b7b8-6f6fb8d18c25" />

