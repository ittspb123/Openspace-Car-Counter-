/*
OpenSpace Sensor Logic - Version 1
Method: Single threshold detection
Issue: Distance jitter caused double counting
*/

#include <HCSR04.h>   

// Arduino pins  connected to the sensor
const int TRIG_PIN = 10;   
const int ECHO_PIN = 9;    


const float THRESHOLD = 30.0;

HCSR04 hc(TRIG_PIN, ECHO_PIN);

// Variable to store total number of cars detected
long carCount = 0;

// Boolean flag to prevent counting the same car multiple times
// false = no car currently detected
// true  = car is currently in front of sensor
bool carDetected = false;

void setup() {
  Serial.begin(9600);

  Serial.println(" Openspace Car Counter Test");
}

void loop() {

  // Read current distance from ultrasonic sensor (in cm)
  float distance = hc.dist();

  // Check if the distance reading 
  // distance > 0 filters out bad readings (like 0)
  if (distance > 0 && distance < THRESHOLD) {

    // If a car is detected AND we were not already detecting one
    if (!carDetected) {

      // Increase car count by 1
      carCount++;

      // Mark that a car is now being detected
      carDetected = true;

      // Print the car count and distance at detection moment
      Serial.print("Car Count: ");
      Serial.print(carCount);
      Serial.print(" | Distance: ");
      Serial.print(distance);
      Serial.println(" cm");
    }

  } else {

    // If object is not within threshold,
    // reset detection flag so next car can be counted
    carDetected = false;
  }

  //  delay for readings 
  delay(200);
}
