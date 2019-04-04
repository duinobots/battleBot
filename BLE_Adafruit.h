#pragma once

#include "Arduino.h"
#include "BLE.h"
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"

#include "BluefruitConfig.h"

#define FACTORYRESET_ENABLE 1
#define MINIMUM_FIRMWARE_VERSION "0.6.6"
#define MODE_LED_BEHAVIOUR "MODE"

#if SOFTWARE_SERIAL_AVAILABLE
#include <SoftwareSerial.h>
#endif

#define BUFFER_LENGTH 20

#define ADAFRUIT_BLE_DEBUG_MODE true

class BLE_Adafruit : public BLE
{
  public:
    BLE_Adafruit(int8_t csPin, int8_t irqPin, int8_t rstPin);
    ~BLE_Adafruit();

    bool init();
    bool isConnected();
    bool writeUART(char *msg);
    bool available();
    void reset();
    uint8_t* getBuffer();
    void clearBuffer();
    void onConnect();

  private:
    Adafruit_BluefruitLE_SPI ble_;
    uint8_t buffer_[BUFFER_LENGTH];

    void writeBleDebug(uint8_t c);
};
