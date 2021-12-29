#pragma once

#include "Arduino.h"

/**
 * @brief Possible positions for motors to be located on a bot
 */
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
  /**
   * @brief Initialize comms with motor driver board, PWM frequency of
   */
  virtual void init(const uint16_t freq) = 0;
  /**
   * @brief Rotate motor at a given speed, limits are clamped at [Motor::MIN, Motor::MAX]
   */
  virtual void setSpeed(const int speed) = 0;

  MotorPositions pos_;
  static constexpr int MIN = -255;
  static constexpr int MAX = 255;
  static constexpr uint16_t PWM_FREQUENCY = 1000;
  
protected:
  inline int clampInput(int speed)
  {
    speed = speed > Motor::MAX ? Motor::MAX : speed;
    speed = speed < Motor::MIN ? Motor::MIN : speed;
    return speed;
  };
};
