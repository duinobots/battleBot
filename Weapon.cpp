#include "Weapon.h"

Weapon::Weapon() { }

Weapon::~Weapon() { }

bool Weapon::isEnabled() const
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

bool Weapon::isPaused() const
{
  return isPaused_;
}

void Weapon::updateConfig(actuator_config conf)
{
  actuator_.setConfig(conf);
};
