#include "PumpStatus.h"

/*
  The second translation unit. It includes <PfeifferTurboTC80.h> independently
  of the .ino and calls the same free functions, so any symbol in the library
  with accidental external linkage collides at link time.
*/

String describePumpSpeed(PfeifferSerialTC80 &pump)
{
    bool ok = false;

    pump.sendQuery(PfeifferVacProtocol::StatusRequest::ActualSpdRpm);
    unsigned long rpm = pump.receiveUInteger(
        PfeifferVacProtocol::StatusRequest::ActualSpdRpm, ok, false, 500);

    if (!ok)
        return String(F("speed unavailable"));

    // Touch the protocol helpers from this unit too.
    String frame = PfeifferVacProtocol::encodeTelegram(pump.getAddress(), '0', 309, "=?");
    PfeifferVacProtocol::PfeifferTelegram decoded = PfeifferVacProtocol::decodeTelegram(frame.c_str());
    (void)decoded;

    return String(rpm) + String(F(" rpm"));
}
