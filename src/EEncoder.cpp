/*
  EEncoder - A clean rotary encoder library for RP2040
  Implementation file
*/

#include "EEncoder.h"

// Constructor with button
EEncoder::EEncoder(uint8_t pinA, uint8_t pinB, uint8_t buttonPin) :
    _pinA(pinA),
    _pinB(pinB),
    _buttonPin(buttonPin),
    _hasButton(true),
    _lastEncoderState(0),
    _encoderState(0),
    _increment(0),
    _buttonState(HIGH),
    _lastButtonState(HIGH),
    _buttonStateChangeTime(0),
    _buttonPressTime(0),
    _longPressHandled(false),
    _debounceInterval(DEFAULT_DEBOUNCE_MS),
    _lastEncoderTime(0),
    _longPressDuration(DEFAULT_LONG_PRESS_MS),
    _accelerationEnabled(false),
    _accelerationRate(DEFAULT_ACCELERATION_RATE),
    _lastRotationTime(0),
    _encoderCallback(nullptr),
    _buttonCallback(nullptr),
    _longPressCallback(nullptr),
    _enabled(true)
{
    // Configure pins with INPUT_PULLUP (though external pullups are expected)
    pinMode(_pinA, INPUT_PULLUP);
    pinMode(_pinB, INPUT_PULLUP);
    pinMode(_buttonPin, INPUT_PULLUP);
    
    // Read initial encoder state
    _lastEncoderState = getEncoderState();
}

// Constructor without button
EEncoder::EEncoder(uint8_t pinA, uint8_t pinB) :
    _pinA(pinA),
    _pinB(pinB),
    _buttonPin(0),
    _hasButton(false),
    _lastEncoderState(0),
    _encoderState(0),
    _increment(0),
    _buttonState(HIGH),
    _lastButtonState(HIGH),
    _buttonStateChangeTime(0),
    _buttonPressTime(0),
    _longPressHandled(false),
    _debounceInterval(DEFAULT_DEBOUNCE_MS),
    _lastEncoderTime(0),
    _longPressDuration(DEFAULT_LONG_PRESS_MS),
    _accelerationEnabled(false),
    _accelerationRate(DEFAULT_ACCELERATION_RATE),
    _lastRotationTime(0),
    _encoderCallback(nullptr),
    _buttonCallback(nullptr),
    _longPressCallback(nullptr),
    _enabled(true)
{
    // Configure pins
    pinMode(_pinA, INPUT_PULLUP);
    pinMode(_pinB, INPUT_PULLUP);
    
    // Read initial encoder state
    _lastEncoderState = getEncoderState();
}

// Main update function - must be called frequently in loop()
void EEncoder::update() {
    if (!_enabled) return;
    
    readEncoder();
    
    if (_hasButton) {
        readButton();
    }
}

// Read current encoder state
uint8_t EEncoder::getEncoderState() {
    return (digitalRead(_pinA) << 1) | digitalRead(_pinB);
}

// Read and process encoder
void EEncoder::readEncoder() {
    _encoderState = getEncoderState();
    
    // Only process if state changed
    if (_encoderState != _lastEncoderState) {
        uint32_t currentTime = millis();
        
        // Simple debouncing - ignore changes too close together
        if ((currentTime - _lastEncoderTime) >= _debounceInterval) {
            // State machine for single count per detent
            // Using a simplified state table for common encoder types
            // This gives us one count per detent in most cases
            
            // Create a 4-bit value from old and new states
            uint8_t combined = (_lastEncoderState << 2) | _encoderState;
            
            // Determine direction based on state transition
            // These magic numbers represent the valid CW and CCW transitions
            // for a typical mechanical encoder with one count per detent
            switch (combined) {
                case 0b0001: // 00 -> 01
                case 0b0111: // 01 -> 11
                case 0b1110: // 11 -> 10
                case 0b1000: // 10 -> 00
                    _increment = 1;  // Clockwise
                    break;
                    
                case 0b0010: // 00 -> 10
                case 0b1011: // 10 -> 11
                case 0b1101: // 11 -> 01
                case 0b0100: // 01 -> 00
                    _increment = -1; // Counter-clockwise
                    break;
                    
                default:
                    _increment = 0;  // Invalid transition
                    break;
            }
            
            // Apply acceleration if enabled
            if (_increment != 0 && _accelerationEnabled) {
                uint32_t timeSinceLastRotation = currentTime - _lastRotationTime;
                
                // If rotating quickly, multiply increment
                if (timeSinceLastRotation < ACCELERATION_THRESHOLD_MS) {
                    _increment *= _accelerationRate;
                }
                
                _lastRotationTime = currentTime;
            }
            
            // Fire callback if we have a valid increment
            if (_increment != 0 && _encoderCallback != nullptr) {
                _encoderCallback(*this);
            }
            
            _lastEncoderTime = currentTime;
        }
        
        _lastEncoderState = _encoderState;
    }
}

// Read and process button
void EEncoder::readButton() {
    bool currentState = digitalRead(_buttonPin);
    
    // Check if state changed
    if (currentState != _lastButtonState) {
        _buttonStateChangeTime = millis();
    }
    
    // Check if we've passed the debounce interval
    if ((millis() - _buttonStateChangeTime) >= _debounceInterval) {
        // State has been stable for debounce interval
        if (currentState != _buttonState) {
            _buttonState = currentState;
            
            // Button pressed (transition to LOW)
            if (_buttonState == LOW) {
                _buttonPressTime = millis();
                _longPressHandled = false;
                
                // Fire regular press callback
                if (_buttonCallback != nullptr) {
                    _buttonCallback(*this);
                }
            }
            // Button released
            else {
                // Reset long press flag
                _longPressHandled = false;
            }
        }
    }
    
    // Check for long press while button is held
    if (_buttonState == LOW && !_longPressHandled && _longPressCallback != nullptr) {
        if ((millis() - _buttonPressTime) >= _longPressDuration) {
            _longPressHandled = true;
            _longPressCallback(*this);
        }
    }
    
    _lastButtonState = currentState;
}

// Set encoder rotation callback
void EEncoder::setEncoderHandler(EncoderCallback callback) {
    _encoderCallback = callback;
}

// Set button press callback
void EEncoder::setButtonHandler(ButtonCallback callback) {
    _buttonCallback = callback;
}

// Set long press callback
void EEncoder::setLongPressHandler(ButtonCallback callback) {
    _longPressCallback = callback;
}

// Set debounce interval in milliseconds
void EEncoder::setDebounceInterval(uint16_t intervalMs) {
    _debounceInterval = intervalMs;
}

// Set long press duration in milliseconds
void EEncoder::setLongPressDuration(uint16_t durationMs) {
    _longPressDuration = durationMs;
}

// Enable or disable acceleration
void EEncoder::setAcceleration(bool enabled) {
    _accelerationEnabled = enabled;
}

// Set acceleration multiplier
void EEncoder::setAccelerationRate(uint8_t rate) {
    _accelerationRate = rate;
}

// Enable or disable the encoder
void EEncoder::enable(bool enabled) {
    _enabled = enabled;
    
    // Reset increment when disabled
    if (!_enabled) {
        _increment = 0;
    }
}