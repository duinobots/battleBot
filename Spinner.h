#pragma once

#include "Weapon.h"
#include "Motor.h"

/**
 * @brief Spinner Weapon Class - Not officially supported in the app as of yet, contains a DC motor
 * that is used to spin the weapon around at high speed
 */
class Spinner : public Weapon
{
public:
  Spinner(const Actuator& actuator);
  ~Spinner() = default;

  void init() override;
  void actuate() override;
  void update() override;
  void writeValue(const int val) override;

protected:
  void onDisable() override;

private:
  Motor *motor_;
  bool isOn_;

  void spin();
  void stop();
  void toggle();
};
