/**
 * @file MPU_Console.h
 * @brief Auto-switching UART console for sending debug text to the PC.
 *
 * @details This header defines the MiP_Console class, which inherits from
 * Arduino's Stream class. It provides transparent redirection of print
 * statements to the PC Serial Monitor over a shared single hardware UART on the
 * Arduino Pro Mini.
 *
 * @author Adam Green (Original Author)
 * @author Samuel Trassare (Maintainer)
 * @copyright Copyright (C) 2018-2026 Samuel Trassare
 * (https://github.com/Tiogaplanet) Licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at
 * http://www.apache.org/licenses/LICENSE-2.0
 */
#ifndef MPU_CONSOLE_H
#define MPU_CONSOLE_H

#include <Arduino.h>
#include <Stream.h>

// Forward declaration of the main MiP class to avoid circular includes
class MiP;

/**
 * @brief Redirects Print/Stream writes to the PC Serial Monitor by temporarily
 *        switching the Pro Mini's UART multiplexer away from the MiP robot.
 */
class MiP_Console : public Stream {
 public:
  // --- Stream Interface Overrides ---
  virtual int available() override;
  virtual int read() override;
  virtual int peek() override;

  // --- Print Interface Overrides ---
  using Print::write;
  virtual size_t write(uint8_t byte) override;
  virtual size_t write(const uint8_t* pBuffer, size_t size) override;
  virtual int availableForWrite() override;
  virtual void flush() override;

  // --- HardwareSerial Compatibility Methods ---
  void begin(unsigned long baud = kDefaultBaudRate);
  void begin(unsigned long baud, uint16_t config);
  void end();

  /**
   * @brief Boolean evaluation operator for Arduino stream checking.
   */
  explicit operator bool() const {
    return true;
  }

 private:
  // Constants
  static constexpr unsigned long kDefaultBaudRate = 115200;
  static constexpr uint16_t kDefaultConfig = SERIAL_8N1;

  /**
   * @brief Constructs the Console manager.
   * @param mip Reference to the main MiP instance for multiplexer control.
   */
  explicit MiP_Console(MiP& mip);

  /**
   * @brief Lazy-initialization helper to ensure HardwareSerial is started.
   */
  void initIfNeeded();

  MiP& m_mip;     ///< Reference to the main orchestrator class
  bool m_isInit;  ///< Flag tracking HardwareSerial initialization status

  friend class MiP;
};

#endif  // MPU_CONSOLE_H
