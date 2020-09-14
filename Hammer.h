#pragma once
#include "Servo.h"
#include "Weapon.h"

/**
 * @brief Hammer Weapon Class
 */
class Hammer : public Weapon
{
public:
  Hammer(const Actuator& actuator);
  ~Hammer() = default;

  void init() override;
  void actuate() override;
  void update() override;
  void writeValue(const int val) override;

protected:
  void onDisable() override;

private:
  /**
   * @brief Cooldown in ms between allowed hammer swings
   */
  static constexpr long HAMMER_COOLDOWN_MS = 500;

  /**
   * @brief Bring the hammer to the upright position
   */
  void home();

  /**
   * @brief Swing the hammer down
   */
  void swing();

  Servo servo_;
  long lastHitMs_;
  long coolDownMs_;
};
