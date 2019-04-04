#include "Arduino.h"
#include "BLE.h"
#include "BLE_TinyCircuits.h"
//#include <TinyCircuits_BLE.h>

BLE_TinyCircuits::BLE_TinyCircuits()
  : ble_()
{ }

BLE_TinyCircuits::~BLE_TinyCircuits()
{
//  delete &ble_;
}

bool BLE_TinyCircuits::init()
{
  Serial.println("entering BLE_TinyCircuits::init()");
  bool success = (bool)ble_.init();
  return success;
}

bool BLE_TinyCircuits::isConnected()
{
  return ble_.isConnected();
}

bool BLE_TinyCircuits::writeUART(char *msg)
{
  return ble_.writeUART(msg);
}

bool BLE_TinyCircuits::available()
{
  return ble_.available();
}

void BLE_TinyCircuits::reset()
{
  return ble_.reset();
}

uint8_t* BLE_TinyCircuits::getBuffer()
{
  return ble_.getBuffer();
}

void BLE_TinyCircuits::clearBuffer()
{
  // todo
}

void BLE_TinyCircuits::onConnect()
{
  return;
};
