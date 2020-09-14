#include "BattleBot.h"

// Change to TINY_CIRCUITS or ADAFRUIT according to your bot version
BattleBot bot = BattleBot(ADAFRUIT);

void setup()
{
  bot.init();
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



