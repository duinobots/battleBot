#include "Arduino.h"
#include "BLE_Adafruit.h"
#include "BluefruitConfig.h"

BLE_Adafruit::BLE_Adafruit()
  : isConnected_(false)
{ }

BLE_Adafruit::~BLE_Adafruit()
{
//  delete &ble_;
}

void connectCb(uint16_t conn_handle)
{
  BLEConnection* conn = Bluefruit.Connection(conn_handle);
  Serial.println("Connected");

  // request PHY changed to 2MB
  Serial.println("Request to change PHY");
  conn->requestPHY();

  // request to update data length
  Serial.println("Request to change Data Length");
  conn->requestDataLengthUpdate();
    
  // request mtu exchange
  Serial.println("Request to change MTU");
  conn->requestMtuExchange(247);

  // request connection interval of 7.5 ms
  //conn->requestConnectionParameter(6); // in unit of 1.25

  // delay a bit for all the request to complete
  delay(1000);
}

void disconnectCb(uint16_t conn_handle, uint8_t reason)
{
  (void) conn_handle;
  (void) reason;

  Serial.println();
  Serial.print("Disconnected, reason = 0x"); Serial.println(reason, HEX);
}

/**
 * Initializes BlueFruit module. returns true if init was successful,
 * returns false if not
 */
bool BLE_Adafruit::init()
{
  /* Initialise the module */
  Serial.print("Initialising the Bluefruit LE module: ");

  Bluefruit.configPrphBandwidth(BANDWIDTH_MAX);
  Bluefruit.begin();
  Bluefruit.setTxPower(4);    // Check bluefruit.h for supported values
  Bluefruit.Periph.setConnInterval(6, 12); // 7.5 - 15 ms
  Bluefruit.Periph.setConnectCallback(connectCb);
  Bluefruit.Periph.setDisconnectCallback(disconnectCb);

  // Configure and Start Device Information Service
  bledis_.setManufacturer("Adafruit Industries");
  bledis_.setModel("Bluefruit Feather 52");
  bledis_.begin();

  bleuart_.begin();

  // reset();
  startAdv();

  return true;
}

bool BLE_Adafruit::isConnected()
{
  return Bluefruit.connected();
}

bool BLE_Adafruit::writeUART(const char *msg)
{
  const uint16_t data_mtu = Bluefruit.Connection(0)->getMtu() - 3;
  return bleuart_.write(msg, data_mtu);
}

bool BLE_Adafruit::available()
{
  return (bool)bleuart_.available();
}

void BLE_Adafruit::reset()
{
  return;
}

uint8_t* BLE_Adafruit::getBuffer()
{
  uint8_t i = 0;

  while (bleuart_.available())
  {
    // uint8_t c = ble_.read();
    uint8_t ch;
    ch = (uint8_t) bleuart_.read();
    Serial.write(ch);

    buffer_[i] = ch;
    i++;

    if (i >= 20)
    {
      break;
    }

    if (ADAFRUIT_BLE_DEBUG_MODE)
    {
      writeBleDebug(ch);
    }

  }
  Serial.print("BLE_Adafruit::getBuffer(), buffer_ -> ");
  Serial.println((char*)buffer_);
  Serial.print("sizeof(buffer_) -> ");
  Serial.println(sizeof(buffer_));
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
  // ble_.setMode(BLUEFRUIT_MODE_DATA);
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
}

void BLE_Adafruit::startAdv(void)
{
  // Advertising packet
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();

  // Include BLE HID service
  Bluefruit.Advertising.addService(bleuart_);

  // There is enough room for the dev name in the advertising packet
  Bluefruit.Advertising.addName();

  /* Start Advertising
   * - Enable auto advertising if disconnected
   * - Interval:  fast mode = 20 ms, slow mode = 152.5 ms
   * - Timeout for fast mode is 30 seconds
   * - Start(timeout) with timeout = 0 will advertise forever (until connected)
   *
   * For recommended advertising interval
   * https://developer.apple.com/library/content/qa/qa1931/_index.html
   */
  Bluefruit.Advertising.restartOnDisconnect(true);
  Bluefruit.Advertising.setInterval(32, 244);    // in unit of 0.625 ms
  Bluefruit.Advertising.setFastTimeout(30);      // number of seconds in fast mode
  Bluefruit.Advertising.start(0);                // 0 = Don't stop advertising after n seconds
}

;
