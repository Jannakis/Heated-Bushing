## Overview
This repository contains a prototype heating and closed-loop temperature control system designed to keep a mechanical bushing at a stable target temperature. The focus is reliable setpoint tracking and improved temperature uniformity across the component.

## Hardware
- 3D-printed bushing with integrated heating cavities/channels filled with conductive silver paste for resistive (Joule) heating
- DC power supply for heating power
- PWM power stage using a logic-level MOSFET (e.g., IRLZ44N) driven directly from an Arduino (5 V)
- Temperature sensing with an NTC thermistor (GA2K7MCD1) in a voltage-divider measurement chain

## Control Strategy
- Step-response experiments were used to characterize the thermal behavior (PT1-like / first-order)
- Classical PID tuning approaches (Ziegler–Nichols, Cohen–Coon) were tested but produced excessive overshoot
- A PI controller was chosen as a simpler and more robust solution for this thermal system
- Anti-windup: integral action is enabled only above a defined temperature threshold to reduce accumulation during warm-up

## Results
- Stable regulation to a defined temperature setpoint was achieved in most test cases
- The system can maintain temperature reliably using PWM power control and sensor feedback
- Limitations and improvement potential are documented (sensor placement, test stand consistency, temperature uniformity)

## Lessons Learned / Improvement Ideas
- Integrating the sensor closer to the thermal hotspot would reduce measurement delay
- A more consistent test setup would reduce ambient disturbances (e.g., airflow, opened windows, people passing by)
- Thermal design adjustments (e.g., denser heating paths near high-loss edges) could improve uniformity
