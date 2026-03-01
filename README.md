# Heated 3D-Printed Bushing (Closed-Loop Temperature Control)

This repository contains a prototype **closed-loop temperature control system** for an **FDM 3D-printed polycarbonate (PC) bushing** with **integrated heating channels**. The channels are filled with a **silver-filled conductive adhesive** (LOCTITE ABLESTIK 2030SC) and act as an embedded resistor for **Joule heating**. Heating power is controlled via an Arduino-driven PWM MOSFET stage using feedback from NTC thermistors.

## Highlights
- Embedded resistive heater: **3D-printed channels + conductive silver adhesive**
- Power stage: **DC supply + PWM + logic-level MOSFET**
- Sensing: **NTC thermistors (GA2K7MCD1)** via voltage divider
- Control: **PI controller** (selected after PID tuning trials) with anti-windup concept
- Target setpoint used in evaluation: **60 °C**

## Repository Structure (recommended)
- `firmware/` – Arduino code (PWM + control + logging)
- `cad/` – CAD files for bushing iterations / channel designs
- `docs/` – results and documentation
- `measurements/` – raw logs and evaluation data
- `images/` or `docs/images/` – figures used in the documentation

## Documentation
For details (manufacturing, wiring, controller design, experiments, thermography), see:
- `docs/results.md`

## Images
Add your key figures here or place them in `docs/images/` and link them below.

Example:
![System overview](docs/images/system_overview.png)
