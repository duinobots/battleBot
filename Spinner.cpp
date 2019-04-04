#include "Spinner.h"
#include "Arduino.h"

Spinner::Spinner(actuator_types type, actuator_config conf)
{
  actuator_ = Actuator(type, conf);
}

Spinner::~Spinner() { }

void Spinner::init()
{
  if (actuator_.type_ != ACTUATOR_TYPE_DCMOTOR)
  {
    Serial.println("Spinner weapons require a DC motor actuator!");
    return;
  }

  shield_.begin();
  motor_ = shield_.getMotor(actuator_.config_.inputPin);

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
  return;
}

/**
 * valid range from [-255, 255], negative values imply reverse motor direction
 */
void Spinner::writeValue(int val)
{
  val = val > 255 ? 255 : val;
  val = val < -255 ? -255 : val;

  motor_->run(val == 0 ? RELEASE : val > 0 ? FORWARD : BACKWARD);
  motor_->setSpeed(abs(val));
}

void Spinner::onDisable()
{
  //  Serial.println("Spinner onDisable called");
  stop();
}

void Spinner::spin()
{
  motor_->run(actuator_.config_.isInverted ? FORWARD : BACKWARD);
  motor_->setSpeed(actuator_.config_.isInverted ? actuator_.config_.min : actuator_.config_.max);

  isOn_ = true;
}

void Spinner::stop()
{
  motor_->setSpeed(actuator_.config_.isInverted ? actuator_.config_.max : actuator_.config_.min);
  motor_->run(RELEASE);

  isOn_ = false;
}

void Spinner::toggle()
{
  if (isOn_)
  {
    stop();
  }
  else
  {
    spin();
  }
};
