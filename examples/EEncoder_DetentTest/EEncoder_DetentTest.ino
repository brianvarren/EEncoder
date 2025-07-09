/*
  EEncoder Detent Detection Test
  
  This sketch helps verify that the improved state machine correctly
  detects every physical detent without drift or false triggers.
  
  Test procedure:
  1. Upload this sketch
  2. Open Serial Monitor at 115200 baud
  3. Rotate encoder exactly 10 clicks clockwise
  4. Verify count shows exactly 10
  5. Rotate encoder exactly 10 clicks counter-clockwise
  6. Verify count shows exactly 0
  7. Try rotating at different speeds
  
  The count should always match the physical clicks!
*/

#include <EEncoder.h>

// Pin definitions - change to match your setup
const uint8_t ENCODER_PIN_A = 2;
const uint8_t ENCODER_PIN_B = 3;
const uint8_t ENCODER_BUTTON = 4;

// Create encoder (adjust last parameter for your hardware)
EEncoder encoder(ENCODER_PIN_A, ENCODER_PIN_B, ENCODER_BUTTON, 4);

// Counter
int clickCount = 0;
int lastClickCount = -1;

// Statistics
unsigned long testStartTime = 0;
int totalClicks = 0;

void onEncoderClick(EEncoder& enc) {
    clickCount += enc.getIncrement();
    totalClicks++;
}

void onButtonPress(EEncoder& enc) {
    // Reset counter
    clickCount = 0;
    totalClicks = 0;
    testStartTime = millis();
    Serial.println("\n=== Counter Reset ===");
}

void setup() {
    Serial.begin(115200);
    while (!Serial) {
        ;
    }
    
    Serial.println("=== EEncoder Detent Detection Test ===");
    Serial.println("v1.2.0 - Robust State Machine");
    Serial.println();
    Serial.println("Instructions:");
    Serial.println("1. Rotate encoder and count physical clicks");
    Serial.println("2. Display should match your count exactly");
    Serial.println("3. Press button to reset counter");
    Serial.println("4. Try rotating at different speeds");
    Serial.println();
    
    // Setup encoder
    encoder.setEncoderHandler(onEncoderClick);
    encoder.setButtonHandler(onButtonPress);
    
    // Disable acceleration for this test
    encoder.setAcceleration(false);
    
    testStartTime = millis();
}

void loop() {
    encoder.update();
    
    // Display updates
    if (clickCount != lastClickCount) {
        Serial.print("Clicks: ");
        Serial.print(clickCount);
        
        // Show direction
        if (clickCount > lastClickCount) {
            Serial.print(" [CW]");
        } else {
            Serial.print(" [CCW]");
        }
        
        // Show statistics
        if (totalClicks > 0) {
            unsigned long elapsed = millis() - testStartTime;
            float clicksPerSecond = (float)totalClicks / (elapsed / 1000.0);
            Serial.print(" | Total: ");
            Serial.print(totalClicks);
            Serial.print(" | Rate: ");
            Serial.print(clicksPerSecond, 1);
            Serial.print(" clicks/sec");
        }
        
        Serial.println();
        lastClickCount = clickCount;
    }
}