/**
 * @file SoftwareHardwareVersion.ino
 * @brief Example sketch that reads MiP's software and hardware version
 * information. It also displays the MPU-Pro Mini library version.
 *
 * @details
 * This sketch demonstrates how to query a MiP for its software version and
 * hardware information using the MiP library. It:
 *   - Initializes communication with the MiP using mip.begin().
 *   - Reads the software version into a MiPSoftwareVersion struct via
 *     version.readSoftware() and prints a formatted date and unique version.
 *   - Reads hardware information into a MiPHardwareInfo struct via
 *     version.readHardware() and prints voice chip and hardware revision
 * details.
 *
 * The example exercises these API calls:
 *   - version.readSoftware()
 *   - version.readHardware()
 *
 * The output is printed to mip.console in a human-readable format so the user can
 * inspect the device's firmware date and build as well as hardware revision
 * information.
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
 * version.readSoftware(), and version.readHardware().
 */
MiP mip;

/**
 * @brief Arduino setup function.
 *
 * @details
 * - Attempts to initialize the MiP connection via mip.begin().
 * - If the connection fails, prints an error to Serial and returns early.
 * - On success, reads the software version into a MiPSoftwareVersion struct
 *   and prints a formatted version string (year-month-day.uniqueVersion).
 * - Reads hardware information into a MiPHardwareInfo struct and prints the
 *   voice chip and hardware revision values.
 *
 * The function prints progress and completion messages to mip.console so the
 * user can observe the retrieved version and hardware details.
 */
void setup() {
  bool connectResult = mip.begin();
  
  if (!connectResult) {
    Serial.println(
      F("SoftwareHardwareVersion.ino: Failed connecting to MiP!"));
    return;
  }

  mip.console.println(
    F("SoftwareHardwareVersion.ino: Use version.readSoftware() and "
      "version.readHardware() functions."));

  mip.console.print(F(" MiP Power Up - Pro Mini library version: "));
  mip.console.println(mip.version.readMPUString());

  /* Read and display software version information. */
  MiPSoftwareVersion softwareVersion;
  mip.version.readSoftware(softwareVersion);
  mip.console.print(F(" Software version: "));
  mip.console.print(softwareVersion.year);
  mip.console.print('-');
  mip.console.print(softwareVersion.month);
  mip.console.print('-');
  mip.console.print(softwareVersion.day);
  mip.console.print('.');
  mip.console.println(softwareVersion.uniqueVersion);

  /* Read and display hardware information. */
  MiPHardwareInfo hardwareInfo;
  mip.version.readHardware(hardwareInfo);
  mip.console.println(F(" Hardware info"));
  mip.console.print(F("  Voice chip version: "));
  mip.console.println(hardwareInfo.voiceChip);
  mip.console.print(F("  Hardware version: "));
  mip.console.println(hardwareInfo.hardware);

  mip.console.println(F("SoftwareHardwareVersion.ino: Done."));
}

/**
 * @brief Arduino loop function.
 *
 * @details This example performs its demonstration in setup() and does not
 * require repeated work in loop(). The function is intentionally left empty
 * so the sketch completes once during initialization.
 */
void loop() {}
