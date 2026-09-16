#ifndef PFEIFFER_VAC_PROTOCOL_INCLUDE_DEBUG
#define PFEIFFER_VAC_PROTOCOL_INCLUDE_DEBUG 1 // Enable debug features by default (uses more rom memory)
#endif

#pragma once

/**
 * @file PfeifferVacProtocolConsts.h
 * @brief Protocol enumerations, parameter numbers, and debug lookup tables.
 *
 * PFEIFFER_VAC_PROTOCOL_INCLUDE_DEBUG is a compile-time size knob. At 1 (the
 * default) the human-readable parameter and error-code tables are built into
 * flash, costing roughly 40 KB, and telegrams can be logged with names and
 * decoded values. Define it as 0 to drop all of that; the protocol itself is
 * unaffected and the debug entry points below simply stop existing.
 *
 * It must be defined before this header is first included - put it in
 * build_flags, not in a sketch header, or include order decides whether it
 * takes effect.
 *
 * Do not confuse it with the runtime verbosity checked by
 * PFTC80_LOG_LEVEL_AT_LEAST() in PfeifferLog.h. This one decides whether the
 * tables exist at all; that one decides whether to render a message now.
 */

#include <Arduino.h>
#include "PfeifferVacProtocolDatatypes.h"

namespace PfeifferVacProtocol
{
    enum class TelegramError
    {
        None,
        InvalidParameter, // sent as "NO_DEF", Parameter no longer used or invalid
        OutOfRange,       // sent as "_RANGE", Parameter value out of range
        LogicError,       // sent as "_LOGIC", Logical access error
        InvalidChecksum,  // Checksum does not match the calculated checksum of the telegram
    };

    enum class Action
    {
        Query = '0',  // Query action
        Command = '1' // Command action
    };

    // ---- Function Enums ----

    // For Heating, Stand-by, etc.: On/Off state
    enum class FuncOnOff
    {
        Off = 0, // Off (0)
        On = 1   // On (1)
    };

    // For EnableVent, Yes/No type
    enum class FuncYesNo
    {
        No = 0, // No (0)
        Yes = 1 // Yes (1)
    };

    // For ErrorAckn: Malfunction acknowledgement
    enum class FuncMalfunctionAckn
    {
        // Only value 1 is valid
        Acknowledge = 1 // Malfunction acknowledgement (1)
    };

    // For PumpgStatn: Pumping station state
    enum class FuncPumpingStation
    {
        Off = 0,      // Off (0)
        OnAndAckn = 1 // On and malfunction acknowledgement (1)
    };

    // For CfgSpdSwPt: Rotation speed switchpoint configuration
    enum class FuncRotationSwitchpointCfg
    {
        Switchpoint1 = 0,     // Rotation speed switchpoint 1 (0)
        Switchpoints1And2 = 1 // Rotation speed switchpoints 1 & 2 (1)
    };

    // For CfgDO2, CfgDO1: Output DO2/DO1 configuration
    enum class FuncOutputDO2Cfg
    {
        RotationSpeedSwitchpointReached = 0, // Rotation speed switchpoint reached (0)
        NoError = 1,                         // No error (1)
        Error = 2,                           // Error (2)
        Warning = 3,                         // Warning (3)
        ErrorAndOrWarning = 4,               // Error and/or warning (4)
        SetRotationSpeedReached = 5,         // Set rotation speed reached (5)
        PumpOn = 6,                          // Pump on (6)
        PumpAccelerating = 7,                // Pump accelerating (7)
        PumpDecelerating = 8,                // Pump decelerating (8)
        Always0 = 9,                         // Always "0" (9)
        Always1 = 10,                        // Always "1" (10)
        RemotePriorityActive = 11,           // Remote priority active (11)
        Heating = 12,                        // Heating (12)
        BackingPump = 13,                    // Backing pump (13)
        SealingGas = 14,                     // Sealing gas (14)
        PumpingStation = 15,                 // Pumping station (15)
        PumpRotating = 16,                   // Pump rotating (16)
        PumpHasStopped = 17,                 // Pump has stopped (17)
        PressureSwitchPoint1NotReached = 19, // Pressure switch point 1 not reached (19)
        PressureSwitchPoint2NotReached = 20, // Pressure switch point 2 not reached (20)
        ForeVacuumValveDelayed = 21,         // Fore-vacuum valve, delayed (21)
        BackingPumpStandby = 22              // Backing pump standby (22)
    };
    typedef FuncOutputDO2Cfg FuncOutputDO1Cfg; // Same as OutputDO2Cfg

    // For OpMode BKP: Backing pump operating mode
    enum class FuncBackingPumpOpMode
    {
        Continuous = 0,     // Continuous operation (0)
        Intermittent = 1,   // Intermittent operation (1)
        DelayedOn = 2,      // Delayed switching on (2)
        DelayedInterval = 3 // Delayed interval operation (3)
    };

    // For GasMode: Gas mode
    enum class FuncGasMode
    {
        HeavyGases = 0, // Heavy gases (0)
        LightGases = 1, // Light gases (1)
        Helium = 2      // Helium (2)
    };

    // For VentMode: Venting mode
    enum class FuncVentMode
    {
        DelayedVenting = 0, // Delayed venting (0)
        NoVenting = 1,      // No venting (1)
        DirectVenting = 2   // Direct venting (2)
    };

    // For CfgAccA1, CfgAccB1, CfgAccC1, CfgAccD1: Accessory connection configuration
    enum class FuncAccA1Cfg
    {
        FanContinuous = 0,            // Fan (continuous operation) (0)
        VentValveClosedNoCurrent = 1, // Venting valve, closed without current (1)
        Heating = 2,                  // Heating (2)
        BackingPump = 3,              // Backing pump (3)
        FanTempControlled = 4,        // Fan (temperature controlled) (4)
        SealingGas = 5,               // Sealing gas (5)
        Always0 = 6,                  // Always "0" (6)
        Always1 = 7,                  // Always "1" (7)
        PowerFailureVentingUnit = 8,  // Power failure venting unit (8)
        SecondVentValve = 12,         // Second venting valve (12)
        NoFunction = 13               // No function (13)
    };
    typedef FuncAccA1Cfg FuncAccB1Cfg; // Same as AccA1Cfg: Accessory connection configuration
    typedef FuncAccA1Cfg FuncAccC1Cfg; // Same as AccA1Cfg: Accessory connection configuration
    typedef FuncAccA1Cfg FuncAccD1Cfg; // Same as AccA1Cfg: Accessory connection configuration

    // For Press1HVen: Release HV sensor integrated
    enum class FuncPress1HVen
    {
        Off = 0,                           // Off (0)
        On = 1,                            // On (1)
        OnWithRotSpdSwitchpoint = 2,       // On, with rotation speed switch point reached (2)
        OnWithPressureSwitchNotReached = 3 // On, with pressure switch point not reached (3)
    };

    // For CfgAO1: Output AO1 configuration
    enum class FuncOutputAO1Cfg
    {
        ActualRotationSpeed = 0, // Actual rotation speed (0)
        Output = 1,              // Output (1)
        Current = 2,             // Current (2)
        Always0V = 3,            // Always 0V (3)
        Always10V = 4,           // Always 10V (4)
        PressureValue1 = 6,      // Pressure value 1 (6)
        PressureValue2 = 7,      // Pressure value 2 (7)
        ForeVacuumControl = 8    // Fore-vacuum control (8)
    };

    // For TmpMgtMode: Temperature management configuration
    enum class FuncTmpMgtMode
    {
        PumpTempLEQ60C = 0,     // Pump housing temperature ≤60C (0)
        PumpTempLEQ80C = 1,     // Pump housing temperature ≤80C (1)
        PowerCharacteristic = 2 // Power characteristic [P:027] (2)
    };

    // For CtrlVialnt: Operate via interface
    enum class FuncCtrlVialnt
    {
        Remote = 1,                    // Remote (1)
        RS485 = 2,                     // RS-485 (2)
        PVCAN = 4,                     // PV.can (4)
        UnlockInterfaceSelection = 255 // Unlock interface selection (255)
    };

    // For CfgDI1, CfgD12: Input DI1/DI2 configuration
    enum class FuncDI1Cfg
    {
        Deactivated = 0,              // Deactivated (0)
        EnableVenting = 1,            // Enable venting (1)
        Heating = 2,                  // Heating (2)
        SealingGas = 3,               // Sealing gas (3)
        RunUpTimeMonitoring = 4,      // Run-up time monitoring (4)
        RotationSpeedSettingMode = 5, // Rotation speed setting mode (5)
        Motor = 6,                    // Motor (6)
        EnableHVSensor1 = 7           // Enable HV sensor 1 (7)
    };
    typedef FuncDI1Cfg FuncDI2Cfg; // Same as DI1Cfg

    // Control command constants from table 6.2: Control Commands in manual - Each const is commented for intellisense
    namespace ControlCommand
    {
        constexpr int Heating = 1;     // Heating: 0=off 1=on | boolean_old | RW | min=0 max=1 default=0
        constexpr int StandBy = 2;     // Stand-by: 0=off 1=on | boolean_old | RW | min=0 max=1 default=0
        constexpr int RUTimeCtrl = 4;  // Run-up time monitoring: 0=off 1=on | boolean_old | RW | min=0 max=1 default=1
        constexpr int ErrorAckn = 9;   // Malfunction acknowledgement: 1=acknowledge | boolean_old | W | min=1 max=1
        constexpr int PumpgStatn = 10; // Pumping station: 0=off 1=on and malfunction acknowledgment | boolean_old | RW | min=0 max=1 default=0
        constexpr int EnableVent = 12; // Enable venting: 0=no 1=yes | boolean_old | RW | min=0 max=1 default=0
        constexpr int CfgSpdSwPt = 17; // Rotation speed switchpoint configuration: 0=Switchpoint1 1=Switchpoints1And2 | u_short_int | RW | min=0 max=1 default=0
        constexpr int CfgDO2 = 19;     // Output DO2 configuration: see OutputDO2Cfg | u_short_int | RW | min=0 max=22 default=1
        constexpr int MotorPump = 23;  // Motor pump: 0=off 1=on | boolean_old | RW | min=0 max=1 default=1
        constexpr int CfgDO1 = 24;     // Output DO1 configuration: see OutputDO1Cfg | u_short_int | RW | min=0 max=22 default=0
        constexpr int OpModeBKP = 25;  // Backing pump operating mode: see BackingPumpOpMode | u_short_int | RW | min=0 max=3 default=0
        constexpr int SpdSetMode = 26; // Rotation speed setting mode: 0=off 1=on | u_short_int | RW | min=0 max=1 default=0
        constexpr int GasMode = 27;    // Gas mode: see GasMode | u_short_int | RW | min=0 max=2 default=0
        constexpr int VentMode = 30;   // Venting mode: see VentMode | u_short_int | RW | min=0 max=2 default=2
        constexpr int CfgAccA1 = 35;   // Configuration accessory connection A1: see AccA1Cfg | u_short_int | RW | min=0 max=13 default=0
        constexpr int CfgAccB1 = 36;   // Configuration accessory connection B1: see AccB1Cfg | u_short_int | RW | min=0 max=13 default=1
        constexpr int Press1HVen = 41; // Release HV sensor integrated: see Press1HVen | u_short_int | RW | min=0 max=3 default=2
        constexpr int SealingGas = 50; // Sealing gas: 0=off 1=on | boolean_old | RW | min=0 max=1 default=0
        constexpr int CfgAO1 = 55;     // Output AO1 configuration: see OutputAO1Cfg | u_short_int | RW | min=0 max=8 default=0
        constexpr int TmpMgtMode = 58; // Temperature management configuration: see TmpMgtMode | u_short_int | RW | min=0 max=2 default=0
        constexpr int CtrlVialnt = 60; // Operate via interface: see CtrlVialnt | u_short_int | RW | min=1 max=255 default=1
        constexpr int IntSelLckd = 61; // Interface selection locked: 0=off 1=on | boolean_old | RW | min=0 max=1 default=0
        constexpr int CfgDI1 = 62;     // Input DI1 configuration: see DI1Cfg | u_short_int | RW | min=0 max=7 default=1
        constexpr int CfgD12 = 63;     // Input D12 configuration: see D12Cfg | u_short_int | RW | min=0 max=7 default=2
        constexpr int CfgAccC1 = 68;   // Configuration accessory connection C1: see AccC1Cfg | u_short_int | RW | min=0 max=13 default=0
        constexpr int CfgAccD1 = 69;   // Configuration accessory connection D1: see AccD1Cfg | u_short_int | RW | min=0 max=13 default=0
    } // namespace ControlCommand

    // Status request constants from table 6.3: Status Requests in manual - Each const is commented for intellisense
    namespace StatusRequest
    {
        constexpr int RemotePrio = 300;    // Remote priority: 0=no 1=yes | boolean_old | R | min=0 max=1
        constexpr int SpdSwPtAtt = 302;    // Rotation speed switchpoint reached: 0=no 1=yes | boolean_old | R | min=0 max=1
        constexpr int ErrorCode = 303;     // Error code | string6 | R
        constexpr int OvTempElec = 304;    // Excess temperature drive electronics: 0=no 1=yes | boolean_old | R | min=0 max=1
        constexpr int OvTempPump = 305;    // Excess temperature pump: 0=no 1=yes | boolean_old | R | min=0 max=1
        constexpr int SetSpdAtt = 306;     // Set rotation speed reached: 0=no 1=yes | boolean_old | R | min=0 max=1
        constexpr int PumpAccel = 307;     // Pump accelerating: 0=no 1=yes | boolean_old | R | min=0 max=1
        constexpr int SetRotSpdHz = 308;   // Set rotation speed (Hz) | u_integer | R | Hz | min=0 max=999999
        constexpr int ActualSpdHz = 309;   // Actual rotational speed (Hz) | u_integer | R | Hz | min=0 max=999999
        constexpr int DrvCurrent = 310;    // Drive current | u_real | R | A | min=0 max=9999.99
        constexpr int OpHrsPump = 311;     // Pump operating hours | u_integer | R | h | min=0 max=65535
        constexpr int FwVersion = 312;     // Drive electronics software version | string6 | R
        constexpr int DrvVoltage = 313;    // Drive voltage | u_real | R | V | min=0 max=9999.99
        constexpr int OpHrsElec = 314;     // Drive electronics operating hours | u_integer | R | h | min=0 max=65535
        constexpr int NominalSpdHz = 315;  // Nominal rotation speed (Hz) | u_integer | R | Hz | min=0 max=999999
        constexpr int DrvPower = 316;      // Drive power | u_integer | R | W | min=0 max=999999
        constexpr int PumpCycles = 319;    // Pump cycles | u_integer | R | min=0 max=65535
        constexpr int TmpPwrStg = 324;     // Temperature power stage | u_integer | R | °C | min=0 max=999999
        constexpr int TempElec = 326;      // Electronics temperature | u_integer | R | °C | min=0 max=999999
        constexpr int TempPmpBot = 330;    // Pump lower part temperature | u_integer | R | °C | min=0 max=999999
        constexpr int AccelDecel = 336;    // Acceleration/deceleration | u_integer | R | rpm/s | min=0 max=999999
        constexpr int ElecName = 349;      // Electronic drive unit designation | string6 | R
        constexpr int HWVersion = 354;     // Hardware version drive electronics | string6 | R
        constexpr int SerialNo = 355;      // Serial number | string16 | R
        constexpr int ErrHist1 = 360;      // Error code history, item 1 | string6 | R
        constexpr int ErrHist2 = 361;      // Error code history, item 2 | string6 | R
        constexpr int ErrHist3 = 362;      // Error code history, item 3 | string6 | R
        constexpr int ErrHist4 = 363;      // Error code history, item 4 | string6 | R
        constexpr int ErrHist5 = 364;      // Error code history, item 5 | string6 | R
        constexpr int ErrHist6 = 365;      // Error code history, item 6 | string6 | R
        constexpr int ErrHist7 = 366;      // Error code history, item 7 | string6 | R
        constexpr int ErrHist8 = 367;      // Error code history, item 8 | string6 | R
        constexpr int ErrHist9 = 368;      // Error code history, item 9 | string6 | R
        constexpr int ErrHist10 = 369;     // Error code history, item 10 | string6 | R
        constexpr int TempRotor = 384;     // Rotor temperature | u_integer | R | °C | min=0 max=999999
        constexpr int OrderCode = 388;     // Order number | string16 | R
        constexpr int AddID = 396;         // Pump ID | u_integer | R
        constexpr int SetRotSpdRpm = 397;  // Set rotation speed (rpm) | u_integer | R | rpm | min=0 max=999999
        constexpr int ActualSpdRpm = 398;  // Actual rotational speed (rpm) | u_integer | R | rpm | min=0 max=999999
        constexpr int NominalSpdRpm = 399; // Nominal rotation speed (rpm) | u_integer | R | rpm | min=0 max=999999
    } // namespace StatusRequest

    // Reference value input constants from table 6.4: Reference Value Inputs in manual - Each const is commented for intellisense
    namespace ReferenceValueInput
    {
        constexpr int RUTimeSVal = 700; // Set value run-up time | u_integer | RW | min=1 max=120 default=8
        constexpr int SpdSwPt1 = 701;   // Rotation speed switch point 1 | u_integer | RW | % | min=50 max=97 default=80
        constexpr int SpdSVal = 707;    // Set value in rotation speed setting mode | u_real | RW | % | min=20 max=100 default=65
        constexpr int PwrSVal = 708;    // Set value power consumption | u_short_int | RW | % | min=10 max=100 default=100
        constexpr int SwoffBKP = 710;   // Backing pump switch-off threshold for intermittent operation | u_integer | RW | W | min=0 max=1000 default=0
        constexpr int SwOnBKP = 711;    // Backing pump switch-on threshold for intermittent operation | u_integer | RW | W | min=0 max=1000 default=0
        constexpr int StdbySVal = 717;  // Rotation speed set value in stand-by operation | u_real | RW | % | min=20 max=100 default=66.7
        constexpr int SpdSwPt2 = 719;   // Rotation speed switch point 2 | u_integer | RW | % | min=5 max=97 default=20
        constexpr int VentSpd = 720;    // Venting at rotation speed, delayed venting | u_short_int | RW | % | min=40 max=98 default=50
        constexpr int VentTime = 721;   // Venting time, delayed venting | u_integer | RW | s | min=6 max=3600 default=3600
        constexpr int mxPwrOutTm = 726; // Max. time for the output voltage in power back-up mode | u_integer | RW | s | min=1 max=255 default=10
        constexpr int fanOnTemp = 728;  // Start-up temperature of the fan in temperature-controlled mode | u_integer | RW | °C | min=6 max=75 default=45
        constexpr int PrsSwPt1 = 730;   // Pressure switch point 1 | u_expo_new | RW | hPa | default=1000
        constexpr int PrsSwPt2 = 732;   // Pressure switch point 2 | u_expo_new | RW | hPa | default=1000
        constexpr int PwrOutVolt = 733; // Output voltage in power back-up mode | u_real | RW | V | min=20.50 max=26.50 default=23.00
        constexpr int PwrOutThrs = 734; // Power threshold from which the voltage is output from P733 | u_integer | RW | W | min=15 max=150 default=20
        constexpr int PrsSn1Name = 739; // Name sensor 1 | string6 | R
        constexpr int Pressure1 = 740;  // Pressure value 1 | u_expo_new | RW | hPa
        constexpr int PrsCorrPi1 = 742; // Correction factor 1 | u_real | RW | min=0.1 max=8.0 default=0
        constexpr int PrsSn2Name = 749; // Name sensor 2 | string6 | R
        constexpr int Pressure2 = 750;  // Pressure value 2 | u_expo_new | RW | hPa
        constexpr int PrsCorrPi2 = 752; // Correction factor 2 | u_real | RW | min=0.1 max=8.0 default=0
        constexpr int NomSpdConf = 777; // Confirmation of nominal rotation speed | u_integer | RW | Hz | min=0 max=1500 default=0
        constexpr int RS485Adr = 797;   // RS-485 Interface address | u_integer | RW | min=1 max=255 default=1
    } // namespace ReferenceValueInput

    // Additional info constants from table 6.5: Additional Parameters for Control Unit in manual - Each const is commented for intellisense
    namespace AdditionalParams
    {
        constexpr int Pressure = 340;    // Actual pressure value (ActiveLine) | u_short_int | R | hPa | min=1e-10 max=1e3
        constexpr int CtrName = 350;     // Control unit type | string6 | R
        constexpr int CtrSoftware = 351; // Control unit software version | string6 | R
        constexpr int GaugeType = 738;   // Type of pressure gauge | string6 | RW
        constexpr int ParamSet = 794;    // Parameter set: 0=Basic parameter set 1=Extended parameter set | u_short_int | RW | min=0 max=1 default=0
        constexpr int Servicelin = 795;  // Insert service line | u_short_int | RW | default=795
    } // namespace AdditionalParams

#if PFEIFFER_VAC_PROTOCOL_INCLUDE_DEBUG

    // ---- Debug helper functions ----
    //
    // Defined in PfeifferVacProtocolConsts.cpp. Keeping the tables out of this
    // header stops them being duplicated into every translation unit that
    // includes it.

    /**
     * @brief Look an index up in an array of flash strings.
     * @return The name, or "Unknown (<func_name>:<idx>)" when out of range or empty.
     */
    String func_unknown_if(const __FlashStringHelper *const names[], size_t size, uint8_t idx, const char *func_name);

    // Enum-to-text lookups, one per Func* enum. Signature matches
    // LookupFuncFunction so they can be stored in ParameterDebugMap.
    const String lookupFuncOnOff(uint8_t idx);
    const String lookupFuncYesNo(uint8_t idx);
    const String lookupFuncMalfunctionAckn(uint8_t idx);
    const String lookupFuncPumpingStation(uint8_t idx);
    const String lookupFuncRotationSwitchpointCfg(uint8_t idx);
    const String lookupFuncOutputDO2Cfg(uint8_t idx);
    const String lookupFuncBackingPumpOpMode(uint8_t idx);
    const String lookupFuncGasMode(uint8_t idx);
    const String lookupFuncVentMode(uint8_t idx);
    const String lookupFuncAccA1Cfg(uint8_t idx);
    const String lookupFuncPress1HVen(uint8_t idx);
    const String lookupFuncOutputAO1Cfg(uint8_t idx);
    const String lookupFuncTmpMgtMode(uint8_t idx);
    const String lookupFuncCtrlVialnt(uint8_t idx);
    const String lookupFuncDI1Cfg(uint8_t idx);

    // Stores details for parameters about datatype, description, and functions for debugging/lookup
    typedef const String (*LookupFuncFunction)(uint8_t idx);
    struct ParameterDebugEntry
    {
        int number;
        const __FlashStringHelper *name; // Short name
        const __FlashStringHelper *description;
        const __FlashStringHelper *details; // Optional details such as function values or unit/min/max/default, can be nullptr
        DataType datatype;
        LookupFuncFunction lookupFuncFunction; // Pointer to function that takes uint8_t and returns String
    };

    /**
     * @brief Look up the debug entry for a parameter number.
     * @param number Parameter number, e.g. 309 for ActualSpdRpm.
     * @param entry  Filled in from flash on success. Untouched on failure.
     * @return true when the parameter is known.
     */
    bool getParameterDebugEntry(int number, ParameterDebugEntry *entry);

    /**
     * @brief Translate a pump error/warning code into readable text.
     * @param errorCode 6-character code as received, e.g. "Err001". Case-insensitive.
     * @return Flash-resident description, or nullptr when the code is unknown.
     *         Wrap in String() to copy it into RAM.
     */
    const __FlashStringHelper *getHumanReadableErrorMessage(const char *errorCode);

#endif // PFEIFFER_VAC_PROTOCOL_INCLUDE_DEBUG

} // namespace PfeifferVacProtocol
