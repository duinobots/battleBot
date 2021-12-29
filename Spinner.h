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

  bool init() override;
  void actuate() override;
  void update() override;
  void writeValue(const int val) override;

protected:
  void onDisable() override;

private:
  Motor *motor_;
  bool isOn_;
  /**
   * @brief Begin spinning the weapon at max speed
   */
  void spin();
  /**
   * @brief Stop spinning the weapon
   */
  void stop();
  /**
   * @brief Convenience function for toggling the spinner on and off
   */
  void toggle();
};
