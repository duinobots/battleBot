#pragma once
#include "Arduino.h"
#include "Motor.h"
#include <Adafruit_MotorShield.h>

class Motor_Adafruit : public Motor
{
public:
  Motor_Adafruit(uint8_t addr, motor_positions pos);
  void init(uint16_t freq);
  void setSpeed(int speed);
  void setFailSafe(uint16_t ms = 1000);

private:
  Adafruit_MotorShield motorShield_;
  Adafruit_DCMotor *motor_;
};
