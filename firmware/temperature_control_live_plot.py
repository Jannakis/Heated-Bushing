import serial
import pandas as pd
import matplotlib.pyplot as plt
import time
import math

# -------------------------
# Einstellungen
# -------------------------
PORT = "COM4"        # <-- ANPASSEN!
BAUD = 9600
EXCEL_FILE = "temperatur_log.xlsx"

# -------------------------
# Serielle Verbindung
# -------------------------
ser = serial.Serial(PORT, BAUD, timeout=1)
time.sleep(2)

print("Verbunden. Starte Live-Logging...")

# Excel vorbereiten
df = pd.DataFrame(columns=["Zeit_s", "TempAvg_C", "PWM"])
df.to_excel(EXCEL_FILE, index=False)

# Live-Plot vorbereiten
plt.ion()
fig, ax = plt.subplots()
ax2 = ax.twinx()

t_vals = []
tavg_vals = []
pwm_vals = []

tavg_line, = ax.plot([], [], linewidth=2, label="TempAvg (°C)")
pwm_line,  = ax2.plot([], [], linewidth=2, c ="red", label="PWM")

ax.set_xlabel("Zeit (s)")
ax.set_ylabel("TempAvg (°C)")
ax2.set_ylabel("PWM (0..255)")
ax.set_title("Live TEMP_AVG / PWM")

lines = [tavg_line, pwm_line]
labels = [l.get_label() for l in lines]
ax.legend(lines, labels, loc="upper left")

start = time.time()

state = {"TempAvg_C": None, "PWM": None}

def safe_float(s: str):
    try:
        v = float(s)
        return v if math.isfinite(v) else None
    except:
        return None

def safe_int(s: str):
    try:
        return int(float(s))
    except:
        return None

def parse_line(raw: str):
    """
    Erwartet genau:
      "TEMP_AVG:58.23"
      "PWM:147"
    """
    if raw.startswith("TEMP_AVG:"):
        val = raw.split("TEMP_AVG:", 1)[1].strip()
        tavg = safe_float(val)
        return {"TempAvg_C": tavg} if tavg is not None else {}

    if raw.startswith("PWM:"):
        val = raw.split("PWM:", 1)[1].strip()
        pwm = safe_int(val)
        return {"PWM": pwm} if pwm is not None else {}

    return {}

while True:
    try:
        raw = ser.readline().decode(errors="ignore").strip()
        if not raw:
            continue

        update = parse_line(raw)
        if not update:
            continue

        state.update(update)

        # loggen/plotten, sobald PWM kommt (typisch Ende des Zyklus)
        if "PWM" not in update:
            continue

        t = round(time.time() - start, 2)
        tavg = state["TempAvg_C"]
        pwm  = state["PWM"]

        print(f"{t}s → TEMP_AVG={tavg} °C | PWM={pwm}")

        # Excel speichern
        df.loc[len(df)] = [t, tavg, pwm]
        df.to_excel(EXCEL_FILE, index=False)

        # Plot aktualisieren
        t_vals.append(t)
        tavg_vals.append(tavg if tavg is not None else float("nan"))
        pwm_vals.append(pwm if pwm is not None else float("nan"))

        tavg_line.set_xdata(t_vals)
        tavg_line.set_ydata(tavg_vals)
        pwm_line.set_xdata(t_vals)
        pwm_line.set_ydata(pwm_vals)

        ax.relim();  ax.autoscale_view()
        ax2.relim(); ax2.autoscale_view()
        plt.pause(0.01)

    except KeyboardInterrupt:
        print("\nLogging beendet.")
        break
