#include <BlynkSimpleEsp32.h>
#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Servo.h>


char auth[] = "";
char ssid[] = "";
char pass[] = "";


#define DHTPIN 5
#define DHTTYPE DHT11
#define ONE_WIRE_BUS 4
#define SOIL_PIN 34
#define RAIN_PIN 35
#define TRIG_PIN 13
#define ECHO_PIN 14
#define RELAY_PUMP 25
#define RELAY_DRAIN 26
#define SERVO_PIN 27


DHT dht(DHTPIN, DHTTYPE);
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature soilTemp(&oneWire);
Servo roofServo;


float SOIL_TEMP_LIMIT = 40.0;
float HUMIDITY_LIMIT = 70.0;
float WATER_LEVEL_LIMIT = 5.0; // cm


unsigned long previousMillis = 0;
const long interval = 2000; // 2 seconds


float getWaterLevel() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH, 30000); 
  float distance = duration * 0.034 / 2; 
  return distance;
}

void setup() {
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);

  dht.begin();
  soilTemp.begin();
  roofServo.attach(SERVO_PIN);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(RELAY_PUMP, OUTPUT);
  pinMode(RELAY_DRAIN, OUTPUT);

  digitalWrite(RELAY_PUMP, HIGH);  
  digitalWrite(RELAY_DRAIN, HIGH); 

  Serial.println("System started...");
}

void loop() {
  Blynk.run();

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // Read sensors
    float airHumidity = dht.readHumidity();
    float airTemp = dht.readTemperature();

    if (isnan(airHumidity) || isnan(airTemp)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }

    soilTemp.requestTemperatures();
    float soilTemperature = soilTemp.getTempCByIndex(0);

    int soilRaw = analogRead(SOIL_PIN);
    int soilMoisture = map(soilRaw, 0, 4095, 100, 0); // 0% dry, 100% wet

    int rainValue = analogRead(RAIN_PIN);
    bool isRaining = rainValue < 2000; 

    float waterDistance = getWaterLevel();
    float waterLevel = 20 - waterDistance; 

  
    Serial.printf("Air Temp: %.2f °C, Humidity: %.2f%%\n", airTemp, airHumidity);
    Serial.printf("Soil Temp: %.2f °C, Soil Moisture: %d%%\n", soilTemperature, soilMoisture);
    Serial.printf("Water Level: %.2f cm, Raining: %d\n", waterLevel, isRaining);

    
    Blynk.virtualWrite(V1, airTemp);
    Blynk.virtualWrite(V2, airHumidity);
    Blynk.virtualWrite(V3, soilTemperature);
    Blynk.virtualWrite(V4, soilMoisture);
    Blynk.virtualWrite(V5, waterLevel);

   
    if (soilMoisture < HUMIDITY_LIMIT) {
      digitalWrite(RELAY_PUMP, LOW);  
    } else {
      digitalWrite(RELAY_PUMP, HIGH); 
    }

    if (soilTemperature > SOIL_TEMP_LIMIT && isRaining) {
      roofServo.write(0); 
    } else {
      roofServo.write(90); 
    }

    if (waterLevel < WATER_LEVEL_LIMIT) {
      digitalWrite(RELAY_DRAIN, LOW); 
    } else {
      digitalWrite(RELAY_DRAIN, HIGH); 
    }
  }
}
