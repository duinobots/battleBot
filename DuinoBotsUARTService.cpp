#include "DuinoBotsUARTService.h"
#include "BLE.h"

DuinoBotsUARTService::DuinoBotsUARTService(uint16_t fifo_depth)
  : BLEService(BLE::DUINOBOTS_SERVICE),
    bleWriteChar1_(BLE::WRITE_CHAR_1),
    bleWriteChar2_(BLE::WRITE_CHAR_2),
    bleWriteChar3_(BLE::WRITE_CHAR_3),
    bleNotifyChar_(BLE::NOTIFY_CHAR),
    isConnected_(false)
{
  _rx_fifo       = NULL;
  _rx_fifo_depth = fifo_depth;

  _rx_cb         = NULL;
  _notify_cb     = NULL;
  _overflow_cb   = NULL;

  _tx_fifo       = NULL;
  _tx_buffered   = false;
}

DuinoBotsUARTService::~DuinoBotsUARTService()
{
    if ( _tx_fifo ) delete _tx_fifo;
}

// Callback when received new data
void DuinoBotsUARTService::bleuart_rxd_cb(uint16_t conn_hdl, BLECharacteristic* chr, uint8_t* data, uint16_t len)
{
  DuinoBotsUARTService& svc = (DuinoBotsUARTService&) chr->parentService();
  uint16_t wrcount = svc._rx_fifo->write(data, len);

  if ( wrcount < len )
  {
    LOG_LV1("MEMORY", "bleuart rxd fifo OVERFLOWED!");

    // invoke overflow callback
    if (svc._overflow_cb) svc._overflow_cb(conn_hdl, len - wrcount);
  }

#if CFG_DEBUG >= 2
  LOG_LV2("BLEUART", "RX: ");
  PRINT_BUFFER(data, len);
#endif

  // invoke user callback
  if ( svc._rx_cb ) svc._rx_cb(conn_hdl);
}

void DuinoBotsUARTService::bleuart_txd_cccd_cb(uint16_t conn_hdl, BLECharacteristic* chr, uint16_t value)
{
  DuinoBotsUARTService& svc = (DuinoBotsUARTService&) chr->parentService();

  if ( svc._notify_cb ) svc._notify_cb(conn_hdl, value & BLE_GATT_HVX_NOTIFICATION);
}

void DuinoBotsUARTService::setRxCallback(rx_callback_t fp, bool deferred)
{
  _rx_cb = fp;

  _rxd.setWriteCallback(DuinoBotsUARTService::bleuart_rxd_cb, deferred);
}

void DuinoBotsUARTService::setRxOverflowCallback(rx_overflow_callback_t fp)
{
  _overflow_cb = fp;
}

void DuinoBotsUARTService::setNotifyCallback(notify_callback_t fp)
{
  _notify_cb = fp;
  _txd.setCccdWriteCallback( fp ? DuinoBotsUARTService::bleuart_txd_cccd_cb : NULL );
}

/**
 * Enable packet buffered for TXD
 * Note: packet is sent right away if it reach MTU bytes
 * @param enable true or false
 */
void DuinoBotsUARTService::bufferTXD(bool enable)
{
  _tx_buffered = enable;

  if ( enable )
  {
    // Create FIFO for TXD
    if ( _tx_fifo == NULL )
    {
      _tx_fifo = new Adafruit_FIFO(1);
      _tx_fifo->begin( Bluefruit.getMaxMtu(BLE_GAP_ROLE_PERIPH) );
    }
  }else
  {
    if ( _tx_fifo ) delete _tx_fifo;
  }
}

err_t DuinoBotsUARTService::begin(void)
{
  _rx_fifo = new Adafruit_FIFO(1);
  _rx_fifo->begin(_rx_fifo_depth);

  // Invoke base class begin()
  VERIFY_STATUS( BLEService::begin() );

  uint16_t max_mtu = Bluefruit.getMaxMtu(BLE_GAP_ROLE_PERIPH);

  // Add TXD Characteristic
  _txd.setProperties(CHR_PROPS_NOTIFY);
  // TODO enable encryption when bonding is enabled
  _txd.setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
  _txd.setMaxLen( max_mtu );
  _txd.setUserDescriptor("TXD");
  VERIFY_STATUS( _txd.begin() );

  // Add RXD Characteristic
  _rxd.setProperties(CHR_PROPS_WRITE | CHR_PROPS_WRITE_WO_RESP);
  _rxd.setWriteCallback(DuinoBotsUARTService::bleuart_rxd_cb, true);

  // TODO enable encryption when bonding is enabled
  _rxd.setPermission(SECMODE_NO_ACCESS, SECMODE_OPEN);
  _rxd.setMaxLen( max_mtu );
  _rxd.setUserDescriptor("RXD");
  VERIFY_STATUS(_rxd.begin());

  return ERROR_NONE;
}

bool DuinoBotsUARTService::notifyEnabled(void)
{
  return this->notifyEnabled(Bluefruit.connHandle());
}

bool DuinoBotsUARTService::notifyEnabled(uint16_t conn_hdl)
{
  return _txd.notifyEnabled(conn_hdl);
}

/*------------------------------------------------------------------*/
/* STREAM API
 *------------------------------------------------------------------*/
int DuinoBotsUARTService::read (void)
{
  uint8_t ch;
  return read(&ch, 1) ? (int) ch : EOF;
}

int DuinoBotsUARTService::read(uint8_t * buf, size_t size)
{
  return _rx_fifo->read(buf, size);
}

uint8_t DuinoBotsUARTService::read8 (void)
{
  uint8_t num;
  return read(&num, sizeof(num)) ? num : 0;
}

uint16_t DuinoBotsUARTService::read16(void)
{
  uint16_t num;
  return read((uint8_t*) &num, sizeof(num)) ? num : 0;
}

uint32_t DuinoBotsUARTService::read32(void)
{
  uint32_t num;
  return read((uint8_t*) &num, sizeof(num)) ? num : 0;
}

size_t DuinoBotsUARTService::write(uint8_t b)
{
  return this->write(Bluefruit.connHandle(), &b, 1);
}

size_t DuinoBotsUARTService::write(uint16_t conn_hdl, uint8_t b)
{
  return this->write(conn_hdl, &b, 1);
}

size_t DuinoBotsUARTService::write(const uint8_t *content, size_t len)
{
  return this->write(Bluefruit.connHandle(), content, len);
}

size_t DuinoBotsUARTService::write(uint16_t conn_hdl, const uint8_t *content, size_t len)
{
  BLEConnection* conn = Bluefruit.Connection(conn_hdl);
  VERIFY(conn, 0);

  // skip if not enabled
  if ( !notifyEnabled(conn_hdl) ) return 0;

  // notify right away if txd buffered is not enabled
  if ( !(_tx_buffered && _tx_fifo) )
  {
    return _txd.notify(conn_hdl, content, len) ? len : 0;
  }else
  {
    uint16_t written = _tx_fifo->write(content, len);

    // Not up to GATT MTU, notify will be sent later by TXD timer handler
    if ( _tx_fifo->count() < (conn->getMtu() - 3) )
    {
      return len;
    }
    else
    {
      // TX fifo has enough data, send notify right away
      VERIFY( flushTXD(conn_hdl), 0);

      // still more data left, send them all
      if ( written < len )
      {
        VERIFY(_txd.notify(conn_hdl, content+written, len-written), written);
      }

      return len;
    }
  }
}

bool DuinoBotsUARTService::isConnected()
{
  return Bluefruit.connected();
}

bool DuinoBotsUARTService::writeUART(const char *msg)
{
  const uint16_t data_mtu = Bluefruit.Connection(0)->getMtu() - 3;
  return bleuart_.write(msg, data_mtu);
}

int DuinoBotsUARTService::available()
{
  return bleuart_.available();
}

void DuinoBotsUARTService::reset()
{
  return;
}

uint8_t* DuinoBotsUARTService::getBuffer()
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

    // if (ADAFRUIT_BLE_DEBUG_MODE)
    // {
    //   writeBleDebug(ch);
    // }

  }
  Serial.print("DuinoBotsUARTService::getBuffer(), buffer_ -> ");
  Serial.println((char*)buffer_);
  Serial.print("sizeof(buffer_) -> ");
  Serial.println(sizeof(buffer_));
  return buffer_;
}

void DuinoBotsUARTService::clearBuffer()
{
  for (uint8_t i = 0; i < BUFFER_LENGTH; i++)
  {
    buffer_[i] = 0;
  }
}

void DuinoBotsUARTService::onConnect()
{
  // ble_.setMode(BLUEFRUIT_MODE_DATA);
  return;
}

/*
 * Write received BLE info to Serial Monitor
 */
// void DuinoBotsUARTService::writeBleDebug(uint8_t c)
// {
//   Serial.print((char)c);
//   // Hex output too, helps w/debugging!
//   Serial.print(" [0x");
//   if (c <= 0xF)
//     Serial.print(F("0"));
//   Serial.print(c, HEX);
//   Serial.print("] ");
//   Serial.println();
// }