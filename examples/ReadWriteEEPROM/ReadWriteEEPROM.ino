/**
 * @file ReadWriteEEPROM.ino
 * @brief Example sketch demonstrating reading from and writing to MiP's EEPROM.
 *
 * @details
 * This sketch demonstrates how to store and retrieve a single byte of user
 * data in MiP's EEPROM using the eeprom.write() and eeprom.read() APIs.
 * It writes a test value (secretPassword) to an EEPROM offset, then reads it
 * back and prints the original, scrambled, and recovered values to mip.console.
 *
 * The example shows how EEPROM data can be preserved across power cycles. To
 * verify persistence, power-cycle MiP (or comment out the write call and reload
 * the sketch) and observe the recovered value.
 *
 * The example exercises these API calls:
 *   - eeprom.read()
 *   - eeprom.write()
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
 * eeprom.write(), and eeprom.read().
 */
MiP mip;

/**
 * @brief EEPROM address offset to use for storing user data.
 *
 * @details Valid offsets are typically in the range 0x00..0x0F.
 * Change this value to store multiple independent bytes.
 */
const uint8_t eepromAddressOffset = 0x00;

/**
 * @brief Example byte to write into EEPROM.
 *
 * @details Change this value to experiment with different stored bytes.
 */
uint8_t secretPassword = 0x0D;

/**
 * @brief Variable used to hold a recovered EEPROM value for display.
 */
uint8_t recoveredPassword;

/**
 * @brief Arduino setup function.
 *
 * @details
 * - Initializes communication with MiP via mip.begin().
 * - If the connection fails, prints an error to Serial and returns early.
 * - Prints the original secretPassword, writes it to EEPROM using
 *   eeprom.write(eepromAddressOffset, secretPassword), then scrambles the
 *   in-memory secretPassword and reads the stored value back using
 *   eeprom.read(eepromAddressOffset). The recovered value is printed to
 *   mip.console so the user can verify the write/read operation.
 *
 * Note: To verify persistence across power cycles, comment out the call to
 * eeprom.write(), reflash or power-cycle the MiP, and observe that the
 * previously written value is still returned by eeprom.read().
 */
void setup() {
  bool connectResult = mip.begin();

  if (!connectResult) {
    Serial.println(F("ReadWriteEEPROM.ino: Failed connecting to MiP!"));
    return;
  }

  mip.console.println(
    F("ReadWriteEEPROM.ino: Writes data to EEPROM and reads it back."));

  mip.console.print(F(" Original password: "));
  // Fix use of mip.console.printf later.  mip.console.printf("0x%02X\n\r", secretPassword);

  /* Write the secret password to MiP's user EEPROM at the configured
   * offset. Comment out this line to test persistence across power cycles.
   */
  mip.eeprom.write(eepromAddressOffset, secretPassword);

  /* "Scramble" the in-memory password to demonstrate that the recovered value
   * comes from EEPROM rather than the local variable.
   */
  secretPassword = 0xFF;
  mip.console.print(F(" Scrambled password: "));
  // Fix use of mip.console.printf later.  mip.console.printf("0x%02X\n\r", secretPassword);

  /* Read the stored value back from EEPROM and print it. */
  recoveredPassword = mip.eeprom.read(eepromAddressOffset);
  mip.console.print(F(" Recovered password: "));
  // Fix use of mip.console.printf later.  mip.console.printf("0x%02X\n\r", recoveredPassword);

  mip.console.println(F("ReadWriteEEPROM.ino: Done."));
}

/**
 * @brief Arduino loop function.
 *
 * @details This example performs its demonstration in setup() and does not
 * require repeated work in loop(). The loop is intentionally left empty so
 * the sketch completes once and remains idle.
 */
void loop() {}

