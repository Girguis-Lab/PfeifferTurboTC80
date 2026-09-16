#pragma once

#include <PfeifferTurboTC80.h>

/// Implemented in PumpStatus.cpp - a second translation unit that also
/// includes the library, which is what this example is here to prove links.
String describePumpSpeed(PfeifferSerialTC80 &pump);
