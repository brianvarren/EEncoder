#ifndef MOMOFFMOMSWITCH_H
#define MOMOFFMOMSWITCH_H

#include <Arduino.h>

class MomOffMomSwitch {
public:
    // Constructor
    MomOffMomSwitch(uint8_t upPin, uint8_t downPin);
    
    // Callback type
    typedef void (*MomOffMomHandler)(MomOffMomSwitch& sw);
    
    // Must be called in loop()
    void update();
    
    // Set callback handler
    void setHandler(MomOffMomHandler handler);
    
    // Get the last increment (-1 for down, 1 for up)
    int8_t getIncrement() const { return _lastIncrement; }
    
    // Configuration methods
    void setDebounceDuration(uint32_t ms) { _debounceDuration = ms; }
    void setRepeatDelay(uint32_t ms) { _repeatDelay = ms; }
    void setRepeatRate(uint32_t ms) { _repeatRate = ms; }
    void setAcceleration(bool enabled) { _accelerationEnabled = enabled; }
    void setAccelerationRate(uint8_t rate) { _accelerationRate = rate; }
    
    // Enable/disable
    void setEnabled(bool enabled) { _enabled = enabled; }
    bool isEnabled() const { return _enabled; }
    
    // State queries
    bool isUpPressed() const { return _upPressed; }
    bool isDownPressed() const { return _downPressed; }
    
private:
    // Pins
    uint8_t _upPin;
    uint8_t _downPin;
    
    // State
    bool _upPressed = false;
    bool _downPressed = false;
    bool _lastUpState = HIGH;
    bool _lastDownState = HIGH;
    bool _enabled = true;
    
    // Timing
    uint32_t _lastUpChange = 0;
    uint32_t _lastDownChange = 0;
    uint32_t _lastRepeatTime = 0;
    uint32_t _pressStartTime = 0;
    
    // Configuration
    uint32_t _debounceDuration = 10;  // 10ms default debounce
    uint32_t _repeatDelay = 500;      // 500ms before repeat starts
    uint32_t _repeatRate = 100;       // 100ms between repeats
    bool _accelerationEnabled = false;
    uint8_t _accelerationRate = 5;
    
    // Callback
    MomOffMomHandler _handler = nullptr;
    
    // Last action
    int8_t _lastIncrement = 0;
    
    // Private methods
    void handleButtonChange(bool isUpButton, bool currentState);
    void handleRepeat();
    uint8_t calculateAcceleration();
};

#endif // MOMOFFMOMSWITCH_H