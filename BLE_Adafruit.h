#pragma once

// #include "Arduino.h"
#include "BLE.h"
// #include "Adafruit_BLE.h"
// #include "Adafruit_BluefruitLE_SPI.h"

// #include "BluefruitConfig.h"

#include <bluefruit.h>
#include <Adafruit_LittleFS.h>
#include <InternalFileSystem.h>

#define FACTORYRESET_ENABLE 1
#define MINIMUM_FIRMWARE_VERSION "0.6.6"
#define MODE_LED_BEHAVIOUR "MODE"

#if SOFTWARE_SERIAL_AVAILABLE
#include <SoftwareSerial.h>
#endif

#define ADAFRUIT_BLE_DEBUG_MODE false

class BLE_Adafruit : public BLE
{
public:
  BLE_Adafruit(int8_t csPin, int8_t irqPin, int8_t rstPin);
  ~BLE_Adafruit();

  bool init() override;
  bool isConnected() override;
  bool writeUART(char *msg) override;
  bool available() override;
  void reset() override;
  uint8_t *getBuffer() override;
  void clearBuffer() override;
  void onConnect() override;

  static bool isConnected_;

private:
  // Adafruit_BluefruitLE_SPI ble_;
  BLEDfu* bledfu;   // OTA DFU service
  BLEDis* bledis;   // device information
  BLEUart* bleuart; // uart over ble
  BLEBas* blebas;   // battery
  uint8_t buffer_[BUFFER_LENGTH];

  static void connectCallback(uint16_t conn_handle);
  static void disconnectCallback(uint16_t conn_handle, uint8_t reason);

  void startAdv(void);

  /**
   * @brief write received BLE info to Serial Monitor, only runs if BLE_DEBUG is true
   */
  void writeBleDebug(uint8_t c);
};
