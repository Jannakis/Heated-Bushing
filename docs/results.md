# Key Results (VERBIND)

This document summarizes the most important outcomes of the VERBIND project: the embedded heating concept, control approach, and achieved performance.

---

## Target
- **Goal:** Stable closed-loop temperature regulation of a mechanical bushing
- **Setpoint used for evaluation:** **60 °C**
- **Focus:** reliable setpoint tracking + improved temperature uniformity across the bushing

---

## Core Concept (What was built)
- **3D-printed (FDM) polycarbonate bushing** with **integrated heating channels/cavities**
- Channels filled with **silver-filled conductive adhesive** (**LOCTITE ABLESTIK 2030SC**)
- Heating via **Joule (resistive) heating** in the filled channel structure
- Power stage: **DC power supply + Arduino PWM + logic-level MOSFET (e.g., IRLZ44N)**
- Temperature feedback via **NTC thermistor(s) GA2K7MCD1** (voltage divider + ADC)

---

## Control Results (Temperature Regulation)
- Classical PID tuning methods (**Ziegler–Nichols, Cohen–Coon**) were tested but produced **high overshoot**
- Based on the system’s thermal behavior (PT1-like), a **PI controller** was chosen
- **Anti-windup concept:** integral action enabled only above a defined threshold (e.g., **50 °C**) to reduce integral accumulation during warm-up

---

## Quantitative Results (Headline Numbers)
> Replace/confirm numbers if you want to reflect your final evaluation exactly.

- **Heat-up time improvement**
  - Baseline (two-point / ON-OFF control): **464 s**
  - Final configuration (best case): **55.49 s**
- **Steady-state accuracy**
  - Mean deviation from 60 °C after first reaching setpoint: **0.322 °C**

---

## Temperature Uniformity (Thermography)
- Thermography was used to evaluate temperature distribution across the bushing surface
- The final channel layout improved overall uniformity compared to earlier iterations
- Remaining limitation: **edge regions show higher heat loss** → colder zones near the outer boundary

---

## Design Iterations (Channels)
Multiple channel designs were tested to improve printability, filling reliability, and thermal distribution.

| Iteration | Channel concept | Main purpose |
|---|---|---|
| V1 | U-shaped channels | Baseline feasibility |
| V2 | Spiral concept | Increase heater length / resistance |
| V3 | Flat spiral (aligned with print bed) | Improved printability + smaller channels |
| V4 | Modified outlets / additional paths | Better filling + distribution |
| V5 | Final multi-loop design | Best stability + improved uniformity |

---

## Key Learnings / Limitations
- **Sensor placement matters:** surface-mounted sensing introduces thermal delay → integrated sensing closer to the hotspot would improve control dynamics
- **Environment affects tuning:** airflow (open windows, people passing by) impacts measurements → a more consistent test stand improves repeatability
- **Uniformity still limited by edge losses:** future designs should increase channel density near the edges or improve insulation
- **Durability open point:** long-term stability of the conductive adhesive under thermal cycling and mechanical loads should be tested

---

## Recommended Figures to Link Here
Add your plots/figures into `docs/images/` and link them:

- Control response (temperature vs time, duty cycle)
- Final CAD / channel layout
- Thermography comparison (early vs final iteration)
- Wiring diagram / block diagram

Example:
