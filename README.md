# PfeifferTurboTC80

Arduino library for controlling and monitoring Pfeiffer turbomolecular pumps driven by a **TC80** electronic drive unit, over RS-485.

It implements the Pfeiffer Vacuum Protocol in arduino CPP. Each parameter and datatype have typed accessors. Pump Fault, Error and warning history can be retrived. By default the library includes human-readable logging of telegrams to a serial port or via the DebugLog library - this can be compiled out to save memory.

> This library is not endorsed by, directly affiliated with, maintained,
> authorized, or sponsored by Pfeiffer Vacuum. It was written against the published protocol documentation with the help of AI.

## Requirements

- An AVR Arduino (e.g. UNO, MEGA, MICRO; developed on an ATmega2560).
- A **hardware** serial port. SoftwareSerial is not supported directly, though
  you can subclass `RS485Serial_Base` to drive one.
- An RS-485 transceiver with **DE/RE direction control** (MAX485, MAX3485, SP3485, or similar). Auto-direction modules work too — pass
  `RS485_PIN_UNUSED` for the pins you have not wired.
- [RingBufCPP](https://github.com/wizard97/Embedded_RingBuf_CPP) 1.3 or later.
  The Arduino Library Manager and PlatformIO environments both install this dependency automatically.

## Wiring

TC80 RS-485 is on the DB15 connector; check your unit's manual for the pinout, Only GND, RS-485 A and RS-485 B pins are required.
Using an Arduino Mega and `Serial1`:

```
  Arduino             RS-485 transceiver           TC80
  --------------------------------------------------------------
  TX1 (pin 18)  ----> DI   (Driver/Data Input)
  RX1 (pin 19)  <---- RO   (Receiver Output)
  pin 23        ----> DE   (driver enable, active HIGH)
  pin 22        ----> RE   (receiver enable, active LOW, altrenatively the converter RE pin can be tied directly to DE to toggle between sending and reciving with one signal wire)
  5V            ----> VCC
  GND           ----> GND --------------------->  GND
                      A   <-------------------->  RS-485 A
                      B   <-------------------->  RS-485 B
```

The library drives DE and RE together — both HIGH to transmit, both LOW to
receive — which is what the usual active-HIGH DE / active-LOW RE pair needs. If
your board ties them into one signal, pass that same pin number for both
constructor arguments, or wire only DE and pass `RS485_PIN_UNUSED` for RE.

The pump defaults to **9600 baud, 8N1, bus address 1**.

## Quick start

```cpp
#include <PfeifferTurboTC80.h>

// The template argument is the RX buffer size in bytes.
RS485HardwareSerial<64> rs485(Serial1, /* DE */ 23, /* RE */ 22);
PfeifferSerialTC80 pump(rs485, /* address */ 1);

void setup() {
    Serial.begin(115200);
    pump.begin(9600, SERIAL_8N1);
}

void loop() {
    bool ok = false;

    pump.sendQuery(PfeifferVacProtocol::StatusRequest::ActualSpdRpm);
    unsigned long rpm = pump.receiveUInteger(
        PfeifferVacProtocol::StatusRequest::ActualSpdRpm, ok, false, 1000);

    if (ok) {
        Serial.print(F("Speed: "));
        Serial.println(rpm);
    }
    delay(1000);
}
```

Every exchange is **two steps**: send, then receive with the accessor matching
that parameter's datatype. Always check the `isValid` reference — on failure
the accessor returns a zero or empty value that is indistinguishable from a
genuine reading.

## Choosing the right accessor

Each parameter has a datatype, listed in section 5.2.5 (Data types) of `docs/Pfeiffer-TC-80_Operating-Instructions.pdf` and in the
`details` string of the parameter's debug entry.

| Protocol type | Accessor | Setter argument | Payload |
|---|---|---|---|
| `boolean_old` (0) | `receiveBooleanOld()` | `BooleanOld(bool)` | 6 chars |
| `u_integer` (1) | `receiveUInteger()` | `UInteger(unsigned long)` | 6 chars |
| `u_real` (2) | `receiveUReal()` | `UReal(float)` | 6 chars |
| `string` (4) | `receiveString6()` | `String6(const char*)` | 6 chars |
| `boolean_new` (6) | `receiveBooleanNew()` | `BooleanNew(bool)` | 1 char |
| `u_short_int` (7) | `receiveUShortInt()` | `UShortInt(uint16_t)` | 3 chars |
| `u_expo_new` (10) | `receiveUExpoNew()` | `UExpoNew(float)` | 6 chars |
| `string16` (11) | `receiveString16()` | `String16(const char*)` | 16 chars |
| `string8` (12) | `receiveString8()` | `String8(const char*)` | 8 chars |

Setting a parameter uses the matching wrapper type:

```cpp
using namespace PfeifferVacProtocol;
pump.sendCommand(ControlCommand::MotorPump, BooleanOld(true));
pump.receiveTelegram();                        // consume the acknowledgement
pump.sendCommand(ReferenceValueInput::SpdSVal, UReal(85.0f));
```

Parameter numbers live in four namespaces: `ControlCommand`, `StatusRequest`,
`ReferenceValueInput`, and `AdditionalParams`.

## Staying responsive

By default the driver waits between receive retries with Arduino `delay()`,
which stalls the whole sketch. Pass a `DelayFunc` to keep working during those
waits:

```cpp
bool cooperativeDelay(unsigned long ms) {
    unsigned long start = millis();
    while (millis() - start < ms) {
        rs485.task();               // required: drains the UART
        if (userWantsToAbort()) return true;   // abandon this operation
    }
    return false;                   // keep waiting
}

PfeifferSerialTC80 pump(rs485, 1, cooperativeDelay);
```

Returning `true` aborts the operation in progress, at the cost of the reply
currently in flight. Call `rs485.task()` from `loop()` as well, so bytes
arriving between polls are not left sitting in the UART. See the
`NonBlockingLoop` example.

If replies look truncated, check `rs485.rxOverflowCount()`. Anything above zero
means bytes were dropped because the RX buffer filled — call `task()` more
often, or raise the template argument.

## Logging

The library has no logging by default. Select a backend with a build flag:

| Flag | Behaviour |
|---|---|
| *(none)* | Silent. All log calls compile out. |
| `PFTC80_LOG_TO_STREAM` | Prints to a `Print`/`Stream` you supply. |
| `PFTC80_USE_DEBUGLOG` | Routes through [DebugLog](https://github.com/hideakitai/DebugLog). |

With `PFTC80_LOG_TO_STREAM`, install the sink and pick a verbosity:

```cpp
PfeifferVacProtocol::setLogStream(Serial);
PfeifferVacProtocol::setLogLevel(PFTC80_LOG_LVL_DEBUG);
```

or pass the stream as the third constructor argument:

```cpp
PfeifferSerialTC80 pump(rs485, 1, Serial, cooperativeDelay);
```

Output looks like:

```
-> Turbo Query Rotation speed (Hz) (p309)
<- Turbo Rotation speed (Hz) is 82350
```

## Compile-time options and size

`PFEIFFER_VAC_PROTOCOL_INCLUDE_DEBUG` controls whether the human-readable
parameter names, descriptions, and error-code text are built into flash.
Define it as `0` to drop them. It **must** be a build flag — putting it in a
sketch header only works if that header is parsed before the library's.

Measured with the `BasicQuery` example on an ATmega2560:

| Configuration | Flash | RAM |
|---|---|---|
| No logging | 10,108 B | 519 B |
| `PFTC80_LOG_TO_STREAM`, tables off | 10,552 B | 519 B |
| `PFTC80_LOG_TO_STREAM`, tables on | 24,702 B | 1,067 B |

Note the first two rows: with logging compiled out, nothing references the
tables and the linker discards them regardless, so
`PFEIFFER_VAC_PROTOCOL_INCLUDE_DEBUG=0` only buys you anything when logging is
switched on. It is worth about **14 KB of flash and 548 bytes of RAM** in that
case.

The RX buffer is the other tunable: `RS485HardwareSerial<N>` reserves `N` bytes.
The longest telegram is 30 bytes, so the 64-byte default holds two full frames.

This library uses Arduino `String` throughout. On a 2 KB board like an Uno,
heap fragmentation is a real risk; an ATmega2560 or larger is recommended.

## Development

The protocol layer is pure logic and is tested on the host — no hardware
needed:

```sh
pio test -e native
```

The library itself builds through its examples rather than on its own, since a
library has no `setup()`/`loop()` to link:

```sh
pio ci --lib=. --board=megaADK --project-option="build_flags=-Wall -Wextra" \
    examples/BasicQuery
```

`examples/MultipleTranslationUnits` compiles the library into two translation
units and links them. It exists purely as a regression test — keep it building.

## Documentation

- `docs/Pfeiffer-TC-80_Operating-Instructions.pdf` - Pfeiffer documentation for the TC-80 control module - including protocol documentation.
- `docs/telegramFormat.md` — telegram structure and checksum - extracted from section 5.2 of Pfeiffer-TC-80_Operating-Instructions.pdf
- `docs/ProtocolConsts.md` — datatypes and parameter tables - extracted from sections 5 & 6 of Pfeiffer-TC-80_Operating-Instructions.pdf
- `docs/ProtocolRefs.md` — source manuals - extracted from section 6 of Pfeiffer-TC-80_Operating-Instructions.pdf

## License

MIT. See `LICENSE`.
