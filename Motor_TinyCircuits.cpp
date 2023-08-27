// #include "Motor_TinyCircuits.h"

// Motor_TinyCircuits::Motor_TinyCircuits(uint8_t addr, motor_positions pos)
//   : motor_(NO_R_REMOVED)
// {
//   // todo: weapon motor for TinyCircuits build not supported yet
//   pos_ = pos;
// }

// void Motor_TinyCircuits::init(uint16_t freq)
// {
//   //The value passed to begin() is the maximum PWM value, which is 16 bit(up to 65535)
//   //This value also determines the output frequency- by default, 8MHz divided by the maxPWM value
//   motor_.begin(freq);

//   // setFailsafe();
//   return;
// }

// void Motor_TinyCircuits::setSpeed(int speed)
// {
//   // todo: figure out scaling
//   // Serial.print("Motor_TinyCircuits::setSpeed() speed -> ");
//   // Serial.print(speed);
//   // Serial.print(", pos_ -> ");
//   // Serial.println(pos_);
//   int k = 4;
//   motor_.setMotor(pos_, (int16_t)speed * k);
//   // Serial.print("Motor_TinyCircuits::setSpeed() scaled speed ->");
//   // Serial.println((int16_t)speed * k);
//   return;
// }

// void Motor_TinyCircuits::setFailSafe(uint16_t ms)
// {
//   // todo
//   // motor_.setFailsafe(ms);
//   return;
// };
