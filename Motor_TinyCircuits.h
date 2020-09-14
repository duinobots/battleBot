#pragma once

#include <Wire.h>
#include "Arduino.h"
#include "Motor.h"
#include <MotorDriver.h>

class Motor_TinyCircuits : public Motor
{
public:
  Motor_TinyCircuits(uint8_t addr, MotorPositions pos);
  void init(uint16_t freq) override;
  void setSpeed(int speed) override;

private:
  MotorDriver motor_;
};
