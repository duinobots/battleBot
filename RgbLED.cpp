#include "RgbLED.h"
#include "Arduino.h"

RgbLED::RgbLED(uint8_t rPin, uint8_t gPin, uint8_t bPin)
  : rPin_(rPin)
  , gPin_(gPin)
  , bPin_(bPin)
  , lastStatusChange_(0)
  , statusTimeout_(0)
  , isOn_(false)
{
}

void RgbLED::init()
{
  pinMode(rPin_, OUTPUT);
  pinMode(gPin_, OUTPUT);
  pinMode(bPin_, OUTPUT);

  lastStatusChange_ = millis();
}

void RgbLED::setStatus(const LEDStatuses& status)
{
  setStatus(LED_STATUSES[status]);
}

void RgbLED::setStatus(LEDStatus status)
{
  bool statusChanged = false;

  analogWrite(rPin_, status.r);
  analogWrite(gPin_, status.g);
  analogWrite(bPin_, status.b);

  // don't snapshot off status
  if (status == LED_STATUSES[LED_STATUS_OFF])
  {
    if (isOn_)
      statusChanged = true;

    isOn_ = false;
  }
  else
  {
    if (!isOn_ || status != currentStatus_)
    {
      if (status != currentStatus_)
      {
        // Serial.println("updating lastStatus_");
        lastStatus_ = currentStatus_;
        statusTimeout_ = millis();
      }

      statusChanged = true;
    }

    currentStatus_ = status;
    isOn_ = true;
  }

  if (statusChanged)
    lastStatusChange_ = millis();
}

void RgbLED::setColor(uint8_t r, uint8_t g, uint8_t b)
{
  analogWrite(rPin_, r);
  analogWrite(gPin_, g);
  analogWrite(bPin_, b);

  if (r == 0 && b == 0 && g == 0)
  {
    isOn_ = false;
  }
  else
  {
    isOn_ = true;
    currentStatus_ = {r, g, b, 0, 0};
  }

  lastStatusChange_ = millis();
}

bool RgbLED::isOn() const
{
  return isOn_;
}

void RgbLED::turnOff()
{
  setStatus(LED_STATUS_OFF);
}

void RgbLED::update()
{
  if (currentStatus_.timeout != 0 && (millis() - statusTimeout_) > currentStatus_.timeout)
    setStatus(lastStatus_);

  if (currentStatus_.blinkInterval == 0)
    return;

  if ((millis() - lastStatusChange_) > currentStatus_.blinkInterval)
  {
    if (isOn_)
      turnOff();
    else
      setStatus(currentStatus_);
  }
};
