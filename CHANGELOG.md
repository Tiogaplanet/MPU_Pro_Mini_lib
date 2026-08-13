# Changelog

All notable changes to the MiP Power Up — Pro Mini library are documented in this file.

## [2.0.0] - 2026-08-13

### Added
- Modular Subsystem API Architecture: Complete migration to the 2.0.0 modular subsystem architecture. Core functionality is now organized into dedicated, encapsulated sub-objects (`motion`, `headLEDs`, `chestLED`, `sound`, `radar`, `gesture`, `clap`, `shake`, `odometer`, `position`, `battery`, `eeprom`, and `console`).
- Auto-Switching UART Console (`MiP_Console`): Built-in support for standard Arduino `Print`/`Stream` operations over the Pro Mini's single hardware UART. Automatically manages hardware multiplexer state switching to PC before transmission and restores communication back to MiP afterward.
- Doxygen Documentation Standard: Comprehensive inline Doxygen comments added across all public interfaces and header files (`MPU_Console.h`, `MPU_Serial.h`, etc.).
- Human-Centric Terminology: Standardized documentation and code comments to refer to **MiP** as an active agent rather than using legacy phrasing like "the MiP robot".

### Changed
- Modern C++ Cleanups: Refactored magic numbers and legacy `#define` values to strongly typed `static constexpr` constants across transport, serial, and console modules.
- Zero-Copy & `constexpr` Logic: Implemented `constexpr` hex-ASCII parsing compatible with C++11/14 compilers (using nested ternary logic for AVR GCC target compatibility).
- Access Control & Encapsulation: Enforced `explicit` constructors and operator overloads (`explicit operator bool() const`), updated method const-correctness, and tightened private/protected access specifiers.
- RAII Hardware Scope Guards: Added protective scope management (`prepareForPcWrite()` / `restoreAfterPcWrite()`) inside `MiP_Console` to prevent multiplexer state corruption and ensure exception-safe hardware restoration.

### Fixed
- UART Multiplexer Synchronization: Resolved state leakage bugs where debug console output interfered with active MiP command packets.
- Initialization State Tracking Bug: Fixed bug where `initIfNeeded()` and `begin()` in `MiP_Console` failed to set initialization flags, preventing continuous hardware re-initialization loops.
- AVR GCC `constexpr` Function Errors: Fixed C++11 build failures caused by multi-statement `constexpr` function bodies in `MPU_Serial.cpp`.
- Debug Macro Signature Mismatches: Corrected debug print macro calls (`MIP_DEBUG_ERROR_PRINT`, `MIP_DEBUG_WARN_PRINTLN`) across transport files.

## [1.0.0] - 2018-05-18

### Added
- Initial release of the MiP Power Up — Pro Mini Arduino library.
- Basic support for drive control, chest LED RGB configuration, sound playback, and basic sensor reading over UART.
- Support for ATmega328p / Arduino Pro Mini microcontroller hardware.
