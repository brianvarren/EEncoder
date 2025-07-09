/*
  EEncoder Multiple Encoders Example
  
  Demonstrates using multiple rotary encoders for a synthesizer-style
  interface with the EEncoder library on RP2040.
  
  This example simulates controlling:
  - Frequency (Encoder 1) - Wide range, benefits from acceleration
  - Filter cutoff (Encoder 2) - Wide range, benefits from acceleration
  - Volume (Encoder 3) - Narrow range, no acceleration needed
  
  Each encoder can also be pressed to reset its parameter to default.
  Long press enters fine-tune mode for precise adjustments.
*/

#include <EEncoder.h>

// Encoder 1 - Frequency control
const uint8_t ENC1_PIN_A = 2;
const uint8_t ENC1_PIN_B = 3;
const uint8_t ENC1_BUTTON = 4;

// Encoder 2 - Filter control
const uint8_t ENC2_PIN_A = 5;
const uint8_t ENC2_PIN_B = 6;
const uint8_t ENC2_BUTTON = 7;

// Encoder 3 - Volume control
const uint8_t ENC3_PIN_A = 8;
const uint8_t ENC3_PIN_B = 9;
const uint8_t ENC3_BUTTON = 10;

// Create encoder instances
// All encoders use 4 counts per detent (typical for most encoders)
EEncoder encFreq(ENC1_PIN_A, ENC1_PIN_B, ENC1_BUTTON, 4);
EEncoder encFilter(ENC2_PIN_A, ENC2_PIN_B, ENC2_BUTTON, 4);
EEncoder encVolume(ENC3_PIN_A, ENC3_PIN_B, ENC3_BUTTON, 4);

// Synthesizer parameters
struct SynthParams {
    int frequency = 440;      // Hz (A4)
    int filterCutoff = 1000;  // Hz
    int volume = 50;          // 0-100
    
    // Fine tune modes
    bool freqFineTune = false;
    bool filterFineTune = false;
} synth;

// Frequency encoder callback
void onFreqChange(EEncoder& enc) {
    int8_t increment = enc.getIncrement();  // Always ±1 per detent (or more with acceleration)
    
    if (synth.freqFineTune) {
        // Fine tune: 1Hz steps
        synth.frequency += increment;
    } else {
        // Normal: 10Hz per click (×10), can accelerate up to 50Hz (×50)
        synth.frequency += increment * 10;
    }
    
    // Constrain to reasonable range
    synth.frequency = constrain(synth.frequency, 20, 20000);
    
    Serial.print("Frequency: ");
    Serial.print(synth.frequency);
    Serial.print(" Hz");
    if (synth.freqFineTune) Serial.print(" [FINE]");
    if (abs(increment) > 1) Serial.print(" [FAST]");
    Serial.println();
}

// Filter encoder callback
void onFilterChange(EEncoder& enc) {
    int8_t increment = enc.getIncrement();  // Always ±1 per detent (or more with acceleration)
    
    if (synth.filterFineTune) {
        // Fine tune: 10Hz steps
        synth.filterCutoff += increment * 10;
    } else {
        // Normal: 50Hz per click, can accelerate
        synth.filterCutoff += increment * 50;
    }
    
    // Constrain to reasonable range
    synth.filterCutoff = constrain(synth.filterCutoff, 20, 10000);
    
    Serial.print("Filter Cutoff: ");
    Serial.print(synth.filterCutoff);
    Serial.print(" Hz");
    if (synth.filterFineTune) Serial.print(" [FINE]");
    if (abs(increment) > 1) Serial.print(" [FAST]");
    Serial.println();
}

// Volume encoder callback (no acceleration needed for 0-100 range)
void onVolumeChange(EEncoder& enc) {
    // Simple increment - always ±1 per physical click
    synth.volume += enc.getIncrement();
    
    // Constrain to 0-100
    synth.volume = constrain(synth.volume, 0, 100);
    
    Serial.print("Volume: ");
    Serial.print(synth.volume);
    Serial.println("%");
}

// Button callbacks to reset parameters
void onFreqReset(EEncoder& enc) {
    synth.frequency = 440;
    synth.freqFineTune = false;
    Serial.println("Frequency reset to 440 Hz");
}

void onFilterReset(EEncoder& enc) {
    synth.filterCutoff = 1000;
    synth.filterFineTune = false;
    Serial.println("Filter reset to 1000 Hz");
}

void onVolumeReset(EEncoder& enc) {
    synth.volume = 50;
    Serial.println("Volume reset to 50%");
}

// Long press callbacks for fine tune mode
void onFreqLongPress(EEncoder& enc) {
    synth.freqFineTune = !synth.freqFineTune;
    Serial.print("Frequency fine tune: ");
    Serial.println(synth.freqFineTune ? "ON" : "OFF");
}

void onFilterLongPress(EEncoder& enc) {
    synth.filterFineTune = !synth.filterFineTune;
    Serial.print("Filter fine tune: ");
    Serial.println(synth.filterFineTune ? "ON" : "OFF");
}

void setup() {
    // Initialize serial
    Serial.begin(115200);
    while (!Serial) {
        ;
    }
    
    Serial.println("EEncoder Multiple Encoders Example");
    Serial.println("Synthesizer Control Demo");
    Serial.println("------------------------");
    Serial.println("Turn quickly for acceleration (freq & filter)");
    Serial.println("Long press for fine-tune mode");
    
    // Set up encoder callbacks
    encFreq.setEncoderHandler(onFreqChange);
    encFreq.setButtonHandler(onFreqReset);
    encFreq.setLongPressHandler(onFreqLongPress);
    
    encFilter.setEncoderHandler(onFilterChange);
    encFilter.setButtonHandler(onFilterReset);
    encFilter.setLongPressHandler(onFilterLongPress);
    
    encVolume.setEncoderHandler(onVolumeChange);
    encVolume.setButtonHandler(onVolumeReset);
    // No long press for volume - not needed
    
    // Enable acceleration for frequency and filter (wide ranges)
    encFreq.setAcceleration(true);
    encFreq.setAccelerationRate(5);  // Up to 50Hz steps
    
    encFilter.setAcceleration(true);
    encFilter.setAccelerationRate(4); // Up to 200Hz steps
    
    // No acceleration for volume (narrow range)
    encVolume.setAcceleration(false);
    
    // Show initial values
    Serial.print("Initial - Freq: ");
    Serial.print(synth.frequency);
    Serial.print(" Hz, Filter: ");
    Serial.print(synth.filterCutoff);
    Serial.print(" Hz, Volume: ");
    Serial.print(synth.volume);
    Serial.println("%");
}

void loop() {
    // Update all encoders
    encFreq.update();
    encFilter.update();
    encVolume.update();
    
    // In a real synthesizer, you would update your
    // audio generation parameters here based on the
    // current synth struct values
}