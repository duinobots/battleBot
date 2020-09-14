#pragma once

#include "Arduino.h"

typedef enum {
  MOTOR_LEFT = 1,
  MOTOR_RIGHT = 2,
  MOTOR_WEAPON = 3
} MotorPositions;

/**
 * @brief Base Motor Class, the type of motor is determined by the motor driver board
 * used to communicate with it
 */
class Motor
{
  public:
    Motor() = default;
    virtual ~Motor() = default;
    virtual void init(const uint16_t freq) = 0;
    virtual void setSpeed(const int speed) = 0;

    static constexpr uint16_t PWM_FREQUENCY = 1000;

  protected:
    MotorPositions pos_;
};