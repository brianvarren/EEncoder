/*
  EEncoder Basic Example
  
  Demonstrates simple usage of the EEncoder library with a rotary
  encoder and push button for RP2040-based boards.
  
  Hardware:
  - Connect encoder A pin to GPIO 2
  - Connect encoder B pin to GPIO 3
  - Connect encoder button to GPIO 4
  - Assumes encoder module has built-in pull-up resistors
  
  This example:
  - Increments/decrements a value with the encoder
  - Resets to zero on button press
  - Enters fine-tune mode on long press
  - Shows acceleration when turning quickly
*/

#include "EEncoder.h"

// Pin definitions
const uint8_t ENCODER_PIN_A = 4;
const uint8_t ENCODER_PIN_B = 5;
const uint8_t ENCODER_BUTTON = 3;

// Create encoder instance
EEncoder encoder(ENCODER_PIN_A, ENCODER_PIN_B, ENCODER_BUTTON);

// Variable to track the current value
int currentValue = 0;
bool fineTuneMode = false;

// Encoder rotation callback
void onEncoderRotate(EEncoder& enc) {
    // Get the direction of rotation
    int8_t increment = enc.getIncrement();
    
    // In fine-tune mode, ignore acceleration
    if (fineTuneMode) {
        currentValue += (increment > 0) ? 1 : -1;
    } else {
        // Normal mode - use the increment (may be accelerated)
        currentValue += increment;
    }
    
    // Print the new value
    Serial.print("Value: ");
    Serial.print(currentValue);
    if (fineTuneMode) Serial.print(" [FINE]");
    if (abs(increment) > 1) Serial.print(" [FAST]");
    Serial.println();
}

// Button press callback
void onButtonPress(EEncoder& enc) {
    // Reset value to zero
    currentValue = 0;
    fineTuneMode = false;
    
    Serial.println("Button pressed - Value reset to 0");
}

// Long press callback
void onLongPress(EEncoder& enc) {
    // Toggle fine-tune mode
    fineTuneMode = !fineTuneMode;
    
    Serial.print("Long press - Fine tune mode ");
    Serial.println(fineTuneMode ? "ON" : "OFF");
}

void setup() {
    // Initialize serial communication
    Serial.begin(115200);
    while (!Serial) {
        ; // Wait for serial port to connect (needed for some boards)
    }
    
    Serial.println("EEncoder Basic Example");
    Serial.println("Rotate encoder to change value, press button to reset");
    Serial.println("Long press to toggle fine-tune mode");
    Serial.println("Turn quickly to see acceleration!");
    
    // Set up the callbacks
    encoder.setEncoderHandler(onEncoderRotate);
    encoder.setButtonHandler(onButtonPress);
    encoder.setLongPressHandler(onLongPress);
    
    // Enable acceleration with 5x multiplier
    encoder.setAcceleration(true);
    encoder.setAccelerationRate(5);
    
    // Optional: adjust timing if needed
    // encoder.setDebounceInterval(5);        // Faster debounce
    // encoder.setLongPressDuration(750);     // Longer press required
}

void loop() {
    // Update the encoder - this must be called as often as possible
    encoder.update();
    
    // Your other code can go here
    // The callbacks will be fired automatically when events occur
}