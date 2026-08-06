# MiP Power Up - Pro Mini
![The MiP Power Up - Pro Mini mounted on MiP](https://github.com/Tiogaplanet/Experimenting-with-the-MiP/blob/48a70aedf950a8fad7d521446181a175cac245fa/images/MPU_Pro_Mini.jpg)

This project provides a library for the Arduino IDE and allows users to take control of [WowWee Labs'](https://github.com/WowWeeLabs/)  [MiP](https://wowwee.com/mip) robot.

MiP is a hacker friendly self-balancing robot. WowWee not only provides the [MiP Protocol Specification on GitHub](https://github.com/WowWeeLabs/MiP-BLE-Protocol), they also provide a [4-pin serial port](https://cdn.sparkfun.com/assets/learn_tutorials/2/8/5/HackingPortAnnotated.png), complete with JST connector, right on the mainboard. This connector makes it easy to connect an external controller such as the [Arduino Pro Mini](https://docs.arduino.cc/retired/boards/arduino-pro-mini/) or compatible boards and take control of your MiP. Once connected, you can:
*   Command the speed and direction of motion for the gravity defying MiP.
*   Command the individual control (on, off, blink) of the 4 LED eye segments on the head.
*   Take full control of the RGB LED in MiP's chest.
*   Command the playback of sound lists using the >100 built-in sounds.
*   Use the head mounted IR sensors to read 'radar' distance measurements or detected user hand gestures.
*   Detect user claps with the built-in microphone.
*   Detect the MiP's current pose via its inertial sensors, the same sensors that make its balancing magic possible.
*   And more!
  
Be sure to check out the [MiP Power Up - Pro Mini](https://github.com/Tiogaplanet/MPU_Pro_Mini) (Coming soon) which conveniently allows you to mount an Arduino Pro Mini to MiP's battery compartment.

## Acknowledgement
*   This project is a fork of adamgreen's [MiP ProMini-Pack](https://github.com/adamgreen/MiP_ProMini-Pack) which is in turn based on the now retired Sparkfun MiP ProMini Pack. 

## Installation
*  The MiP Power Up - Pro Mini library is intended for use with the Arduino IDE.  Installation is the same as for other libraries.  Download the zip and select `Sketch->Include Library->Add .ZIP Library...`.  Browse to the downloaded zip file and the Arduino IDE will do the rest.

## Usage
A very thorough guide to using the MiP Power Up - Pro Mini library is provided in the [wiki](https://github.com/Tiogaplanet/MiP_Power_Up_Pro_Mini_lib/wiki). (Coming soon.)

## Contributing
This project is intended to make programming MiP easy and fun.  To that end, contributions are highly encouraged!  Please see [CONTRIBUTING.md](https://github.com/Tiogaplanet/MPU_Pro_Mini_lib/blob/master/CONTRIBUTING.md) for more information.
