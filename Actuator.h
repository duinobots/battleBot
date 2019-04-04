#pragma once
#include "Arduino.h"

typedef enum
{
  ACTUATOR_TYPE_UNKNOWN = (0),
  ACTUATOR_TYPE_DCMOTOR = (1),
  ACTUATOR_TYPE_SERVO = (2),
  ACTUATOR_TYPE_SOLENOID = (3)
} actuator_types;

typedef enum
{
  ACTUATOR_INPUT_TYPE_BITS = (0),
  ACTUATOR_INPUT_TYPE_VOLTAGE = (1),
  ACTUATOR_INPUT_TYPE_DEGREES = (2),
  ACTUATOR_INPUT_TYPE_MICROSECONDS = (3)
} actuator_input_units;

typedef struct
{
  uint8_t inputPin;
  uint8_t min;
  uint8_t max;
  bool isInverted;
  // actuator_input_units unit; /* todo: not implemented anywhere yet */
} actuator_config;

class Actuator
{
public:
  Actuator();
  Actuator(actuator_types type);
  Actuator(actuator_types type, actuator_config conf);
  ~Actuator();

  void setType(actuator_types type);
  void setConfig(actuator_config conf);

  actuator_types getType() const;
  actuator_config getConfig() const;

  //private:
  actuator_types type_;
  actuator_config config_;
};
