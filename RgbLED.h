#pragma once

#include "Arduino.h"

typedef struct
{
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint32_t blinkInterval; /* time in ms to alternate LED between currentStatus_/off. If 0, LED remains on constantly */
  uint32_t timeout;       /* time in ms to display current status. after this amount of time, LED reverts to previous status. If 0, current status will remain indefinitely */
} LEDStatus;

inline bool operator==(const LEDStatus &lhs, const LEDStatus &rhs)
{
  return lhs.r == rhs.r && lhs.g == rhs.g && lhs.b == rhs.b && lhs.blinkInterval == rhs.blinkInterval && lhs.timeout == rhs.timeout;
}

inline bool operator!=(const LEDStatus &lhs, const LEDStatus &rhs)
{
  return lhs.r != rhs.r || lhs.g != rhs.g || lhs.b != rhs.b || lhs.blinkInterval != rhs.blinkInterval && lhs.timeout != rhs.timeout;
}

/* note: enum values correspond to index of LED_STATUSES, must be updated simultaneously */
typedef enum
{
  LED_STATUS_OFF = (0),
  LED_STATUS_OK = (1),
  LED_STATUS_WARNING = (2),
  LED_STATUS_DANGER = (3),
  LED_STATUS_WAITING_FOR_CONNECT = (4),
  LED_STATUS_DISCONNECTED = (5),
  LED_STATUS_HIT_DETECTED = (6),
  LED_STATUS_TEST_ACK = (7)
} LEDStatuses;

const LEDStatus LED_STATUSES[8] = {
  {  0,   0,   0,    0,    0},
  {  0, 255,   0,    0,    0},
  {255, 180,   0,  500,    0},
  {255,   0,   0,  200,    0},
  {  0,   0, 255, 1000,    0},
  {255,   0,   0, 1000,    0},
  {255,   0,   0,  100, 1000},
  {255, 255, 255,  100, 2000}
};

class RgbLED
{
public:
  RgbLED(uint8_t rPin, uint8_t gPin, uint8_t bPin);
  ~RgbLED() = default;

  void init();

  void setStatus(const LEDStatuses& status);
  void setStatus(LEDStatus status);
  void setColor(uint8_t r, uint8_t g, uint8_t b);

  bool isOn() const;
  void turnOff();

  void update();

private:
  uint8_t rPin_;
  uint8_t gPin_;
  uint8_t bPin_;
  bool isOn_;

  LEDStatus currentStatus_;
  LEDStatus lastStatus_;

  long lastStatusChange_;
  long statusTimeout_; /* timer for statuses with a non-zero timeout */
};
