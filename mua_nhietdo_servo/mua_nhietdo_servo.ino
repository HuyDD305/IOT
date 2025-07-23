#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP32Servo.h>

// Pins
const int tempPin = 4;
const int rainPin = 35;
const int servoPin = 32;

// Setup sensors
OneWire oneWire(tempPin);
DallasTemperature tempSensor(&oneWire);
Servo myServo;

void setup() {
  Serial.begin(115200);
  pinMode(rainPin, INPUT);
  tempSensor.begin();
  myServo.attach(servoPin);
  myServo.write(0); // Start position
}

void loop() {
  // Read temperature
  tempSensor.requestTemperatures();
  float temp = tempSensor.getTempCByIndex(0);
  
  // Read rain sensor
  bool raining = (digitalRead(rainPin) == LOW);
  
  // Display current readings
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.print("°C | Rain: ");
  Serial.print(raining ? "YES" : "NO");
  Serial.print(" | ");
  
  // Check conditions and move servo
  if (temp > 35 && raining) {
    myServo.write(45); // Open awning
    Serial.println("Awning OPEN - Hot & Raining");
  } else {
    myServo.write(0);  // Close awning
    Serial.println("Awning CLOSED");
  }
  
  delay(2000);
}