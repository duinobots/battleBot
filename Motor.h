#pragma once

#include "Arduino.h"

typedef enum {
  MOTOR_LEFT = 1,
  MOTOR_RIGHT = 2,
  MOTOR_WEAPON = 3
} motor_positions;

class Motor
{
  public:
    Motor() {};
    virtual ~Motor() {};
    virtual void init(uint16_t freq) = 0;
    virtual void setSpeed(int speed) = 0;
//    virtual void setFailsafe(uint16_t ms = 1000) = 0;

  protected:
    motor_positions pos_;
};
