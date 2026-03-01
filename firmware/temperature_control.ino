// NTC 2000 Ω @ 25 °C, Beta = 3499
// Teiler an 5V, Messung an A5 und A4
// Heizung an PWM Pin 9
// Serial: 9600

#include <Arduino.h>
#include <math.h>

// -------------------- Pins --------------------
const uint8_t PIN_ADC    = A5;
const uint8_t PIN_ADC_2  = A4;
const uint8_t PIN_HEATER = 9;     // PWM Pin

// -------------------- NTC Parameter --------------------
const float R_FIXED = 10000.0f;   // Festwiderstand (Spannungsteiler)
const float R0      = 2000.0f;    // NTC Widerstand bei 25°C
const float T0_K    = 25.0f + 273.15f;
const float BETA    = 3499.0f;

const uint16_t SAMPLES = 10;      // ADC Mittelung

// -------------------- Sollwert --------------------
double Setpoint = 60.0;

// -------------------- PI Parameter --------------------
// Kp: Proportionalverstärkung
// Ki: Integrationsverstärkung (pro Sekunde, weil wir mit dt integrieren)
double Kp = 15.0;
double Ki = 0.0;   // Start (wie bei dir), wird unten ggf. aktiviert

// -------------------- Regler Zustände --------------------
double Input  = 0.0;   // Temperatur Mittelwert (Istwert)
double Output = 0.0;   // PWM 0..255

double I_term = 0.0;   // Integralanteil (in PWM-Einheiten gespeichert)

// Zeitmessung für dt
unsigned long lastTimeMs = 0;

// Output Grenzen (PWM Bereich)
const double OUT_MIN = 0.0;
const double OUT_MAX = 255.0;


// -------------------- Hilfsfunktionen --------------------
uint16_t readAdcAvg(uint8_t pin) {
  uint32_t acc = 0;
  for (uint16_t i = 0; i < SAMPLES; ++i) {
    acc += (uint16_t)analogRead(pin);
    delayMicroseconds(300);
  }
  return (uint16_t)(acc / SAMPLES);
}

float adcToResistance(uint16_t adc) {
  // Schutz gegen 0 und 1023 (sonst Division durch 0)
  if (adc <= 0) adc = 1;
  if (adc >= 1023) adc = 1022;
  return R_FIXED * (float)adc / (1023.0f - (float)adc);
}

float ntcBetaToTempC(float R) {
  // Schutz gegen log(0)
  if (R <= 0.01f) R = 0.01f;
  float invT = (1.0f / T0_K) + (1.0f / BETA) * logf(R / R0);
  float T_K = 1.0f / invT;
  return T_K - 273.15f;
}


/**
 * PI-Regler Berechnung ohne Bibliothek
 *
 * Formel:
 *  error = Setpoint - Input
 *  P = Kp * error
 *  I = I + Ki * error * dt
 *  Output = P + I
 *
 * Anti-Windup:
 *  Wenn Output saturiert (0..255), wird I so begrenzt,
 *  dass Output wieder in Grenzen bleibt.
 */
void computePI(double dt_s, double Ki_eff) {
  // Fehler berechnen
  double error = Setpoint - Input;

  // Proportional-Anteil
  double P = Kp * error;

  // Integral-Anteil aktualisieren (nur wenn Ki_eff > 0)
  if (Ki_eff > 0.0) {
    I_term += (Ki_eff * error * dt_s);
  } else {
    // Wenn Integrator deaktiviert (z.B. unter 50°C), Integral zurücksetzen
    I_term = 0.0;
  }

  // Reglerausgang berechnen
  Output = P + I_term;

  // ---- Anti-Windup / Begrenzung ----
  // Output auf PWM-Grenzen clampen
  if (Output > OUT_MAX) {
    Output = OUT_MAX;
    // Integral so begrenzen, dass Output = P + I_term weiterhin stimmt
    I_term = OUT_MAX - P;
  } 
  else if (Output < OUT_MIN) {
    Output = OUT_MIN;
    I_term = OUT_MIN - P;
  }
}


// -------------------- Setup --------------------
void setup() {
  analogReference(DEFAULT);
  Serial.begin(9600);
  pinMode(PIN_HEATER, OUTPUT);

  // Dummy reads (stabilere erste Messung)
  analogRead(PIN_ADC);
  analogRead(PIN_ADC_2);

  // Startzeit setzen
  lastTimeMs = millis();
}


// -------------------- Loop --------------------
void loop() {
  // --- Messung ---
  uint16_t adc1 = readAdcAvg(PIN_ADC);
  uint16_t adc2 = readAdcAvg(PIN_ADC_2);

  float R1 = adcToResistance(adc1);
  float R2 = adcToResistance(adc2);

  float temp1 = ntcBetaToTempC(R1);
  float temp2 = ntcBetaToTempC(R2);

  float tempAvg = 0.5f * (temp1 + temp2);
  Input = (double)tempAvg;

  // --- dt berechnen (Sekunden) ---
  unsigned long nowMs = millis();
  double dt_s = (nowMs - lastTimeMs) / 1000.0;
  if (dt_s <= 0.0) dt_s = 0.001; // Schutz gegen 0
  lastTimeMs = nowMs;

  // --- Ki abhängig von Temperatur aktivieren/deaktivieren (wie bei dir gedacht) ---
  // Unter 50°C: keine Integration
  // Ab 50°C: Integration aktiv (hier z.B. Ki = 1.0)
  double Ki_eff = 0.0;

  if (tempAvg < 50.0f) {
    Ki_eff = 0.0;
  } else {
    Ki_eff = 0.5;  // Beispiel: ab 50°C Integrator aktiv
  }

  // --- PI berechnen ---
  computePI(dt_s, Ki_eff);

  // --- PWM ausgeben ---
  analogWrite(PIN_HEATER, (int)(Output + 0.5));

  // --- Serial Output (für Python Script) ---
  // Bitte Format beibehalten:
  Serial.print("TEMP_AVG:");
  Serial.println(tempAvg);

  Serial.print("PWM:");
  Serial.println((int)(Output + 0.5));

  // Wie in deinem Original: Ausgabe ca. 1Hz
  // (Wenn du schneller regeln willst: kleiner machen oder ganz entfernen)
  delay(1000);
}
