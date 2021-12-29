#include "Motor_Adafruit.h"
#include "Motor.h"

Motor_Adafruit::Motor_Adafruit(uint8_t addr, MotorPositions pos)
  : motorShield_(Adafruit_MotorShield())
  , motor_(Motor_Adafruit::motorShield_.getMotor(pos))
  // , pos_(pos)
{
  pos_ = pos;
}

void Motor_Adafruit::init(const uint16_t freq)
{
  // Serial.println("initializing Motor_Adafruit");
  motorShield_.begin(freq);
  // setFailsafe();
}

void Motor_Adafruit::setSpeed(const int speed)
{
  // Serial.print("Motor_Adafruit::setSpeed(int8_t speed), motor: ");
  // Serial.print(pos_);
  // Serial.print(", speed: ");
  // Serial.println(speed);
  int clampedSpeed = clampInput(speed);
  motor_->run(speed == 0 ? RELEASE : speed > 0 ? FORWARD : BACKWARD);
  motor_->setSpeed(abs(clampedSpeed));
}
