#pragma once

#include "BLE.h"
#include "bluefruit_common.h"
#include <bluefruit.h>

#include "BLECharacteristic.h"
#include "BLEService.h"

#ifndef BUFFER_LENGTH
#define BUFFER_LENGTH 20
#endif

#define ADAFRUIT_BLE_DEBUG_MODE 1

class BLE_Adafruit : public BLE, BLEService
{
  public:
    BLE_Adafruit();
    ~BLE_Adafruit();

    bool init();
    bool isConnected();
    bool writeUART(const char *msg);
    bool available();
    void reset();
    uint8_t* getBuffer();
    void clearBuffer();
    void onConnect();

  private:
    BLEDis bledis_;
    BLEUart bleuart_;
    BLECharacteristic bleWriteChar1_;
    BLECharacteristic bleWriteChar2_;
    BLECharacteristic bleWriteChar3_;
    BLECharacteristic bleNotifyChar_;
    bool isConnected_;
    uint8_t buffer_[BUFFER_LENGTH];

    void writeBleDebug(uint8_t c);
    void startAdv(void);
};
