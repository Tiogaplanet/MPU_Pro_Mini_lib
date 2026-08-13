# MiP Power Up - Pro Mini Library
**Transform your WowWee MiP into an autonomous, programmable robotics platform with Arduino.**

![The MiP Power Up - Pro Mini mounted on MiP](https://github.com/Tiogaplanet/Experimenting-with-the-MiP/blob/48a70aedf950a8fad7d521446181a175cac245fa/images/MPU_Pro_Mini.jpg)

This Arduino library provides total control over [WowWee Labs’ MiP](https://wowwee.com/mip) — the self-balancing, hacker-friendly robot. Paired with an ATmega328p board (such as the [Arduino Pro Mini](https://docs.arduino.cc/retired/boards/arduino-pro-mini/)), this library unlocks full access to MiP’s motion, lighting, sound, sensors, and telemetry through a clean, object-oriented C++ API.

MiP natively provides a 4-pin expansion port and a published [BLE protocol specification](https://github.com/WowWeeLabs/MiP-BLE-Protocol). The **MiP Power Up — Pro Mini** library abstracts that low-level hardware protocol into intuitive subsystems, making MiP completely programmable for custom behavior, obstacle navigation, interactive games, and sensor tracking.

---

## What's New in Version 2.0
The latest release brings substantial architectural cleanups, improved execution efficiency, and robust hardware transport reliability:

- **Auto-Switching UART Console:** Transparent redirection of `Print`/`Stream` messages to the PC Serial Monitor over the Pro Mini's shared hardware UART using automatic multiplexer toggling.
- **Hardware Multiplexer Safety:** Protected hardware multiplexer transitions maintain state integrity across I/O operations and eliminate cross-talk between debug prints and MiP commands.
- **Zero-Copy & Modern C++ Cleanups:** Full C++11 standard compliance, `explicit` conversions, `static constexpr` constants replacing legacy `#define`s, and `constexpr` hex parsing.
- **Enhanced Verification & Retries:** Automatic command retry loops with verified read-back checks for mode changes, chest/head LED colors, and settings updates.
- **Human-Centric Terminology:** Refactored documentation and inline Doxygen comments treating MiP as an active agent.

---

## Why This Library?
- **Modular Subsystem API:** Dedicated, encapsulated classes for `motion`, `headLEDs`, `chestLED`, `sound`, `radar`, `gesture`, `clap`, `shake`, `odometer`, `position`, `battery`, and `eeprom`.
- **Transparent Console Output:** Built-in console routing allows seamless sharing of the Pro Mini's single UART between MiP and PC.
- **Hardware Integration:** Tailored to work seamlessly with the [MiP Power Up - Pro Mini](https://github.com/Tiogaplanet/MPU_Pro_Mini) adapter board mounted directly to MiP's battery compartment.

---

## What You Can Do
- **Motion & Drive Control:** Execute continuous velocity/turn commands, drive fixed distances (cm), execute timed moves, turn by precise angles, or command self-righting stand-ups.
- **Custom Lighting:** Independently control the four head/eye LEDs (on, off, slow/fast blink) and configure the full RGB chest LED with flash timing.
- **Audio Sequences:** Play any of over 100 built-in sound clips and voice lines, adjust volume dynamically, or assemble multi-sound audio queues.
- **Sensor Tracking:** Read front IR radar distances, detect swipe/hold hand gestures, listen for claps, detect physical shakes, and track posture pose (upright, face down, on back, picked up).
- **Telemetry & Storage:** Measure wheel encoder distances with the wheel odometer, monitor battery voltage in Volts, and read/write non-volatile user EEPROM memory.

---

## Hardware
This library is designed for ATmega328p boards running at 5V / 16MHz or 3.3V / 8MHz connected to MiP’s 4-pin expansion port:
- **[Arduino Pro Mini](https://docs.arduino.cc/retired/boards/arduino-pro-mini/)** (or ATmega328p compatibles)
- **[MiP Power Up — Pro Mini Board](https://github.com/Tiogaplanet/MPU_Pro_Mini):** Dedicated expansion shield with a hardware UART multiplexer that mounts cleanly on MiP’s rear battery compartment.

---

## Installation
1. In the Arduino IDE, go to **Sketch → Include Library → Add .ZIP Library…**
2. Browse to and select the downloaded `.zip` release of this library.
3. Alternatively, extract the library folder directly into your `Arduino/libraries/` directory.

---

## Quick Start

```cpp
#include <MiP_Power_Up_-_Pro_Mini.h>

MiP mip;

void setup() {
  // Initialize console and hardware UART connection to MiP
  mip.begin();
  
  // Set chest LED to green and turn on eyes
  mip.chestLED.write(0, 255, 0);
  mip.headLEDs.write(MIP_HEAD_LED_ON, MIP_HEAD_LED_ON, MIP_HEAD_LED_ON, MIP_HEAD_LED_ON);

  // Greet user
  mip.sound.play(MIP_SOUND_MIP_HI_CONFIDENT);
}

void loop() {
  // Drive forward 20 cm
  mip.motion.distanceDrive(MIP_DRIVE_FORWARD, 20, MIP_TURN_LEFT, 0);
  delay(3000);

  // Print debug info to PC Serial Monitor (multiplexer handles state automatically)
  mip.console.print(F("Battery Voltage: "));
  mip.console.println(mip.battery.readVoltage());

  delay(2000);
}
```

Full example sketches are included under File → Examples → MiP Power Up - Pro Mini and detailed guides are available on the [wiki](https://github.com/Tiogaplanet/MPU_Pro_Mini_lib)
## Acknowledgements
Forked from Adam Green's MiP_ProMini-Pack, which originated from the SparkFun MiP ProMini Pack.

Official protocol documentation provided by WowWee Labs.

## Contributing
Contributions are welcome! Whether fixing bugs, adding examples, or enhancing documentation, please review CONTRIBUTING.md before opening pull requests.

---


**Ready to start hacking with MiP?**
Grab the library, flash your Arduino Pro Mini, and bring your robot to life!
