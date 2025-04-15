# ☀️ Solar Panel Controller
*An intelligent sun-tracking system for optimal water heating*

## Project Overview
This system automatically orients heavy solar panels to maximize sun exposure while preventing mechanical strain. Born from the need to efficiently heat water using solar energy, it combines precise light sensing with robust safety mechanisms.

## Key Features
- **Smart Sun Tracking**: Uses dual LDR sensors to precisely detect sun position
- **Safety First**: Physical limit switches prevent over-rotation damage
- **Energy Efficient**: Operates in 30-second active / 10-minute sleep cycles
- **Reliable Control**: 4-channel relay manages panel adjustment motor
- **Clear Status**: LED indicators show system state

## Technical Implementation
- **Core Logic**: Compares light levels from two LDR sensors
- **Motor Control**: Activates relays to rotate panel toward stronger light
- **Safety**: Immediately stops at limit switch activation
- **Optimization**: Includes calibration factors for sensor balancing

## Implementation Options
- **Arduino Version**: `/Arduino` - Traditional C++ implementation with:
  - Custom LDR light sensor class
  - Debounced Button class for switches
  - Timer-based operation
- **XOD Version**: `/XOD` - Visual programming alternative

## Tools & Libraries
- **Arduino IDE** - Primary development environment
- **XOD** - Visual programming alternative
- **Timer library** - For scheduling active/sleep cycles
- **Custom Classes**:
  - `LDR.h/cpp` - Light sensor interface
  - `Button.h/cpp` - Debounced switch input

## Project Structure
```
SolarController/
├── Arduino/            # Arduino sketch files
├── Firmware/          # Core component classes
└── XOD/               # Visual programming version
```

*"Harnessing the sun's power, one careful rotation at a time"*
