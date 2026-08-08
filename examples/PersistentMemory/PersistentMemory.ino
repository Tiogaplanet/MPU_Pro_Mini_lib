/**
* @file PersistentMemory.ino
* @brief Robust EEPROM read/write example with MiP chest-LED verification.
*
* @details
* Writes a password to EEPROM starting at address 0, reads it back, compares
* the contents, and indicates success or failure using MiP's chest LED:
*   - Violet when the read matches the written password.
*   - Red when the read does not match or an error occurs.
* After verification, the chest LED is restored to green.
*
* Demonstrates these APIs:
*   - mip.begin()
*   - mip.chestLED.write()
*
* This sketch primarily shows how MiP can be used as an output device, rather
* than strictly testing MiP or its API.
*
*
* @author Samuel Trassare (Original Author)
* @copyright Copyright (C) 2018-2026 Samuel Trassare
* (https://github.com/Tiogaplanet) Licensed under the Apache License,
* Version 2.0 (the "License"); you may not use this file except in compliance
* with the License. You may obtain a copy of the License at
* http://www.apache.org/licenses/LICENSE-2.0
*/
#include <Arduino.h>
#include <EEPROM.h>
#include <MiP_Power_Up_-_Pro_Mini.h>

/**
 * @brief Global MiP instance used to communicate with MiP.
 *
 * @details Use this object to call MiP API functions such as begin(), etc.
 */
MiP mip;

// Starting EEPROM address where password data will be stored
const int EEPROM_START_ADDR = 0;

/**
 * @brief Arduino setup function.
 *
 * @details
 * - Initializes mip.console for diagnostics.
 * - Attempts to initialize MiP's connection via mip.begin() and sets the
 *   global `connectResult` flag.
 * - Writes a password to /f.txt, reads it back, trims whitespace, compares
 *   to the original, and sets the chest LED to violet on match or red on
 * mismatch.
 * - Deletes the temporary file and restores the chest LED to green after a
 * delay.
 *
 * If MiP's connection fails, the function halts in a safe loop
 * after printing an error so loop() will not run and cause further errors.
 */
void setup() {
  // Initialize MiP's connection.
  bool connectResult = mip.begin();

  if (!connectResult) {
    Serial.println(F("PersistentMemory.ino: Failed connecting to MiP!"));
    return;
  }

  // Define test password as fixed-size char array to save RAM.
  const char writePassword[16] = "1234secret";
  char readPassword[16] = { 0 };

  mip.console.println(
    F("PersistentMemory.ino: Read and write internal EEPROM on Pro Mini."));
  mip.console.println(F(" Chest turns violet if read matches write, else red."));

  // Write password structure to EEPROM
  EEPROM.put(EEPROM_START_ADDR, writePassword);
  mip.console.println(F(" Wrote password to EEPROM address 0."));

  // Read password structure back from EEPROM
  EEPROM.get(EEPROM_START_ADDR, readPassword);

  mip.console.print(F(" Password is: "));
  mip.console.println(writePassword);
  mip.console.print(F(" EEPROM contained: "));
  mip.console.println(readPassword);

  // Compare read data against written data
  if (strcmp(writePassword, readPassword) == 0) {
    // Violet: R=0xB6, G=0x00, B=0xFF
    mip.chestLED.write(0xB6, 0x00, 0xFF);
    mip.console.println(
      F(" Read matches write. Chest set to violet."));
  } else {
    // Red: R=0xFF, G=0x00, B=0x00
    mip.chestLED.write(0xFF, 0x00, 0x00);
    mip.console.println(
      F(" Read does NOT match write. Chest set to red."));
  }

  // Allow observing chest LED, then restore to green
  delay(5000);
  mip.chestLED.write(0x00, 0xFF, 0x00);
  mip.console.println(F("PersistentMemory.ino: Done."));
}

/**
 * @brief Arduino loop function.
 *
 * @details
 * This example performs its demonstration in setup() and does not require
 * repeated work in loop().
 */
void loop() {}
