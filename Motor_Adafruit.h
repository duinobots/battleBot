#pragma once
#include "Arduino.h"
#include "Motor.h"
#include <Adafruit_MotorShield.h>

class Motor_Adafruit : public Motor
{
public:
  Motor_Adafruit(uint8_t addr, MotorPositions pos);
  void init(const uint16_t freq) override;
  void setSpeed(const int speed) override;

private:
  Adafruit_MotorShield motorShield_;
  Adafruit_DCMotor *motor_;
};
