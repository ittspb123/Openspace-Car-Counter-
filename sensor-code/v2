#include <HCSR04.h>

//  PINS 
HCSR04 hc(TRIG_PIN, ECHO_PIN); // TRIG_PIN, ECHO_PIN

//  SETTINGS 
const float TRIGGER_DROP_CM    = 12.0;  // controls how big the distance drop must be.
const unsigned long COOLDOWN_MS = 2500; // wait between counts
const int LOOP_DELAY_MS         = 80;   // ms between readings

//  STATE 
float prevDistance         = -1;
unsigned long lastTrigger  = 0;
long carCount              = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  float dist = hc.dist();

  if (dist <= 0 || dist > 500) { delay(LOOP_DELAY_MS); return; } // bad reading
  if (prevDistance < 0) { prevDistance = dist; return; }         // first reading

  float drop    = prevDistance - dist;
  bool  ready   = (millis() - lastTrigger) >= COOLDOWN_MS;

  if (drop > TRIGGER_DROP_CM && ready) {
    carCount++;
    Serial.print("Distance: "); Serial.print(dist);
    Serial.print(" cm  |  Cars: "); Serial.println(carCount);
    lastTrigger = millis();
  }

  prevDistance = dist;
  delay(LOOP_DELAY_MS);
}
