#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRsend.h>
#include <IRutils.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Pins
#define IR_RECEIVE_PIN 15
#define IR_SEND_PIN 4

// IR objects
IRrecv irrecv(IR_RECEIVE_PIN);
IRsend irsend(IR_SEND_PIN);
decode_results results;

// LCD setup
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Store up to 10 codes
unsigned long storedCodes[10];
int storedCount = 0;

void setup() {
  Serial.begin(115200);
  irrecv.enableIRIn();
  irsend.begin();
  
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("IR Ready & Wait");
  
  Serial.println("IR Receiver Ready.");
  Serial.println("When IR signal received, will store and print.");
  Serial.println("Use command: SEND <index> to re-send.");
}

void loop() {
  // Check for IR signals
  if (irrecv.decode(&results)) {
    unsigned long receivedCode = results.value;

    // Store if space available
    if (storedCount < 10) {
      storedCodes[storedCount] = receivedCode;
      Serial.print("Stored code at index ");
      Serial.print(storedCount);
      Serial.print(": 0x");
      Serial.println(receivedCode, HEX);

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Saved #");
      lcd.print(storedCount);
      lcd.setCursor(0, 1);
      lcd.print("0x");
      lcd.print(receivedCode, HEX);

      storedCount++;
    } else {
      Serial.println("Storage full! Max 10 codes.");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Storage Full!");
    }

    irrecv.resume();
    delay(1500);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("IR Ready & Wait");
  }

  // Check for serial input commands
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();

    if (command.startsWith("SEND")) {
      int index = command.substring(5).toInt();
      if (index >= 0 && index < storedCount) {
        unsigned long codeToSend = storedCodes[index];
        Serial.print("Sending code at index ");
        Serial.print(index);
        Serial.print(": 0x");
        Serial.println(codeToSend, HEX);

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Sending #");
        lcd.print(index);
        irsend.sendNEC(codeToSend, 32); // Assuming NEC; adjust if needed

        lcd.setCursor(0, 1);
        lcd.print("0x");
        lcd.print(codeToSend, HEX);
        delay(1000);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("IR Ready & Wait");
      } else {
        Serial.println("Invalid index.");
      }
    } else if (command == "LIST") {
      Serial.println("Stored IR codes:");
      for (int i = 0; i < storedCount; i++) {
        Serial.print("Index ");
        Serial.print(i);
        Serial.print(": 0x");
        Serial.println(storedCodes[i], HEX);
      }
    } else if (command == "CLEAR") {
      storedCount = 0;
      Serial.println("All stored codes cleared.");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Codes Cleared");
      delay(1000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("IR Ready & Wait");
    } else {
      Serial.println("Unknown command. Use SEND <index>, LIST, or CLEAR.");
    }
  }
}
