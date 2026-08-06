/**
 * @file Clap.ino
 * @brief Example sketch demonstrating MiP clap event APIs.
 * @details
 * This Arduino sketch demonstrates how to use the MiP library's clap-related
 * functions to enable and disable clap event reporting, configure the clap
 * detection delay, and read detected clap events. The sketch performs the
 * following sequence in setup():
 *   - Initializes communication with MiP.
 *   - Disables clap events and verifies the disabled state using
 *     clap.areEventsEnabled().
 *   - Writes a clap delay value using clap.writeDelay() and reads it back with
 *     clap.readDelay().
 *   - Enables clap events and verifies the enabled state.
 * After initialization, the loop() continuously checks for available clap
 * events using clap.availableEvents() and reads each event with
 * clap.readEvent(), printing the number of detected claps to Serial.
 *
 * The example exercises these API calls:
 *   - clap.enableEvents()
 *   - clap.disableEvents()
 *   - clap.areEventsEnabled()
 *   - clap.writeDelay(uint16_t delay)
 *   - clap.readDelay()
 *   - clap.availableEvents()
 *   - clap.readEvent()
 *
 * This sketch prints status and results to Serial and is intended for use
 * with the MiP Power Up - D1 mini library and a WowWee MiP robot.
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
 * @brief Global MiP instance used to communicate with the robot.
 * @details
 * This object is used throughout the sketch to call MiP API functions such as
 * begin(), clap.enableEvents(), clap.disableEvents(), clap.writeDelay(),
 * clap.readDelay(), clap.availableEvents(), and clap.readEvent().
 */
MiP mip;

/**
 * @brief Tracks whether the initial connection to MiP succeeded.
 *
 * @details Stored so other parts of the sketch could check connection state
 * if extended.
 */
bool connectResult;

/**
 * @brief Arduino setup function.
 * @details
 * Called once after the board powers up or resets. This function:
 *  - Attempts to initialize communication with MiP via mip.begin().
 *  - If connection fails, prints an error to Serial and returns early.
 *  - Demonstrates disabling clap events and verifies the disabled state.
 *  - Writes a clap delay value (501 ms) and reads it back to verify.
 *  - Enables clap events and verifies the enabled state.
 *  - Prints a message indicating the sketch is waiting for clap events.
 *
 * The function prints status messages and verification results to Serial.
 */
void setup() {
  connectResult = mip.begin();
  if (!connectResult) {
    Serial.println(F("Clap.ino: Failed connecting to MiP!"));
    return;
  }

  Serial.println(F("Clap.ino: Use clap related functions."));

  Serial.println(F(" Calling clap.disableEvents()"));
  mip.clap.disableEvents();
  bool isEnabled = mip.clap.areEventsEnabled();
  Serial.print(F(" clap.areEventsEnabled() returns "));
  if (isEnabled) {
    Serial.println(F("true - fail"));
  } else {
    Serial.println(F("false - pass"));
  }

  Serial.println(F(" Calling clap.writeDelay(501)"));
  mip.clap.writeDelay(501);
  uint16_t delay = mip.clap.readDelay();
  Serial.print(F(" clap.readDelay() returns "));
  Serial.println(delay);

  Serial.println(F(" Calling clap.enableEvents()"));
  mip.clap.enableEvents();
  isEnabled = mip.clap.areEventsEnabled();
  Serial.print(F(" clap.areEventsEnabled() returns "));
  if (isEnabled) {
    Serial.println(F("true - pass"));
  } else {
    Serial.println(F("false - fail"));
  }

  Serial.println();
  Serial.println(F(" Waiting for clap events!"));
}

/**
 * @brief Arduino loop function.
 * @details
 * Called repeatedly after setup() completes. This implementation polls
 * MiP for pending clap events. While clap.availableEvents() reports
 * one or more events, clap.readEvent() is called to retrieve the clap count
 * for each event and the result is printed to Serial.
 *
 * The loop is non-blocking except for the time taken to process available
 * events; it returns quickly when no events are pending.
 */
void loop() {
  if (!connectResult)
    return;  // If connecting to MiP failed in setup(), exit now.

  while (mip.clap.availableEvents() > 0) {
    uint8_t clapCount = mip.clap.readEvent();
    Serial.print(F(" Detected "));
    Serial.print(clapCount);
    Serial.println(F(" claps"));
  }
}
