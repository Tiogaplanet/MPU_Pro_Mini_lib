/**
 * @file Status.ino
 * @brief Example sketch that monitors and reports MiP's status changes.
 *
 * @details
 * This sketch demonstrates how to query and report various status values from
 * MiP. It connects to MiP, then continuously polls for changes to the battery
 * voltage and the MiP's positional state. When a change is detected the new 
 * value is printed to mip.console. The sketch is intended as a simple 
 * diagnostic example to show how to use the status-related API calls.
 *
 * The example exercises these API calls:
 *   - battery.readVoltage()
 *   - position.read()
 *   - position.isOnBack()
 *   - position.isFaceDown()
 *   - position.isUpright()
 *   - position.isPickedUp()
 *   - position.isHandStanding()
 *   - position.isFaceDownOnTray()
 *   - position.isOnBackWithKickstand()
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
 * @details Use this object to call the MiP status APIs such as
 * battery.readVoltage() and position.read().
 */
MiP mip;

/**
 * @brief Last reported battery voltage (volts).
 *
 * @details Stored so the sketch only prints battery updates when the value
 * actually changes.
 */
static float lastBatteryLevel = 0.0f;

/**
 * @brief Last reported position enum value.
 *
 * @details Initialized to an invalid value so the first position.read() will
 * always be treated as a change and printed.
 */
static MiPPosition lastPosition = (MiPPosition)-1;

/**
 * @brief Tracks whether the initial connection to MiP succeeded.
 *
 * @details Stored so other parts of the sketch could check connection state
 * if extended.
 */
bool connectResult;

/**
 * @brief Arduino setup function.
 *
 * @details
 * - initializes MiP's connection via mip.begin().
 * - If the connection fails, prints an error to Serial and returns early.
 * - On success, prints a short banner indicating the sketch will display
 *   status changes.
 */
void setup() {
  connectResult = mip.begin();

  if (!connectResult) {
    Serial.println(F("Status.ino: Failed connecting to MiP!"));
    return;
  }

  mip.console.println(F("Status.ino: Display MiP's status as it changes."));
}

/**
 * @brief Arduino loop function.
 *
 * @details
 * - Polls MiP for the current battery voltage and position.
 * - If the battery voltage differs from the last reported value, prints the
 *   new voltage and updates lastBatteryLevel.
 * - If the position differs from the last reported position, queries the
 *   various position predicates (isOnBack(), isFaceDown(), isUpright(), etc.)
 *   and prints each matching position description. Updates lastPosition to
 *   avoid repeated prints for the same state.
 *
 * The loop is intentionally lightweight and prints only on changes to avoid
 * spamming mip.console.
 */
void loop() {
  if (!connectResult) return;  // If connecting to MiP failed in setup(), exit now.

  float currentBatteryLevel = mip.battery.readVoltage();
  MiPPosition currentPosition = mip.position.read();

  /* Report battery voltage when it changes. */
  if (currentBatteryLevel != lastBatteryLevel) {
    mip.console.print(F(" Battery: "));
    mip.console.print(currentBatteryLevel);
    mip.console.println(F("V"));
    lastBatteryLevel = currentBatteryLevel;
  }

  /* Report position changes by evaluating all position predicates. */
  if (currentPosition != lastPosition) {
    if (mip.position.isOnBack()) {
      mip.console.println(F(" Position: On Back"));
    }
    if (mip.position.isFaceDown()) {
      mip.console.println(F(" Position: Face Down"));
    }
    if (mip.position.isUpright()) {
      mip.console.println(F(" Position: Upright"));
    }
    if (mip.position.isPickedUp()) {
      mip.console.println(F(" Position: Picked Up"));
    }
    if (mip.position.isHandStanding()) {
      mip.console.println(F(" Position: Hand Stand"));
    }
    if (mip.position.isFaceDownOnTray()) {
      mip.console.println(F(" Position: Face Down on Tray"));
    }
    if (mip.position.isOnBackWithKickstand()) {
      mip.console.println(F(" Position: On Back With Kickstand"));
    }

    lastPosition = currentPosition;
  }
}
