#pragma once

#include "Arduino.h"
#include <SPI.h>
#include "Hammer.h"
#include "Weapon.h"
#include "RgbLED.h"
#include "Motor.h"
#include "BLE.h"
#include <Wire.h>

typedef enum
{
  ADAFRUIT = 1,
  TINY_CIRCUITS = 2
} bot_versions;

typedef enum
{
  DAMAGE_TYPE_FRONT = 1,
  DAMAGE_TYPE_BACK = 2
} battleBot_damage_types;

class BattleBot
{
public:
  BattleBot(bot_versions version);

  void init();

  const bool isEnabled() const;
  const bool isConnected() const;
  const bool isAlive() const;

  void battle();         /* main update function */
  void deathRattle();    /* just for fun, specific moves when bot dies */

  void waitForReset();   /* wait for reset command from phone, resets health and gets robot ready for battle */

private:
  void bleInit();
  void waitForConfig();
  void setConfig(uint8_t *config);
  void shutdown();

  void enable();
  void disable();
  void onConnect();
  void onDisconnect();

  void drive(int leftSpeed, int rightSpeed);
  void forward(uint8_t speed = 255);
  void backward(uint8_t speed = 255);
  void left(uint8_t speed = 255);
  void right(uint8_t speed = 255);
  void stop();

  void detectDamage();
  void notifyDamage(battleBot_damage_types type);
  void updateHealth(int health);
  void updateLED();

  void handleInputs();
  void sendTestMessageAck();
  void parseCommand(uint8_t *buf);

  bot_versions botVersion_;

  Weapon *weapon_;
  Motor *leftMotor_;
  Motor *rightMotor_;
  BLE *ble_;
  RgbLED *led_;

  int health_ = 100;
  bool isEnabled_ = false;
  bool isConnected_ = false;
  long lastUpdateMs_ = 0;

  bool hardwareInitComplete_ = false; /* used to flag that serial, motor shield, and hit sensor/LED pins have been initialized */
  bool configReceived_ = false;

  int frontHitSensorPin_ = 0;
  int backHitSensorPin_ = 0;

  int hitDebounceMs_ = 800;  /* debounce timer for hit detection via hit sensors */
  long lastFrontHitMs_ = 0;  /* time of last hit recorded by front hit sensor */
  long lastBackHitMs_ = 0;   /* time of last hit recorded by back hit sensor */
};
