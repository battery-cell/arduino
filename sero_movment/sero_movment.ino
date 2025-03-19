#include <BleKeyboard.h>

BleKeyboard bleKeyboard("ESP32 Keyboard", "Espressif", 100);

void setup() {
  Serial.begin(115200);
  bleKeyboard.begin();
}

void loop() {
  if (bleKeyboard.isConnected()) {
    Serial.println("Sending keystrokes...");
    bleKeyboard.print("Hello from ESP32!");
    bleKeyboard.write(KEY_RETURN);
    delay(5000); // send every 5 seconds
  } else {
    Serial.println("Waiting for connection...");
  }
  delay(1000);
}
