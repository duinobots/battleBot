#include "Spinner.h"
#include "Arduino.h"

Spinner::Spinner(const Actuator& actuator)
  : Weapon(actuator)
{ }

void Spinner::init()
{
  if (actuator_.getType() != ACTUATOR_TYPE_DCMOTOR)
  {
    Serial.println("Spinner weapons require a DC motor actuator!");
    return;
  }

  motor_->init(Motor::PWM_FREQUENCY);

  enable();
  stop();

  Serial.println("Spinner initialized!!");
}

void Spinner::actuate()
{
  if (!isEnabled())
  {
    return;
  }

  toggle();
}

void Spinner::update()
{
  // do nothing
}

/**
 * valid range from [-255, 255], negative values imply reverse motor direction
 */
void Spinner::writeValue(int val)
{
  val = val > 255 ? 255 : val;
  val = val < -255 ? -255 : val;

  motor_->setSpeed(abs(val));
}

void Spinner::onDisable()
{
  stop();
}

void Spinner::spin()
{
  motor_->setSpeed(actuator_.getConfig().isInverted_ ? actuator_.getConfig().min_ : actuator_.getConfig().max_);
  isOn_ = true;
}

void Spinner::stop()
{
  motor_->setSpeed(actuator_.getConfig().isInverted_ ? actuator_.getConfig().max_ : actuator_.getConfig().min_);
  isOn_ = false;
}

void Spinner::toggle()
{
  isOn_ ? stop() : spin();
};
