#pragma once

#include "Arduino.h"
#include "BLE.h"
#include <SPI.h>
#include<TinyCircuits_BLE.h>
//Debug output adds extra flash and memory requirements!
#ifndef BLE_DEBUG
#define BLE_DEBUG true
#endif

class BLE_TinyCircuits : public BLE
{
  public:
    BLE_TinyCircuits();
    ~BLE_TinyCircuits();

    bool init();
    bool isConnected();
    bool writeUART(char *msg);
    bool available();
    void reset();
    uint8_t* getBuffer();
    void clearBuffer();
    void onConnect();

  private:
    TinyCircuits_BLE ble_;
};
