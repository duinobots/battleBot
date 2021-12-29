#include "Hammer.h"
#include "Arduino.h"

Hammer::Hammer(const Actuator& actuator)
  : Weapon(actuator)
  , lastHitMs_(millis())
  , coolDownMs_(Hammer::HAMMER_COOLDOWN_MS)
{ }

bool Hammer::init()
{
  if (actuator_.getType() != ACTUATOR_TYPE_SERVO)
  {
    Serial.println("Hammer type weapons require a servo actuator!");
    return false;
  }

  servo_.attach(actuator_.getConfig().inputPin_);
  enable();
  home();
  Serial.println("Hammer initialized!!");
  return true;
}

void Hammer::actuate()
{
  Serial.println("hammer actuated!");
  if (!isEnabled())
    return;

  swing();
  lastHitMs_ = millis();
}

void Hammer::update()
{
  if (isPaused())
    return;

  if (millis() - lastHitMs_ > coolDownMs_)
    home();
}

void Hammer::writeValue(int val)
{
  val = val > 180 ? 180 : val;
  val = val < 0 ? 0 : val;
  servo_.write(val);
}

void Hammer::onDisable() {
  home();
}

void Hammer::home() {
  servo_.write(actuator_.getConfig().isInverted_ ? actuator_.getConfig().max_ : actuator_.getConfig().min_);
}

void Hammer::swing() {
  servo_.write(actuator_.getConfig().isInverted_ ? actuator_.getConfig().min_ : actuator_.getConfig().max_);
}
