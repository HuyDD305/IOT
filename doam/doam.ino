// ESP32 Soil Moisture with Water Pump
#define SOIL_PIN 34
#define PUMP_PIN 25
#define LED_PIN 2

const int DRY_VALUE = 3000;
const int WET_VALUE = 1000;
const int PUMP_THRESHOLD = 30;

unsigned long lastPump = 0;

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  pinMode(PUMP_PIN, OUTPUT);
  digitalWrite(PUMP_PIN, LOW);
}

void loop() {
  int analog = analogRead(SOIL_PIN);
  int moisture = map(analog, DRY_VALUE, WET_VALUE, 0, 100);
  moisture = constrain(moisture, 0, 100);
  
  // LED control
  digitalWrite(LED_PIN, moisture < 30 ? HIGH : LOW);
  
  // Pump control
  if (moisture < PUMP_THRESHOLD && millis() - lastPump > 30000) {
    digitalWrite(PUMP_PIN, HIGH);
    delay(2000);
    digitalWrite(PUMP_PIN, LOW);
    lastPump = millis();
    Serial.println("Watered!");
  }
  
  Serial.print("Moisture: ");
  Serial.print(moisture);
  Serial.println("%");
  
  delay(2000);
}