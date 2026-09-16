/*
  NonBlockingLoop - keep the sketch responsive while the pump is being polled.

  By default the driver waits between receive retries with Arduino delay(),
  which stalls everything else. Passing your own DelayFunc lets the sketch keep
  working during those waits. The callback must do two things:

    1. Service whatever needs servicing - at minimum rs485.task(), which moves
       incoming bytes out of the UART and turns the bus around.
    2. Return true to abandon the operation in progress, false to continue.

  Returning true is how you let an operator command pre-empt a long pump
  sequence. It costs you the reply currently in flight, so only do it when you
  genuinely want to stop.

  Wiring is the same as the BasicQuery example.
*/

#include <PfeifferTurboTC80.h>

#define PIN_RS485_DE 23
#define PIN_RS485_RE 22
#define PUMP_ADDRESS 1

RS485HardwareSerial<64> rs485(Serial1, PIN_RS485_DE, PIN_RS485_RE);

bool cooperativeDelay(unsigned long ms);

// Pass the callback as the last constructor argument.
PfeifferSerialTC80 pump(rs485, PUMP_ADDRESS, cooperativeDelay);

bool abortRequested = false;

/*
  Called by the driver instead of delay(). Spins for `ms` milliseconds while
  keeping the transport and the rest of the sketch alive.
*/
bool cooperativeDelay(unsigned long ms)
{
    const unsigned long start = millis();
    while (millis() - start < ms)
    {
        rs485.task(); // required: drains the UART into the RX buffer

        // Anything else that must keep running goes here. Typing 'x' into the
        // serial monitor aborts whatever the pump driver is waiting for.
        if (Serial.available() && Serial.read() == 'x')
        {
            abortRequested = true;
            return true; // abandon the pump operation in progress
        }
    }
    return false; // keep waiting
}

void setup()
{
    Serial.begin(115200);
    pump.begin(9600, SERIAL_8N1);
    Serial.println(F("Polling TC80. Send 'x' to abort a poll in progress."));
}

void loop()
{
    // Call task() from loop() as well, so bytes arriving between polls are not
    // left sitting in the UART.
    rs485.task();

    bool ok = false;
    pump.sendQuery(PfeifferVacProtocol::StatusRequest::TempElec);
    unsigned long tempC = pump.receiveUInteger(
        PfeifferVacProtocol::StatusRequest::TempElec, ok, false, 1000);

    if (abortRequested)
    {
        Serial.println(F("Poll aborted by operator."));
        abortRequested = false;
    }
    else if (ok)
    {
        Serial.print(F("Electronics temperature: "));
        Serial.print(tempC);
        Serial.println(F(" C"));
    }
    else
    {
        Serial.println(F("No valid reply."));
    }

    // Dropped bytes corrupt telegrams. A climbing count means task() is not
    // being called often enough, or the RX buffer is too small.
    if (rs485.rxOverflowCount() > 0)
    {
        Serial.print(F("RX overflows: "));
        Serial.println(rs485.rxOverflowCount());
    }

    cooperativeDelay(1000);
}
