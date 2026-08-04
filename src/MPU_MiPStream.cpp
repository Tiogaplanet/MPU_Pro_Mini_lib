// This class can be used instead of Serial for sending text to the PC. It makes
// sure that the MiP ProMini Pack switches the UART signals away from the MiP
// and to the PC before actually performing the Serial write.
#include "MPU_MiPStream.h"

MiPStream::MiPStream() {
  m_isInit = false;
}

// Methods that must be implemented for Stream subclasses.
int MiPStream::available() {
  initIfNeeded();
  return Serial.available();
}

int MiPStream::read() {
  initIfNeeded();
  return Serial.read();
}

int MiPStream::peek() {
  initIfNeeded();
  return Serial.peek();
}

// Methods that must be implemented for Print subclasses.
size_t MiPStream::write(uint8_t byte) {
  initIfNeeded();
  bool needToRestore = MiP::isInstanceSerialGoingToMiP();
  MiP::switchInstanceSerialToPC();
  size_t result = Serial.write(byte);
  if (needToRestore) {
    MiP::switchInstanceSerialToMiP();
  }
  return result;
}

size_t MiPStream::write(const uint8_t* pBuffer, size_t size) {
  initIfNeeded();
  bool needToRestore = MiP::isInstanceSerialGoingToMiP();
  MiP::switchInstanceSerialToPC();
  size_t result = Serial.write(pBuffer, size);
  if (needToRestore) {
    MiP::switchInstanceSerialToMiP();
  }
  return result;
}

int MiPStream::availableForWrite() {
  initIfNeeded();
  return Serial.availableForWrite();
}

void MiPStream::flush() {
  initIfNeeded();
  return Serial.flush();
}

void MiPStream::begin(unsigned long baud, uint8_t mode) {
  // Silence compiler warnings about unused parameters.
  (void)baud;
  (void)mode;

  if (m_isInit) {
    // Ignore redundant begin() calls.
    return;
  }
  m_isInit = true;

  // Fix the baud rate / mode at 115200-8-N-1 since that is required by the MiP.
  Serial.begin(MIP_BAUD_RATE, SERIAL_8N1);
}

void MiPStream::end() {
  if (!m_isInit) {
    // Ignore end() if no begin() call has been made.
    return;
  }

  Serial.end();
  m_isInit = false;
}

void MiPStream::initIfNeeded() {
  // Make sure that Serial stream has been initialized by user or MiP.
  if (!m_isInit) {
    begin(MIP_BAUD_RATE);
  }
}

// Instantiate the single instance of this stream.
class MiPStream MiPStream;