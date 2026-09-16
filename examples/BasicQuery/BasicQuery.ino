/*
  BasicQuery - read the turbo pump's rotation speed over RS-485.

  Wiring (an Arduino Mega, 'Serial1' UART port; adjust the pins and port for your board):
  TC80 RS-485 is on the DB15 connector; check your unit's manual for the pinout, Only GND, RS-485 A and RS-485 B pins are required.

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

  The TC80 defaults to 9600 baud, 8N1, bus address 1.

  This sketch uses the library's blocking default delay, so it stalls while
  waiting for a reply. See the NonBlockingLoop example for the alternative.
*/

#include <PfeifferTurboTC80.h>

#define PIN_RS485_DE 23
#define PIN_RS485_RE 22
#define PUMP_ADDRESS 1

// The template argument is the receive buffer size in bytes. 64 holds two full
// telegrams, which is plenty when task() is called regularly.
RS485HardwareSerial<64> rs485(Serial1, PIN_RS485_DE, PIN_RS485_RE);
PfeifferSerialTC80 pump(rs485, PUMP_ADDRESS);

void setup()
{
    Serial.begin(115200);
    while (!Serial)
    {
        ; // wait for the USB serial monitor on boards that need it
    }

    pump.begin(9600, SERIAL_8N1);
    Serial.println(F("Querying TC80..."));
}

void loop()
{
    bool ok = false;

    // Every read is a query followed by the receive for that parameter's
    // datatype. ActualSpdRpm is a u_integer, so receiveUInteger() matches.
    pump.sendQuery(PfeifferVacProtocol::StatusRequest::ActualSpdRpm);
    unsigned long rpm = pump.receiveUInteger(
        PfeifferVacProtocol::StatusRequest::ActualSpdRpm, ok, false, 1000);

    if (ok)
    {
        Serial.print(F("Rotation speed: "));
        Serial.print(rpm);
        Serial.println(F(" rpm"));
    }
    else
    {
        Serial.println(F("No valid reply - check wiring, baud rate and address."));
    }

    delay(1000);
}
