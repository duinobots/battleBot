#include "BattleBot.h"

// Change to TINY_CIRCUITS or ADAFRUIT according to your bot version
BattleBot bot = BattleBot(ADAFRUIT);

void setup()
{
  bot.init();
//  Serial.begin(115200);
//  while (!Serial) delay(10);

  Serial.println("hello....");
}

void loop()
{
  // to the death!
  while (bot.isAlive())
  {
    bot.battle();
  }

  bot.deathRattle();

  bot.waitForReset();
}
