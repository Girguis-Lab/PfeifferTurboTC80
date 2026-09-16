/*
  MultipleTranslationUnits - regression test for the library's linkage.

  This example exists to be compiled, not to be useful. The sketch and
  PumpStatus.cpp both include <PfeifferTurboTC80.h>, so the library is parsed
  into two translation units that are then linked together.

  That used to fail. The library was header-only and several functions
  (encodeTelegram, decodeTelegram, printTelegramHumanReadable,
  getParameterDebugEntry, getHumanReadableErrorMessage, the default delay) were
  defined in headers without `inline`, giving them external linkage and one
  definition per including .cpp -> "multiple definition of ..." at link time.
  The ~40 KB of debug tables were also duplicated into every unit. The project
  this library grew up in only ever included it from one .cpp, so none of that
  showed up until now.

  If this example links, that class of bug is gone. Keep it in CI.
*/

#include <PfeifferTurboTC80.h>
#include "PumpStatus.h"

#define PIN_RS485_DE 23
#define PIN_RS485_RE 22

RS485HardwareSerial<64> rs485(Serial1, PIN_RS485_DE, PIN_RS485_RE);
PfeifferSerialTC80 pump(rs485, 1);

void setup()
{
    Serial.begin(115200);
    pump.begin();

    // Exercise the free functions from this translation unit. The other unit
    // calls them too, which is the whole point.
    String telegram = PfeifferVacProtocol::encodeTelegram("001", '0', 309, "=?");
    Serial.println(telegram);
}

void loop()
{
    Serial.println(describePumpSpeed(pump));
    delay(1000);
}
