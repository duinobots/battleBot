#include "Actuator.h"

Actuator::Actuator()
{
  type_ = ACTUATOR_TYPE_UNKNOWN;
  config_ = {};
}

Actuator::Actuator(actuator_types type)
{
  type_ = type;
  config_ = {};
}

Actuator::Actuator(actuator_types type, actuator_config conf)
{
  type_ = type;
  config_ = conf;
}

Actuator::~Actuator()
{
}

void Actuator::setType(actuator_types type)
{
  type_ = type;
}

void Actuator::setConfig(actuator_config conf)
{
  config_ = conf;
}

actuator_types Actuator::getType() const
{
  return type_;
}

actuator_config Actuator::getConfig() const
{
  return config_;
};
