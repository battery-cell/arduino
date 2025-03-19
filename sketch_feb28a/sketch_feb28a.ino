#include <Arduino.h>

// Define the servo control pin (use an available GPIO pin on ESP32)
#define SERVO_PIN 22  // Change this to your connected GPIO pin (e.g., GPIO 13)

// Servo settings
#define SERVO_MIN 500    // Pulse width for 0 degrees
#define SERVO_MAX 2500   // Pulse width for 180 degrees

void setup() {
    Serial.begin(115200);  // Start serial communication
    pinMode(SERVO_PIN, OUTPUT);  // Set the servo control pin as an output
    Serial.println("Servo Controller Ready!");
}

void moveServo(int position) {
    // Map the position (0-180 degrees) to the pulse width (500-2500)
    int pulseWidth = map(position, 0, 180, SERVO_MIN, SERVO_MAX);
    
    // Generate the PWM signal to control the servo
    ledcWriteTone(0, 50);   // Set the frequency to 50 Hz (standard servo frequency)
    ledcWrite(0, pulseWidth);  // Adjust the duty cycle based on the position
    Serial.print("Moved Servo to ");
    Serial.print(position);
    Serial.println(" degrees");
}

void loop() {
    if (Serial.available()) {
        String command = Serial.readStringUntil('\n');  // Read input string
        command.trim();  // Remove leading/trailing spaces

        // Print received command
        Serial.print("Received Command: ");
        Serial.println(command);  // Debug: print the received command to Serial Monitor

        // Convert to lowercase for case-insensitive comparison
        command.toLowerCase();

        // Check received command and move the servo accordingly
        if (command == "happy") {
            Serial.println("Command 'Happy' received. Moving to 0°");
            moveServo(0);   // Move to 0° for "Happy"
        } else if (command == "suprise") {
            Serial.println("Command 'Suprise' received. Moving to 30°");
            moveServo(30);  // Move to 30° for "Suprise"
        } else if (command == "sad") {
            Serial.println("Command 'Sad' received. Moving to 90°");
            moveServo(90);  // Move to 90° for "Sad"
        } else if (command == "neutral") {
            Serial.println("Command 'Neutral' received. Moving to 45°");
            moveServo(45);  // Move to 45° for "Neutral"
        } else if (command == "angry") {
            Serial.println("Command 'Angry' received. Moving to 180°");
            moveServo(180); // Move to 180° for "Angry"
        } else {
            Serial.println("Invalid command!"); // If an invalid command is received
        }
    }
}
