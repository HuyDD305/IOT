#include <OneWire.h>
#include <DallasTemperature.h>

// GPIO where the DS18B20 is connected to
const int oneWireBus = 4;     

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

void setup() {
  // Start the Serial Monitor
  Serial.begin(115200); // Higher baud rate for ESP32
  
  // External 4.7kΩ pull-up resistor used instead of internal pull-up
  
  Serial.println("DS18B20 with Internal Pull-up");
  
  // Start the DS18B20 sensor
  sensors.begin();
  
  // Check if sensor is found
  Serial.print("Found ");
  Serial.print(sensors.getDeviceCount());
  Serial.println(" device(s)");
  
  delay(1000);
}

void loop() {
  sensors.requestTemperatures(); 
  
  float temperatureC = sensors.getTempCByIndex(0);
  
  // Check for sensor error
  if (temperatureC == DEVICE_DISCONNECTED_C) {
    Serial.println("Error: Sensor not found or disconnected!");
    Serial.println("Check wiring connections");
    delay(2000);
    return;
  }
  
  float temperatureF = sensors.getTempFByIndex(0);
  
  Serial.print("Temperature: ");
  Serial.print(temperatureC);
  Serial.print("°C  |  ");
  Serial.print(temperatureF);
  Serial.println("°F");
  
  delay(1000);
}