#pragma once

#include "Arduino.h"

typedef struct
{
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint32_t blinkInterval; /* time in ms to alternate LED between currentStatus_/off. If 0, LED remains on constantly */
  uint32_t timeout;       /* time in ms to display current status. after this amount of time, LED reverts to previous status. If 0, current status will remain indefinitely */
} led_status;

inline bool operator==(const led_status &lhs, const led_status &rhs)
{
  return lhs.r == rhs.r && lhs.g == rhs.g && lhs.b == rhs.b && lhs.blinkInterval == rhs.blinkInterval && lhs.timeout == rhs.timeout;
}

inline bool operator!=(const led_status &lhs, const led_status &rhs)
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
} led_statuses;

const led_status LED_STATUSES[8] = {
    {  0,   0,   0,    0,    0},
    {  0, 255,   0,    0,    0},
    {255, 180,   0,  500,    0},
    {255,   0,   0,  200,    0},
    {  0,   0, 255, 1000,    0},
    {255,   0,   0, 1000,    0},
    {255,   0,   0,  100, 1000},
    {255, 255, 255,  100, 2000}};

class RgbLED
{
public:
  RgbLED(int rPin, int gPin, int bPin);
  ~RgbLED();

  void init();

  void setStatus(led_statuses status);
  void setStatus(led_status status);
  void setColor(int r, int g, int b);

  bool isOn() const;
  void turnOff();

  void update();

private:
  int rPin_;
  int gPin_;
  int bPin_;
  bool isOn_;

  led_status currentStatus_;
  led_status lastStatus_;

  long lastStatusChange_;
  long statusTimeout_; /* timer for statuses with a non-zero timeout */
};
