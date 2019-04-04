#pragma once
#include "Actuator.h"

typedef enum
{
  WEAPON_TYPE_HAMMER = (1),
  WEAPON_TYPE_SPINNER = (2)
} weapon_types;

// this is an abstract class
class Weapon
{
public:
  Weapon();
  virtual ~Weapon();

  /**
   * these four are 'pure virtual' functions (a function that ends in zero is defined to be a pure virtual).
   * these don't get a definition in the Weapon.cpp file. this makes the class abstract by definition. Any
   * concrete class that inherits from Weapon must define a matching function for all virtual functions in Weapon.
   */
  virtual void init() = 0;
  virtual void actuate() = 0;
  virtual void update() = 0; // must be called in main loop
  virtual void writeValue(int val) = 0; // write a specific value to the actuator. used for special move sequences

  void enable();
  void disable();
  bool isEnabled() const;
  void pause();
  void resume();
  bool isPaused() const;

  void updateConfig(actuator_config conf);

// protected member properties and functions can only be accessed by sub classes of Weapon
protected:
  // the type of actuator used to power the weapon. for hammer weapons, this represents a servo,
  // for spinner weapons, this represents a dc motor
  Actuator actuator_;

  virtual void onDisable() = 0; // callback function used to shut down weapon when disable() is called

private:
  bool isEnabled_;
  bool isPaused_; // a weapon that is paused does not evaluate its update() function in the main loop, used for special move sequences
};
