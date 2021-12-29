#include "Arduino.h"
#include "BLE_Adafruit.h"
#include "BluefruitConfig.h"

BLE_Adafruit::BLE_Adafruit(int8_t csPin, int8_t irqPin, int8_t rstPin)
    : ble_(csPin, irqPin, rstPin)
{ }

/**
 * Initializes BlueFruit module. returns true if init was successful,
 * returns false if not
 */
bool BLE_Adafruit::init()
{
  /* Initialise the module */
  Serial.print("Initialising the Bluefruit LE module: ");

  if (!ble_.begin(VERBOSE_MODE))
  {
    Serial.println("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?");
    return false;
  }
  Serial.println("OK!");

  reset();

  /* Disable command echo from Bluefruit */
  ble_.echo(false);
  ble_.verbose(BLE_DEBUG); // debug info is a little annoying after this point!

  return true;
}

bool BLE_Adafruit::isConnected()
{
  return ble_.isConnected();
}

bool BLE_Adafruit::writeUART(char *msg)
{
  return ble_.writeBLEUart(msg);
}

bool BLE_Adafruit::available()
{
  return (bool)ble_.available();
}

void BLE_Adafruit::reset()
{
  if (FACTORYRESET_ENABLE)
  {
    /* Perform a factory reset to make sure everything is in a known state */
    Serial.println(F("Performing a factory reset: "));
    if (!ble_.factoryReset())
    {
      Serial.println(F("Couldn't factory reset"));
      return;
    }
  }
  return;
}

uint8_t* BLE_Adafruit::getBuffer()
{
  uint8_t i = 0;

  while (ble_.available())
  {
    uint8_t c = ble_.read();

    buffer_[i] = c;
    i++;

    if (i >= 20)
    {
      break;
    }

    if (BLE_DEBUG)
    {
      writeBleDebug(c);
    }

  }

#if BLE_DEBUG
  Serial.print("BLE_Adafruit::getBuffer(), buffer_ -> ");
  Serial.println((char*)buffer_);
  Serial.print("sizeof(buffer_) -> ");
  Serial.println(sizeof(buffer_));
#endif

  return buffer_;
}

void BLE_Adafruit::clearBuffer()
{
  for (uint8_t i = 0; i < BUFFER_LENGTH; i++)
  {
    buffer_[i] = 0;
  }
}

void BLE_Adafruit::onConnect()
{
  ble_.setMode(BLUEFRUIT_MODE_DATA);
  return;
}

/*
 * Write received BLE info to Serial Monitor
 */
void BLE_Adafruit::writeBleDebug(uint8_t c)
{
  Serial.print((char)c);
  // Hex output too, helps w/debugging!
  Serial.print(" [0x");
  if (c <= 0xF)
    Serial.print(F("0"));
  Serial.print(c, HEX);
  Serial.print("] ");
  Serial.println();
};
