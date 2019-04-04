#pragma once

#include <Wire.h>
#include "Arduino.h"
#include "Motor.h"
#include <MotorDriver.h>

class Motor_TinyCircuits : public Motor
{
public:
  Motor_TinyCircuits(uint8_t addr, motor_positions pos);
  void init(uint16_t freq);
  void setSpeed(int speed);
  void setFailSafe(uint16_t ms = 1000);

private:
  MotorDriver motor_;
};
