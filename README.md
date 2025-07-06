# EEncoder

A clean and simple rotary encoder library optimized for RP2040-based boards, perfect for digital synthesizer UI controls.

## Features

- **Callback-based event handling** - Keeps your main sketch clean and maintainable
- **Built-in debouncing** - Reliable operation without false triggers
- **Single count per detent** - Ideal for UI controls
- **Long press detection** - Separate callback for extended button presses
- **Acceleration support** - Automatically speeds up when turning quickly
- **Simple API** - Easy to set up and use
- **Multiple encoder support** - Use as many encoders as you have pins for
- **Minimal dependencies** - Just Arduino core, no external libraries needed

## Installation

### Via Arduino Library Manager (when available)
1. Open the Arduino IDE
2. Go to Sketch → Include Library → Manage Libraries
3. Search for "EEncoder"
4. Click Install

### Manual Installation
1. Download the library as a ZIP file
2. In the Arduino IDE, go to Sketch → Include Library → Add .ZIP Library
3. Select the downloaded ZIP file
4. The library is now ready to use

## Basic Usage

```cpp
#include <EEncoder.h>

// Create encoder with button on pins 2, 3, and 4
EEncoder encoder(2, 3, 4);

// Encoder rotation callback
void onEncoderTurn(EEncoder& enc) {
    int8_t increment = enc.getIncrement(); // Returns -1, 1, or more with acceleration
    Serial.println(increment > 0 ? "Clockwise" : "Counter-clockwise");
}

// Button press callback
void onButtonPress(EEncoder& enc) {
    Serial.println("Button pressed!");
}

// Long press callback
void onLongPress(EEncoder& enc) {
    Serial.println("Long press detected!");
}

void setup() {
    Serial.begin(115200);
    
    // Set up callbacks
    encoder.setEncoderHandler(onEncoderTurn);
    encoder.setButtonHandler(onButtonPress);
    encoder.setLongPressHandler(onLongPress);
    
    // Enable acceleration (optional)
    encoder.setAcceleration(true);
}

void loop() {
    // Must call update() as often as possible
    encoder.update();
}
```

## API Reference

### Constructors

```cpp
// Encoder with button
EEncoder(uint8_t pinA, uint8_t pinB, uint8_t buttonPin);

// Encoder without button
EEncoder(uint8_t pinA, uint8_t pinB);
```

### Methods

#### `void update()`
Must be called in your `loop()` function as often as possible. This reads the encoder and button states and fires callbacks when appropriate.

#### `void setEncoderHandler(EncoderCallback callback)`
Sets the function to be called when the encoder is rotated. The callback receives a reference to the EEncoder object.

#### `void setButtonHandler(ButtonCallback callback)`
Sets the function to be called when the button is pressed. The callback receives a reference to the EEncoder object.

#### `void setLongPressHandler(ButtonCallback callback)`
Sets the function to be called when the button is held down for the long press duration. The callback receives a reference to the EEncoder object.

#### `int8_t getIncrement()`
Returns the direction and magnitude of the last encoder movement:
- Without acceleration: returns -1 or 1
- With acceleration: returns -(rate) or +(rate) when turning quickly

#### `void setDebounceInterval(uint16_t intervalMs)`
Sets the debounce time in milliseconds. Default is 10ms.

#### `void setLongPressDuration(uint16_t durationMs)`
Sets how long the button must be held to trigger a long press. Default is 500ms.

#### `void setAcceleration(bool enabled)`
Enables or disables acceleration. When enabled, fast rotation multiplies the increment value.

#### `void setAccelerationRate(uint8_t rate)`
Sets the multiplication factor for acceleration. Default is 5. When turning quickly, increments are multiplied by this value.

#### `void enable(bool enabled = true)`
Enables or disables the encoder. When disabled, no callbacks will be fired.

#### `bool isEnabled()`
Returns true if the encoder is enabled.

## Acceleration Feature

The acceleration feature helps navigate large value ranges quickly:

```cpp
// Enable acceleration with 10x multiplier
encoder.setAcceleration(true);
encoder.setAccelerationRate(10);

// In your callback
void onEncoderTurn(EEncoder& enc) {
    int8_t increment = enc.getIncrement();
    // Normal turn: increment = -1 or 1
    // Fast turn: increment = -10 or 10
    
    value += increment;
}
```

Acceleration kicks in when you turn faster than 100ms between detents. This is perfect for:
- Frequency controls (20Hz to 20kHz)
- Time parameters (1ms to 5000ms)
- Large preset banks

## Long Press Feature

Long press is perfect for mode changes or secondary functions:

```cpp
void onButtonPress(EEncoder& enc) {
    // Quick press - adjust parameter
    selectParameter();
}

void onLongPress(EEncoder& enc) {
    // Long press - enter fine-tune mode
    toggleFineTuneMode();
}

// Adjust the duration if needed
encoder.setLongPressDuration(750); // 750ms for long press
```

## Hardware Setup

This library assumes you're using rotary encoder modules with built-in pull-up resistors. Connect your encoder as follows:

- Encoder A → Any GPIO pin
- Encoder B → Any GPIO pin  
- Encoder Button → Any GPIO pin (optional)
- Common → Ground
- + → 3.3V

## Examples

The library includes several example sketches:

1. **EEncoder_Basic** - Single encoder with value tracking, acceleration, and long press
2. **EEncoder_Multiple** - Multiple encoders for synthesizer-style control with different acceleration settings
3. **EEncoder_SynthParameter** - Real-world oscillator frequency control with musical note detection
4. **EEncoder_Menu** - Menu navigation with wraparound, demonstrating position tracking in application code

## Tips for Best Results

1. Call `update()` as frequently as possible in your main loop
2. Keep your callback functions short and fast
3. The default 10ms debounce time works well for most encoders
4. For high-quality encoders, you can reduce debounce time to 5ms
5. For noisy/cheap encoders, increase debounce time to 20-50ms
6. Use acceleration for parameters with wide ranges
7. Consider implementing fine-tune mode with long press for precise adjustments

## Memory Usage

The library is optimized for minimal memory usage:
- ~28 bytes RAM per encoder instance
- ~2KB flash for the library code

## Acknowledgments

This library was inspired by several excellent encoder libraries, particularly:
- [EncoderButton](https://github.com/Stutchbury/EncoderButton) by Stutchbury - for its clean callback-based API design
- [Encoder](https://github.com/PaulStoffregen/Encoder) by Paul Stoffregen - for reliable quadrature decoding concepts
- [Bounce2](https://github.com/thomasfredericks/Bounce2) by Thomas Fredericks - for effective debouncing strategies

While EEncoder shares no code with these libraries, they influenced the design philosophy of providing a clean, event-driven interface. EEncoder was built from scratch specifically for RP2040-based synthesizers, focusing on:
- Minimal memory footprint
- Single count per detent (ideal for UI controls)
- Synthesizer-specific features (acceleration, long-press)
- Zero dependencies

Development was assisted by Claude (Anthropic) in collaboration with Brian Varren.

## Contributing

Contributions are welcome! Please feel free to submit issues or pull requests.

## License

This library is released under the MIT License. See LICENSE file for details.