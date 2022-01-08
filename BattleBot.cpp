#include "BattleBot.h"
// #include "BLE_TinyCircuits.h"
// #include "Motor_TinyCircuits.h"
#include "BLE_Adafruit.h"
// #include "BluefruitConfig.h"
#include "Motor_Adafruit.h"
#include "Hammer.h"
#include "Spinner.h"
#include "Weapon.h"

/*
   Bot constructor
*/
BattleBot::BattleBot(BotVersions version)
    : botVersion_(version), health_(100), isEnabled_(false), hardwareInitComplete_(false), configReceived_(false), lastFrontHitMs_(millis()), lastBackHitMs_(millis()), lastUpdateMs_(millis())
{
  //  if (botVersion_ == TINY_CIRCUITS)
  //  {
  //    ble_ = new BLE_TinyCircuits();
  //    leftMotor_ = new Motor_TinyCircuits(0, MOTOR_LEFT);
  //    rightMotor_ = new Motor_TinyCircuits(0, MOTOR_RIGHT);
  //    led_ = new RgbLED(4, 3, 7);
  //    frontHitSensorPin_ = 8;
  //    backHitSensorPin_ = 6;
  //  }
  if (botVersion_ == ADAFRUIT)
  {
    ble_ = new BLE_Adafruit(1, 2, 3);
    leftMotor_ = new Motor_Adafruit(0, MOTOR_LEFT);
    rightMotor_ = new Motor_Adafruit(0, MOTOR_RIGHT);
    led_ = new RgbLED(9, 10, 11);
    frontHitSensorPin_ = 12;
    backHitSensorPin_ = 6;
  }
}

BattleBot::~BattleBot()
{
  delete ble_, leftMotor_, rightMotor_, led_;
}

/*
   Public member functions
*/

/*
   bot setup function, does the following:
   1. initializes pins/LED
   2. inits BLE/connects to central
   3. receives/sets robot config
*/
void BattleBot::init()
{
  Serial.begin(115200);
  while (!Serial)
    delay(10);

  Serial.println("Serial init");
  //  Serial.println("entering init!");
  //  if (botVersion_ == TINY_CIRCUITS)
  //    Wire.begin();
  //
  if (!hardwareInitComplete_)
  {
    Serial.println("hardwareInit not complete!");
    // give everything a second to set up I guess?
    delay(1000);

    leftMotor_->init(1000);
    rightMotor_->init(1000);

    // set up hit sensor pins
    pinMode(frontHitSensorPin_, INPUT);
    pinMode(backHitSensorPin_, INPUT);

    led_->init();
  }

  Serial.println("about to initialize BLE!");
  // connect to phone
  bleInit();

  // upload weapon config
  waitForConfig();

  Serial.println("Config received!");

  enable();
  weapon_->init();

  Serial.println("Ready for battle!!");
}

void BattleBot::enable()
{
  isEnabled_ = true;
}

void BattleBot::disable()
{
  isEnabled_ = false;
}

const bool BattleBot::isEnabled() const
{
  return isEnabled_;
}

const bool BattleBot::isConnected() const
{
  return isConnected_;
}

const bool BattleBot::isAlive() const
{
  return health_ > 0;
}

/*
   Main update function, performs four main tasks:
   1. handle any inputs received from central
   2. detect hits/damage and notify central if so
   3. update weapon
   4. update LED display status
*/
void BattleBot::battle()
{
  handleInputs();
  detectDamage();
  weapon_->update();
  updateLED();
}

/*
   Execute a dramatic death spiral
*/
void BattleBot::deathRattle()
{
  for (int i = 255; i >= 0; i--)
  {
    led_->setColor(i, 0, 0);
    right(i);
    delay(10);
  }
}

/*
   Shut down bot and wait for health command to revive and re-initialize
*/
void BattleBot::waitForReset()
{
  shutdown();

  // wait for health update command to revive the bot
  while (!isAlive())
    handleInputs();

  //re-initialize
  init();
}

/*
   Private member functions
*/

/*
   Initialize BLE and wait for connect
*/
void BattleBot::bleInit()
{
  Serial.println("bleInit()!");
  bool wasReset = false;

  if (!hardwareInitComplete_)
  {
    // Initialise the module
    Serial.print("Initialising the BLE module:");
    ble_->init();
    Serial.print("BLE initialized");

    // set flag to true after ble module is initialized
    hardwareInitComplete_ = true;
    wasReset = true;
  }

  if (!wasReset)
    ble_->reset();

  Serial.println("Waiting to connect..");

  led_->setStatus(LED_STATUS_WAITING_FOR_CONNECT);

  // Wait for connection to central
  while (!ble_->isConnected())
  {
    ble_->available();
    led_->update();
    delay(50);
  }

  onConnect();
}

/*
   Blocking loop until weapon config received from central
*/
void BattleBot::waitForConfig()
{
  Serial.println("waiting for config packet..");

  led_->setStatus(LED_STATUS_WARNING);

  while (!configReceived_)
  {
    led_->update();
    handleInputs();
  }

  led_->setStatus(LED_STATUS_OK);
}

/*
   Shutdown and stop the bot, reset config
*/
void BattleBot::shutdown()
{
  stop();
  disable();
  weapon_->disable();
  configReceived_ = false;
}

/*
   set up weapon from config message
   todo: the entire config packet is probably kind of overkill, could reduce to just
   weapon class and store configs for each type as a member variable.
*/
void BattleBot::setConfig(uint8_t *config)
{
  int weaponClass, actuatorInputPin, actuatorMin, actuatorMax, actuatorIsInverted;
  bool configSet = false;
  Serial.println("got config:");
  Serial.println(config[1]);
  Serial.println(config[2]);
  Serial.println(config[3]);
  Serial.println(config[4]);
  Serial.println(config[5]);
  Serial.println(config[6]);

  weaponClass = config[2];
  actuatorInputPin = config[3];
  actuatorMin = config[4];
  actuatorMax = config[5];
  actuatorIsInverted = config[6];

  ActuatorConfig inputConfig(actuatorInputPin, actuatorMin, actuatorMax, actuatorIsInverted == 1 ? true : false);

  // default weapon configs - this creates a persistent instance of each weapon type scoped only to this function
  static Hammer hammer = Hammer(Actuator(ActuatorTypes::ACTUATOR_TYPE_SERVO, inputConfig));
  static Spinner spinner = Spinner(Actuator(ActuatorTypes::ACTUATOR_TYPE_DCMOTOR, inputConfig));

  // switching weapon class points our weapon_ pointer to the address of (&) our hammer and spinner objects
  switch (weaponClass)
  {
  case (WEAPON_TYPE_HAMMER):
    Serial.println("initializing hammer!");
    weapon_ = &hammer;
    configReceived_ = true;
    break;
  case (WEAPON_TYPE_SPINNER):
    Serial.println("initializing spinner!");
    weapon_ = &spinner;
    configReceived_ = true;
    break;
  default:
    Serial.println("Got Invalid Weapon Type!");
    break;
  }
}

/*
   Callback executed on connection to central
*/
void BattleBot::onConnect()
{
  led_->setStatus(LED_STATUS_OK);
  ble_->onConnect();
  isConnected_ = true;
  enable();

  Serial.println("BLE connected!!");
}

/*
   Callback executed on disconnection from central
*/
void BattleBot::onDisconnect()
{
  Serial.println("BLE Disconnected!!");

  shutdown();

  isConnected_ = false;
  led_->setStatus(LED_STATUS_DISCONNECTED);
  health_ = 100;

  init();
}

void BattleBot::drive(int leftSpeed, int rightSpeed)
{
  leftMotor_->setSpeed(leftSpeed);
  rightMotor_->setSpeed(rightSpeed);
}

void BattleBot::forward(uint8_t speed)
{
  leftMotor_->setSpeed(speed);
  rightMotor_->setSpeed(speed);
}

void BattleBot::backward(uint8_t speed)
{
  leftMotor_->setSpeed(-speed);
  rightMotor_->setSpeed(-speed);
}

void BattleBot::left(uint8_t speed)
{
  leftMotor_->setSpeed(-speed);
  rightMotor_->setSpeed(speed);
}

void BattleBot::right(uint8_t speed)
{
  leftMotor_->setSpeed(speed);
  rightMotor_->setSpeed(-speed);
}

void BattleBot::stop()
{
  leftMotor_->setSpeed(0);
  rightMotor_->setSpeed(0);
}

/*
   Determine if damage was taken
*/
void BattleBot::detectDamage()
{
  bool frontHit, backHit;
  frontHit = (bool)digitalRead(frontHitSensorPin_);
  backHit = (bool)digitalRead(backHitSensorPin_);

  if (frontHit && ((millis() - lastFrontHitMs_) > hitDebounceMs_))
  {
    notifyDamage(DAMAGE_TYPE_FRONT);
    lastFrontHitMs_ = millis();
  }

  if (backHit && ((millis() - lastBackHitMs_) > hitDebounceMs_))
  {
    notifyDamage(DAMAGE_TYPE_BACK);
    lastBackHitMs_ = millis();
  }
}

/*
   Update central with type of hit received
*/
void BattleBot::notifyDamage(BattleBotDamageTypes type)
{
  switch (type)
  {
  case DAMAGE_TYPE_FRONT:
    Serial.println("Front Hit Detected!");
    ble_->writeUART("!F");
    break;
  case DAMAGE_TYPE_BACK:
    Serial.println("Back Hit Detected!");
    ble_->writeUART("!B");
    break;
  default:
    Serial.println("damageDetected() case hit default..");
    break;
  }

  led_->setStatus(LED_STATUS_HIT_DETECTED);
}

/*
   Update local health. Central calculates new health value after
   a hit notification is received, then sends it back to the bot
*/
void BattleBot::updateHealth(int health)
{
  health_ = health;
}

/*
   Set LED status based on current health and call led update function
*/
void BattleBot::updateLED()
{
  // https://www.arduino.cc/reference/en/language/variables/variable-scope--qualifiers/static/
  static int lastHealth = 0;

  if (health_ != lastHealth)
  {
    if (health_ > 40 && lastHealth <= 40)
    {
      led_->setStatus(LED_STATUS_OK);
    }
    else if (lastHealth > 40 && health_ <= 40)
    {
      led_->setStatus(LED_STATUS_WARNING);
    }
    else if (lastHealth > 20 && health_ <= 20)
    {
      Serial.println("in danger");
      led_->setStatus(LED_STATUS_DANGER);
    }
  }

  lastHealth = health_;

  led_->update();
}

/*
   handle any inputs/commands received from central
*/
void BattleBot::handleInputs()
{
  if (isConnected_)
  {
    // disconnected suddenly
    if (!ble_->isConnected())
    {
      Serial.println("BattleBot::handleInputs() disconnected!!");
      onDisconnect();
    }

    if (ble_->available())
    {
      // Serial.print("BLE Data available! -> ");
      uint8_t *buff = ble_->getBuffer();
      // Serial.println((char*)buff);
      parseCommand(buff);
      ble_->clearBuffer();
    }
  }
}

/*
   Send an acknowledgement and update LED when a Test message is received
*/
void BattleBot::sendTestMessageAck()
{
  led_->setStatus(LED_STATUS_TEST_ACK);
  // for dramatic effect
  delay(1000);
  ble_->writeUART("!A");
}

/*
   Parse a command received from central and perform the appropriate action
*/
void BattleBot::parseCommand(uint8_t *buf)
{
  Serial.println("parseCommand buf is -> ");
  //  Serial.println(*buf);
  // min msg length is 4 bytes, so can only have up to 5 in single packet
  int msgStartIndices[5] = {0, 0, 0, 0, 0};
  int msgEndIndices[5] = {0, 0, 0, 0, 0};
  int msgStart = -1;
  int msgEnd = -1;
  int msgStartsFound = 0, msgEndsFound = 0;

  for (int i = 0; i < 20; i++)
  {
    Serial.println(buf[i]);
    if (buf[i] == 33)
    {
      msgStartIndices[msgStartsFound] = i;
      msgStartsFound++;
    }
    if (buf[i] == 126)
    {
      msgEndIndices[msgEndsFound] = i;
      msgEndsFound++;
    }
  }

  Serial.println("-----------");
  if (msgStartsFound != msgEndsFound)
  {
    Serial.println("found partial message at trailing end of buffer, only processing full messages");
    // don't iterate over last incomplete message
    msgStartsFound--;
  }

  for (int i = 0; i < msgStartsFound; i++)
  {
    int msg, val;
    int msgLength = msgEndIndices[i] - msgStartIndices[i];
    uint8_t msgBuffer[msgLength];
    for (int j = 0; j <= msgLength; j++)
    {
      msgBuffer[j] = buf[msgStartIndices[i] + j];
      Serial.print("adding to msgBuffer -> ");
      Serial.println((int)msgBuffer[j]);
    }
    msg = msgBuffer[1];
    Serial.print("parseCommand msg is -> ");
    Serial.println((char)msg);
    val = msgBuffer[2];
    Serial.print("parseCommand val is -> ");
    Serial.println((int)val);

    switch (msg)
    {
    // drive
    case 'D':
      int leftMagnitude, rightMagnitude, leftCmd, rightCmd;
      bool leftReversed, rightReversed;

      leftReversed = buf[2];
      leftMagnitude = buf[3];
      rightReversed = buf[4];
      rightMagnitude = buf[5];

      leftCmd = (leftReversed ? (-1) : 1) * leftMagnitude;
      rightCmd = (rightReversed ? (-1) : 1) * rightMagnitude;

      drive(leftCmd, rightCmd);
      break;
    case 'F':
      forward(val);
      break;
    case 'B':
      backward(val);
      break;
    case 'L':
      left(val);
      break;
    case 'R':
      right(val);
      break;
    // weapon commands
    case 'W':
      if (!val || (char)val == '~')
      {
        Serial.println("actuating weapon!");
        weapon_->actuate();
      }
      else
      {
        Serial.println("writing value to weapon!");
        weapon_->writeValue(val);
      }
      break;
    // pause weapon updates
    case 'P':
      (bool)val ? weapon_->pause() : weapon_->resume();
      break;
    // health update
    case 'H':
      updateHealth(val);
      break;
    // config command
    case 'C':
      Serial.println("config command received");
      setConfig(msgBuffer);
      break;
    // test message
    case 'T':
      sendTestMessageAck();
      break;
    // stop command
    case 'S':
    default:
      stop();
      break;
    }
  }
};
