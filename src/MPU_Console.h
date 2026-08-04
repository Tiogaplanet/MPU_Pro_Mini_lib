/**
 * @file MPU_Console.h
 * @brief Auto-switching UART console for sending debug text to the PC.
 *
 * This header defines the MiP_Console class, which inherits from Arduino's
 * Stream class. It provides transparent redirection of print statements to
 * the PC Serial Monitor over a shared single hardware UART on the Arduino Pro
 * Mini.
 *
 * @author Adam Green, Samuel Trassare
 * @date 2026-08-04
 * @version 2.0.0
 * @copyright Apache License, Version 2.0
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
  /**
   * @brief Constructs the Console manager.
   * @param mip Reference to the main MiP instance for multiplexer control.
   */
  MiP_Console(MiP& mip);

  // --- Stream Interface Overrides ---
  virtual int available() override;
  virtual int read() override;
  virtual int peek() override;

  // --- Print Interface Overrides ---
  virtual size_t write(uint8_t byte) override;
  virtual size_t write(const uint8_t* pBuffer, size_t size) override;
  virtual int availableForWrite() override;
  virtual void flush() override;

  // --- HardwareSerial Compatibility Methods ---
  void begin(unsigned long baud = 115200);
  void begin(unsigned long baud, uint8_t mode);
  void end();

  // --- Inline Disambiguation Overloads for Arduino Print API ---
  inline size_t write(unsigned long n) {
    return write((uint8_t)n);
  }
  inline size_t write(long n) {
    return write((uint8_t)n);
  }
  inline size_t write(unsigned int n) {
    return write((uint8_t)n);
  }
  inline size_t write(int n) {
    return write((uint8_t)n);
  }

  /**
   * @brief Boolean evaluation operator for Arduino stream checking.
   */
  operator bool() {
    return true;
  }

 private:
  /**
   * @brief Lazy-initialization helper to ensure HardwareSerial is started.
   */
  void initIfNeeded();

  MiP& m_mip;     ///< Reference to the main orchestrator class
  bool m_isInit;  ///< Flag tracking HardwareSerial initialization status
};

#endif  // MPU_CONSOLE_H
