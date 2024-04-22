const int sensorPin = 23;  // Hall effect sensor input pin
volatile int rpmCount = 0; // Variable to count the number of revolutions
unsigned int rpm = 0;      // Variable to store the calculated RPM value
unsigned long previousMillis = 0;
unsigned long interval = 1000; // Interval for calculating RPM (in milliseconds)

void setup() {
  pinMode(sensorPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(sensorPin), countRPM, RISING);
  Serial.begin(9600);
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    // Calculate RPM every interval
    detachInterrupt(digitalPinToInterrupt(sensorPin));
    rpm = (rpmCount / 2) * 60; // Assuming 2 pulses per revolution
    Serial.print("RPM: ");
    Serial.println(rpm);
    rpmCount = 0;
    previousMillis = currentMillis;
    attachInterrupt(digitalPinToInterrupt(sensorPin), countRPM, RISING);
  }
  // Your other loop code goes here
}

void countRPM() {
  rpmCount++;
}
