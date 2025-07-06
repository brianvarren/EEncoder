/*
  EEncoder - A clean rotary encoder library for RP2040
  Optimized for digital synthesizer UI controls
  
  Features:
  - Callback-based event handling
  - Built-in debouncing
  - Single count per detent
  - Simple, clean API
*/

#ifndef EEncoder_h
#define EEncoder_h

#include <Arduino.h>

// Default debounce time in milliseconds (same as Bounce2)
#define DEFAULT_DEBOUNCE_MS 10

// Default long press duration in milliseconds
#define DEFAULT_LONG_PRESS_MS 500

// Default acceleration threshold - turns faster than this trigger acceleration
#define ACCELERATION_THRESHOLD_MS 100

// Default acceleration multiplier
#define DEFAULT_ACCELERATION_RATE 5

// Forward declaration
class EEncoder;

// Callback function types
typedef void (*EncoderCallback)(EEncoder& encoder);
typedef void (*ButtonCallback)(EEncoder& encoder);

class EEncoder {
public:
    // Constructor for encoder with button
    EEncoder(uint8_t pinA, uint8_t pinB, uint8_t buttonPin);
    
    // Constructor for encoder without button
    EEncoder(uint8_t pinA, uint8_t pinB);
    
    // Must be called in loop() as often as possible
    void update();
    
    // Set callback handlers
    void setEncoderHandler(EncoderCallback callback);
    void setButtonHandler(ButtonCallback callback);
    void setLongPressHandler(ButtonCallback callback);
    
    // Get the increment value since last callback
    // Returns base increment multiplied by acceleration factor
    int8_t getIncrement() const { return _increment; }
    
    // Configuration methods
    void setDebounceInterval(uint16_t intervalMs);
    void setLongPressDuration(uint16_t durationMs);
    void setAcceleration(bool enabled);
    void setAccelerationRate(uint8_t rate);
    
    // Enable/disable the encoder
    void enable(bool enabled = true);
    bool isEnabled() const { return _enabled; }
    
private:
    // Pin assignments
    uint8_t _pinA;
    uint8_t _pinB;
    uint8_t _buttonPin;
    bool _hasButton;
    
    // State tracking for encoder
    uint8_t _lastEncoderState;
    uint8_t _encoderState;
    int8_t _increment;
    
    // State tracking for button
    bool _buttonState;
    bool _lastButtonState;
    uint32_t _buttonStateChangeTime;
    uint32_t _buttonPressTime;
    bool _longPressHandled;
    
    // Debouncing
    uint16_t _debounceInterval;
    uint32_t _lastEncoderTime;
    
    // Long press
    uint16_t _longPressDuration;
    
    // Acceleration
    bool _accelerationEnabled;
    uint8_t _accelerationRate;
    uint32_t _lastRotationTime;
    
    // Callbacks
    EncoderCallback _encoderCallback;
    ButtonCallback _buttonCallback;
    ButtonCallback _longPressCallback;
    
    // Enable state
    bool _enabled;
    
    // Internal methods
    void readEncoder();
    void readButton();
    uint8_t getEncoderState();
};

#endif // EEncoder_h