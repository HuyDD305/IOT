#define PUMP_PIN 26

void setup() {
  pinMode(PUMP_PIN, OUTPUT);

}

void loop() {
  digitalWrite(PUMP_PIN, HIGH);  // Turn LED on
  delay(1000);                  // Wait 1 second
  digitalWrite(PUMP_PIN, LOW);   // Turn LED off
  delay(1000);     
  }             // Wait 1 second