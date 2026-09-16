# Changelog

All notable changes to this project are documented here. This project adheres
to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.0.1] - 2026-09-15

First release as a standalone library. Previously this lived inside the ISMS
project, where it was only ever included from a single translation unit — which
hid a class of bugs that would break any other consumer.

### Breaking changes

- **`RS485HardwareSerial` is now a class template**, with the receive buffer
  size as its parameter: `RS485HardwareSerial<64>`. The buffer was previously
  hard-coded at 600 bytes and could not be changed. The default of 64 holds two
  full telegrams; existing code must add the angle brackets.
- **`RS485Serial_Base` is now abstract and derives from `Print`.** All of its
  methods are pure virtual and it has a virtual destructor. Subclasses must
  implement every method; in exchange, `print()`/`println()` now work
  polymorphically through a base reference.
- **`getHumanReadableErrorMessage()` now takes a `const char*` and returns a
  `const __FlashStringHelper*`** instead of taking a `String` by value and
  returning a `const ErrorCodeDebugEntry*`. The table it searches moved into
  PROGMEM, so a pointer into it can no longer be dereferenced by the caller.
  `ErrorCodeDebugEntry` is no longer part of the public API.
- **Logging no longer goes through DebugLog by default.** The library used to
  `#include <DebugLog.h>` unconditionally, forcing that dependency and its
  unprefixed `LOG_*` macros on every consumer. Logging is now silent unless you
  build with `-D PFTC80_USE_DEBUGLOG` or `-D PFTC80_LOG_TO_STREAM`.
- **Renamed for clarity**, since these were public despite the leading
  underscore: `_sendTelegramRaw` → `sendTelegramRaw`, `_sendTelegram` →
  `sendTelegram`, `_receiveTelegramRaw` → `receiveTelegramRaw`,
  `_injestRxBytes` → `ingestRxBytes` (also correcting the spelling).
  `_printInvalidResponseDebug` is now private.
- `defaultDelayFunc` is renamed `pfeifferDefaultDelay`; the old name was too
  generic for a global symbol in a published library.
- The `Stream&` constructor parameter, previously stored and never used, is now
  an optional third argument that installs the logging sink.
- `architectures` is now `avr` rather than `*`. The debug tables use AVR
  pgmspace APIs (`memcpy_P`, `strcasecmp_P`, `PROGMEM`) that are not available
  on every core.

### Fixed

- **Multiple-definition link errors with more than one translation unit.**
  `encodeTelegram`, `decodeTelegram`, `printTelegramHumanReadable`,
  `getParameterDebugEntry`, `getHumanReadableErrorMessage` and the default delay
  function were all defined in headers without `inline`. Implementations moved
  to `.cpp` files. `examples/MultipleTranslationUnits` is the regression test.
- **`PFEIFFER_VAC_PROTOCOL_INCLUDE_DEBUG=0` did not compile.** Three call sites
  used debug-only types and functions outside the `#if` that defines them.
- **Undefined vtable for `RS485Serial_Base`.** Its virtual methods were declared
  with no body and without `= 0`, leaving no key function to anchor the vtable;
  it linked only because the optimizer elided the dead vptr store. It also had
  no virtual destructor.
- **Buffer overflow in the constructor.** `sprintf(_address, "%03d", address)`
  wrote into a 4-byte buffer from a `uint16_t`, overrunning it for any address
  of 1000 or more. Addresses are now validated and the constructor shares the
  bounds-checked `setAddress()` path.
- **`RS485HardwareSerial::read()` returned 255 instead of -1** on an empty
  buffer (`uint8_t readByte = -1`), and that byte was appended to the frame.
- **The bus was released mid-telegram.** The body and its terminating carriage
  return were written as two separate calls, each asserting and dropping DE, so
  the line went idle between the last data byte and the CR.
- **`decodeTelegram()` accepted frames as short as 8 bytes** when the minimum
  valid telegram is 13, letting the checksum slice overlap the parameter field.
  It now also verifies that the declared payload length matches the frame
  length, so a truncated frame cannot pass on a coincidental checksum.
- **Unbounded `String` growth while receiving.** Line noise could grow the
  receive buffer for the entire timeout window. Frames are now capped at the
  maximum telegram length and resynchronised.
- **Parameter comparison truncated to `uint8_t`** in the diagnostic path, which
  made it meaningless for every `StatusRequest` (300-399) and
  `ReferenceValueInput` (700-797) parameter and misreported the failure reason.
- **Two `delay()` calls bypassed the injected `DelayFunc`**, stalling
  cooperative sketches during `queryLatestError()` and `queryErrorHistory()`.
- `encodeTelegram()` now rejects out-of-range parameters and oversized payloads
  instead of silently truncating them into a well-formed but wrong telegram.
- Replaced non-ASCII characters (`!=`, `<=`) in program strings.
- **`u_expo_new` values were a factor of 10 out.** The specification gives
  `100023` = 1.0 × 10³, i.e. the 4-digit mantissa is scaled by 1000; the
  implementation scaled by 100, and `encode()` normalised into `[1, 100)` to
  match. Round-trips through the class therefore looked correct while every
  value exchanged with the pump was wrong — including **p340 (Pressure)**.
  `decode("100023")` now returns `1000.0` rather than `10000.0`. Both worked
  examples from the specification are pinned by unit tests.

### Changed

- **Memory.** A 256-entry pointer array in `lookupFuncCtrlVialnt` (512 bytes of
  SRAM, rebuilt on every call, for four used entries) is now a sparse PROGMEM
  table. The error-code table moved from SRAM to PROGMEM. With the default
  64-byte receive buffer, RAM use drops by roughly 1.3 KB versus 1.0.0.
- The nine `receive*` methods now share one retry loop rather than repeating it,
  and the magic `50` and `/4` constants are named.
- `receiveTelegramRaw()`'s default timeout is 200 ms, up from 20 ms — shorter
  than the ~31 ms a full telegram takes to arrive at 9600 baud.
- Pin 0 is a valid pin on most boards, so `RS485_PIN_UNUSED` (255) replaces 0
  as the "not connected" sentinel for DE and RE.
- `RS485HardwareSerial::rxOverflowCount()` reports bytes dropped because the
  receive buffer was full, making a previously silent corruption diagnosable.

### Added

- `examples/BasicQuery`, `examples/NonBlockingLoop`, and
  `examples/MultipleTranslationUnits`.
- Host-side unit tests for the protocol layer (`pio test -e native`), covering
  telegram encoding, decoding, the frame-validation rules above, and every
  payload datatype.
- `library.json`, `keywords.txt`, a full `README.md`, and a CI workflow running
  the tests and building every example across the supported flag combinations.

## [0.1.0]

- Initial version, developed as part of the ISMS project.
