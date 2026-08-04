/**
 * @file Odometer.ino
 * @brief Example sketch demonstrating MiP odometer read and reset.
 *
 * @details This sketch shows how to use the MiP library to read the total
 * distance the robot has traveled since the last reset and how to reset that
 * odometer value. It prints the current distance in centimeters to Serial,
 * then calls odometer.reset() to clear the measurement.
 *
 * The example exercises these API calls:
 *   - odometer.read()
 *   - odometer.reset()
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
 *
 * @details Use this object to call MiP API functions such as begin(),
 * odometer.read(), and odometer.reset().
 */
MiP mip;

/**
 * @brief Arduino setup function.
 *
 * @details Called once after the board powers up or resets. This function:
 *   - Initializes communication with the MiP robot via mip.begin().
 *   - If the connection fails, prints an error to Serial and returns early.
 *   - Reads the current odometer value (in centimeters) using
 *     odometer.read() and prints it to Serial.
 *   - Resets the odometer using odometer.reset().
 *
 * The function prints progress and completion messages to Serial so the
 * user can observe the odometer reading and the reset action.
 */
void setup() {
  bool connectResult = mip.begin();
  if (!connectResult) {
    Serial.println(F("Odometer.ino: Failed connecting to MiP!"));
    return;
  }

  Serial.println(F("Odometer.ino: Read out current odometer reading and reset."));

  float cm = mip.odometer.read();
  Serial.print(F(" MiP has travelled "));
  Serial.print(cm);
  Serial.println(F(" cm since the last reset."));

  mip.odometer.reset();

  Serial.print(F(" MiP has travelled "));
  Serial.print(cm);
  Serial.println(F(" cm since the last reset."));

  Serial.println();
  Serial.println(F("Odometer.ino: Done."));
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

