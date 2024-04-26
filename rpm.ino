#include <Arduino.h>

const int sensorPin = 23;  // Hall effect sensor input pin
volatile uint32_t rpmCount = 0; // Variable to count the number of revolutions
uint32_t rpm = 0;         // Variable to store the calculated RPM value
uint32_t previousMillis = 0;
uint32_t interval = 1000; // Interval for calculating RPM (in milliseconds)
portMUX_TYPE rpmMux = portMUX_INITIALIZER_UNLOCKED; // Mutex to protect rpmCount variable

void IRAM_ATTR countRPM() {
    portENTER_CRITICAL_ISR(&rpmMux);
    rpmCount++;
    portEXIT_CRITICAL_ISR(&rpmMux);
}

void setup() {
    pinMode(sensorPin, INPUT);
    attachInterrupt(digitalPinToInterrupt(sensorPin), countRPM, RISING);
    Serial.begin(9600);
}

void loop() {
    uint32_t currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
        // Calculate RPM every interval
        portENTER_CRITICAL(&rpmMux);
        rpm = (rpmCount / 2) * 60; // Assuming 2 pulses per revolution
        rpmCount = 0;
        portEXIT_CRITICAL(&rpmMux);

        Serial.print("RPM: ");
        Serial.println(rpm);

        previousMillis = currentMillis;
    }
    // Your other loop code goes here
}
