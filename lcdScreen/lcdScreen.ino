#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Change address if needed
String inputString = "";
const int sensorPin = 34;  // Analog input for sensor (A0)
const int digitalPin = 32; // Digital input for D0 pin

void setup() {
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();

  pinMode(sensorPin, INPUT);      // Set analog pin as input
  pinMode(digitalPin, INPUT);     // Set D0 pin as digital input

  Serial.println("Type something and press Enter:");
}

void loop() {
  // Handle serial input
  while (Serial.available()) {
    char c = Serial.read();

    if (c == '\n' || c == '\r') {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(inputString.substring(0, 16));  // Print on line 1
      inputString = "";  // Clear for next input
    } else {
      inputString += c;
    }
  }

  // Read the D0 pin for the gas threshold
  int digitalValue = digitalRead(digitalPin);
  
  // Display the digital output status (alert or normal)
  lcd.setCursor(0, 1);
  if (digitalValue == HIGH) {
    lcd.print("ALERT! High Gas");  // Gas level is too high
  } else {
    lcd.print("Gas Normal     ");  // Gas level is normal
  }

  // Read the analog sensor value
  int sensorValue = analogRead(sensorPin);

  // Optionally display the analog value (if you want to monitor it too)
  // lcd.setCursor(0, 1);
  // lcd.print("Gas: ");
  // lcd.print(sensorValue);
  // lcd.print("   ");  // Clear old digits if value gets shorter

  delay(500);  // Update every 0.5s
}
