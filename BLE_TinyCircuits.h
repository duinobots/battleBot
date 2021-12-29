#pragma once

#include "Arduino.h"
#include "BLE.h"
#include <SPI.h>
#include<TinyCircuits_BLE.h>

class BLE_TinyCircuits : public BLE
{
public:
  BLE_TinyCircuits();
  ~BLE_TinyCircuits() = default;

  bool init() override;
  bool isConnected() override;
  bool writeUART(char *msg) override;
  bool available() override;
  void reset() override;
  uint8_t* getBuffer() override;
  void clearBuffer() override;
  void onConnect() override;

private:
  TinyCircuits_BLE ble_;
};
