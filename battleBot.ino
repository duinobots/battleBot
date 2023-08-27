#include "BattleBot.h"
#include <Adafruit_LittleFS.h>
#include <InternalFileSystem.h>
// Change to TINY_CIRCUITS or ADAFRUIT according to your bot version
BattleBot bot = BattleBot(ADAFRUIT);

void setup()
{
  Serial.begin(115200);

#if CFG_DEBUG
  // Blocking wait for connection when debug mode is enabled via IDE
  while ( !Serial ) yield();
#endif
  
  Serial.println("Bluefruit52 BLEUART Example");
  Serial.println("---------------------------\n");
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
