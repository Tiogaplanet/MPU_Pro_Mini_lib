/**
 * @file Volume.ino
 * @brief Example sketch demonstrating reading and writing MiP's audio volume.
 *
 * @details
 * This sketch connects to MiP and demonstrates how to set its 
 * volume using sound.writeVolume() and then read the current volume back using
 * sound.readVolume(). The example sets the volume to the predefined constant
 * MIP_VOLUME_OFF (mute) and prints the resulting volume level to mip.console.
 *
 * The example exercises these API calls:
 *   - sound.writeVolume()
 *   - sound.readVolume()
 *
 * Usage notes:
 *   - Ensure MiP is powered and able to accept UART commands.
 *   - Adjust the volume value passed to sound.writeVolume() to experiment with
 *     different audio levels supported by MiP.
 *
 * @author Adam Green (Original Author)
 * @author Samuel Trassare (Maintainer)
 * @copyright Copyright (C) 2018-2026 Samuel Trassare
 * (https://github.com/Tiogaplanet) Licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at
 * http://www.apache.org/licenses/LICENSE-2.0
 */
#include <MiP_Power_Up_-_Pro_Mini.h>

/**
 * @brief Global MiP instance used to control the robot.
 *
 * @details Use this object to call MiP API functions such as begin(),
 * sound.writeVolume(), and sound.readVolume(). Keeping the instance at file
 * scope makes it available in both setup() and loop().
 */
MiP mip;

/**
 * @brief Arduino setup function.
 *
 * @details
 * - initializes MiP's connection via mip.begin().
 * - If the connection fails, prints an error to Serial and returns early.
 * - On success, sets MiP's volume to MIP_VOLUME_OFF using sound.writeVolume(),
 *   reads the current volume back with readVolume(), and prints the value to
 *   mip.console for verification.
 */
void setup() {
  bool connectResult = mip.begin();
  if (!connectResult) {
    Serial.println(F("Volume.ino: Failed connecting to MiP!"));
    return;
  }

  mip.console.println(
    F("Volume.ino: Use sound.readVolume() and sound.writeVolume(). Set "
      "volume level to off (0) and read out afterwards."));

  // Set the device volume to the predefined "off" constant.
  mip.sound.writeVolume(MIP_VOLUME_OFF);

  // Read the current volume level from the device.
  uint8_t volume = mip.sound.readVolume();

  mip.console.print(F(" Volume = "));
  mip.console.println(volume);

  mip.console.println();
  mip.console.println(F("Volume.ino: Done."));
}

/**
 * @brief Arduino loop function.
 *
 * @details This example performs its demonstration in setup() and does not
 * require repeated work in loop(). The function is intentionally left empty
 * so the sketch completes once during initialization.
 */
void loop() {}

