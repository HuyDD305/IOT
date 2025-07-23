#include <ESP32Servo.h>

#define SOIL_PIN 34      
#define RELAY_PIN 25     
#define SERVO_PIN 32     

const int DRY_VALUE = 3000;
const int WET_VALUE = 1000;

Servo myServo;

void setup() {
  Serial.begin(115200);

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);  

  myServo.attach(SERVO_PIN);
  myServo.write(0); // default position

  Serial.println("System Ready!");
}

void loop() {
  // Read soil moisture
  int analogValue = analogRead(SOIL_PIN);
  int moisturePercent = map(analogValue, DRY_VALUE, WET_VALUE, 0, 100);
  moisturePercent = constrain(moisturePercent, 0, 100);

  Serial.print("Soil moisture: ");
  Serial.print(moisturePercent);
  Serial.println("%");

  // Logic for relay + servo
  if (moisturePercent < 30) {
    Serial.println("Soil DRY → Pump ON + Move servo");
    digitalWrite(RELAY_PIN, HIGH);
    myServo.write(90);
  } 
  else if (moisturePercent > 70) {
    Serial.println("Soil WET → Pump OFF + Reset servo");
    digitalWrite(RELAY_PIN, LOW);
    myServo.write(0);
  }

  delay(2000);
}