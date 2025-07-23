#define RAIN_DIGITAL 35  // DO connected to GPIO 25

void setup() {
  Serial.begin(115200);
  pinMode(RAIN_DIGITAL, INPUT);
}

void loop() {
  int rainDigitalValue = digitalRead(RAIN_DIGITAL);

  Serial.print("Analog Value: ");

  Serial.print("\t Digital Value: ");
  Serial.println(rainDigitalValue);

  if (rainDigitalValue == LOW) {
    Serial.println("💧 Rain detected!");
  } else {
    Serial.println("☀️ No rain.");
  }

  delay(1000);
}