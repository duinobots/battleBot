#pragma once
#include "Actuator.h"

typedef enum
{
  WEAPON_TYPE_HAMMER = (1),
  WEAPON_TYPE_SPINNER = (2)
} WeaponTypes;

/**
 * @brief Base Weapon Class - Represents a hammer or spinner weapon that contains an actuator. A weapon *must*
 * call its `update()` function in the main loop
 */
class Weapon
{
public:
  /**
   * @brief Instantiate a weapon with an actuator
   */
  Weapon(const Actuator& actuator);
  virtual ~Weapon() = default;
  /**
   * @brief Initialize the weapon, set up actuator comms, home
   */
  virtual bool init() = 0;
  /**
   * @brief Actuate the weapon. This is the action that should be performed when a Weapon Command ('W')
   * is received from central
   */
  virtual void actuate() = 0;
  /**
   * @brief *MUST BE CALLED IN MAIN LOOP* Used to update state of the weapon during battle
   */
  virtual void update() = 0;
  /**
   * @brief Write a specific value to the actuator. Used for special move sequences
   */
  virtual void writeValue(const int val) = 0;
  void enable();
  void disable();
  const bool isEnabled() const;
  /**
   * @brief pause weapon updates in the main loop, used for special move sequences
   */
  void pause();
  /**
   * @brief resume weapon updates in the main loop, used for special move sequences
   */
  void resume();
  const bool isPaused() const;
  /**
   * @brief set the configuration for the underlying actuator
   */
  void updateConfig(const ActuatorConfig& conf);

protected:
  /**
   * @brief The actuator used to power the weapon. for hammer weapons, this represents a servo,
   * for spinner weapons, this represents a DC motor
   */
  Actuator actuator_;
  /**
   * @brief Callback function used to shut down weapon when disable() is called
   */
  virtual void onDisable() = 0;

private:
  bool isEnabled_;
  /**
   * @brief A weapon that is paused does not evaluate its update() function in the main loop, used for special move sequences
   */
  bool isPaused_;
};
