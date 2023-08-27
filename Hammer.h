#pragma once
#include <Arduino.h>
#include <Servo.h>
#include "Weapon.h"

class Hammer : public Weapon
{
public:
  Hammer(actuator_types type, actuator_config conf);
  ~Hammer();

  void init();
  void actuate();
  void update();
  void writeValue(int val);

protected:
  void onDisable();

private:
  Servo servo_;
  long lastHitMs_;
  long coolDownMs_;
};
