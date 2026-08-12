/**
 * @file MPU_Serial.cpp
 * @brief Implements serial transport for the MiP library.
 *
 * @details This source file implements low-level request, response, and event
 * processing for communicating with MiP.
 *
 * @author Adam Green (Original Author)
 * @author Samuel Trassare (Maintainer)
 * @copyright Copyright (C) 2018-2026 Samuel Trassare
 * (https://github.com/Tiogaplanet) Licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at
 * http://www.apache.org/licenses/LICENSE-2.0
 */
#include "MPU_Serial.h"
#include "MiP_Power_Up_-_Pro_Mini.h"

MiP_Serial::MiP_Serial(MiP& mip) : m_mip(mip) {
  clear();
}

// ---------------------------------------------------------------------------
// Public API
// ---------------------------------------------------------------------------

void MiP_Serial::rawSend(const uint8_t request[], size_t requestLength) {
  transportSendRequest(request, requestLength, MIP_EXPECT_NO_RESPONSE);
}

uint8_t MiP_Serial::rawReceive(const uint8_t request[],
                               size_t requestLength,
                               uint8_t responseBuffer[],
                               size_t responseBufferSize,
                               size_t& responseLength) {
  transportSendRequest(request, requestLength, MIP_EXPECT_RESPONSE);
  return transportGetResponse(
      responseBuffer, responseBufferSize, &responseLength);
}

bool MiP_Serial::processAllResponseData() {
  bool responseFound = false;
  uint8_t buffer[(MIP_RESPONSE_MAX_LEN - 1) * 2];
  size_t bytesToRead = 0;
  size_t bytesRead = 0;

  while (Serial.available() >= 2) {
    uint8_t highNibble = Serial.read();
    uint8_t lowNibble = Serial.read();
    uint8_t commandByte =
        (parseHexDigit(highNibble) << 4) | parseHexDigit(lowNibble);

    if (m_expectedResponseCommand != 0 &&
        commandByte == m_expectedResponseCommand) {
      m_responseBuffer[0] = commandByte;

      bytesToRead = m_expectedResponseSize - 1;
      bytesRead =
          Serial.readBytes(reinterpret_cast<char*>(buffer), bytesToRead * 2);

      if (bytesRead == bytesToRead * 2) {
        copyHexTextToBinary(&m_responseBuffer[1], buffer, bytesToRead);
        responseFound = true;
      } else {
        // Reset expected response tracking state upon incomplete read
        m_expectedResponseCommand = 0;
        m_expectedResponseSize = 0;
        m_responseBuffer[0] = 0;

        // Flush trailing partial nibbles from RX buffer to prevent byte misalignment
        discardUnexpectedSerialData();

        char buf[64];
        snprintf(buf,
                 sizeof(buf),
                 "MiP: Response too short: %u, expected %u\r\n",
                 static_cast<unsigned>(bytesRead),
                 static_cast<unsigned>(bytesToRead * 2));
        MIP_DEBUG_ERROR_PRINT(buf);
        break;
      }
    } else {
      processOobResponseData(commandByte);
    }
  }
  return responseFound;
}

// ---------------------------------------------------------------------------
// Protected / Private Helpers
// ---------------------------------------------------------------------------

void MiP_Serial::clear() {
  // Allow the first request to be sent immediately.
  m_lastRequestTime = millis() - MIP_REQUEST_DELAY;
  m_expectedResponseSize = 0;
  m_expectedResponseCommand = 0;
  memset(m_responseBuffer, 0, sizeof(m_responseBuffer));
}

uint8_t MiP_Serial::transportGetResponse(uint8_t* pResponseBuffer,
                                         size_t responseBufferSize,
                                         size_t* pResponseLength) {
  MIP_ASSERT(m_mip.isInitialized());
  MIP_ASSERT(responseBufferSize <= MIP_RESPONSE_MAX_LEN);
  MIP_ASSERT(m_expectedResponseCommand != 0);

  m_expectedResponseSize = static_cast<uint8_t>(responseBufferSize);

  uint32_t startTime = millis();
  bool responseFound = false;

  do {
    responseFound = processAllResponseData();
  } while (!responseFound && (millis() - startTime) < MIP_RESPONSE_TIMEOUT);

  if (!responseFound) {
    MIP_DEBUG_WARN_PRINTLN(F("MiP: Response timeout"));
    return MiP::MIP_ERROR_TIMEOUT;
  }

  // Copy the collected response to the caller and reset state.
  memcpy(pResponseBuffer, m_responseBuffer, m_expectedResponseSize);
  *pResponseLength = m_expectedResponseSize;
  m_expectedResponseCommand = 0;
  m_expectedResponseSize = 0;
  m_responseBuffer[0] = 0;

  return MiP::MIP_ERROR_NONE;
}

void MiP_Serial::transportSendRequest(const uint8_t* pRequest,
                                      size_t requestLength,
                                      bool expectResponse) {
  MIP_ASSERT(m_mip.isInitialized());

  // Honor the minimum inter-request delay.
  while (millis() - m_lastRequestTime < MIP_REQUEST_DELAY) {
    delay(1);
  }

  if (expectResponse) {
    m_expectedResponseCommand = pRequest[0];
  } else {
    m_expectedResponseCommand = 0;
  }

  m_expectedResponseSize = 0;
  m_responseBuffer[0] = 0;

  // MiP UART protocol = raw binary
  while (requestLength-- > 0) {
    Serial.write(*pRequest++);
  }

  m_lastRequestTime = millis();
}

void MiP_Serial::processOobResponseData(uint8_t commandByte) {
  size_t length = 0;
  size_t bytesRead;

  // Determine payload length from the command byte.
  switch (commandByte) {
    case MiP_Radar::MIP_CMD_GET_RADAR_RESPONSE:
    case MiP_Gesture::MIP_CMD_GET_GESTURE_RESPONSE:
    case MiP_Clap::MIP_CMD_CLAP_RESPONSE:
    case MiP_Weight::MIP_CMD_GET_WEIGHT:
    case MiP_Infrared::MIP_CMD_GET_DETECTED_MIP:
      length = 1;
      break;

    case MiP_Shake::MIP_CMD_SHAKE_RESPONSE:
      length = 0;
      break;

    case MiP::MIP_CMD_GET_STATUS:
      length = 2;
      break;

    case MiP_Infrared::MIP_CMD_RECEIVE_IR_DONGLE_CODE:
      // Variable-length message – length is the next byte.
      if (!readIrLength(length)) {
        return;
      }
      break;

    default: {
      uint8_t discarded = discardUnexpectedSerialData();
      char buf[64];
      snprintf(buf,
               sizeof(buf),
               "MiP: Bad OOB command byte: 0x%02x (discarded %d bytes)\r\n",
               commandByte,
               discarded);
      MIP_DEBUG_ERROR_PRINT(buf);
    }
      return;
  }

  // Read the remaining payload (in hex-ASCII pairs).
  uint8_t buffer[4 * 2];  // Max payload for IR dongle code is 4 bytes
  bytesRead = Serial.readBytes(reinterpret_cast<char*>(buffer), length * 2);
  if (bytesRead != length * 2) {
    char buf[64];
    snprintf(buf,
             sizeof(buf),
             "MiP: OOB too short: %u, %u\r\n",
             static_cast<unsigned>(bytesRead),
             static_cast<unsigned>(length * 2));
    MIP_DEBUG_ERROR_PRINT(buf);
    return;
  }

  // Convert to binary and hand off to main class for dispatching.
  uint8_t response[MIP_RESPONSE_MAX_LEN];
  response[0] = commandByte;
  copyHexTextToBinary(&response[1], buffer, length);

  m_mip.dispatchEvent(commandByte, response, length + 1);
}

bool MiP_Serial::readIrLength(size_t& length) {
  uint8_t nibbles[2];
  if (Serial.readBytes(reinterpret_cast<char*>(nibbles), sizeof(nibbles)) != sizeof(nibbles)) {
    MIP_DEBUG_ERROR_PRINTLN(F("MiP: Missing IR code length"));
    return false;
  }

  length = (parseHexDigit(nibbles[0]) << 4) | parseHexDigit(nibbles[1]);

  if (length < 2 || length > 4) {
    uint8_t discarded = discardUnexpectedSerialData();
    char buf[48];
    snprintf(buf,
             sizeof(buf),
             "MiP: Bad IR code length: 0x%02x (discarded %d bytes)\r\n",
             static_cast<unsigned>(length),
             discarded);
    MIP_DEBUG_ERROR_PRINT(buf);
    return false;
  }
  return true;
}

uint8_t MiP_Serial::discardUnexpectedSerialData() {
  uint8_t discarded = 0;

  // Clear incoming hardware UART RX buffer.
  while (Serial.available() > 0) {
    discarded++;
    Serial.read();
    delayMicroseconds(100);
  }
  return discarded;
}

void MiP_Serial::copyHexTextToBinary(uint8_t* pDest,
                                     const uint8_t* pSrc,
                                     size_t length) {
  while (length-- > 0) {
    *pDest++ = (parseHexDigit(pSrc[0]) << 4) | parseHexDigit(pSrc[1]);
    pSrc += 2;
  }
}

constexpr uint8_t MiP_Serial::parseHexDigit(uint8_t digit) {
  return (digit >= '0' && digit <= '9') ? static_cast<uint8_t>(digit - '0') :
         (digit >= 'a' && digit <= 'f') ? static_cast<uint8_t>(digit - 'a' + 10) :
         (digit >= 'A' && digit <= 'F') ? static_cast<uint8_t>(digit - 'A' + 10) : 0;
}
