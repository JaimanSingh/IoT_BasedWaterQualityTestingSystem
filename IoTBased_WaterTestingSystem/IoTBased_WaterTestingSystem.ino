#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <ThingSpeak.h>

const char *ssid = "ESP 32";
const char *password = "Water123";
const char *api_key = "1COKS7SK8RDUWRG4";
const unsigned long channelID = 2516069; // Replace 123456 with your ThingSpeak channel ID

LiquidCrystal_I2C lcd(0x27, 16, 2); // Change the I2C address if different

const int tdsSensorPin = 34; // Digital pin for TDS sensor
const int turbiditySensorPin = 35; // Digital pin for turbidity sensor
const int temperatureSensorPin = 4; // Analog pin for temperature sensor

WiFiClient client; // Create a WiFi client object

void setup() {
  lcd.begin(16, 2); // Initialize LCD with 16 columns and 2 rows
  lcd.backlight();
  
  Serial.begin(115200);
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  
  ThingSpeak.begin(client); // Initialize ThingSpeak client with WiFi client
}

void loop() {
  float tdsValue = analogRead(tdsSensorPin);
  float turbidityValue = digitalRead(turbiditySensorPin);
  float temperatureValue = analogRead(temperatureSensorPin);
  
  // Convert analog readings to actual values
  float tds = tdsValue; // Assuming TDS sensor output is digital
  float turbidity = turbidityValue; // Assuming turbidity sensor output is digital
  
  // Assuming LM35 temperature sensor output is in millivolts (10 mV per degree Celsius)
  // Formula to convert millivolts to Celsius: temperature = (analogReading / 1023.0) * 500.0;
  float temperature = (temperatureValue / 1023.0) * 500.0;
  
  // Display readings on LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("TDS: ");
  lcd.print(tds);
  lcd.print(" ppm");
  lcd.setCursor(0, 1);
  lcd.print("Turbidity: ");
  lcd.print(turbidity);
  lcd.print(" NTU");
  
  lcd.display(); // Ensure LCD displays the content
  
  // Update ThingSpeak channel with sensor data
  ThingSpeak.setField(1, tds);
  ThingSpeak.setField(2, turbidity);
  ThingSpeak.setField(3, temperature);
  
  int status = ThingSpeak.writeFields(channelID, api_key);
  if (status == 200) {
    Serial.println("Data sent to ThingSpeak successfully");
  } else {
    Serial.println("Failed to send data to ThingSpeak");
  }
  
  delay(15000); // Upload data every 15 seconds (ThingSpeak limit)
}




