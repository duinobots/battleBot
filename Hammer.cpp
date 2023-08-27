#include "Hammer.h"

Hammer::Hammer(actuator_types type, actuator_config conf)
{
  actuator_ = Actuator(type, conf);
  lastHitMs_ = millis();
  coolDownMs_ = 500;
}

Hammer::~Hammer()
{
}

void Hammer::init()
{
  if (actuator_.type_ != ACTUATOR_TYPE_SERVO)
  {
    Serial.println("Hammer type weapons require a servo actuator!");
    return;
  }

  servo_.attach(actuator_.config_.inputPin);
  enable();
  servo_.write(actuator_.config_.isInverted ? actuator_.config_.max : actuator_.config_.min);
  Serial.println("Hammer initialized!!");
}

void Hammer::actuate()
{
  Serial.println("hammer actuated!");
  if (!isEnabled())
  {
    return;
  }

  servo_.write(actuator_.config_.isInverted ? actuator_.config_.min : actuator_.config_.max);
  lastHitMs_ = millis();
}

void Hammer::update()
{
  if (isPaused())
  {
    return;
  }

  if (millis() - lastHitMs_ > coolDownMs_)
  {
    servo_.write(actuator_.config_.isInverted ? actuator_.config_.max : actuator_.config_.min);
  }
}

void Hammer::writeValue(int val)
{
  val = val > 180 ? 180 : val;
  val = val < 0 ? 0 : val;
  servo_.write(val);
}

void Hammer::onDisable(){
    servo_.write(actuator_.config_.isInverted ? actuator_.config_.max : actuator_.config_.min);
};
