#pragma once

#include "Weapon.h"
#include <Adafruit_MotorShield.h>

class Spinner : public Weapon
{
public:
  Spinner(actuator_types type, actuator_config conf);
  ~Spinner();

  /*
  * define matching member functions for the virtual methods in Weapon
  */
  void init();
  void actuate();
  void update();
  void writeValue(int val);

protected:
  void onDisable(); // this is a virtual Weapon method

private:
  Adafruit_DCMotor *motor_;
  Adafruit_MotorShield shield_;
  bool isOn_;

  void spin();
  void stop();
  void toggle();
};
