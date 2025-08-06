#include "MomOffMomSwitch.h"

MomOffMomSwitch::MomOffMomSwitch(uint8_t upPin, uint8_t downPin) 
    : _upPin(upPin), _downPin(downPin) {
    // Set up pins with internal pull-up resistors
    pinMode(_upPin, INPUT_PULLUP);
    pinMode(_downPin, INPUT_PULLUP);
    
    // Read initial states
    _lastUpState = digitalRead(_upPin);
    _lastDownState = digitalRead(_downPin);
}

void MomOffMomSwitch::update() {
    if (!_enabled) return;
    
    uint32_t now = millis();
    
    // Read current states (LOW when pressed with INPUT_PULLUP)
    bool upState = digitalRead(_upPin);
    bool downState = digitalRead(_downPin);
    
    // Handle up button
    if (upState != _lastUpState) {
        handleButtonChange(true, upState);
        _lastUpState = upState;
    }
    
    // Handle down button
    if (downState != _lastDownState) {
        handleButtonChange(false, downState);
        _lastDownState = downState;
    }
    
    // Handle auto-repeat if a button is held
    if ((_upPressed || _downPressed) && _handler) {
        handleRepeat();
    }
}

void MomOffMomSwitch::handleButtonChange(bool isUpButton, bool currentState) {
    uint32_t now = millis();
    uint32_t* lastChangeTime = isUpButton ? &_lastUpChange : &_lastDownChange;
    bool* pressedState = isUpButton ? &_upPressed : &_downPressed;
    
    // Check if enough time has passed for debouncing
    if (now - *lastChangeTime < _debounceDuration) {
        return;
    }
    
    *lastChangeTime = now;
    
    // Button is pressed (LOW with pull-up)
    if (currentState == LOW && !*pressedState) {
        *pressedState = true;
        _pressStartTime = now;
        _lastRepeatTime = now;
        
        // Fire callback immediately
        if (_handler) {
            _lastIncrement = isUpButton ? 1 : -1;
            _handler(*this);
        }
    }
    // Button is released
    else if (currentState == HIGH && *pressedState) {
        *pressedState = false;
        _pressStartTime = 0;
    }
}

void MomOffMomSwitch::handleRepeat() {
    if (!(_upPressed || _downPressed)) return;
    
    uint32_t now = millis();
    uint32_t timeSincePress = now - _pressStartTime;
    
    // Wait for repeat delay before starting auto-repeat
    if (timeSincePress < _repeatDelay) {
        return;
    }
    
    // Calculate repeat interval with acceleration if enabled
    uint32_t repeatInterval = _repeatRate;
    if (_accelerationEnabled) {
        uint8_t accel = calculateAcceleration();
        repeatInterval = _repeatRate / accel;
    }
    
    // Check if it's time for another repeat
    if (now - _lastRepeatTime >= repeatInterval) {
        _lastRepeatTime = now;
        
        // Determine direction and fire callback
        _lastIncrement = _upPressed ? 1 : -1;
        
        // Apply acceleration multiplier if enabled
        if (_accelerationEnabled) {
            _lastIncrement *= calculateAcceleration();
        }
        
        _handler(*this);
    }
}

uint8_t MomOffMomSwitch::calculateAcceleration() {
    if (!_accelerationEnabled) return 1;
    
    uint32_t holdTime = millis() - _pressStartTime;
    
    // Progressive acceleration based on hold time
    if (holdTime < 1000) return 1;        // First second: normal speed
    if (holdTime < 2000) return 2;        // 1-2 seconds: 2x speed
    if (holdTime < 3000) return 3;        // 2-3 seconds: 3x speed
    if (holdTime < 4000) return 4;        // 3-4 seconds: 4x speed
    return _accelerationRate;              // After 4 seconds: max acceleration
}

void MomOffMomSwitch::setHandler(MomOffMomHandler handler) {
    _handler = handler;
}