/*
  EEncoder Hardware Test
  
  This sketch helps you determine how many counts your encoder
  produces per physical detent (click). Run this sketch first
  to find the right value for your encoder hardware.
  
  Instructions:
  1. Upload this sketch
  2. Open Serial Monitor at 115200 baud
  3. Slowly rotate encoder exactly ONE click
  4. Note the "Counts per detent" value
  5. Use this value when creating your EEncoder instance
  
  Common values:
  - 1: Some encoders produce 1 count per detent
  - 2: Some encoders produce 2 counts per detent  
  - 4: Most encoders produce 4 counts per detent
*/

// Pin definitions - change these to match your setup
const uint8_t ENCODER_PIN_A = 2;
const uint8_t ENCODER_PIN_B = 3;

// Variables for counting
volatile int rawCount = 0;
int lastDisplayedCount = 0;
uint8_t lastEncoderState = 0;

void setup() {
    Serial.begin(115200);
    while (!Serial) {
        ; // Wait for serial
    }
    
    Serial.println("=== EEncoder Hardware Test ===");
    Serial.println("Rotate encoder slowly, one click at a time");
    Serial.println("Note how many counts occur per physical click");
    Serial.println();
    
    // Configure pins
    pinMode(ENCODER_PIN_A, INPUT_PULLUP);
    pinMode(ENCODER_PIN_B, INPUT_PULLUP);
    
    // Read initial state
    lastEncoderState = (digitalRead(ENCODER_PIN_A) << 1) | digitalRead(ENCODER_PIN_B);
    
    Serial.println("Ready! Rotate encoder one click...");
    Serial.println();
}

void loop() {
    // Read current state
    uint8_t currentState = (digitalRead(ENCODER_PIN_A) << 1) | digitalRead(ENCODER_PIN_B);
    
    // Check if state changed
    if (currentState != lastEncoderState) {
        // Create 4-bit value from old and new states
        uint8_t combined = (lastEncoderState << 2) | currentState;
        
        // Determine direction
        switch (combined) {
            case 0b0001: case 0b0111: case 0b1110: case 0b1000:
                rawCount++;
                break;
            case 0b0010: case 0b1011: case 0b1101: case 0b0100:
                rawCount--;
                break;
        }
        
        lastEncoderState = currentState;
    }
    
    // Display updates
    if (rawCount != lastDisplayedCount) {
        Serial.print("Raw count: ");
        Serial.print(rawCount);
        Serial.print(" | Counts per detent: ");
        Serial.println(abs(rawCount));
        
        // Hint about common values
        if (abs(rawCount) > 0 && abs(rawCount) <= 4) {
            Serial.print("  → Use this value: EEncoder enc(");
            Serial.print(ENCODER_PIN_A);
            Serial.print(", ");
            Serial.print(ENCODER_PIN_B);
            Serial.print(", buttonPin, ");
            Serial.print(abs(rawCount));
            Serial.println(");");
            Serial.println();
        }
        
        lastDisplayedCount = rawCount;
    }
    
    // Reset counter with serial input
    if (Serial.available()) {
        while (Serial.available()) Serial.read(); // Clear buffer
        rawCount = 0;
        lastDisplayedCount = 0;
        Serial.println("\n--- Counter reset ---");
        Serial.println("Rotate encoder one click...\n");
    }
}