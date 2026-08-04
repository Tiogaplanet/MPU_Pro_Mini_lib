// This class can be used instead of Serial for sending text to the PC. It makes sure that the MiP ProMini Pack switches
// the UART signals away from the MiP and to the PC before actually performing the Serial write.
class MiPStream : public Stream
{
public:
    MiPStream();

    // Methods that must be implemented for Stream subclasses.
    virtual int available();
    virtual int read();
    virtual int peek();

    // Methods that must be implemented for Print subclasses.
    virtual size_t write(uint8_t);
    virtual size_t write(const uint8_t *buffer, size_t size);
    virtual int    availableForWrite();
    virtual void   flush();

    // Additional methods defined by HardwareSerial that user might call.
    void begin(unsigned long baud) { begin(baud, SERIAL_8N1); }
    void begin(unsigned long, uint8_t);
    void end();
    inline size_t write(unsigned long n) { return write((uint8_t)n); }
    inline size_t write(long n) { return write((uint8_t)n); }
    inline size_t write(unsigned int n) { return write((uint8_t)n); }
    inline size_t write(int n) { return write((uint8_t)n); }
    operator bool() { return true; }

protected:
    void initIfNeeded();

    bool m_isInit;
} extern MiPStream;

// This macro will force user code which does something like: Serial.println("Interesting text")
// to be redirected to MiPStream which will make sure that it actually gets sent to the PC and not the MiP.
#define Serial MiPStream
