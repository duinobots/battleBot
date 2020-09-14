#include "Weapon.h"

Weapon::Weapon(const Actuator& actuator)
  : actuator_(actuator)
{ }

const bool Weapon::isEnabled() const
{
  return isEnabled_;
}

void Weapon::enable()
{
  isEnabled_ = true;
  isPaused_ = false;
}

void Weapon::disable()
{
  onDisable();
  isEnabled_ = false;
  isPaused_ = true;
}

void Weapon::pause()
{
  isPaused_ = true;
}

void Weapon::resume()
{
  isPaused_ = false;
}

const bool Weapon::isPaused() const
{
  return isPaused_;
}

void Weapon::updateConfig(const ActuatorConfig& conf)
{
  actuator_.setConfig(conf);
};
