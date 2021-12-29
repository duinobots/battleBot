#pragma once
#include "Arduino.h"

typedef enum
{
  ACTUATOR_TYPE_UNKNOWN = (-1),
  ACTUATOR_TYPE_DCMOTOR = (1),
  ACTUATOR_TYPE_SERVO = (2)
} ActuatorTypes;

/**
 * @brief Represents the configuration of an actuator, used to specify what pin to
 * send control signals to as well as clamp input values appropriately
 */
struct ActuatorConfig
{
  ActuatorConfig(uint8_t inputPin, uint8_t min, uint8_t max, bool isInverted)
    : inputPin_(inputPin), min_(min), max_(max), isInverted_(isInverted)
    {
    }
  uint8_t inputPin_;
  uint8_t min_;
  uint8_t max_;
  bool isInverted_;
};

/**
 * @brief Actuator Class, represents a controllable motor or servo (type) with a given
 * configuration
 */
class Actuator
{
public:
  Actuator(const ActuatorTypes type, const ActuatorConfig& conf);

  void setType(const ActuatorTypes type);
  void setConfig(const ActuatorConfig& conf);

  const ActuatorTypes getType() const;
  const ActuatorConfig& getConfig() const;

private:
  ActuatorTypes type_;
  ActuatorConfig config_;
};
