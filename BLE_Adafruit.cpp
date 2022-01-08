#include "Arduino.h"
#include "BLE_Adafruit.h"
#include "BLE.h"
// #include "BluefruitConfig.h"

bool BLE_Adafruit::isConnected_ = false;

BLE_Adafruit::BLE_Adafruit(int8_t csPin, int8_t irqPin, int8_t rstPin)
//     : isConnected_(false)
{
}

BLE_Adafruit::~BLE_Adafruit()
{
  delete bledfu, bledis, bleuart, blebas;
}

/**
 * Initializes BlueFruit module. returns true if init was successful,
 * returns false if not
 */
bool BLE_Adafruit::init()
{
  /* Initialise the module */
  Serial.print("Initialising the Bluefruit LE module: ");

  Serial.println("Bluefruit52 BLEUART Example");
  Serial.println("---------------------------\n");

  bledfu = new BLEDfu();
  bledis = new BLEDis();
  bleuart = new BLEUart();
  blebas = new BLEBas();

  // Setup the BLE LED to be enabled on CONNECT
  // Note: This is actually the default behavior, but provided
  // here in case you want to control this LED manually via PIN 19
  Bluefruit.autoConnLed(true);

  // Config the peripheral connection with maximum bandwidth
  // more SRAM required by SoftDevice
  // Note: All config***() function must be called before begin()
  Bluefruit.configPrphBandwidth(BANDWIDTH_MAX);

  Bluefruit.begin();
  Bluefruit.setTxPower(4); // Check bluefruit.h for supported values
  //Bluefruit.setName(getMcuUniqueID()); // useful testing with multiple central connections
  Bluefruit.Periph.setConnectCallback(connectCallback);
  Bluefruit.Periph.setDisconnectCallback(disconnectCallback);

  // To be consistent OTA DFU should be added first if it exists
  bledfu->begin();

  // Configure and Start Device Information Service
  bledis->setManufacturer("Adafruit Industries");
  bledis->setModel("Bluefruit Feather52");
  bledis->begin();

  // Configure and Start BLE Uart Service
  bleuart->begin();

  // Start BLE Battery Service
  blebas->begin();
  blebas->write(100);

  // Set up and start advertising
  startAdv();

  Serial.println("Please use Adafruit's Bluefruit LE app to connect in UART mode");
  Serial.println("Once connected, enter character(s) that you wish to send");

  return true;
}

// callback invoked when central connects
void BLE_Adafruit::connectCallback(uint16_t conn_handle)
{
//  BLE_Adafruit::isConnected_ = true;
  // Get the reference to current connection
  BLEConnection *connection = Bluefruit.Connection(conn_handle);

  BLE_Adafruit::isConnected_ = true;

  char central_name[32] = {0};
  connection->getPeerName(central_name, sizeof(central_name));

  Serial.print("Connected to ");
  Serial.println(central_name);
}

/**
 * Callback invoked when a connection is dropped
 * @param conn_handle connection where this event happens
 * @param reason is a BLE_HCI_STATUS_CODE which can be found in ble_hci.h
 */
void BLE_Adafruit::disconnectCallback(uint16_t conn_handle, uint8_t reason)
{
//  BLE_Adafruit::isConnected_ = false;
  (void)conn_handle;
  (void)reason;
  BLE_Adafruit::isConnected_ = false;

  Serial.println();
  Serial.print("Disconnected, reason = 0x");
  Serial.println(reason, HEX);
}

bool BLE_Adafruit::isConnected()
{
  return BLE_Adafruit::isConnected_;
}

bool BLE_Adafruit::writeUART(char *msg)
{
  return bleuart->write(msg);
}

bool BLE_Adafruit::available()
{
  return (bool)bleuart->available();
}

void BLE_Adafruit::reset()
{
  // if (FACTORYRESET_ENABLE)
  // {
  //   /* Perform a factory reset to make sure everything is in a known state */
  //   Serial.println(F("Performing a factory reset: "));
  //   if (!ble_.factoryReset())
  //   {
  //     Serial.println(F("Couldn't factory reset"));
  //     return;
  //   }
  // }
  return;
}

void BLE_Adafruit::startAdv(void)
{
  // Advertising packet
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();

  // Include bleuart 128-bit uuid
  Bluefruit.Advertising.addService(*bleuart);

  // Secondary Scan Response packet (optional)
  // Since there is no room for 'Name' in Advertising packet
  Bluefruit.ScanResponse.addName();

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
  Bluefruit.Advertising.setInterval(32, 244); // in unit of 0.625 ms
  Bluefruit.Advertising.setFastTimeout(30);   // number of seconds in fast mode
  Bluefruit.Advertising.start(0);             // 0 = Don't stop advertising after n seconds
}

uint8_t *BLE_Adafruit::getBuffer()
{
  uint8_t i = 0;

  while (available())
  {
    Serial.print("ble is available");
    uint8_t c = bleuart->read();

    Serial.print("reading char -> ");
    Serial.println((char)c);
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
  Serial.println((char *)buffer_);
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
};
