#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define DHTPIN 2
#define DHTTYPE DHT22

const int pirPin = 3;
const int ledPin = 4;
const int buzzerPin = 5;
const int ldrPin = A0;

DHT dht(DHTPIN, DHTTYPE);

// Initialize LCD (address 0x27, 16 columns x 2 rows)
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600);
  pinMode(pirPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  dht.begin();
  lcd.init();             // Initialize the LCD
  lcd.backlight();        // Turn on backlight
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Smart Home Init");
  delay(2000);
  lcd.clear();
}

void loop() {
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  int ldrValue = analogRead(ldrPin);
  bool motion = digitalRead(pirPin);

  // Light control: if LDR value is low, turn on LED
  if (ldrValue < 500) {
    digitalWrite(ledPin, HIGH);  // Turn on LED
  } else {
    digitalWrite(ledPin, LOW);   // Turn off LED
  }

  // Intruder detection: if PIR detects motion, turn on buzzer
  if (motion) {
    digitalWrite(buzzerPin, HIGH);  // Turn on buzzer
    lcd.clear();                    // Clear screen before updating
    lcd.setCursor(0, 0);
    lcd.print("Motion Detected!");
    Serial.println("Intruder detected!");
  } else {
    digitalWrite(buzzerPin, LOW);   // Turn off buzzer
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("No Motion");
  }

  // Serial output for debugging
  Serial.print("Temp: ");
  Serial.print(temp);
  Serial.print("°C, Hum: ");
  Serial.print(hum);
  Serial.print("%, Light: ");
  Serial.println(ldrValue);

  // LCD output (Temperature, Humidity, Light, Motion)
  lcd.setCursor(0, 1);
  lcd.print("T:");
  lcd.print(temp);
  lcd.print("C LDR:");
  lcd.print(ldrValue);

  delay(1000);  // Update every second
}
