# Changelog

All notable changes to the EEncoder library will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.2.0] - 2025-07-08

### Changed
- Improved state machine for more robust detent detection
- Removed encoder debouncing (not needed with proper quadrature decoding)
- Debouncing now applies to button only
- Added idle timeout to resynchronize position and prevent drift
- More reliable detection at all rotation speeds

### Fixed
- Missing detents during fast rotation
- Out-of-sync detection causing false or missed detents
- Drift accumulation from missed state transitions

## [1.1.0] - 2025-07-08

### Added
- Hardware normalization: New `countsPerDetent` parameter in constructors
- Automatic handling of encoder hardware differences (1, 2, or 4 counts per detent)
- Always outputs ±1 increment per physical detent regardless of hardware

### Changed
- Constructor signature now includes optional `countsPerDetent` parameter (defaults to 4)
- `getIncrement()` now returns normalized values
- Examples updated to demonstrate hardware normalization

### Backwards Compatibility
- Existing code will work without changes (defaults to 4 counts per detent)
- Only need to specify `countsPerDetent` if your encoder differs from the default

## [1.0.0] - 2025-07-06

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