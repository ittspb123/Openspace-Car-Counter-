/*
OpenSpace Sensor Logic - Version 2 
Fix: no double counting from sensor jitter
*/

#include <HCSR04.h>

// Pins
const int TRIG_PIN = 10;
const int ECHO_PIN = 9;

HCSR04 hc(TRIG_PIN, ECHO_PIN);

long carCount = 0;

// Hysteresis thresholds (make adjustments)
const float ENTER_CM = 30.0;  // car considered present at/under this
const float EXIT_CM  = 40.0;  // car considered gone at/over this (must be > ENTER)

// Stability requirements (make adjustments)
const int STABLE_HITS = 3;     // consecutive reads needed to confirm a state change

// State
bool carPresent = false;
int enterHits = 0;
int exitHits  = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("OpenSpace Car Counter Test v2");
}

void loop() {
  float d = hc.dist();

  // Treat invalid readings as "no useful data" (don’t change state)
  if (d <= 0) {
    delay(60);
    return;
  }

  if (!carPresent) {
    // Looking for a car to ENTER
    if (d <= ENTER_CM) {
      enterHits++;
      if (enterHits >= STABLE_HITS) {
        carCount++;
        carPresent = true;

        // reset counters
        enterHits = 0;
        exitHits = 0;

        Serial.print("Car Count: ");
        Serial.print(carCount);
        Serial.print(" | ENTER Distance: ");
        Serial.print(d);
        Serial.println(" cm");
      }
    } else {
      // not close enough, reset enter confirmation
      enterHits = 0;
    }
  } else {
    // Car is present; wait for it to EXIT
    if (d >= EXIT_CM) {
      exitHits++;
      if (exitHits >= STABLE_HITS) {
        carPresent = false;

        // reset counters
        exitHits = 0;
        enterHits = 0;

        Serial.print("EXIT confirmed | Distance: ");
        Serial.print(d);
        Serial.println(" cm");
      }
    } else {
      // still in the zone, reset exit confirmation
      exitHits = 0;
    }
  }

  delay(60); 
}
