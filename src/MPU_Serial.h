/**
 * @file MPU_Serial.h
 * @brief Defines the public interface for serial transport in the MiP library.
 *
 * @details This header declares the low-level serial API used for communicating with MiP.
 *
 * @author Adam Green (Original Author)
 * @author Samuel Trassare (Maintainer)
 * @copyright Copyright (C) 2018-2026 Samuel Trassare
 * (https://github.com/Tiogaplanet) Licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at
 * http://www.apache.org/licenses/LICENSE-2.0
 */
#ifndef MPU_SERIAL_H
#define MPU_SERIAL_H

#include <Arduino.h>

// Forward-declare the main MiP class to avoid circular include dependencies.
class MiP;

/**
 * @brief Manages low-level UART transport and OOB event demuxing for MiP.
 *
 * @details Handles raw request/response communication over hardware serial,
 *          decodes hex-ASCII response strings from MiP, and routes out-of-band events.
 */
class MiP_Serial {
 public:
  /** @brief Minimum delay in milliseconds required between consecutive UART requests sent to MiP. */
  static constexpr uint8_t MIP_REQUEST_DELAY = 8;

  /** @brief Time in milliseconds to wait for a response from MiP before timing out. */
  static constexpr uint8_t MIP_RESPONSE_TIMEOUT = 100;

  /** @brief Flag for transportSendRequest() indicating no response is expected from MiP. */
  static constexpr bool MIP_EXPECT_NO_RESPONSE = false;

  /** @brief Flag for transportSendRequest() indicating a response is expected from MiP. */
  static constexpr bool MIP_EXPECT_RESPONSE = true;

  /** @brief Maximum buffer length for outgoing request commands sent to MiP. */
  static constexpr size_t MIP_REQUEST_MAX_LEN = 17 + 1;  // Longest request is MIP_CMD_PLAY_SOUND.

  /** @brief Maximum buffer length for incoming response buffers received from MiP. */
  static constexpr size_t MIP_RESPONSE_MAX_LEN = 5 + 1;   // Longest response is MIP_CMD_REQUEST_CHEST_LED.

  /** @brief Maximum number of retry attempts for verified command operations. */
  static constexpr uint8_t MIP_MAX_RETRIES = 2;

  /** @brief Delay in milliseconds to pause between command retries. */
  static constexpr uint16_t MIP_RETRY_WAIT = 50;

  /**
   * @brief Constructs the serial transport manager instance.
   *
   * @param mip Reference to the main MiP instance for system access and event dispatching.
   */
  explicit MiP_Serial(MiP& mip);

  /**
   * @brief Sends a raw command to MiP without waiting for a response (fire-and-forget).
   *
   * @details Used for asynchronous commands or internally by higher-level verified methods.
   *
   * @param request       Buffer containing the raw byte array command to send to MiP.
   * @param requestLength Number of bytes in the request buffer.
   */
  void rawSend(const uint8_t request[], size_t requestLength);

  /**
   * @brief Sends a raw command to MiP and blocks until the expected response is received or times out.
   *
   * @param request            Buffer containing the raw byte command.
   * @param requestLength       Number of bytes in the request buffer.
   * @param responseBuffer     Buffer where received response data from MiP will be written.
   * @param responseBufferSize Maximum capacity of the response buffer.
   * @param responseLength     Reference receiving the actual number of response bytes written.
   * @return uint8_t Status code (returns MIP_ERROR_NONE on success, or an error code on timeout/failure).
   */
  uint8_t rawReceive(const uint8_t request[],
                     size_t requestLength,
                     uint8_t responseBuffer[],
                     size_t responseBufferSize,
                     size_t& responseLength);

  /**
   * @brief Processes all incoming UART data from MiP in the serial receive buffer.
   *
   * @details Parses incoming hex-ASCII character pairs into binary payload bytes, matching expected
   *          command responses or routing out-of-band events (e.g., radar, clap, gesture) to MiP.
   *
   * @return true if an expected command response was successfully found and processed.
   * @return false if no expected command response was encountered.
   */
  bool processAllResponseData();

 protected:
  /**
   * @brief Resets transport state tracking variables to default settings.
   */
  void clear();

 private:
  /**
   * @brief Flushes unexpected or corrupt data from the UART RX buffer.
   * @return uint8_t Number of bytes discarded.
   */
  uint8_t discardUnexpectedSerialData();

  /**
   * @brief Parses and dispatches out-of-band events received from MiP.
   * @param commandByte First byte identifying the event type.
   */
  void processOobResponseData(uint8_t commandByte);

  /**
   * @brief Waits for and reads a response frame matching the expected command from MiP.
   * @param pResponseBuffer    Target memory location for response data.
   * @param responseBufferSize Size of destination buffer.
   * @param pResponseLength   Pointer where total byte count read will be stored.
   * @return uint8_t Result status code.
   */
  uint8_t transportGetResponse(uint8_t* pResponseBuffer,
                               size_t responseBufferSize,
                               size_t* pResponseLength);

  /**
   * @brief Sends a request packet to MiP over hardware serial, enforcing delay timing.
   * @param pRequest       Pointer to command bytes.
   * @param requestLength Length of command buffer.
   * @param expectResponse Whether a response is expected back from MiP.
   */
  void transportSendRequest(const uint8_t* pRequest,
                            size_t requestLength,
                            bool expectResponse);

  /**
   * @brief Converts hex-ASCII formatted text into binary representation.
   * @param pDest Destination binary byte array.
   * @param pSrc  Source hex-ASCII character array.
   * @param length Number of binary bytes to produce.
   */
  static void copyHexTextToBinary(uint8_t* pDest, const uint8_t* pSrc, size_t length);

  /**
   * @brief Converts a single hex character to its 4-bit numerical value.
   * @param digit ASCII character representing a hex digit.
   * @return uint8_t Binary value (0 - 15).
   */
  static constexpr uint8_t parseHexDigit(uint8_t digit);

  /**
   * @brief Helper to parse variable-length IR dongle message lengths from MiP.
   * @param length Output parameter receiving the parsed length.
   * @return true if length was valid and successfully read.
   */
  bool readIrLength(size_t& length);

  MiP& m_mip;                             ///< Reference to the main MiP orchestrator object
  uint32_t m_lastRequestTime;             ///< Timestamp of last request sent to MiP
  uint8_t m_expectedResponseSize;         ///< Size in bytes of expected response from MiP
  uint8_t m_expectedResponseCommand;      ///< Command byte of expected response
  uint8_t m_responseBuffer[MIP_RESPONSE_MAX_LEN];  ///< Internal buffer for response construction

  friend class MiP;
};

#endif  // MPU_SERIAL_H
