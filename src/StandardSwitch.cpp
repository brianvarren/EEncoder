// ========== StandardSwitch.cpp ==========
#include "StandardSwitch.h"

StandardSwitch::StandardSwitch(uint8_t pin, bool activeLow) 
    : _pin(pin), _activeLow(activeLow) {
    pinMode(_pin, INPUT_PULLUP);
    
    // Read initial state
    _lastReading = digitalRead(_pin);
    _state = _activeLow ? !_lastReading : _lastReading;
}

void StandardSwitch::update() {
    if (!_enabled) return;
    
    bool currentReading = digitalRead(_pin);
    
    // Check if reading has changed
    if (currentReading != _lastReading) {
        _lastChangeTime = millis();
    }
    
    // If reading has been stable for debounce period
    if ((millis() - _lastChangeTime) > _debounceDuration) {
        bool newState = _activeLow ? !currentReading : currentReading;
        
        // State has changed
        if (newState != _state) {
            _state = newState;
            _changed = true;
            
            // Fire appropriate callbacks
            if (_changeHandler) {
                _changeHandler(*this);
            }
            
            if (_state && _onHandler) {
                _onHandler(*this);
            } else if (!_state && _offHandler) {
                _offHandler(*this);
            }
        } else {
            _changed = false;
        }
    }
    
    _lastReading = currentReading;
}