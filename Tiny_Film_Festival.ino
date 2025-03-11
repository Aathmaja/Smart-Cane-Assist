/*
 * TinyFilmFestival - Multi-Threshold Distance Animation Example
 * 
 *Tiny Film Festival
    *Aathmaja Ananth
    *DIGF 2002-501: Physical Computing
    *Professor Kate Hartman
    *March 11, 2025
    */

#include "TinyFilmFestival.h"
#include "HCSR04.h"
#include "Caution.h"
#include "Safe.h"
#include "Stop.h"

// Create instance of TinyFilmFestival
TinyFilmFestival film;

// Create Animation objects
Animation idleAnim = Safe;
Animation cautionAnim = Caution;
Animation goAnim = Stop;

// Distance sensor setup
int triggerPin = 14;          // Trigger pin for HC-SR04
int echoPin = 15;             // Echo pin for HC-SR04
int maxDistance = 200;        // Maximum distance to measure (cm)
UltraSonicDistanceSensor distanceSensor(triggerPin, echoPin, maxDistance);

// Distance control variables
float distanceThreshold1 = 25.0;  // First threshold (cm)
float distanceThreshold2 = 50.0;  // Second threshold (cm)
float distanceThreshold3 = 75.0;  // Third threshold (cm)
int sensorInterval = 100;         // How often to read sensor (ms)
unsigned long lastSensorRead = 0;       // Last sensor reading time
float currentDistance = 0.0;            // Variable to store distance reading

void setup() 
{
    // Initialize serial for debug output
    Serial.begin(9600);
    
    // Initialize the LED matrix
    film.begin();
    
    // Start with idle animation
    film.startAnimation(idleAnim, LOOP);
    Serial.println("Starting with idle animation");
}

void loop() 
{
    // Read distance value with timing control
    float distance = readDistance();
    
    if (distance >= 0)
    {
        Serial.print("Distance : ");
        Serial.print(distance);
        Serial.println(" cm");

        // Switch animations based on distance ranges
        // Very Close Range: 0cm to 25cm (exclusive)
        // Triggers most dynamic animation for closest interactions
        if ((distance >= 0) && (distance < distanceThreshold1))
        {
            film.startAnimation(cautionAnim, LOOP);
            Serial.println("Being Caution (Very Close)");
        }
        // Close Range: 25cm (inclusive) to 50cm (exclusive)
        // Active interaction range for direct manipulation
        else if ((distance >= distanceThreshold1) && (distance < distanceThreshold2))
        {
            film.startAnimation(goAnim, LOOP);
            Serial.println("Playing Go (Close)");
        }
        // Far Range: 75cm and beyond
        // Default state for no active interaction
        else if (distance >= distanceThreshold3)
        {
            film.startAnimation(idleAnim, LOOP);
            Serial.println("Playing Idle (Far)");
        }
    }
    
    // Update the animation frame
    film.update();
}

// Reads distance from the HC-SR04 sensor at specified intervals
// Function returns float since HC-SR04 can measure partial centimeters
float readDistance()
{
    unsigned long currentTime = millis();
    
    // Only read sensor if interval has elapsed
    if (currentTime - lastSensorRead >= sensorInterval)
    {
        currentDistance = distanceSensor.measureDistanceCm();
        lastSensorRead = currentTime;
    }
    
    return currentDistance;
}
