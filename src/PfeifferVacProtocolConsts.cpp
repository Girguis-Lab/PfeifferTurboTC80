#include "PfeifferVacProtocolConsts.h"

/**
 * @file PfeifferVacProtocolConsts.cpp
 * @brief Definitions for the human-readable debug tables.
 *
 * These live in a .cpp rather than the header for two reasons: the functions
 * would otherwise be multiply defined once more than one translation unit
 * includes the header, and the lookup tables (~40 KB of flash) would be
 * duplicated into every one of them.
 *
 * Everything here is compiled out when PFEIFFER_VAC_PROTOCOL_INCLUDE_DEBUG
 * is 0.
 */

#if PFEIFFER_VAC_PROTOCOL_INCLUDE_DEBUG

namespace PfeifferVacProtocol
{

    String func_unknown_if(const __FlashStringHelper *const names[], size_t size, uint8_t idx, const char *func_name)
    {
        if (idx < size && names[idx] != nullptr)
        {
            // Copy from PROGMEM to RAM and return as String
            String result = String((const __FlashStringHelper *)(names[idx]));
            if (result.length() > 0)
            {
                return result;
            }
        }
        char unknown_buf[32];
        snprintf(unknown_buf, sizeof(unknown_buf), "Unknown (%s:%u)", func_name, idx);
        return String(unknown_buf);
    }

    // Lookup functions for each enum type, returning a String (copying from PROGMEM to RAM) or "Unknown" if out of range or empty
    // Note: These functions are only included if PFEIFFER_VAC_PROTOCOL_INCLUDE_DEBUG is set to 1

    const String lookupFuncOnOff(uint8_t idx)
    {
        static const __FlashStringHelper *names[] = {
            F("Off"),
            F("On")};
        return func_unknown_if(names, sizeof(names) / sizeof(names[0]), idx, "FuncOnOff");
    }

    const String lookupFuncYesNo(uint8_t idx)
    {
        static const __FlashStringHelper *names[] = {
            F("No"),
            F("Yes")};
        return func_unknown_if(names, sizeof(names) / sizeof(names[0]), idx, "FuncYesNo");
    }

    const String lookupFuncMalfunctionAckn(uint8_t idx)
    {
        static const __FlashStringHelper *names[] = {
            F("No acknowledge"),
            F("Acknowledge")};
        return func_unknown_if(names, sizeof(names) / sizeof(names[0]), idx, "FuncMalfunctionAckn");
    }

    const String lookupFuncPumpingStation(uint8_t idx)
    {
        static const __FlashStringHelper *names[] = {
            F("Off"),
            F("On and malfunction acknowledgement")};
        return func_unknown_if(names, sizeof(names) / sizeof(names[0]), idx, "FuncPumpingStation");
    }

    const String lookupFuncRotationSwitchpointCfg(uint8_t idx)
    {
        static const __FlashStringHelper *names[] = {
            F("Rotation speed switchpoint 1"),
            F("Rotation speed switchpoints 1 & 2")};
        return func_unknown_if(names, sizeof(names) / sizeof(names[0]), idx, "FuncRotationSwitchpointCfg");
    }

    const String lookupFuncOutputDO2Cfg(uint8_t idx)
    {
        static const __FlashStringHelper *names[] = {
            F("Rotation speed switchpoint reached"),
            F("No error"),
            F("Error"),
            F("Warning"),
            F("Error and/or warning"),
            F("Set rotation speed reached"),
            F("Pump on"),
            F("Pump accelerating"),
            F("Pump decelerating"),
            F(""), // 9
            F("Always '0'"),
            F("Always '1'"),
            F("Remote priority active"),
            F("Heating"),
            F("Backing pump"),
            F("Sealing gas"),
            F("Pumping station"),
            F("Pump rotating"),
            F("Pump has stopped"),
            F(""), // 18
            F("Pressure switch point 1 not reached"),
            F("Pressure switch point 2 not reached"),
            F("Fore-vacuum valve, delayed"),
            F("Backing pump standby")};
        return func_unknown_if(names, sizeof(names) / sizeof(names[0]), idx, "FuncOutputDO2Cfg");
    }

    const String lookupFuncBackingPumpOpMode(uint8_t idx)
    {
        static const __FlashStringHelper *names[] = {
            F("Continuous operation"),
            F("Intermittent operation"),
            F("Delayed switching on"),
            F("Delayed interval operation")};
        return func_unknown_if(names, sizeof(names) / sizeof(names[0]), idx, "FuncBackingPumpOpMode");
    }

    const String lookupFuncGasMode(uint8_t idx)
    {
        static const __FlashStringHelper *names[] = {
            F("Heavy gases"),
            F("Light gases"),
            F("Helium")};
        return func_unknown_if(names, sizeof(names) / sizeof(names[0]), idx, "FuncGasMode");
    }

    const String lookupFuncVentMode(uint8_t idx)
    {
        static const __FlashStringHelper *names[] = {
            F("Delayed venting"),
            F("No venting"),
            F("Direct venting")};
        return func_unknown_if(names, sizeof(names) / sizeof(names[0]), idx, "FuncVentMode");
    }

    const String lookupFuncAccA1Cfg(uint8_t idx)
    {
        static const __FlashStringHelper *names[] = {
            F("Fan (continuous operation)"),
            F("Venting valve, closed without current"),
            F("Heating"),
            F("Backing pump"),
            F("Fan (temperature controlled)"),
            F("Sealing gas"),
            F("Always '0'"),
            F("Always '1'"),
            F("Power failure venting unit"),
            F(""), // 9
            F(""), // 10
            F(""), // 11
            F("Second venting valve"),
            F("No function")};
        return func_unknown_if(names, sizeof(names) / sizeof(names[0]), idx, "FuncAccA1Cfg");
    }

    const String lookupFuncPress1HVen(uint8_t idx)
    {
        static const __FlashStringHelper *names[] = {
            F("Off"),
            F("On"),
            F("On, with rotation speed switch point reached"),
            F("On, with pressure switch point not reached")};
        return func_unknown_if(names, sizeof(names) / sizeof(names[0]), idx, "FuncPress1HVen");
    }

    const String lookupFuncOutputAO1Cfg(uint8_t idx)
    {
        static const __FlashStringHelper *names[] = {
            F("Actual rotation speed"),
            F("Output"),
            F("Current"),
            F("Always 0V"),
            F("Always 10V"),
            F(""), // 5
            F("Pressure value 1"),
            F("Pressure value 2"),
            F("Fore-vacuum control")};
        return func_unknown_if(names, sizeof(names) / sizeof(names[0]), idx, "FuncOutputAO1Cfg");
    }

    const String lookupFuncTmpMgtMode(uint8_t idx)
    {
        static const __FlashStringHelper *names[] = {
            F("Pump housing temperature ≤60C"),
            F("Pump housing temperature ≤80C"),
            F("Power characteristic [P:027]")};
        return func_unknown_if(names, sizeof(names) / sizeof(names[0]), idx, "FuncTmpMgtMode");
    }

    // FuncCtrlVialnt is sparse - only four of the 256 possible values are
    // defined. The previous implementation declared a static 256-pointer array
    // (512 bytes of SRAM on AVR) and rewrote four of its slots on every call.
    const char ctrlVialnt_1[] PROGMEM = "Remote";
    const char ctrlVialnt_2[] PROGMEM = "RS-485";
    const char ctrlVialnt_4[] PROGMEM = "PV.can";
    const char ctrlVialnt_255[] PROGMEM = "Unlock interface selection";

    struct SparseNameEntry
    {
        uint8_t index;
        const char *name;
    };

    const SparseNameEntry ctrlVialntNames[] PROGMEM = {
        {1, ctrlVialnt_1},
        {2, ctrlVialnt_2},
        {4, ctrlVialnt_4},
        {255, ctrlVialnt_255}};

    const String lookupFuncCtrlVialnt(uint8_t idx)
    {
        const size_t entryCount = sizeof(ctrlVialntNames) / sizeof(ctrlVialntNames[0]);
        for (size_t i = 0; i < entryCount; ++i)
        {
            SparseNameEntry entry;
            memcpy_P(&entry, &ctrlVialntNames[i], sizeof(SparseNameEntry));
            if (entry.index == idx)
                return String((const __FlashStringHelper *)entry.name);
        }
        char unknown_buf[32];
        snprintf(unknown_buf, sizeof(unknown_buf), "Unknown (FuncCtrlVialnt:%u)", (unsigned)idx);
        return String(unknown_buf);
    }

    const String lookupFuncDI1Cfg(uint8_t idx)
    {
        static const __FlashStringHelper *names[] = {
            F("Deactivated"),
            F("Enable venting"),
            F("Heating"),
            F("Sealing gas"),
            F("Run-up time monitoring"),
            F("Rotation speed setting mode"),
            F("Motor"),
            F("Enable HV sensor 1")};
        return func_unknown_if(names, sizeof(names) / sizeof(names[0]), idx, "FuncDI1Cfg");
    }
    // debug entries

    // ControlCommand::Heating debug strings
    const char debug_Heating_name[] PROGMEM = "Heating";
    const char debug_Heating_sdesc[] PROGMEM = "Heating";
    const char debug_Heating_ldesc[] PROGMEM = "FuncOnOff, Values: Off=0, On=1, Type: boolean_old, RW, min=0, max=1, default=0";

    // ControlCommand::StandBy debug strings
    const char debug_StandBy_name[] PROGMEM = "StandBy";
    const char debug_StandBy_sdesc[] PROGMEM = "Stand-by";
    const char debug_StandBy_ldesc[] PROGMEM = "FuncOnOff, Values: Off=0, On=1, Type: boolean_old, RW, min=0, max=1, default=0";

    // ControlCommand::RUTimeCtrl debug strings
    const char debug_RUTimeCtrl_name[] PROGMEM = "RUTimeCtrl";
    const char debug_RUTimeCtrl_sdesc[] PROGMEM = "Run-up time monitoring";
    const char debug_RUTimeCtrl_ldesc[] PROGMEM = "FuncOnOff, Values: Off=0, On=1, Type: boolean_old, RW, min=0, max=1, default=1";

    // ControlCommand::ErrorAckn debug strings
    const char debug_ErrorAckn_name[] PROGMEM = "ErrorAckn";
    const char debug_ErrorAckn_sdesc[] PROGMEM = "Malfunction acknowledgement";
    const char debug_ErrorAckn_ldesc[] PROGMEM = "FuncMalfunctionAckn, Values: Acknowledge=1, Type: boolean_old, W, min=1, max=1";

    // ControlCommand::PumpgStatn debug strings
    const char debug_PumpgStatn_name[] PROGMEM = "PumpgStatn";
    const char debug_PumpgStatn_sdesc[] PROGMEM = "Pumping station";
    const char debug_PumpgStatn_ldesc[] PROGMEM = "FuncPumpingStation, Values: Off=0, OnAndAckn=1, Type: boolean_old, RW, min=0, max=1, default=0";

    // ControlCommand::EnableVent debug strings
    const char debug_EnableVent_name[] PROGMEM = "EnableVent";
    const char debug_EnableVent_sdesc[] PROGMEM = "Enable venting";
    const char debug_EnableVent_ldesc[] PROGMEM = "FuncYesNo, Values: No=0, Yes=1, Type: boolean_old, RW, min=0, max=1, default=0";

    // ControlCommand::CfgSpdSwPt debug strings
    const char debug_CfgSpdSwPt_name[] PROGMEM = "CfgSpdSwPt";
    const char debug_CfgSpdSwPt_sdesc[] PROGMEM = "Rotation speed switchpoint configuration";
    const char debug_CfgSpdSwPt_ldesc[] PROGMEM = "FuncRotationSwitchpointCfg, Values: Switchpoint1=0, Switchpoints1And2=1, Type: u_short_int, RW, min=0, max=1, default=0";

    // ControlCommand::CfgDO2 debug strings
    const char debug_CfgDO2_name[] PROGMEM = "CfgDO2";
    const char debug_CfgDO2_sdesc[] PROGMEM = "Output DO2 configuration";
    const char debug_CfgDO2_ldesc[] PROGMEM = "FuncOutputDO2Cfg, Type: u_short_int, RW, min=0, max=22, default=1";

    // ControlCommand::MotorPump debug strings
    const char debug_MotorPump_name[] PROGMEM = "MotorPump";
    const char debug_MotorPump_sdesc[] PROGMEM = "Motor pump";
    const char debug_MotorPump_ldesc[] PROGMEM = "FuncOnOff, Values: Off=0, On=1, Type: boolean_old, RW, min=0, max=1, default=1";

    // ControlCommand::CfgDO1 debug strings
    const char debug_CfgDO1_name[] PROGMEM = "CfgDO1";
    const char debug_CfgDO1_sdesc[] PROGMEM = "Output DO1 configuration";
    const char debug_CfgDO1_ldesc[] PROGMEM = "FuncOutputDO1Cfg, Type: u_short_int, RW, min=0, max=22, default=0";

    // ControlCommand::OpModeBKP debug strings
    const char debug_OpModeBKP_name[] PROGMEM = "OpModeBKP";
    const char debug_OpModeBKP_sdesc[] PROGMEM = "Backing pump operating mode";
    const char debug_OpModeBKP_ldesc[] PROGMEM = "FuncBackingPumpOpMode, Values: Continuous=0, Intermittent=1, DelayedOn=2, DelayedInterval=3, Type: u_short_int, RW, min=0, max=3, default=0";

    // ControlCommand::SpdSetMode debug strings
    const char debug_SpdSetMode_name[] PROGMEM = "SpdSetMode";
    const char debug_SpdSetMode_sdesc[] PROGMEM = "Rotation speed setting mode";
    const char debug_SpdSetMode_ldesc[] PROGMEM = "FuncOnOff, Values: Off=0, On=1, Type: u_short_int, RW, min=0, max=1, default=0";

    // ControlCommand::GasMode debug strings
    const char debug_GasMode_name[] PROGMEM = "GasMode";
    const char debug_GasMode_sdesc[] PROGMEM = "Gas mode";
    const char debug_GasMode_ldesc[] PROGMEM = "FuncGasMode, Values: HeavyGases=0, LightGases=1, Helium=2, Type: u_short_int, RW, min=0, max=2, default=0";

    // ControlCommand::VentMode debug strings
    const char debug_VentMode_name[] PROGMEM = "VentMode";
    const char debug_VentMode_sdesc[] PROGMEM = "Venting mode";
    const char debug_VentMode_ldesc[] PROGMEM = "FuncVentMode, Values: DelayedVenting=0, NoVenting=1, DirectVenting=2, Type: u_short_int, RW, min=0, max=2, default=2";

    // ControlCommand::CfgAccA1 debug strings
    const char debug_CfgAccA1_name[] PROGMEM = "CfgAccA1";
    const char debug_CfgAccA1_sdesc[] PROGMEM = "Configuration accessory connection A1";
    const char debug_CfgAccA1_ldesc[] PROGMEM = "FuncAccA1Cfg, Type: u_short_int, RW, min=0, max=13, default=0";

    // ControlCommand::CfgAccB1 debug strings
    const char debug_CfgAccB1_name[] PROGMEM = "CfgAccB1";
    const char debug_CfgAccB1_sdesc[] PROGMEM = "Configuration accessory connection B1";
    const char debug_CfgAccB1_ldesc[] PROGMEM = "FuncAccB1Cfg, Type: u_short_int, RW, min=0, max=13, default=1";

    // ControlCommand::Press1HVen debug strings
    const char debug_Press1HVen_name[] PROGMEM = "Press1HVen";
    const char debug_Press1HVen_sdesc[] PROGMEM = "Release HV sensor integrated";
    const char debug_Press1HVen_ldesc[] PROGMEM = "FuncPress1HVen, Values: Off=0, On=1, OnWithRotSpdSwitchpoint=2, OnWithPressureSwitchNotReached=3, Type: u_short_int, RW, min=0, max=3, default=2";

    // ControlCommand::SealingGas debug strings
    const char debug_SealingGas_name[] PROGMEM = "SealingGas";
    const char debug_SealingGas_sdesc[] PROGMEM = "Sealing gas";
    const char debug_SealingGas_ldesc[] PROGMEM = "FuncOnOff, Values: Off=0, On=1, Type: boolean_old, RW, min=0, max=1, default=0";

    // ControlCommand::CfgAO1 debug strings
    const char debug_CfgAO1_name[] PROGMEM = "CfgAO1";
    const char debug_CfgAO1_sdesc[] PROGMEM = "Output AO1 configuration";
    const char debug_CfgAO1_ldesc[] PROGMEM = "FuncOutputAO1Cfg, Type: u_short_int, RW, min=0, max=8, default=0";

    // ControlCommand::TmpMgtMode debug strings
    const char debug_TmpMgtMode_name[] PROGMEM = "TmpMgtMode";
    const char debug_TmpMgtMode_sdesc[] PROGMEM = "Temperature management configuration";
    const char debug_TmpMgtMode_ldesc[] PROGMEM = "FuncTmpMgtMode, Values: PumpTempLEQ60C=0, PumpTempLEQ80C=1, PowerCharacteristic=2, Type: u_short_int, RW, min=0, max=2, default=0";

    // ControlCommand::CtrlVialnt debug strings
    const char debug_CtrlVialnt_name[] PROGMEM = "CtrlVialnt";
    const char debug_CtrlVialnt_sdesc[] PROGMEM = "Operate via interface";
    const char debug_CtrlVialnt_ldesc[] PROGMEM = "FuncCtrlVialnt, Values: Remote=1, RS485=2, PVCAN=4, UnlockInterfaceSelection=255, Type: u_short_int, RW, min=1, max=255, default=1";

    // ControlCommand::IntSelLckd debug strings
    const char debug_IntSelLckd_name[] PROGMEM = "IntSelLckd";
    const char debug_IntSelLckd_sdesc[] PROGMEM = "Interface selection locked";
    const char debug_IntSelLckd_ldesc[] PROGMEM = "FuncOnOff, Values: Off=0, On=1, Type: boolean_old, RW, min=0, max=1, default=0";

    // ControlCommand::CfgDI1 debug strings
    const char debug_CfgDI1_name[] PROGMEM = "CfgDI1";
    const char debug_CfgDI1_sdesc[] PROGMEM = "Input DI1 configuration";
    const char debug_CfgDI1_ldesc[] PROGMEM = "FuncDI1Cfg, Type: u_short_int, RW, min=0, max=7, default=1";

    // ControlCommand::CfgD12 debug strings
    const char debug_CfgD12_name[] PROGMEM = "CfgD12";
    const char debug_CfgD12_sdesc[] PROGMEM = "Input D12 configuration";
    const char debug_CfgD12_ldesc[] PROGMEM = "FuncDI2Cfg, Type: u_short_int, RW, min=0, max=7, default=2";

    // ControlCommand::CfgAccC1 debug strings
    const char debug_CfgAccC1_name[] PROGMEM = "CfgAccC1";
    const char debug_CfgAccC1_sdesc[] PROGMEM = "Configuration accessory connection C1";
    const char debug_CfgAccC1_ldesc[] PROGMEM = "FuncAccC1Cfg, Type: u_short_int, RW, min=0, max=13, default=0";

    // ControlCommand::CfgAccD1 debug strings
    const char debug_CfgAccD1_name[] PROGMEM = "CfgAccD1";
    const char debug_CfgAccD1_sdesc[] PROGMEM = "Configuration accessory connection D1";
    const char debug_CfgAccD1_ldesc[] PROGMEM = "FuncAccD1Cfg, Type: u_short_int, RW, min=0, max=13, default=0";

    // StatusRequest::RemotePrio debug strings
    const char debug_RemotePrio_name[] PROGMEM = "RemotePrio";
    const char debug_RemotePrio_sdesc[] PROGMEM = "Remote priority";
    const char debug_RemotePrio_ldesc[] PROGMEM = "Type: boolean_old, R, min=0, max=1";

    // StatusRequest::SpdSwPtAtt debug strings
    const char debug_SpdSwPtAtt_name[] PROGMEM = "SpdSwPtAtt";
    const char debug_SpdSwPtAtt_sdesc[] PROGMEM = "Rotation speed switchpoint reached";
    const char debug_SpdSwPtAtt_ldesc[] PROGMEM = "Type: boolean_old, R, min=0, max=1";

    // StatusRequest::ErrorCode debug strings
    const char debug_ErrorCode_name[] PROGMEM = "ErrorCode";
    const char debug_ErrorCode_sdesc[] PROGMEM = "Error code";
    const char debug_ErrorCode_ldesc[] PROGMEM = "Type: string6, R";

    // StatusRequest::OvTempElec debug strings
    const char debug_OvTempElec_name[] PROGMEM = "OvTempElec";
    const char debug_OvTempElec_sdesc[] PROGMEM = "Excess temperature drive electronics";
    const char debug_OvTempElec_ldesc[] PROGMEM = "Type: boolean_old, R, min=0, max=1";

    // StatusRequest::OvTempPump debug strings
    const char debug_OvTempPump_name[] PROGMEM = "OvTempPump";
    const char debug_OvTempPump_sdesc[] PROGMEM = "Excess temperature pump";
    const char debug_OvTempPump_ldesc[] PROGMEM = "Type: boolean_old, R, min=0, max=1";

    // StatusRequest::SetSpdAtt debug strings
    const char debug_SetSpdAtt_name[] PROGMEM = "SetSpdAtt";
    const char debug_SetSpdAtt_sdesc[] PROGMEM = "Set rotation speed reached";
    const char debug_SetSpdAtt_ldesc[] PROGMEM = "Type: boolean_old, R, min=0, max=1";

    // StatusRequest::PumpAccel debug strings
    const char debug_PumpAccel_name[] PROGMEM = "PumpAccel";
    const char debug_PumpAccel_sdesc[] PROGMEM = "Pump accelerating";
    const char debug_PumpAccel_ldesc[] PROGMEM = "Type: boolean_old, R, min=0, max=1";

    // StatusRequest::SetRotSpdHz debug strings
    const char debug_SetRotSpdHz_name[] PROGMEM = "SetRotSpdHz";
    const char debug_SetRotSpdHz_sdesc[] PROGMEM = "Set rotation speed (Hz)";
    const char debug_SetRotSpdHz_ldesc[] PROGMEM = "Type: u_integer, R, Hz, min=0, max=999999";

    // StatusRequest::ActualSpdHz debug strings
    const char debug_ActualSpdHz_name[] PROGMEM = "ActualSpdHz";
    const char debug_ActualSpdHz_sdesc[] PROGMEM = "Actual rotational speed (Hz)";
    const char debug_ActualSpdHz_ldesc[] PROGMEM = "Type: u_integer, R, Hz, min=0, max=999999";

    // StatusRequest::DrvCurrent debug strings
    const char debug_DrvCurrent_name[] PROGMEM = "DrvCurrent";
    const char debug_DrvCurrent_sdesc[] PROGMEM = "Drive current";
    const char debug_DrvCurrent_ldesc[] PROGMEM = "Type: u_real, R, A, min=0, max=9999.99";

    // StatusRequest::OpHrsPump debug strings
    const char debug_OpHrsPump_name[] PROGMEM = "OpHrsPump";
    const char debug_OpHrsPump_sdesc[] PROGMEM = "Pump operating hours";
    const char debug_OpHrsPump_ldesc[] PROGMEM = "Type: u_integer, R, h, min=0, max=65535";

    // StatusRequest::FwVersion debug strings
    const char debug_FwVersion_name[] PROGMEM = "FwVersion";
    const char debug_FwVersion_sdesc[] PROGMEM = "Drive electronics software version";
    const char debug_FwVersion_ldesc[] PROGMEM = "Type: string6, R";

    // StatusRequest::DrvVoltage debug strings
    const char debug_DrvVoltage_name[] PROGMEM = "DrvVoltage";
    const char debug_DrvVoltage_sdesc[] PROGMEM = "Drive voltage";
    const char debug_DrvVoltage_ldesc[] PROGMEM = "Type: u_real, R, V, min=0, max=9999.99";

    // StatusRequest::OpHrsElec debug strings
    const char debug_OpHrsElec_name[] PROGMEM = "OpHrsElec";
    const char debug_OpHrsElec_sdesc[] PROGMEM = "Drive electronics operating hours";
    const char debug_OpHrsElec_ldesc[] PROGMEM = "Type: u_integer, R, h, min=0, max=65535";

    // StatusRequest::NominalSpdHz debug strings
    const char debug_NominalSpdHz_name[] PROGMEM = "NominalSpdHz";
    const char debug_NominalSpdHz_sdesc[] PROGMEM = "Nominal rotation speed (Hz)";
    const char debug_NominalSpdHz_ldesc[] PROGMEM = "Type: u_integer, R, Hz, min=0, max=999999";

    // StatusRequest::DrvPower debug strings
    const char debug_DrvPower_name[] PROGMEM = "DrvPower";
    const char debug_DrvPower_sdesc[] PROGMEM = "Drive power";
    const char debug_DrvPower_ldesc[] PROGMEM = "Type: u_integer, R, W, min=0, max=999999";

    // StatusRequest::PumpCycles debug strings
    const char debug_PumpCycles_name[] PROGMEM = "PumpCycles";
    const char debug_PumpCycles_sdesc[] PROGMEM = "Pump cycles";
    const char debug_PumpCycles_ldesc[] PROGMEM = "Type: u_integer, R, min=0, max=65535";

    // StatusRequest::TmpPwrStg debug strings
    const char debug_TmpPwrStg_name[] PROGMEM = "TmpPwrStg";
    const char debug_TmpPwrStg_sdesc[] PROGMEM = "Temperature power stage";
    const char debug_TmpPwrStg_ldesc[] PROGMEM = "Type: u_integer, R, °C, min=0, max=999999";

    // StatusRequest::TempElec debug strings
    const char debug_TempElec_name[] PROGMEM = "TempElec";
    const char debug_TempElec_sdesc[] PROGMEM = "Electronics temperature";
    const char debug_TempElec_ldesc[] PROGMEM = "Type: u_integer, R, °C, min=0, max=999999";

    // StatusRequest::TempPmpBot debug strings
    const char debug_TempPmpBot_name[] PROGMEM = "TempPmpBot";
    const char debug_TempPmpBot_sdesc[] PROGMEM = "Pump lower part temperature";
    const char debug_TempPmpBot_ldesc[] PROGMEM = "Type: u_integer, R, °C, min=0, max=999999";

    // StatusRequest::AccelDecel debug strings
    const char debug_AccelDecel_name[] PROGMEM = "AccelDecel";
    const char debug_AccelDecel_sdesc[] PROGMEM = "Acceleration/deceleration";
    const char debug_AccelDecel_ldesc[] PROGMEM = "Type: u_integer, R, rpm/s, min=0, max=999999";

    // StatusRequest::ElecName debug strings
    const char debug_ElecName_name[] PROGMEM = "ElecName";
    const char debug_ElecName_sdesc[] PROGMEM = "Electronic drive unit designation";
    const char debug_ElecName_ldesc[] PROGMEM = "Type: string6, R";

    // StatusRequest::HWVersion debug strings
    const char debug_HWVersion_name[] PROGMEM = "HWVersion";
    const char debug_HWVersion_sdesc[] PROGMEM = "Hardware version drive electronics";
    const char debug_HWVersion_ldesc[] PROGMEM = "Type: string6, R";

    // StatusRequest::SerialNo debug strings
    const char debug_SerialNo_name[] PROGMEM = "SerialNo";
    const char debug_SerialNo_sdesc[] PROGMEM = "Serial number";
    const char debug_SerialNo_ldesc[] PROGMEM = "Type: string16, R";

    // StatusRequest::ErrHist1 debug strings
    const char debug_ErrHist1_name[] PROGMEM = "ErrHist1";
    const char debug_ErrHist1_sdesc[] PROGMEM = "Error code history, item 1";
    const char debug_ErrHist1_ldesc[] PROGMEM = "Type: string6, R";

    // StatusRequest::ErrHist2 debug strings
    const char debug_ErrHist2_name[] PROGMEM = "ErrHist2";
    const char debug_ErrHist2_sdesc[] PROGMEM = "Error code history, item 2";
    const char debug_ErrHist2_ldesc[] PROGMEM = "Type: string6, R";

    // StatusRequest::ErrHist3 debug strings
    const char debug_ErrHist3_name[] PROGMEM = "ErrHist3";
    const char debug_ErrHist3_sdesc[] PROGMEM = "Error code history, item 3";
    const char debug_ErrHist3_ldesc[] PROGMEM = "Type: string6, R";

    // StatusRequest::ErrHist4 debug strings
    const char debug_ErrHist4_name[] PROGMEM = "ErrHist4";
    const char debug_ErrHist4_sdesc[] PROGMEM = "Error code history, item 4";
    const char debug_ErrHist4_ldesc[] PROGMEM = "Type: string6, R";

    // StatusRequest::ErrHist5 debug strings
    const char debug_ErrHist5_name[] PROGMEM = "ErrHist5";
    const char debug_ErrHist5_sdesc[] PROGMEM = "Error code history, item 5";
    const char debug_ErrHist5_ldesc[] PROGMEM = "Type: string6, R";

    // StatusRequest::ErrHist6 debug strings
    const char debug_ErrHist6_name[] PROGMEM = "ErrHist6";
    const char debug_ErrHist6_sdesc[] PROGMEM = "Error code history, item 6";
    const char debug_ErrHist6_ldesc[] PROGMEM = "Type: string6, R";

    // StatusRequest::ErrHist7 debug strings
    const char debug_ErrHist7_name[] PROGMEM = "ErrHist7";
    const char debug_ErrHist7_sdesc[] PROGMEM = "Error code history, item 7";
    const char debug_ErrHist7_ldesc[] PROGMEM = "Type: string6, R";

    // StatusRequest::ErrHist8 debug strings
    const char debug_ErrHist8_name[] PROGMEM = "ErrHist8";
    const char debug_ErrHist8_sdesc[] PROGMEM = "Error code history, item 8";
    const char debug_ErrHist8_ldesc[] PROGMEM = "Type: string6, R";

    // StatusRequest::ErrHist9 debug strings
    const char debug_ErrHist9_name[] PROGMEM = "ErrHist9";
    const char debug_ErrHist9_sdesc[] PROGMEM = "Error code history, item 9";
    const char debug_ErrHist9_ldesc[] PROGMEM = "Type: string6, R";

    // StatusRequest::ErrHist10 debug strings
    const char debug_ErrHist10_name[] PROGMEM = "ErrHist10";
    const char debug_ErrHist10_sdesc[] PROGMEM = "Error code history, item 10";
    const char debug_ErrHist10_ldesc[] PROGMEM = "Type: string6, R";

    // StatusRequest::TempRotor debug strings
    const char debug_TempRotor_name[] PROGMEM = "TempRotor";
    const char debug_TempRotor_sdesc[] PROGMEM = "Rotor temperature";
    const char debug_TempRotor_ldesc[] PROGMEM = "Type: u_integer, R, °C, min=0, max=999999";

    // StatusRequest::OrderCode debug strings
    const char debug_OrderCode_name[] PROGMEM = "OrderCode";
    const char debug_OrderCode_sdesc[] PROGMEM = "Order number";
    const char debug_OrderCode_ldesc[] PROGMEM = "Type: string16, R";

    // StatusRequest::AddID debug strings
    const char debug_AddID_name[] PROGMEM = "AddID";
    const char debug_AddID_sdesc[] PROGMEM = "Pump ID";
    const char debug_AddID_ldesc[] PROGMEM = "Type: u_integer, R";

    // StatusRequest::SetRotSpdRpm debug strings
    const char debug_SetRotSpdRpm_name[] PROGMEM = "SetRotSpdRpm";
    const char debug_SetRotSpdRpm_sdesc[] PROGMEM = "Set rotation speed (rpm)";
    const char debug_SetRotSpdRpm_ldesc[] PROGMEM = "Type: u_integer, R, rpm, min=0, max=999999";

    // StatusRequest::ActualSpdRpm debug strings
    const char debug_ActualSpdRpm_name[] PROGMEM = "ActualSpdRpm";
    const char debug_ActualSpdRpm_sdesc[] PROGMEM = "Actual rotational speed (rpm)";
    const char debug_ActualSpdRpm_ldesc[] PROGMEM = "Type: u_integer, R, rpm, min=0, max=999999";

    // StatusRequest::NominalSpdRpm debug strings
    const char debug_NominalSpdRpm_name[] PROGMEM = "NominalSpdRpm";
    const char debug_NominalSpdRpm_sdesc[] PROGMEM = "Nominal rotation speed (rpm)";
    const char debug_NominalSpdRpm_ldesc[] PROGMEM = "Type: u_integer, R, rpm, min=0, max=999999";

    // ReferenceValueInput::RUTimeSVal debug strings
    const char debug_RUTimeSVal_name[] PROGMEM = "RUTimeSVal";
    const char debug_RUTimeSVal_sdesc[] PROGMEM = "Set value run-up time";
    const char debug_RUTimeSVal_ldesc[] PROGMEM = "Type: u_integer, RW, min=1, max=120, default=8";

    // ReferenceValueInput::SpdSwPt1 debug strings
    const char debug_SpdSwPt1_name[] PROGMEM = "SpdSwPt1";
    const char debug_SpdSwPt1_sdesc[] PROGMEM = "Rotation speed switch point 1";
    const char debug_SpdSwPt1_ldesc[] PROGMEM = "Type: u_integer, RW, %, min=50, max=97, default=80";

    // ReferenceValueInput::SpdSVal debug strings
    const char debug_SpdSVal_name[] PROGMEM = "SpdSVal";
    const char debug_SpdSVal_sdesc[] PROGMEM = "Set value in rotation speed setting mode";
    const char debug_SpdSVal_ldesc[] PROGMEM = "Type: u_real, RW, %, min=20, max=100, default=65";

    // ReferenceValueInput::PwrSVal debug strings
    const char debug_PwrSVal_name[] PROGMEM = "PwrSVal";
    const char debug_PwrSVal_sdesc[] PROGMEM = "Set value power consumption";
    const char debug_PwrSVal_ldesc[] PROGMEM = "Type: u_short_int, RW, %, min=10, max=100, default=100";

    // ReferenceValueInput::SwoffBKP debug strings
    const char debug_SwoffBKP_name[] PROGMEM = "SwoffBKP";
    const char debug_SwoffBKP_sdesc[] PROGMEM = "Backing pump switch-off threshold for intermittent operation";
    const char debug_SwoffBKP_ldesc[] PROGMEM = "Type: u_integer, RW, W, min=0, max=1000, default=0";

    // ReferenceValueInput::SwOnBKP debug strings
    const char debug_SwOnBKP_name[] PROGMEM = "SwOnBKP";
    const char debug_SwOnBKP_sdesc[] PROGMEM = "Backing pump switch-on threshold for intermittent operation";
    const char debug_SwOnBKP_ldesc[] PROGMEM = "Type: u_integer, RW, W, min=0, max=1000, default=0";

    // ReferenceValueInput::StdbySVal debug strings
    const char debug_StdbySVal_name[] PROGMEM = "StdbySVal";
    const char debug_StdbySVal_sdesc[] PROGMEM = "Rotation speed set value in stand-by operation";
    const char debug_StdbySVal_ldesc[] PROGMEM = "Type: u_real, RW, %, min=20, max=100, default=66.7";

    // ReferenceValueInput::SpdSwPt2 debug strings
    const char debug_SpdSwPt2_name[] PROGMEM = "SpdSwPt2";
    const char debug_SpdSwPt2_sdesc[] PROGMEM = "Rotation speed switch point 2";
    const char debug_SpdSwPt2_ldesc[] PROGMEM = "Type: u_integer, RW, %, min=5, max=97, default=20";

    // ReferenceValueInput::VentSpd debug strings
    const char debug_VentSpd_name[] PROGMEM = "VentSpd";
    const char debug_VentSpd_sdesc[] PROGMEM = "Venting at rotation speed, delayed venting";
    const char debug_VentSpd_ldesc[] PROGMEM = "Type: u_short_int, RW, %, min=40, max=98, default=50";

    // ReferenceValueInput::VentTime debug strings
    const char debug_VentTime_name[] PROGMEM = "VentTime";
    const char debug_VentTime_sdesc[] PROGMEM = "Venting time, delayed venting";
    const char debug_VentTime_ldesc[] PROGMEM = "Type: u_integer, RW, s, min=6, max=3600, default=3600";

    // ReferenceValueInput::mxPwrOutTm debug strings
    const char debug_mxPwrOutTm_name[] PROGMEM = "mxPwrOutTm";
    const char debug_mxPwrOutTm_sdesc[] PROGMEM = "Max. time for the output voltage in power back-up mode";
    const char debug_mxPwrOutTm_ldesc[] PROGMEM = "Type: u_integer, RW, s, min=1, max=255, default=10";

    // ReferenceValueInput::fanOnTemp debug strings
    const char debug_fanOnTemp_name[] PROGMEM = "fanOnTemp";
    const char debug_fanOnTemp_sdesc[] PROGMEM = "Start-up temperature of the fan in temperature-controlled mode";
    const char debug_fanOnTemp_ldesc[] PROGMEM = "Type: u_integer, RW, °C, min=6, max=75, default=45";

    // ReferenceValueInput::PrsSwPt1 debug strings
    const char debug_PrsSwPt1_name[] PROGMEM = "PrsSwPt1";
    const char debug_PrsSwPt1_sdesc[] PROGMEM = "Pressure switch point 1";
    const char debug_PrsSwPt1_ldesc[] PROGMEM = "Type: u_expo_new, RW, hPa, default=1000";

    // ReferenceValueInput::PrsSwPt2 debug strings
    const char debug_PrsSwPt2_name[] PROGMEM = "PrsSwPt2";
    const char debug_PrsSwPt2_sdesc[] PROGMEM = "Pressure switch point 2";
    const char debug_PrsSwPt2_ldesc[] PROGMEM = "Type: u_expo_new, RW, hPa, default=1000";

    // ReferenceValueInput::PwrOutVolt debug strings
    const char debug_PwrOutVolt_name[] PROGMEM = "PwrOutVolt";
    const char debug_PwrOutVolt_sdesc[] PROGMEM = "Output voltage in power back-up mode";
    const char debug_PwrOutVolt_ldesc[] PROGMEM = "Type: u_real, RW, V, min=20.50, max=26.50, default=23.00";

    // ReferenceValueInput::PwrOutThrs debug strings
    const char debug_PwrOutThrs_name[] PROGMEM = "PwrOutThrs";
    const char debug_PwrOutThrs_sdesc[] PROGMEM = "Power threshold from which the voltage is output from P733";
    const char debug_PwrOutThrs_ldesc[] PROGMEM = "Type: u_integer, RW, W, min=15, max=150, default=20";

    // ReferenceValueInput::PrsSn1Name debug strings
    const char debug_PrsSn1Name_name[] PROGMEM = "PrsSn1Name";
    const char debug_PrsSn1Name_sdesc[] PROGMEM = "Name sensor 1";
    const char debug_PrsSn1Name_ldesc[] PROGMEM = "Type: string6, R";

    // ReferenceValueInput::Pressure1 debug strings
    const char debug_Pressure1_name[] PROGMEM = "Pressure1";
    const char debug_Pressure1_sdesc[] PROGMEM = "Pressure value 1";
    const char debug_Pressure1_ldesc[] PROGMEM = "Type: u_expo_new, RW, hPa";

    // ReferenceValueInput::PrsCorrPi1 debug strings
    const char debug_PrsCorrPi1_name[] PROGMEM = "PrsCorrPi1";
    const char debug_PrsCorrPi1_sdesc[] PROGMEM = "Correction factor 1";
    const char debug_PrsCorrPi1_ldesc[] PROGMEM = "Type: u_real, RW, min=0.1, max=8.0, default=0";

    // ReferenceValueInput::PrsSn2Name debug strings
    const char debug_PrsSn2Name_name[] PROGMEM = "PrsSn2Name";
    const char debug_PrsSn2Name_sdesc[] PROGMEM = "Name sensor 2";
    const char debug_PrsSn2Name_ldesc[] PROGMEM = "Type: string6, R";

    // ReferenceValueInput::Pressure2 debug strings
    const char debug_Pressure2_name[] PROGMEM = "Pressure2";
    const char debug_Pressure2_sdesc[] PROGMEM = "Pressure value 2";
    const char debug_Pressure2_ldesc[] PROGMEM = "Type: u_expo_new, RW, hPa";

    // ReferenceValueInput::PrsCorrPi2 debug strings
    const char debug_PrsCorrPi2_name[] PROGMEM = "PrsCorrPi2";
    const char debug_PrsCorrPi2_sdesc[] PROGMEM = "Correction factor 2";
    const char debug_PrsCorrPi2_ldesc[] PROGMEM = "Type: u_real, RW, min=0.1, max=8.0, default=0";

    // ReferenceValueInput::NomSpdConf debug strings
    const char debug_NomSpdConf_name[] PROGMEM = "NomSpdConf";
    const char debug_NomSpdConf_sdesc[] PROGMEM = "Confirmation of nominal rotation speed";
    const char debug_NomSpdConf_ldesc[] PROGMEM = "Type: u_integer, RW, Hz, min=0, max=1500, default=0";

    // ReferenceValueInput::RS485Adr debug strings
    const char debug_RS485Adr_name[] PROGMEM = "RS485Adr";
    const char debug_RS485Adr_sdesc[] PROGMEM = "RS-485 Interface address";
    const char debug_RS485Adr_ldesc[] PROGMEM = "Type: u_integer, RW, min=1, max=255, default=1";

    // AdditionalParams::Pressure debug strings
    const char debug_Pressure_name[] PROGMEM = "Pressure";
    const char debug_Pressure_sdesc[] PROGMEM = "Actual pressure value (ActiveLine)";
    const char debug_Pressure_ldesc[] PROGMEM = "Type: u_short_int, R, hPa, min=1e-10, max=1e3";

    // AdditionalParams::CtrName debug strings
    const char debug_CtrName_name[] PROGMEM = "CtrName";
    const char debug_CtrName_sdesc[] PROGMEM = "Control unit type";
    const char debug_CtrName_ldesc[] PROGMEM = "Type: string6, R";

    // AdditionalParams::CtrSoftware debug strings
    const char debug_CtrSoftware_name[] PROGMEM = "CtrSoftware";
    const char debug_CtrSoftware_sdesc[] PROGMEM = "Control unit software version";
    const char debug_CtrSoftware_ldesc[] PROGMEM = "Type: string6, R";

    // AdditionalParams::GaugeType debug strings
    const char debug_GaugeType_name[] PROGMEM = "GaugeType";
    const char debug_GaugeType_sdesc[] PROGMEM = "Type of pressure gauge";
    const char debug_GaugeType_ldesc[] PROGMEM = "Type: string6, RW";

    // AdditionalParams::ParamSet debug strings
    const char debug_ParamSet_name[] PROGMEM = "ParamSet";
    const char debug_ParamSet_sdesc[] PROGMEM = "Parameter set";
    const char debug_ParamSet_ldesc[] PROGMEM = "FuncOnOff, Values: Basic=0, Extended=1, Type: u_short_int, RW, min=0, max=1, default=0";

    // AdditionalParams::Servicelin debug strings
    const char debug_Servicelin_name[] PROGMEM = "Servicelin";
    const char debug_Servicelin_sdesc[] PROGMEM = "Insert service line";
    const char debug_Servicelin_ldesc[] PROGMEM = "Type: u_short_int, RW, default=795";

    // Static array of all parameters, sorted by number
    static const ParameterDebugEntry ParameterDebugMap[] PROGMEM = {
        // Control Commands
        {(uint16_t)ControlCommand::Heating, (const __FlashStringHelper *)(debug_Heating_name), (const __FlashStringHelper *)(debug_Heating_sdesc), (const __FlashStringHelper *)(debug_Heating_ldesc), DataType::BooleanOld, lookupFuncOnOff},
        {(uint16_t)ControlCommand::StandBy, (const __FlashStringHelper *)(debug_StandBy_name), (const __FlashStringHelper *)(debug_StandBy_sdesc), (const __FlashStringHelper *)(debug_StandBy_ldesc), DataType::BooleanOld, lookupFuncOnOff},
        {(uint16_t)ControlCommand::RUTimeCtrl, (const __FlashStringHelper *)(debug_RUTimeCtrl_name), (const __FlashStringHelper *)(debug_RUTimeCtrl_sdesc), (const __FlashStringHelper *)(debug_RUTimeCtrl_ldesc), DataType::BooleanOld, lookupFuncOnOff},
        {(uint16_t)ControlCommand::ErrorAckn, (const __FlashStringHelper *)(debug_ErrorAckn_name), (const __FlashStringHelper *)(debug_ErrorAckn_sdesc), (const __FlashStringHelper *)(debug_ErrorAckn_ldesc), DataType::BooleanOld, lookupFuncMalfunctionAckn},
        {(uint16_t)ControlCommand::PumpgStatn, (const __FlashStringHelper *)(debug_PumpgStatn_name), (const __FlashStringHelper *)(debug_PumpgStatn_sdesc), (const __FlashStringHelper *)(debug_PumpgStatn_ldesc), DataType::BooleanOld, lookupFuncPumpingStation},
        {(uint16_t)ControlCommand::EnableVent, (const __FlashStringHelper *)(debug_EnableVent_name), (const __FlashStringHelper *)(debug_EnableVent_sdesc), (const __FlashStringHelper *)(debug_EnableVent_ldesc), DataType::BooleanOld, lookupFuncYesNo},
        {(uint16_t)ControlCommand::CfgSpdSwPt, (const __FlashStringHelper *)(debug_CfgSpdSwPt_name), (const __FlashStringHelper *)(debug_CfgSpdSwPt_sdesc), (const __FlashStringHelper *)(debug_CfgSpdSwPt_ldesc), DataType::UShortInt, lookupFuncRotationSwitchpointCfg},
        {(uint16_t)ControlCommand::CfgDO2, (const __FlashStringHelper *)(debug_CfgDO2_name), (const __FlashStringHelper *)(debug_CfgDO2_sdesc), (const __FlashStringHelper *)(debug_CfgDO2_ldesc), DataType::UShortInt, lookupFuncOutputDO2Cfg},
        {(uint16_t)ControlCommand::MotorPump, (const __FlashStringHelper *)(debug_MotorPump_name), (const __FlashStringHelper *)(debug_MotorPump_sdesc), (const __FlashStringHelper *)(debug_MotorPump_ldesc), DataType::BooleanOld, lookupFuncOnOff},
        {(uint16_t)ControlCommand::CfgDO1, (const __FlashStringHelper *)(debug_CfgDO1_name), (const __FlashStringHelper *)(debug_CfgDO1_sdesc), (const __FlashStringHelper *)(debug_CfgDO1_ldesc), DataType::UShortInt, lookupFuncOutputDO2Cfg},
        {(uint16_t)ControlCommand::OpModeBKP, (const __FlashStringHelper *)(debug_OpModeBKP_name), (const __FlashStringHelper *)(debug_OpModeBKP_sdesc), (const __FlashStringHelper *)(debug_OpModeBKP_ldesc), DataType::UShortInt, lookupFuncBackingPumpOpMode},
        {(uint16_t)ControlCommand::SpdSetMode, (const __FlashStringHelper *)(debug_SpdSetMode_name), (const __FlashStringHelper *)(debug_SpdSetMode_sdesc), (const __FlashStringHelper *)(debug_SpdSetMode_ldesc), DataType::UShortInt, lookupFuncOnOff},
        {(uint16_t)ControlCommand::GasMode, (const __FlashStringHelper *)(debug_GasMode_name), (const __FlashStringHelper *)(debug_GasMode_sdesc), (const __FlashStringHelper *)(debug_GasMode_ldesc), DataType::UShortInt, lookupFuncGasMode},
        {(uint16_t)ControlCommand::VentMode, (const __FlashStringHelper *)(debug_VentMode_name), (const __FlashStringHelper *)(debug_VentMode_sdesc), (const __FlashStringHelper *)(debug_VentMode_ldesc), DataType::UShortInt, lookupFuncVentMode},
        {(uint16_t)ControlCommand::CfgAccA1, (const __FlashStringHelper *)(debug_CfgAccA1_name), (const __FlashStringHelper *)(debug_CfgAccA1_sdesc), (const __FlashStringHelper *)(debug_CfgAccA1_ldesc), DataType::UShortInt, lookupFuncAccA1Cfg},
        {(uint16_t)ControlCommand::CfgAccB1, (const __FlashStringHelper *)(debug_CfgAccB1_name), (const __FlashStringHelper *)(debug_CfgAccB1_sdesc), (const __FlashStringHelper *)(debug_CfgAccB1_ldesc), DataType::UShortInt, lookupFuncAccA1Cfg},
        {(uint16_t)ControlCommand::Press1HVen, (const __FlashStringHelper *)(debug_Press1HVen_name), (const __FlashStringHelper *)(debug_Press1HVen_sdesc), (const __FlashStringHelper *)(debug_Press1HVen_ldesc), DataType::UShortInt, lookupFuncPress1HVen},
        {(uint16_t)ControlCommand::SealingGas, (const __FlashStringHelper *)(debug_SealingGas_name), (const __FlashStringHelper *)(debug_SealingGas_sdesc), (const __FlashStringHelper *)(debug_SealingGas_ldesc), DataType::BooleanOld, lookupFuncOnOff},
        {(uint16_t)ControlCommand::CfgAO1, (const __FlashStringHelper *)(debug_CfgAO1_name), (const __FlashStringHelper *)(debug_CfgAO1_sdesc), (const __FlashStringHelper *)(debug_CfgAO1_ldesc), DataType::UShortInt, lookupFuncOutputAO1Cfg},
        {(uint16_t)ControlCommand::TmpMgtMode, (const __FlashStringHelper *)(debug_TmpMgtMode_name), (const __FlashStringHelper *)(debug_TmpMgtMode_sdesc), (const __FlashStringHelper *)(debug_TmpMgtMode_ldesc), DataType::UShortInt, lookupFuncTmpMgtMode},
        {(uint16_t)ControlCommand::CtrlVialnt, (const __FlashStringHelper *)(debug_CtrlVialnt_name), (const __FlashStringHelper *)(debug_CtrlVialnt_sdesc), (const __FlashStringHelper *)(debug_CtrlVialnt_ldesc), DataType::UShortInt, lookupFuncCtrlVialnt},
        {(uint16_t)ControlCommand::IntSelLckd, (const __FlashStringHelper *)(debug_IntSelLckd_name), (const __FlashStringHelper *)(debug_IntSelLckd_sdesc), (const __FlashStringHelper *)(debug_IntSelLckd_ldesc), DataType::BooleanOld, lookupFuncOnOff},
        {(uint16_t)ControlCommand::CfgDI1, (const __FlashStringHelper *)(debug_CfgDI1_name), (const __FlashStringHelper *)(debug_CfgDI1_sdesc), (const __FlashStringHelper *)(debug_CfgDI1_ldesc), DataType::UShortInt, lookupFuncDI1Cfg},
        {(uint16_t)ControlCommand::CfgD12, (const __FlashStringHelper *)(debug_CfgD12_name), (const __FlashStringHelper *)(debug_CfgD12_sdesc), (const __FlashStringHelper *)(debug_CfgD12_ldesc), DataType::UShortInt, lookupFuncDI1Cfg},
        {(uint16_t)ControlCommand::CfgAccC1, (const __FlashStringHelper *)(debug_CfgAccC1_name), (const __FlashStringHelper *)(debug_CfgAccC1_sdesc), (const __FlashStringHelper *)(debug_CfgAccC1_ldesc), DataType::UShortInt, lookupFuncAccA1Cfg},
        {(uint16_t)ControlCommand::CfgAccD1, (const __FlashStringHelper *)(debug_CfgAccD1_name), (const __FlashStringHelper *)(debug_CfgAccD1_sdesc), (const __FlashStringHelper *)(debug_CfgAccD1_ldesc), DataType::UShortInt, lookupFuncAccA1Cfg},
        // Status Requests
        {(uint16_t)StatusRequest::RemotePrio, (const __FlashStringHelper *)(debug_RemotePrio_name), (const __FlashStringHelper *)(debug_RemotePrio_sdesc), (const __FlashStringHelper *)(debug_RemotePrio_ldesc), DataType::BooleanOld, lookupFuncYesNo},
        {(uint16_t)StatusRequest::SpdSwPtAtt, (const __FlashStringHelper *)(debug_SpdSwPtAtt_name), (const __FlashStringHelper *)(debug_SpdSwPtAtt_sdesc), (const __FlashStringHelper *)(debug_SpdSwPtAtt_ldesc), DataType::BooleanOld, lookupFuncYesNo},
        {(uint16_t)StatusRequest::ErrorCode, (const __FlashStringHelper *)(debug_ErrorCode_name), (const __FlashStringHelper *)(debug_ErrorCode_sdesc), (const __FlashStringHelper *)(debug_ErrorCode_ldesc), DataType::String6, nullptr},
        {(uint16_t)StatusRequest::OvTempElec, (const __FlashStringHelper *)(debug_OvTempElec_name), (const __FlashStringHelper *)(debug_OvTempElec_sdesc), (const __FlashStringHelper *)(debug_OvTempElec_ldesc), DataType::BooleanOld, lookupFuncYesNo},
        {(uint16_t)StatusRequest::OvTempPump, (const __FlashStringHelper *)(debug_OvTempPump_name), (const __FlashStringHelper *)(debug_OvTempPump_sdesc), (const __FlashStringHelper *)(debug_OvTempPump_ldesc), DataType::BooleanOld, lookupFuncYesNo},
        {(uint16_t)StatusRequest::SetSpdAtt, (const __FlashStringHelper *)(debug_SetSpdAtt_name), (const __FlashStringHelper *)(debug_SetSpdAtt_sdesc), (const __FlashStringHelper *)(debug_SetSpdAtt_ldesc), DataType::BooleanOld, lookupFuncYesNo},
        {(uint16_t)StatusRequest::PumpAccel, (const __FlashStringHelper *)(debug_PumpAccel_name), (const __FlashStringHelper *)(debug_PumpAccel_sdesc), (const __FlashStringHelper *)(debug_PumpAccel_ldesc), DataType::BooleanOld, lookupFuncYesNo},
        {(uint16_t)StatusRequest::SetRotSpdHz, (const __FlashStringHelper *)(debug_SetRotSpdHz_name), (const __FlashStringHelper *)(debug_SetRotSpdHz_sdesc), (const __FlashStringHelper *)(debug_SetRotSpdHz_ldesc), DataType::UInteger, nullptr},
        {(uint16_t)StatusRequest::ActualSpdHz, (const __FlashStringHelper *)(debug_ActualSpdHz_name), (const __FlashStringHelper *)(debug_ActualSpdHz_sdesc), (const __FlashStringHelper *)(debug_ActualSpdHz_ldesc), DataType::UInteger, nullptr},
        {(uint16_t)StatusRequest::DrvCurrent, (const __FlashStringHelper *)(debug_DrvCurrent_name), (const __FlashStringHelper *)(debug_DrvCurrent_sdesc), (const __FlashStringHelper *)(debug_DrvCurrent_ldesc), DataType::UReal, nullptr},
        {(uint16_t)StatusRequest::OpHrsPump, (const __FlashStringHelper *)(debug_OpHrsPump_name), (const __FlashStringHelper *)(debug_OpHrsPump_sdesc), (const __FlashStringHelper *)(debug_OpHrsPump_ldesc), DataType::UInteger, nullptr},
        {(uint16_t)StatusRequest::FwVersion, (const __FlashStringHelper *)(debug_FwVersion_name), (const __FlashStringHelper *)(debug_FwVersion_sdesc), (const __FlashStringHelper *)(debug_FwVersion_ldesc), DataType::String6, nullptr},
        {(uint16_t)StatusRequest::DrvVoltage, (const __FlashStringHelper *)(debug_DrvVoltage_name), (const __FlashStringHelper *)(debug_DrvVoltage_sdesc), (const __FlashStringHelper *)(debug_DrvVoltage_ldesc), DataType::UReal, nullptr},
        {(uint16_t)StatusRequest::OpHrsElec, (const __FlashStringHelper *)(debug_OpHrsElec_name), (const __FlashStringHelper *)(debug_OpHrsElec_sdesc), (const __FlashStringHelper *)(debug_OpHrsElec_ldesc), DataType::UInteger, nullptr},
        {(uint16_t)StatusRequest::NominalSpdHz, (const __FlashStringHelper *)(debug_NominalSpdHz_name), (const __FlashStringHelper *)(debug_NominalSpdHz_sdesc), (const __FlashStringHelper *)(debug_NominalSpdHz_ldesc), DataType::UInteger, nullptr},
        {(uint16_t)StatusRequest::DrvPower, (const __FlashStringHelper *)(debug_DrvPower_name), (const __FlashStringHelper *)(debug_DrvPower_sdesc), (const __FlashStringHelper *)(debug_DrvPower_ldesc), DataType::UInteger, nullptr},
        {(uint16_t)StatusRequest::PumpCycles, (const __FlashStringHelper *)(debug_PumpCycles_name), (const __FlashStringHelper *)(debug_PumpCycles_sdesc), (const __FlashStringHelper *)(debug_PumpCycles_ldesc), DataType::UInteger, nullptr},
        {(uint16_t)StatusRequest::TmpPwrStg, (const __FlashStringHelper *)(debug_TmpPwrStg_name), (const __FlashStringHelper *)(debug_TmpPwrStg_sdesc), (const __FlashStringHelper *)(debug_TmpPwrStg_ldesc), DataType::UInteger, nullptr},
        {(uint16_t)StatusRequest::TempElec, (const __FlashStringHelper *)(debug_TempElec_name), (const __FlashStringHelper *)(debug_TempElec_sdesc), (const __FlashStringHelper *)(debug_TempElec_ldesc), DataType::UInteger, nullptr},
        {(uint16_t)StatusRequest::TempPmpBot, (const __FlashStringHelper *)(debug_TempPmpBot_name), (const __FlashStringHelper *)(debug_TempPmpBot_sdesc), (const __FlashStringHelper *)(debug_TempPmpBot_ldesc), DataType::UInteger, nullptr},
        {(uint16_t)StatusRequest::AccelDecel, (const __FlashStringHelper *)(debug_AccelDecel_name), (const __FlashStringHelper *)(debug_AccelDecel_sdesc), (const __FlashStringHelper *)(debug_AccelDecel_ldesc), DataType::UInteger, nullptr},
        {(uint16_t)StatusRequest::ElecName, (const __FlashStringHelper *)(debug_ElecName_name), (const __FlashStringHelper *)(debug_ElecName_sdesc), (const __FlashStringHelper *)(debug_ElecName_ldesc), DataType::String6, nullptr},
        {(uint16_t)StatusRequest::HWVersion, (const __FlashStringHelper *)(debug_HWVersion_name), (const __FlashStringHelper *)(debug_HWVersion_sdesc), (const __FlashStringHelper *)(debug_HWVersion_ldesc), DataType::String6, nullptr},
        {(uint16_t)StatusRequest::SerialNo, (const __FlashStringHelper *)(debug_SerialNo_name), (const __FlashStringHelper *)(debug_SerialNo_sdesc), (const __FlashStringHelper *)(debug_SerialNo_ldesc), DataType::String16, nullptr},
        {(uint16_t)StatusRequest::ErrHist1, (const __FlashStringHelper *)(debug_ErrHist1_name), (const __FlashStringHelper *)(debug_ErrHist1_sdesc), (const __FlashStringHelper *)(debug_ErrHist1_ldesc), DataType::String6, nullptr},
        {(uint16_t)StatusRequest::ErrHist2, (const __FlashStringHelper *)(debug_ErrHist2_name), (const __FlashStringHelper *)(debug_ErrHist2_sdesc), (const __FlashStringHelper *)(debug_ErrHist2_ldesc), DataType::String6, nullptr},
        {(uint16_t)StatusRequest::ErrHist3, (const __FlashStringHelper *)(debug_ErrHist3_name), (const __FlashStringHelper *)(debug_ErrHist3_sdesc), (const __FlashStringHelper *)(debug_ErrHist3_ldesc), DataType::String6, nullptr},
        {(uint16_t)StatusRequest::ErrHist4, (const __FlashStringHelper *)(debug_ErrHist4_name), (const __FlashStringHelper *)(debug_ErrHist4_sdesc), (const __FlashStringHelper *)(debug_ErrHist4_ldesc), DataType::String6, nullptr},
        {(uint16_t)StatusRequest::ErrHist5, (const __FlashStringHelper *)(debug_ErrHist5_name), (const __FlashStringHelper *)(debug_ErrHist5_sdesc), (const __FlashStringHelper *)(debug_ErrHist5_ldesc), DataType::String6, nullptr},
        {(uint16_t)StatusRequest::ErrHist6, (const __FlashStringHelper *)(debug_ErrHist6_name), (const __FlashStringHelper *)(debug_ErrHist6_sdesc), (const __FlashStringHelper *)(debug_ErrHist6_ldesc), DataType::String6, nullptr},
        {(uint16_t)StatusRequest::ErrHist7, (const __FlashStringHelper *)(debug_ErrHist7_name), (const __FlashStringHelper *)(debug_ErrHist7_sdesc), (const __FlashStringHelper *)(debug_ErrHist7_ldesc), DataType::String6, nullptr},
        {(uint16_t)StatusRequest::ErrHist8, (const __FlashStringHelper *)(debug_ErrHist8_name), (const __FlashStringHelper *)(debug_ErrHist8_sdesc), (const __FlashStringHelper *)(debug_ErrHist8_ldesc), DataType::String6, nullptr},
        {(uint16_t)StatusRequest::ErrHist9, (const __FlashStringHelper *)(debug_ErrHist9_name), (const __FlashStringHelper *)(debug_ErrHist9_sdesc), (const __FlashStringHelper *)(debug_ErrHist9_ldesc), DataType::String6, nullptr},
        {(uint16_t)StatusRequest::ErrHist10, (const __FlashStringHelper *)(debug_ErrHist10_name), (const __FlashStringHelper *)(debug_ErrHist10_sdesc), (const __FlashStringHelper *)(debug_ErrHist10_ldesc), DataType::String6, nullptr},
        {(uint16_t)StatusRequest::TempRotor, (const __FlashStringHelper *)(debug_TempRotor_name), (const __FlashStringHelper *)(debug_TempRotor_sdesc), (const __FlashStringHelper *)(debug_TempRotor_ldesc), DataType::UInteger, nullptr},
        {(uint16_t)StatusRequest::OrderCode, (const __FlashStringHelper *)(debug_OrderCode_name), (const __FlashStringHelper *)(debug_OrderCode_sdesc), (const __FlashStringHelper *)(debug_OrderCode_ldesc), DataType::String16, nullptr},
        {(uint16_t)StatusRequest::AddID, (const __FlashStringHelper *)(debug_AddID_name), (const __FlashStringHelper *)(debug_AddID_sdesc), (const __FlashStringHelper *)(debug_AddID_ldesc), DataType::UInteger, nullptr},
        {(uint16_t)StatusRequest::SetRotSpdRpm, (const __FlashStringHelper *)(debug_SetRotSpdRpm_name), (const __FlashStringHelper *)(debug_SetRotSpdRpm_sdesc), (const __FlashStringHelper *)(debug_SetRotSpdRpm_ldesc), DataType::UInteger, nullptr},
        {(uint16_t)StatusRequest::ActualSpdRpm, (const __FlashStringHelper *)(debug_ActualSpdRpm_name), (const __FlashStringHelper *)(debug_ActualSpdRpm_sdesc), (const __FlashStringHelper *)(debug_ActualSpdRpm_ldesc), DataType::UInteger, nullptr},
        {(uint16_t)StatusRequest::NominalSpdRpm, (const __FlashStringHelper *)(debug_NominalSpdRpm_name), (const __FlashStringHelper *)(debug_NominalSpdRpm_sdesc), (const __FlashStringHelper *)(debug_NominalSpdRpm_ldesc), DataType::UInteger, nullptr},
        // Reference Value Inputs
        {(uint16_t)ReferenceValueInput::RUTimeSVal, (const __FlashStringHelper *)(debug_RUTimeSVal_name), (const __FlashStringHelper *)(debug_RUTimeSVal_sdesc), (const __FlashStringHelper *)(debug_RUTimeSVal_ldesc), DataType::UInteger, nullptr},
        {(uint16_t)ReferenceValueInput::SpdSwPt1, (const __FlashStringHelper *)(debug_SpdSwPt1_name), (const __FlashStringHelper *)(debug_SpdSwPt1_sdesc), (const __FlashStringHelper *)(debug_SpdSwPt1_ldesc), DataType::UInteger, nullptr},
        {(uint16_t)ReferenceValueInput::SpdSVal, (const __FlashStringHelper *)(debug_SpdSVal_name), (const __FlashStringHelper *)(debug_SpdSVal_sdesc), (const __FlashStringHelper *)(debug_SpdSVal_ldesc), DataType::UReal, nullptr},
        {(uint16_t)ReferenceValueInput::PwrSVal, (const __FlashStringHelper *)(debug_PwrSVal_name), (const __FlashStringHelper *)(debug_PwrSVal_sdesc), (const __FlashStringHelper *)(debug_PwrSVal_ldesc), DataType::UShortInt, nullptr},
        {(uint16_t)ReferenceValueInput::SwoffBKP, (const __FlashStringHelper *)(debug_SwoffBKP_name), (const __FlashStringHelper *)(debug_SwoffBKP_sdesc), (const __FlashStringHelper *)(debug_SwoffBKP_ldesc), DataType::UInteger, nullptr},
        {(uint16_t)ReferenceValueInput::SwOnBKP, (const __FlashStringHelper *)(debug_SwOnBKP_name), (const __FlashStringHelper *)(debug_SwOnBKP_sdesc), (const __FlashStringHelper *)(debug_SwOnBKP_ldesc), DataType::UInteger, nullptr},
        {(uint16_t)ReferenceValueInput::StdbySVal, (const __FlashStringHelper *)(debug_StdbySVal_name), (const __FlashStringHelper *)(debug_StdbySVal_sdesc), (const __FlashStringHelper *)(debug_StdbySVal_ldesc), DataType::UReal, nullptr},
        {(uint16_t)ReferenceValueInput::SpdSwPt2, (const __FlashStringHelper *)(debug_SpdSwPt2_name), (const __FlashStringHelper *)(debug_SpdSwPt2_sdesc), (const __FlashStringHelper *)(debug_SpdSwPt2_ldesc), DataType::UInteger, nullptr},
        {(uint16_t)ReferenceValueInput::VentSpd, (const __FlashStringHelper *)(debug_VentSpd_name), (const __FlashStringHelper *)(debug_VentSpd_sdesc), (const __FlashStringHelper *)(debug_VentSpd_ldesc), DataType::UShortInt, nullptr},
        {(uint16_t)ReferenceValueInput::VentTime, (const __FlashStringHelper *)(debug_VentTime_name), (const __FlashStringHelper *)(debug_VentTime_sdesc), (const __FlashStringHelper *)(debug_VentTime_ldesc), DataType::UInteger, nullptr},
        {(uint16_t)ReferenceValueInput::mxPwrOutTm, (const __FlashStringHelper *)(debug_mxPwrOutTm_name), (const __FlashStringHelper *)(debug_mxPwrOutTm_sdesc), (const __FlashStringHelper *)(debug_mxPwrOutTm_ldesc), DataType::UInteger, nullptr},
        {(uint16_t)ReferenceValueInput::fanOnTemp, (const __FlashStringHelper *)(debug_fanOnTemp_name), (const __FlashStringHelper *)(debug_fanOnTemp_sdesc), (const __FlashStringHelper *)(debug_fanOnTemp_ldesc), DataType::UInteger, nullptr},
        {(uint16_t)ReferenceValueInput::PrsSwPt1, (const __FlashStringHelper *)(debug_PrsSwPt1_name), (const __FlashStringHelper *)(debug_PrsSwPt1_sdesc), (const __FlashStringHelper *)(debug_PrsSwPt1_ldesc), DataType::UExpoNew, nullptr},
        {(uint16_t)ReferenceValueInput::PrsSwPt2, (const __FlashStringHelper *)(debug_PrsSwPt2_name), (const __FlashStringHelper *)(debug_PrsSwPt2_sdesc), (const __FlashStringHelper *)(debug_PrsSwPt2_ldesc), DataType::UExpoNew, nullptr},
        {(uint16_t)ReferenceValueInput::PwrOutVolt, (const __FlashStringHelper *)(debug_PwrOutVolt_name), (const __FlashStringHelper *)(debug_PwrOutVolt_sdesc), (const __FlashStringHelper *)(debug_PwrOutVolt_ldesc), DataType::UReal, nullptr},
        {(uint16_t)ReferenceValueInput::PwrOutThrs, (const __FlashStringHelper *)(debug_PwrOutThrs_name), (const __FlashStringHelper *)(debug_PwrOutThrs_sdesc), (const __FlashStringHelper *)(debug_PwrOutThrs_ldesc), DataType::UInteger, nullptr},
        {(uint16_t)ReferenceValueInput::PrsSn1Name, (const __FlashStringHelper *)(debug_PrsSn1Name_name), (const __FlashStringHelper *)(debug_PrsSn1Name_sdesc), (const __FlashStringHelper *)(debug_PrsSn1Name_ldesc), DataType::String6, nullptr},
        {(uint16_t)ReferenceValueInput::Pressure1, (const __FlashStringHelper *)(debug_Pressure1_name), (const __FlashStringHelper *)(debug_Pressure1_sdesc), (const __FlashStringHelper *)(debug_Pressure1_ldesc), DataType::UExpoNew, nullptr},
        {(uint16_t)ReferenceValueInput::PrsCorrPi1, (const __FlashStringHelper *)(debug_PrsCorrPi1_name), (const __FlashStringHelper *)(debug_PrsCorrPi1_sdesc), (const __FlashStringHelper *)(debug_PrsCorrPi1_ldesc), DataType::UReal, nullptr},
        {(uint16_t)ReferenceValueInput::PrsSn2Name, (const __FlashStringHelper *)(debug_PrsSn2Name_name), (const __FlashStringHelper *)(debug_PrsSn2Name_sdesc), (const __FlashStringHelper *)(debug_PrsSn2Name_ldesc), DataType::String6, nullptr},
        {(uint16_t)ReferenceValueInput::Pressure2, (const __FlashStringHelper *)(debug_Pressure2_name), (const __FlashStringHelper *)(debug_Pressure2_sdesc), (const __FlashStringHelper *)(debug_Pressure2_ldesc), DataType::UExpoNew, nullptr},
        {(uint16_t)ReferenceValueInput::PrsCorrPi2, (const __FlashStringHelper *)(debug_PrsCorrPi2_name), (const __FlashStringHelper *)(debug_PrsCorrPi2_sdesc), (const __FlashStringHelper *)(debug_PrsCorrPi2_ldesc), DataType::UReal, nullptr},
        {(uint16_t)ReferenceValueInput::NomSpdConf, (const __FlashStringHelper *)(debug_NomSpdConf_name), (const __FlashStringHelper *)(debug_NomSpdConf_sdesc), (const __FlashStringHelper *)(debug_NomSpdConf_ldesc), DataType::UInteger, nullptr},
        {(uint16_t)ReferenceValueInput::RS485Adr, (const __FlashStringHelper *)(debug_RS485Adr_name), (const __FlashStringHelper *)(debug_RS485Adr_sdesc), (const __FlashStringHelper *)(debug_RS485Adr_ldesc), DataType::UInteger, nullptr},
        // Additional Parameters
        {(uint16_t)AdditionalParams::Pressure, (const __FlashStringHelper *)(debug_Pressure_name), (const __FlashStringHelper *)(debug_Pressure_sdesc), (const __FlashStringHelper *)(debug_Pressure_ldesc), DataType::UShortInt, nullptr},
        {(uint16_t)AdditionalParams::CtrName, (const __FlashStringHelper *)(debug_CtrName_name), (const __FlashStringHelper *)(debug_CtrName_sdesc), (const __FlashStringHelper *)(debug_CtrName_ldesc), DataType::String6, nullptr},
        {(uint16_t)AdditionalParams::CtrSoftware, (const __FlashStringHelper *)(debug_CtrSoftware_name), (const __FlashStringHelper *)(debug_CtrSoftware_sdesc), (const __FlashStringHelper *)(debug_CtrSoftware_ldesc), DataType::String6, nullptr},
        {(uint16_t)AdditionalParams::GaugeType, (const __FlashStringHelper *)(debug_GaugeType_name), (const __FlashStringHelper *)(debug_GaugeType_sdesc), (const __FlashStringHelper *)(debug_GaugeType_ldesc), DataType::String6, nullptr},
        {(uint16_t)AdditionalParams::ParamSet, (const __FlashStringHelper *)(debug_ParamSet_name), (const __FlashStringHelper *)(debug_ParamSet_sdesc), (const __FlashStringHelper *)(debug_ParamSet_ldesc), DataType::UShortInt, nullptr},
        {(uint16_t)AdditionalParams::Servicelin, (const __FlashStringHelper *)(debug_Servicelin_name), (const __FlashStringHelper *)(debug_Servicelin_sdesc), (const __FlashStringHelper *)(debug_Servicelin_ldesc), DataType::UShortInt, nullptr}};

    bool getParameterDebugEntry(int number, ParameterDebugEntry *entry)
    {
        const size_t entryCount = sizeof(ParameterDebugMap) / sizeof(ParameterDebugMap[0]);

        // Find the parameter by number
        ParameterDebugEntry temp;
        for (size_t i = 0; i < entryCount; ++i)
        {
            // Copy one struct from FLASH (ParameterDebugMap[i]) to RAM (temp)
            memcpy_P(&temp, &ParameterDebugMap[i], sizeof(ParameterDebugEntry));
            // check if the the number matches.
            if (temp.number == number)
            {
                // copy out the correct debug entry and return.
                memcpy(entry, &temp, sizeof(ParameterDebugEntry));
                return true; // return true to indicate the parameter was found
            }
        }
        return false; // return false to indicate the parameter could not be found
    }


    // ---- Pump error/warning code lookup ----
    //
    // The table lives at namespace scope in PROGMEM so only the flash copy
    // exists; the previous function-local `static const` array kept 67 pointer
    // pairs in SRAM. F() cannot be used in a namespace-scope initialiser (it
    // expands to a statement expression), hence the named string arrays, which
    // is the same pattern ParameterDebugMap above uses.

    struct ErrorCodeDebugEntry
    {
        const __FlashStringHelper *errorCode; // 6 character code like "Err001"
        const __FlashStringHelper *problem;   // Human-readable description
    };


    const char errdbg_Err001_code[] PROGMEM = "Err001";
    const char errdbg_Err001_text[] PROGMEM = "Excess rotation speed";
    const char errdbg_Err002_code[] PROGMEM = "Err002";
    const char errdbg_Err002_text[] PROGMEM = "Excess voltage";
    const char errdbg_Err006_code[] PROGMEM = "Err006";
    const char errdbg_Err006_text[] PROGMEM = "Run-up error";
    const char errdbg_Err007_code[] PROGMEM = "Err007";
    const char errdbg_Err007_text[] PROGMEM = "Operating fluid low";
    const char errdbg_Err008_code[] PROGMEM = "Err008";
    const char errdbg_Err008_text[] PROGMEM = "Electronic drive unit - turbopump connection faulty";
    const char errdbg_Err010_code[] PROGMEM = "Err010";
    const char errdbg_Err010_text[] PROGMEM = "Internal device error";
    const char errdbg_Err021_code[] PROGMEM = "Err021";
    const char errdbg_Err021_text[] PROGMEM = "Electronic drive unit does not detect turbo-pump";
    const char errdbg_Err041_code[] PROGMEM = "Err041";
    const char errdbg_Err041_text[] PROGMEM = "Drive fault";
    const char errdbg_Err043_code[] PROGMEM = "Err043";
    const char errdbg_Err043_text[] PROGMEM = "Internal configuration error";
    const char errdbg_Err044_code[] PROGMEM = "Err044";
    const char errdbg_Err044_text[] PROGMEM = "Excess temperature, electronics";
    const char errdbg_Err045_code[] PROGMEM = "Err045";
    const char errdbg_Err045_text[] PROGMEM = "Excess temperature, motor";
    const char errdbg_Err046_code[] PROGMEM = "Err046";
    const char errdbg_Err046_text[] PROGMEM = "Internal initialization error";
    const char errdbg_Err073_code[] PROGMEM = "Err073";
    const char errdbg_Err073_text[] PROGMEM = "Axial magnetic bearing overload";
    const char errdbg_Err074_code[] PROGMEM = "Err074";
    const char errdbg_Err074_text[] PROGMEM = "Radial magnetic bearing overload";
    const char errdbg_Err089_code[] PROGMEM = "Err089";
    const char errdbg_Err089_text[] PROGMEM = "Rotor instable";
    const char errdbg_Err091_code[] PROGMEM = "Err091";
    const char errdbg_Err091_text[] PROGMEM = "Internal device error";
    const char errdbg_Err092_code[] PROGMEM = "Err092";
    const char errdbg_Err092_text[] PROGMEM = "Unknown connection panel";
    const char errdbg_Err093_code[] PROGMEM = "Err093";
    const char errdbg_Err093_text[] PROGMEM = "Motor temperature evaluation faulty";
    const char errdbg_Err094_code[] PROGMEM = "Err094";
    const char errdbg_Err094_text[] PROGMEM = "Electronics temperature evaluation faulty";
    const char errdbg_Err098_code[] PROGMEM = "Err098";
    const char errdbg_Err098_text[] PROGMEM = "Internal communication error";
    const char errdbg_Err106_code[] PROGMEM = "Err106";
    const char errdbg_Err106_text[] PROGMEM = "High rotor temperature";
    const char errdbg_Err107_code[] PROGMEM = "Err107";
    const char errdbg_Err107_text[] PROGMEM = "Final stage group error";
    const char errdbg_Err108_code[] PROGMEM = "Err108";
    const char errdbg_Err108_text[] PROGMEM = "Rotation speed measurement faulty";
    const char errdbg_Err109_code[] PROGMEM = "Err109";
    const char errdbg_Err109_text[] PROGMEM = "Software not released";
    const char errdbg_Err110_code[] PROGMEM = "Err110";
    const char errdbg_Err110_text[] PROGMEM = "Operating fluid evaluation faulty";
    const char errdbg_Err111_code[] PROGMEM = "Err111";
    const char errdbg_Err111_text[] PROGMEM = "Operating fluid pump communication error";
    const char errdbg_Err112_code[] PROGMEM = "Err112";
    const char errdbg_Err112_text[] PROGMEM = "Operating fluid pump group error";
    const char errdbg_Err113_code[] PROGMEM = "Err113";
    const char errdbg_Err113_text[] PROGMEM = "Rotor temperature evaluation faulty";
    const char errdbg_Err114_code[] PROGMEM = "Err114";
    const char errdbg_Err114_text[] PROGMEM = "Final stage temperature evaluation faulty";
    const char errdbg_Err117_code[] PROGMEM = "Err117";
    const char errdbg_Err117_text[] PROGMEM = "Excess temperature, pump lower part";
    const char errdbg_Err118_code[] PROGMEM = "Err118";
    const char errdbg_Err118_text[] PROGMEM = "Excess temperature, final stage";
    const char errdbg_Err119_code[] PROGMEM = "Err119";
    const char errdbg_Err119_text[] PROGMEM = "Excess temperature, bearing";
    const char errdbg_Err143_code[] PROGMEM = "Err143";
    const char errdbg_Err143_text[] PROGMEM = "Operating fluid pump excess temperature";
    const char errdbg_Err777_code[] PROGMEM = "Err777";
    const char errdbg_Err777_text[] PROGMEM = "Nominal rotation speed not confirmed";
    const char errdbg_Err800_code[] PROGMEM = "Err800";
    const char errdbg_Err800_text[] PROGMEM = "Magnetic bearing overflow";
    const char errdbg_Err802_code[] PROGMEM = "Err802";
    const char errdbg_Err802_text[] PROGMEM = "Magnetic bearing sensor technology fault";
    const char errdbg_Err810_code[] PROGMEM = "Err810";
    const char errdbg_Err810_text[] PROGMEM = "Internal configuration error";
    const char errdbg_Err815_code[] PROGMEM = "Err815";
    const char errdbg_Err815_text[] PROGMEM = "Magnetic bearing overflow";
    const char errdbg_Err890_code[] PROGMEM = "Err890";
    const char errdbg_Err890_text[] PROGMEM = "Safety bearing worn";
    const char errdbg_Err891_code[] PROGMEM = "Err891";
    const char errdbg_Err891_text[] PROGMEM = "Rotor imbalance too high";
    const char errdbg_Wrn001_code[] PROGMEM = "Wrn001";
    const char errdbg_Wrn001_text[] PROGMEM = "TMS heat-up time expired";
    const char errdbg_Wrn003_code[] PROGMEM = "Wrn003";
    const char errdbg_Wrn003_text[] PROGMEM = "TMS temperature invalid";
    const char errdbg_Wrn007_code[] PROGMEM = "Wrn007";
    const char errdbg_Wrn007_text[] PROGMEM = "Undervoltage or power failure";
    const char errdbg_Wrn016_code[] PROGMEM = "Wrn016";
    const char errdbg_Wrn016_text[] PROGMEM = "Accessory configuration invalid";
    const char errdbg_Wrn018_code[] PROGMEM = "Wrn018";
    const char errdbg_Wrn018_text[] PROGMEM = "Operating supremacy conflict";
    const char errdbg_Wrn021_code[] PROGMEM = "Wrn021";
    const char errdbg_Wrn021_text[] PROGMEM = "Blocking signal invalid";
    const char errdbg_Wrn034_code[] PROGMEM = "Wrn034";
    const char errdbg_Wrn034_text[] PROGMEM = "Sealing gas flow too low";
    const char errdbg_Wrn045_code[] PROGMEM = "Wrn045";
    const char errdbg_Wrn045_text[] PROGMEM = "Motor high temperature";
    const char errdbg_Wrn076_code[] PROGMEM = "Wrn076";
    const char errdbg_Wrn076_text[] PROGMEM = "Electronics high temperature";
    const char errdbg_Wrn089_code[] PROGMEM = "Wrn089";
    const char errdbg_Wrn089_text[] PROGMEM = "Imbalance high";
    const char errdbg_Wrn097_code[] PROGMEM = "Wrn097";
    const char errdbg_Wrn097_text[] PROGMEM = "Invalid pump information";
    const char errdbg_Wrn098_code[] PROGMEM = "Wrn098";
    const char errdbg_Wrn098_text[] PROGMEM = "Incomplete pump information";
    const char errdbg_Wrn100_code[] PROGMEM = "Wrn100";
    const char errdbg_Wrn100_text[] PROGMEM = "Minimum speed not reached";
    const char errdbg_Wrn106_code[] PROGMEM = "Wrn106";
    const char errdbg_Wrn106_text[] PROGMEM = "High rotor temperature";
    const char errdbg_Wrn113_code[] PROGMEM = "Wrn113";
    const char errdbg_Wrn113_text[] PROGMEM = "Inaccurate rotor temperature";
    const char errdbg_Wrn115_code[] PROGMEM = "Wrn115";
    const char errdbg_Wrn115_text[] PROGMEM = "Pump lower part temperature evaluation faulty";
    const char errdbg_Wrn116_code[] PROGMEM = "Wrn116";
    const char errdbg_Wrn116_text[] PROGMEM = "Bearing temperature evaluation faulty";
    const char errdbg_Wrn117_code[] PROGMEM = "Wrn117";
    const char errdbg_Wrn117_text[] PROGMEM = "Pump lower part high temperature";
    const char errdbg_Wrn118_code[] PROGMEM = "Wrn118";
    const char errdbg_Wrn118_text[] PROGMEM = "Final stage high temperature";
    const char errdbg_Wrn119_code[] PROGMEM = "Wrn119";
    const char errdbg_Wrn119_text[] PROGMEM = "Bearing high temperature";
    const char errdbg_Wrn143_code[] PROGMEM = "Wrn143";
    const char errdbg_Wrn143_text[] PROGMEM = "High operating fluid pump temperature";
    const char errdbg_Wrn168_code[] PROGMEM = "Wrn168";
    const char errdbg_Wrn168_text[] PROGMEM = "High delay";
    const char errdbg_Wrn801_code[] PROGMEM = "Wrn801";
    const char errdbg_Wrn801_text[] PROGMEM = "Braking transistor defective";
    const char errdbg_Wrn806_code[] PROGMEM = "Wrn806";
    const char errdbg_Wrn806_text[] PROGMEM = "Brake resistance defective";
    const char errdbg_Wrn807_code[] PROGMEM = "Wrn807";
    const char errdbg_Wrn807_text[] PROGMEM = "Calibration requirement";
    const char errdbg_Wrn890_code[] PROGMEM = "Wrn890";
    const char errdbg_Wrn890_text[] PROGMEM = "Safety bearing wear too high";
    const char errdbg_Wrn891_code[] PROGMEM = "Wrn891";
    const char errdbg_Wrn891_text[] PROGMEM = "High rotor imbalance";

    const ErrorCodeDebugEntry ErrorCodeDebugMap[] PROGMEM = {
        {(const __FlashStringHelper *)errdbg_Err001_code, (const __FlashStringHelper *)errdbg_Err001_text},
        {(const __FlashStringHelper *)errdbg_Err002_code, (const __FlashStringHelper *)errdbg_Err002_text},
        {(const __FlashStringHelper *)errdbg_Err006_code, (const __FlashStringHelper *)errdbg_Err006_text},
        {(const __FlashStringHelper *)errdbg_Err007_code, (const __FlashStringHelper *)errdbg_Err007_text},
        {(const __FlashStringHelper *)errdbg_Err008_code, (const __FlashStringHelper *)errdbg_Err008_text},
        {(const __FlashStringHelper *)errdbg_Err010_code, (const __FlashStringHelper *)errdbg_Err010_text},
        {(const __FlashStringHelper *)errdbg_Err021_code, (const __FlashStringHelper *)errdbg_Err021_text},
        {(const __FlashStringHelper *)errdbg_Err041_code, (const __FlashStringHelper *)errdbg_Err041_text},
        {(const __FlashStringHelper *)errdbg_Err043_code, (const __FlashStringHelper *)errdbg_Err043_text},
        {(const __FlashStringHelper *)errdbg_Err044_code, (const __FlashStringHelper *)errdbg_Err044_text},
        {(const __FlashStringHelper *)errdbg_Err045_code, (const __FlashStringHelper *)errdbg_Err045_text},
        {(const __FlashStringHelper *)errdbg_Err046_code, (const __FlashStringHelper *)errdbg_Err046_text},
        {(const __FlashStringHelper *)errdbg_Err073_code, (const __FlashStringHelper *)errdbg_Err073_text},
        {(const __FlashStringHelper *)errdbg_Err074_code, (const __FlashStringHelper *)errdbg_Err074_text},
        {(const __FlashStringHelper *)errdbg_Err089_code, (const __FlashStringHelper *)errdbg_Err089_text},
        {(const __FlashStringHelper *)errdbg_Err091_code, (const __FlashStringHelper *)errdbg_Err091_text},
        {(const __FlashStringHelper *)errdbg_Err092_code, (const __FlashStringHelper *)errdbg_Err092_text},
        {(const __FlashStringHelper *)errdbg_Err093_code, (const __FlashStringHelper *)errdbg_Err093_text},
        {(const __FlashStringHelper *)errdbg_Err094_code, (const __FlashStringHelper *)errdbg_Err094_text},
        {(const __FlashStringHelper *)errdbg_Err098_code, (const __FlashStringHelper *)errdbg_Err098_text},
        {(const __FlashStringHelper *)errdbg_Err106_code, (const __FlashStringHelper *)errdbg_Err106_text},
        {(const __FlashStringHelper *)errdbg_Err107_code, (const __FlashStringHelper *)errdbg_Err107_text},
        {(const __FlashStringHelper *)errdbg_Err108_code, (const __FlashStringHelper *)errdbg_Err108_text},
        {(const __FlashStringHelper *)errdbg_Err109_code, (const __FlashStringHelper *)errdbg_Err109_text},
        {(const __FlashStringHelper *)errdbg_Err110_code, (const __FlashStringHelper *)errdbg_Err110_text},
        {(const __FlashStringHelper *)errdbg_Err111_code, (const __FlashStringHelper *)errdbg_Err111_text},
        {(const __FlashStringHelper *)errdbg_Err112_code, (const __FlashStringHelper *)errdbg_Err112_text},
        {(const __FlashStringHelper *)errdbg_Err113_code, (const __FlashStringHelper *)errdbg_Err113_text},
        {(const __FlashStringHelper *)errdbg_Err114_code, (const __FlashStringHelper *)errdbg_Err114_text},
        {(const __FlashStringHelper *)errdbg_Err117_code, (const __FlashStringHelper *)errdbg_Err117_text},
        {(const __FlashStringHelper *)errdbg_Err118_code, (const __FlashStringHelper *)errdbg_Err118_text},
        {(const __FlashStringHelper *)errdbg_Err119_code, (const __FlashStringHelper *)errdbg_Err119_text},
        {(const __FlashStringHelper *)errdbg_Err143_code, (const __FlashStringHelper *)errdbg_Err143_text},
        {(const __FlashStringHelper *)errdbg_Err777_code, (const __FlashStringHelper *)errdbg_Err777_text},
        {(const __FlashStringHelper *)errdbg_Err800_code, (const __FlashStringHelper *)errdbg_Err800_text},
        {(const __FlashStringHelper *)errdbg_Err802_code, (const __FlashStringHelper *)errdbg_Err802_text},
        {(const __FlashStringHelper *)errdbg_Err810_code, (const __FlashStringHelper *)errdbg_Err810_text},
        {(const __FlashStringHelper *)errdbg_Err815_code, (const __FlashStringHelper *)errdbg_Err815_text},
        {(const __FlashStringHelper *)errdbg_Err890_code, (const __FlashStringHelper *)errdbg_Err890_text},
        {(const __FlashStringHelper *)errdbg_Err891_code, (const __FlashStringHelper *)errdbg_Err891_text},
        {(const __FlashStringHelper *)errdbg_Wrn001_code, (const __FlashStringHelper *)errdbg_Wrn001_text},
        {(const __FlashStringHelper *)errdbg_Wrn003_code, (const __FlashStringHelper *)errdbg_Wrn003_text},
        {(const __FlashStringHelper *)errdbg_Wrn007_code, (const __FlashStringHelper *)errdbg_Wrn007_text},
        {(const __FlashStringHelper *)errdbg_Wrn016_code, (const __FlashStringHelper *)errdbg_Wrn016_text},
        {(const __FlashStringHelper *)errdbg_Wrn018_code, (const __FlashStringHelper *)errdbg_Wrn018_text},
        {(const __FlashStringHelper *)errdbg_Wrn021_code, (const __FlashStringHelper *)errdbg_Wrn021_text},
        {(const __FlashStringHelper *)errdbg_Wrn034_code, (const __FlashStringHelper *)errdbg_Wrn034_text},
        {(const __FlashStringHelper *)errdbg_Wrn045_code, (const __FlashStringHelper *)errdbg_Wrn045_text},
        {(const __FlashStringHelper *)errdbg_Wrn076_code, (const __FlashStringHelper *)errdbg_Wrn076_text},
        {(const __FlashStringHelper *)errdbg_Wrn089_code, (const __FlashStringHelper *)errdbg_Wrn089_text},
        {(const __FlashStringHelper *)errdbg_Wrn097_code, (const __FlashStringHelper *)errdbg_Wrn097_text},
        {(const __FlashStringHelper *)errdbg_Wrn098_code, (const __FlashStringHelper *)errdbg_Wrn098_text},
        {(const __FlashStringHelper *)errdbg_Wrn100_code, (const __FlashStringHelper *)errdbg_Wrn100_text},
        {(const __FlashStringHelper *)errdbg_Wrn106_code, (const __FlashStringHelper *)errdbg_Wrn106_text},
        {(const __FlashStringHelper *)errdbg_Wrn113_code, (const __FlashStringHelper *)errdbg_Wrn113_text},
        {(const __FlashStringHelper *)errdbg_Wrn115_code, (const __FlashStringHelper *)errdbg_Wrn115_text},
        {(const __FlashStringHelper *)errdbg_Wrn116_code, (const __FlashStringHelper *)errdbg_Wrn116_text},
        {(const __FlashStringHelper *)errdbg_Wrn117_code, (const __FlashStringHelper *)errdbg_Wrn117_text},
        {(const __FlashStringHelper *)errdbg_Wrn118_code, (const __FlashStringHelper *)errdbg_Wrn118_text},
        {(const __FlashStringHelper *)errdbg_Wrn119_code, (const __FlashStringHelper *)errdbg_Wrn119_text},
        {(const __FlashStringHelper *)errdbg_Wrn143_code, (const __FlashStringHelper *)errdbg_Wrn143_text},
        {(const __FlashStringHelper *)errdbg_Wrn168_code, (const __FlashStringHelper *)errdbg_Wrn168_text},
        {(const __FlashStringHelper *)errdbg_Wrn801_code, (const __FlashStringHelper *)errdbg_Wrn801_text},
        {(const __FlashStringHelper *)errdbg_Wrn806_code, (const __FlashStringHelper *)errdbg_Wrn806_text},
        {(const __FlashStringHelper *)errdbg_Wrn807_code, (const __FlashStringHelper *)errdbg_Wrn807_text},
        {(const __FlashStringHelper *)errdbg_Wrn890_code, (const __FlashStringHelper *)errdbg_Wrn890_text},
        {(const __FlashStringHelper *)errdbg_Wrn891_code, (const __FlashStringHelper *)errdbg_Wrn891_text},
    };

    const __FlashStringHelper *getHumanReadableErrorMessage(const char *errorCode)
    {
        if (errorCode == nullptr || errorCode[0] == '\0')
            return nullptr;

        const size_t entryCount = sizeof(ErrorCodeDebugMap) / sizeof(ErrorCodeDebugMap[0]);
        for (size_t i = 0; i < entryCount; ++i)
        {
            ErrorCodeDebugEntry entry;
            memcpy_P(&entry, &ErrorCodeDebugMap[i], sizeof(ErrorCodeDebugEntry));
            if (strcasecmp_P(errorCode, reinterpret_cast<PGM_P>(entry.errorCode)) == 0)
                return entry.problem;
        }
        return nullptr; // Not found
    }
}

#endif // PFEIFFER_VAC_PROTOCOL_INCLUDE_DEBUG
