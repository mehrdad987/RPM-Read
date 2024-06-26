#include <Arduino.h>

#define INTERRUPT_PIN 12  // Change this to the appropriate interrupt pin
#define TIMER_DIVIDER 80  // Timer counter frequency is APB_CLK / TIMER_DIVIDER
#define TIMER_SCALE 1000000.0  // Converts the timer counter value to seconds

hw_timer_t* timer;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

volatile uint64_t interruptTime = 0;  // Time of the last interrupt in microseconds
volatile float frequency = 0.0;       // Frequency calculated from the last two interrupts
float rpm = 0.0;                      // Calculated RPM
const float Co_F = 0.5;               // Constant for RPM calculation

void IRAM_ATTR handleInterrupt() {
    portENTER_CRITICAL_ISR(&timerMux);
    uint64_t currentTime = timerRead(timer);
    interruptTime = currentTime;
    frequency = TIMER_SCALE / (currentTime - interruptTime);
    portEXIT_CRITICAL_ISR(&timerMux);
}

void setup() {
    Serial.begin(115200);
    pinMode(INTERRUPT_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), handleInterrupt, RISING);

    timer = timerBegin(0, TIMER_DIVIDER, true);
    timerAttachInterrupt(timer, &handleInterrupt, true);
    timerAlarmEnable(timer);
}

void loop() {
    portENTER_CRITICAL(&timerMux);
    rpm = 30.0 * frequency * Co_F;
    portEXIT_CRITICAL(&timerMux);

    // Apply noise filtering
    if (rpm > 10000.0) {
        rpm = 0.0;
    }

    // Print the calculated RPM
    Serial.print("RPM: ");
    Serial.println(rpm);

    delay(1000);  // Adjust the delay as needed
}
