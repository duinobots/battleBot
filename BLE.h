#pragma once

#ifndef BUFFER_LENGTH
#define BUFFER_LENGTH 20
#endif

// Debug output adds extra flash and memory requirements!
#ifndef BLE_DEBUG
#define BLE_DEBUG false
#endif

/**
 * @brief Base BLE class
 */
class BLE
{
public:
  virtual ~BLE() = default;
  /**
   * @brief initialize BLE chip
   */
  virtual bool init() = 0;
  /**
   * @brief returns true if connected to central, false otherwise
   */
  virtual bool isConnected() = 0;
  /**
   * @brief send a message to central via UART
   */
  virtual bool writeUART(char *msg) = 0;
  /**
   * @brief check for new messages from central
   * @note must be run in main loop!
   */
  virtual bool available() = 0;
  /**
   * @brief reset the bluetooth chip
   */
  virtual void reset() = 0;
  /**
   * @brief retrieve any values from the read buffer that have been received from central
   */
  virtual uint8_t* getBuffer() = 0;
  /**
   * @brief clear read buffer
   */
  virtual void clearBuffer() = 0;
  /**
   * @brief hook to be called on connection to central
   */
  virtual void onConnect() = 0;
};
