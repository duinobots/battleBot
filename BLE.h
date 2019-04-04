#pragma once

class BLE
{
  public:
    BLE() {};
    virtual ~BLE() {};

    virtual bool init() = 0;
    virtual bool isConnected() = 0;
    virtual bool writeUART(char *msg) = 0;
    virtual bool available() = 0; /** must be run in main loop */
    virtual void reset() = 0;
    virtual uint8_t* getBuffer() = 0;
    virtual void clearBuffer() = 0;
    virtual void onConnect() = 0;
};
