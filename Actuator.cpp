#include "Actuator.h"

Actuator::Actuator(const ActuatorTypes type, const ActuatorConfig& conf)
  : type_(type), config_(conf)
{
}

void Actuator::setType(const ActuatorTypes type)
{
  type_ = type;
}

void Actuator::setConfig(const ActuatorConfig& conf)
{
  config_ = conf;
}

const ActuatorTypes Actuator::getType() const
{
  return type_;
}

const ActuatorConfig& Actuator::getConfig() const
{
  return config_;
};
