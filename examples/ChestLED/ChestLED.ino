/**
 * @file ChestLED.ino
 * @brief Example sketch demonstrating MiP chest LED read/write operations.
 *
 * @details This sketch shows how to use the MiP library to set and read the
 * chest LED color and blink timing. It demonstrates the verified write APIs,
 * chestLED.read() and chestLED.write(), and the unverified write APIs,
 * chestLED.unverifiedWrite() which may not always be accepted by the robot.
 * This sketch exhaustively tests the MiP_ChestLED class.
 *
 * The example exercises these API calls:
 *   - chestLED.read()
 *   - chestLED.write()
 *   - chestLED.unverifiedWrite()
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
 * This object is used throughout the sketch to call MiP API functions such as
 * begin(), chestLED.read(), chestLED.write(), and chestLED.unverifiedWrite().
 */
MiP mip;

/**
 * @brief Arduino setup function.
 *
 * Initializes communication with MiP and demonstrates several
 * chest LED operations:
 *  - Set a static color (magenta) with no timing specified.
 *  - Set a blinking color with explicit on/off times.
 *  - Set a static color using a MiPChestLED struct.
 *  - Repeat the above sequence using the unverified write APIs.
 *
 * The function prints status messages to mip.console and uses
 * printCurrentChestLEDSetting() to display the current chest LED state after
 * each verified write.
 */
void setup() {
  bool connectResult = mip.begin();
  if (!connectResult) {
    mip.console.println(F("ChestLED.ino: Failed connecting to MiP!"));
    return;
  }

  mip.console.println(F("ChestLED.ino: Set Chest LED to different colors."));

  mip.console.println(F(" Set chest LED to magenta, no time specified."));
  uint8_t red = 0xff;
  uint8_t green = 0x01;
  uint8_t blue = 0xfe;
  mip.chestLED.write(red, green, blue);
  printCurrentChestLEDSetting();
  delay(1000);

  mip.console.println(
    F(" Set chest LED to blink red, on time: 990, off time: 989."));
  red = 0xff;
  green = 0x01;
  blue = 0x05;
  const uint16_t onTime = 990;
  const uint16_t offTime = 989;
  mip.chestLED.write(red, green, blue, onTime, offTime);
  printCurrentChestLEDSetting();
  delay(4000);

  mip.console.println(F(" Set chest LED back to green, no time specified."));
  MiPChestLED chestLED;
  chestLED.red = 0x00;
  chestLED.green = 0xff;
  chestLED.blue = 0x00;
  chestLED.onTime = 0;
  chestLED.offTime = 0;
  mip.chestLED.write(chestLED);
  printCurrentChestLEDSetting();
  delay(1000);

  // Attempt to run through the same sequence of chest LED changes using the
  // chestLED.unverifiedWrite() functions which don't always get accepted by
  // MiP.
  mip.console.println(F(" Trying to set chest LED to magenta, no time specified."));
  red = 0xff;
  green = 0x01;
  blue = 0xfe;
  mip.chestLED.unverifiedWrite(red, green, blue);
  delay(1000);

  mip.console.println(
    F(" Trying to set chest LED to blink red, no time specified."));
  red = 0xff;
  green = 0x01;
  blue = 0x05;
  mip.chestLED.unverifiedWrite(red, green, blue, onTime, offTime);
  delay(4000);

  mip.console.println(
    F(" Trying to set chest LED back to green with a flash time."));
  chestLED.red = 0x00;
  chestLED.green = 0xff;
  chestLED.blue = 0x00;
  chestLED.onTime = 200;
  chestLED.offTime = 200;
  mip.chestLED.unverifiedWrite(chestLED);
  delay(1000);

  mip.console.println(F("ChestLED.ino: Done."));
}

/**
 * @brief Arduino loop function.
 *
 * This example performs all actions in setup() and does not require repeated
 * work in loop(). The function is intentionally left empty.
 */
void loop() {}

/**
 * @brief Print the current chest LED setting to mip.console.
 *
 * Reads the current chest LED state from MiP using chestLED.read()
 * and prints the red, green, blue, onTime, and offTime values in a human
 * readable format.
 *
 * @note This function is declared static to limit its linkage to this file.
 */
static void printCurrentChestLEDSetting() {
  MiPChestLED chestLED;
  mip.chestLED.read(chestLED);

  mip.console.println(F(" Current Chest LED Setting"));
  mip.console.print(F("    red: "));
  mip.console.println(chestLED.red);
  mip.console.print(F("    green: "));
  mip.console.println(chestLED.green);
  mip.console.print(F("    blue: "));
  mip.console.println(chestLED.blue);
  mip.console.print(F("    on time: "));
  mip.console.print(chestLED.onTime);
  mip.console.println(F(" milliseconds"));
  mip.console.print(F("    off time: "));
  mip.console.print(chestLED.offTime);
  mip.console.println(F(" milliseconds"));
  mip.console.println();
}
