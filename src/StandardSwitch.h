// ========== StandardSwitch.h ==========
/*
 * StandardSwitch - Handler for standard on-off toggle switches
 * Part of the EEncoder library family
 */

#ifndef STANDARDSWITCH_H
#define STANDARDSWITCH_H

#include <Arduino.h>

class StandardSwitch {
public:
    // Constructor
    StandardSwitch(uint8_t pin, bool activeLow = true);
    
    // Callback types
    typedef void (*SwitchHandler)(StandardSwitch& sw);
    
    // Must be called in loop()
    void update();
    
    // Set callback handlers
    void setOnHandler(SwitchHandler handler) { _onHandler = handler; }
    void setOffHandler(SwitchHandler handler) { _offHandler = handler; }
    void setChangeHandler(SwitchHandler handler) { _changeHandler = handler; }
    
    // Get state
    bool isOn() const { return _state; }
    bool isOff() const { return !_state; }
    bool hasChanged() const { return _changed; }
    
    // Configuration
    void setDebounceDuration(uint32_t ms) { _debounceDuration = ms; }
    void setEnabled(bool enabled) { _enabled = enabled; }
    bool isEnabled() const { return _enabled; }
    
private:
    uint8_t _pin;
    bool _activeLow;
    bool _state = false;
    bool _lastReading = false;
    bool _changed = false;
    bool _enabled = true;
    
    uint32_t _lastChangeTime = 0;
    uint32_t _debounceDuration = 10;
    
    SwitchHandler _onHandler = nullptr;
    SwitchHandler _offHandler = nullptr;
    SwitchHandler _changeHandler = nullptr;
};

#endif // STANDARDSWITCH_H

