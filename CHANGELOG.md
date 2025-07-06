# Changelog

All notable changes to the EEncoder library will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.0.0] - 2024-XX-XX

### Initial Release

#### Features
- Callback-based event handling for encoder rotation and button presses
- Built-in debouncing with configurable interval
- Single count per detent optimized for UI controls
- Long press detection with configurable duration
- Acceleration support for fast navigation of large value ranges
- Enable/disable functionality
- Support for encoders with or without buttons
- Minimal memory footprint (~28 bytes per instance)
- Zero dependencies - uses only Arduino core functions

#### Examples
- Basic single encoder with acceleration and long press
- Multiple encoders for synthesizer control
- Practical synthesizer parameter adjustment with note detection

#### Platform Support
- Optimized for RP2040 (Raspberry Pi Pico)
- Should work with other Arduino-compatible boards