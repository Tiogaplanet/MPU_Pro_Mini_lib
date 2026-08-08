/**
 * @file FallDown.ino
 * @brief Example sketch demonstrating MiP's fall forward and backward actions.
 *
 * @details This sketch demonstrates how to use the MiP library to command MiP
 * to intentionally fall forward and backward using the motion.fallForward()
 * and motion.fallBackward() APIs. The sketch first waits for MiP to be
 * standing upright (position.isUpright()) before issuing each fall command and prints
 * status messages to mip.console so the sequence can be observed.
 *
 * The example exercises these API calls:
 *   - motion.fallForward()
 *   - motion.fallBackward()
 *   - position.isUpright()
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
 * @brief Global MiP instance used to communicate with MiP.
 *
 * @details Use this object to call MiP API functions such as begin(),
 * position.isUpright(), motion.fallForward(), and motion.fallBackward().
 */
MiP mip;

/**
 * @brief Arduino setup function.
 *
 * @details Initializes communication with MiP by calling mip.begin().
 * If the connection fails, an error message is printed to mip.console and setup
 * returns early. On success, the function:
 *   - Waits until MiP reports it is upright using position.isUpright().
 *   - Pauses briefly to ensure stability.
 *   - Commands MiP to fall forward with motion.fallForward().
 *   - Waits again for MiP to be upright, then commands a fall
 *     backward with motion.fallBackward().
 *
 * The function prints progress and status messages to mip.console to make the
 * demonstration easy to follow.
 */
void setup() {
  bool connectResult = mip.begin();

  if (!connectResult) {
    Serial.println(F("FallDown.ino: Failed connecting to MiP!"));
    return;
  }

  mip.console.println(F("FallDown.ino: Fall forward and backward.\n"));

  mip.console.println(F(" Waiting for MiP to be standing upright."));
  while (!mip.position.isUpright()) {
    // Waiting for the robot to report upright state.
  }
  delay(1000);

  mip.console.println(F(" Falling forward."));
  mip.motion.fallForward();

  delay(1000);
  mip.console.println(F(" Waiting for robot to be standing upright again."));
  while (!mip.position.isUpright()) {
    // Waiting for the robot to report upright state after falling.
  }
  delay(1000);

  mip.console.println(F(" Falling backward."));
  mip.motion.fallBackward();

  mip.console.println();
  mip.console.println(F("FallDown.ino: Done."));
}

/**
 * @brief Arduino loop function.
 *
 * @details This example performs all actions in setup() and does not require
 * repeated work in loop(). The function is intentionally left empty so the
 * demonstration runs only once during initialization.
 */
void loop() {
}
