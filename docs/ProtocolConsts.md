### 5.2.5 Data Types

| No. | Data Type       | Description                                                                                   | Length (l1 – l0) | Example                                                                    |
| :-- | :-------------- | :-------------------------------------------------------------------------------------------- | :--------------- | :------------------------------------------------------------------------- |
| 0   | **boolean_old** | Logical value (false/true)                                                                    | 6                | 000000 is equivalent to false; 111111 is equivalent to true                |
| 1   | **u_integer**   | Positive whole number                                                                         | 6                | 000000 to 999999                                                           |
| 2   | **u_real**      | Fixed point number (unsigned)                                                                 | 6                | 001571 corresponds with 15.71                                              |
| 4   | **string**      | Any character string with 6 characters. ASCII codes between 32 and 127                        | 6                | TC_110, TM_700                                                             |
| 6   | **boolean_new** | Logical value (false/true)                                                                    | 1                | 0 is equivalent to false; 1 is equivalent to true                          |
| 7   | **u_short_int** | Positive whole number                                                                         | 3                | 000 to 999                                                                 |
| 11  | **string16**    | Any character string with 16 characters. ASCII codes between 32 and 127                       | 16               | BrezelBier&Wurst                                                           |
| 12  | **string8**     | Any character string with 8 characters. ASCII codes between 32 and 127                        | 8                | Example                                                                    |
| 10  | **u_expo_new**  | Positive exponential number. The last of both digits are the exponent with a deduction of 20. | 6                | 100023 is equivalent to 1.0 \* 10^3; 100000 is equivalent to 1.0 \* 10^-20 |

## 6 Parameter set

### 6.1 General

Import m pump starts in standard mode with factory default pre-set parameters.

Non-volatile data storage

When switching off or in the event of unintentional voltage drop, the parameters and the

operating hours stay saved in the electronics.

**Table columns:**

\# | Three digit number of the parameter

Indicator | Display of parameter description

Description | Brief description of the parameters

Functions | Function description of the parameters

Data type | Type of formatting of the parameter for the use with the Pfeiffer Vacuum protocol

Access type | R (read): Read access; W (write): Write access

Unit | Physical unit of the described variable

min. / max. | Permissible limit values for the entry of a value

default | Factory default pre-setting (partially pump-specific)

## 6.2 Control Commands

| #   | Display    | Description                              | Functions                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  | Data type | Access type | Unit | min | max | default |
| --- | ---------- | ---------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ | --------- | ----------- | ---- | --- | --- | ------- |
| 001 | Heating    | Heating                                  | 0= off 1= on                                                                                                                                                                                                                                                                                                                                                                                                                                                                               | 0         | RW          |      | 0   | 1   | 0       |
| 002 | Stand-by   | Stand-by                                 | 0= off 1= on                                                                                                                                                                                                                                                                                                                                                                                                                                                                               | 0         | RW          |      | 0   | 1   | 0       |
| 004 | RUTimeCtrl | Run-up time monitoring                   | 0= off 1= on                                                                                                                                                                                                                                                                                                                                                                                                                                                                               | 0         | RW          |      | 0   | 1   | 1       |
| 009 | ErrorAckn  | Malfunction acknowledgement              | 1= Malfunction acknowledgement                                                                                                                                                                                                                                                                                                                                                                                                                                                             | 0         | W           |      | 1   | 1   |         |
| 010 | PumpgStatn | Pumping station                          | 0= off 1= on and malfunction acknowledgment                                                                                                                                                                                                                                                                                                                                                                                                                                                | 0         | RW          |      | 0   | 1   | 0       |
| 012 | EnableVent | Enable venting                           | 0= no 1= yes                                                                                                                                                                                                                                                                                                                                                                                                                                                                               | 0         | RW          |      | 0   | 1   | 0       |
| 017 | CfgSpdSwPt | Rotation speed switchpoint configuration | 0= Rotation speed switchpoint 1 1= Rotation speed switch-points 1 & 2                                                                                                                                                                                                                                                                                                                                                                                                                      | 7         | RW          |      | 0   | 1   | 0       |
| 019 | Cfg DO2    | Output DO2 configuration                 | 0= Rotation speed switchpoint reached 1= No erroг 2= Error 3= Warning 4= Error and/or warning 5= Set rotation speed reached 6= Pump on 7= Pump accelerating 8= Pump decelerating 9= Always "0" 10= Always "1" 11= Remote priority active 12= Heating 13= Backing pump 14= Sealing gas 15= Pumping station 16= Pump rotating 17= Pump has stopped 19= Pressure switch point 1 not reached 20= Pressure switch point 2 not reached 21= Fore-vacuum valve, de-layed 22= Backing pump standby- | 7         | RW          |      | 0   | 22  | 1       |
| 023 | MotorPump  | Motor pump                               | 0= off 1= on                                                                                                                                                                                                                                                                                                                                                                                                                                                                               | 0         | RW          |      | 0   | 1   | 1       |
| 024 | Cfg DO1    | Output DO1 configuration                 | Functions, see [P:019]                                                                                                                                                                                                                                                                                                                                                                                                                                                                     | 7         | RW          |      | 0   | 22  | 0       |
| 025 | OpMode BKP | Backing pump operating mode              | 0= continuous operation 1= intermittent operation 2= Delayed switching on 3= Delayed interval operation                                                                                                                                                                                                                                                                                                                                                                                    | 7         | RW          |      | 0   | 3   | 0       |
| 026 | SpdSetMode | Rotation speed setting mode              | 0= off 1= on                                                                                                                                                                                                                                                                                                                                                                                                                                                                               | 7         | RW          |      | 0   | 1   | 0       |
| 027 | GasMode    | Gas mode                                 | 0= heavy gases 1= light gases 2= Helium                                                                                                                                                                                                                                                                                                                                                                                                                                                    | 7         | RW          |      | 0   | 2   | 0       |
| 030 | VentMode   | Venting mode                             | 0= delayed venting 1= no venting 2= direct venting                                                                                                                                                                                                                                                                                                                                                                                                                                         | 7         | RW          |      | 0   | 2   | 2       |
| 035 | Cfg Acc A1 | Configuration accessory connection A1    | 0= fan (continuous operation) 1= Venting valve, closed without current 2= Heating 3= Backing pump 4= Fan (temperature controlled) 5= Sealing gas 6= Always "0" 7= Always "1" 8= Power failure venting unit 12= Second venting valve 13= No function                                                                                                                                                                                                                                        | 7         | RW          |      | 0   | 13  | 0       |
| 036 | Cfg Acc B1 | Configuration accessory connection B1    | Functions, see [P:035]                                                                                                                                                                                                                                                                                                                                                                                                                                                                     | 7         | RW          |      | 0   | 13  | 1       |
| 041 | Press1HVen | Release HV sensor integrated (only IKT)  | 0= off 1= on 2= On, with rotation speed switch point reached 3= On, with pressure switch point not reached                                                                                                                                                                                                                                                                                                                                                                                 | 7         | RW          |      | 0   | 3   | 2       |
| 050 | SealingGas | Sealing gas                              | 0= off 1 = on                                                                                                                                                                                                                                                                                                                                                                                                                                                                              | 0         | RW          |      | 0   | 1   | 0       |
| 055 | Cfg AO1    | Output A01 configuration                 | 0= actual rotation speed 1= output 2= current 3= Always 0V 4= Always 10V 6= Pressure value 1 7= Pressure value 2 8= Fore-vacuum control                                                                                                                                                                                                                                                                                                                                                    | 7         | RW          |      | 0   | 8   | 0       |
| 058 | TmpMgtMode | Temperature management configuration     | 0= Pump housing temperature ≤60C 1= Pump housing temperature ≤80C 2= Power characteristic [P:027]                                                                                                                                                                                                                                                                                                                                                                                          | 7         | RW          |      | 0   | 2   | 0       |
| 060 | CtrlVialnt | Operate via interface                    | 1= remote 2= RS-485 4= PV.can 255= Unlock interface selection                                                                                                                                                                                                                                                                                                                                                                                                                              | 7         | RW          |      | 1   | 255 | 1       |
| 061 | IntSelLckd | Interface selection locked               | 0= of 1 = on                                                                                                                                                                                                                                                                                                                                                                                                                                                                               | 0         | RW          |      | 0   | 1   | 0       |
| 062 | Cfg DI1    | Input DI1 configuration                  | 0= Deactivated 1= Enable venting 2= Heating 3= Sealing gas 4= Run-up time monitoring 5= Rotation speed setting mode 6= Motor 7= Enable HV sensor 1                                                                                                                                                                                                                                                                                                                                         | 7         | RW          |      | 0   | 7   | 1       |
| 063 | Cfg DI2    | Input DI2 configuration                  | Functions, see [P:062]                                                                                                                                                                                                                                                                                                                                                                                                                                                                     | 7         | RW          |      | 0   | 7   | 2       |
| 068 | Cfg Acc C1 | Configuration accessory connection C1    | Functions, see [P:035]                                                                                                                                                                                                                                                                                                                                                                                                                                                                     | 7         | RW          |      | 0   | 13  | 0       |
| 069 | Cfg Acc D1 | Configuration accessory connection D1    | Functions, see [P:035]                                                                                                                                                                                                                                                                                                                                                                                                                                                                     | 7         | RW          |      | 0   | 13  | 0       |

## 6.3 Status requests

| #   | Display     | Description                          | Functions  | Data type | Access type | Unit  | min | max     | default |
| --- | ----------- | ------------------------------------ | ---------- | --------- | ----------- | ----- | --- | ------- | ------- |
| 300 | Remote Prio | Remote priority                      | 0=no 1=yes | 0         | R           |       | 0   | 1       |         |
| 302 | SpdSwPtAtt  | Rotation speed switchpoint reached   | 0=no 1=yes | 0         | R           |       | 0   | 1       |         |
| 303 | Error code  | Error code                           |            | 4         | R           |       |     |         |         |
| 304 | OvTempElec  | Excess temperature drive electronics | 0=no 1=yes | 0         | R           |       | 0   | 1       |         |
| 305 | OvTempPump  | Excess temperature pump              | 0=no 1=yes | 0         | R           |       | 0   | 1       |         |
| 306 | SetSpdAtt   | Set rotation speed reached           | 0=no 1=yes | 0         | R           |       | 0   | 1       |         |
| 307 | PumpAccel   | Pump accelerating                    | 0=no 1=yes | 0         | R           |       | 0   | 1       |         |
| 308 | SetRotSpd   | Set rotation speed (Hz)              |            | 1         | R           | Hz    | 0   | 999999  |         |
| 309 | ActualSpd   | Actual rotational speed (Hz)         |            | 1         | R           | Hz    | 0   | 999999  |         |
| 310 | DrvCurrent  | Drive current                        |            | 2         | R           | A     | 0   | 9999.99 |         |
| 311 | OpHrsPump   | Pump operating hours                 |            | 1         | R           | h     | 0   | 65535   |         |
| 312 | Fw version  | Drive electronics software version   |            | 4         | R           |       |     |         |         |
| 313 | DrvVoltage  | Drive voltage                        |            | 2         | R           | V     | 0   | 9999.99 |         |
| 314 | OpHrsElec   | Drive electronics operating hours    |            | 1         | R           | h     | 0   | 65535   |         |
| 315 | Nominal Spd | Nominal rotation speed (Hz)          |            | 1         | R           | Hz    | 0   | 999999  |         |
| 316 | DrvPower    | Drive power                          |            | 1         | R           | W     | 0   | 999999  |         |
| 319 | PumpCycles  | Pump cycles                          |            | 1         | R           |       | 0   | 65535   |         |
| 324 | TmpPwrStg   | Temperature power stage              |            | 1         | R           | °C    | 0   | 999999  |         |
| 326 | TempElec    | Electronics temperature              |            | 1         | R           | °C    | 0   | 999999  |         |
| 330 | TempPmpBot  | Pump lower part temperature          |            | 1         | R           | °C    | 0   | 999999  |         |
| 336 | AccelDecel  | Acceleration/deceleration            |            | 1         | R           | rpm/s | 0   | 999999  |         |
| 349 | ElecName    | Electronic drive unit designation    |            | 4         | R           |       |     |         |         |
| 354 | HW Version  | Hardware version drive electronics   |            | 4         | R           |       |     |         |         |
| 355 | Serial No   | Serial number                        |            | 11        | R           |       |     |         |         |
| 360 | ErrHist1    | Error code history, item 1           |            | 4         | R           |       |     |         |         |
| 361 | ErrHist2    | Error code history, item 2           |            | 4         | R           |       |     |         |         |
| 362 | ErrHist3    | Error code history, item 3           |            | 4         | R           |       |     |         |         |
| 363 | ErrHist4    | Error code history, item 4           |            | 4         | R           |       |     |         |         |
| 364 | ErrHist5    | Error code history, item 5           |            | 4         | R           |       |     |         |         |
| 365 | ErrHist6    | Error code history, item 6           |            | 4         | R           |       |     |         |         |
| 366 | ErrHist7    | Error code history, item 7           |            | 4         | R           |       |     |         |         |
| 367 | ErrHist8    | Error code history, item 8           |            | 4         | R           |       |     |         |         |
| 368 | ErrHist9    | Error code history, item 9           |            | 4         | R           |       |     |         |         |
| 369 | ErrHist10   | Error code history, item 10          |            | 4         | R           |       |     |         |         |
| 384 | TempRotor   | Rotor temperature                    |            | 1         | R           | °C    | 0   | 999999  |         |
| 388 | Order Code  | Order number                         |            | 11        | R           |       |     |         |         |
| 396 | AddID       | Pump ID                              |            | 1         | R           |       |     |         |         |
| 397 | SetRotSpd   | Set rotation speed (rpm)             |            | 1         | R           | rpm   | 0   | 999999  |         |
| 398 | ActualSpd   | Actual rotational speed (rpm)        |            | 1         | R           | rpm   | 0   | 999999  |         |
| 399 | NominalSpd  | Nominal rotation speed (rpm)         |            | 1         | R           | rpm   | 0   | 999999  |         |

## 6.4 Reference value inputs

| #   | Display     | Description                                                    | Functions | Data type | Access type | Unit | min.  | max.  | default |
| --- | ----------- | -------------------------------------------------------------- | --------- | --------- | ----------- | ---- | ----- | ----- | ------- |
| 700 | RUTimeSVal  | Set value run-up time                                          |           | 1         | RW          | min  | 1     | 120   | 8       |
| 701 | SpdSwPt1    | Rotation speed switch point 1                                  |           | 1         | RW          | %    | 50    | 97    | 80      |
| 707 | SpdSVal     | Set value in rotation speed setting mode                       |           | 2         | RW          | %    | 20    | 100   | 65      |
| 708 | PwrSVal     | Set value power consumption                                    |           | 7         | RW          | %    | 10    | 100   | 100     |
| 710 | Swoff BKP   | Backing pump switch-off threshold for intermittent operation   |           | 1         | RW          | W    | 0     | 1000  | 0       |
| 711 | SwOn BKP    | Backing pump switch-on threshold for intermittent operation    |           | 1         | RW          | W    | 0     | 1000  | 0       |
| 717 | StdbySVal   | Rotation speed set value in stand-by operation                 |           | 2         | RW          | %    | 20    | 100   | 66.7    |
| 719 | SpdSwPt2    | Rotation speed switch point 2                                  |           | 1         | RW          | %    | 5     | 97    | 20      |
| 720 | VentSpd     | Venting at rotation speed, delayed venting                     |           | 7         | RW          | %    | 40    | 98    | 50      |
| 721 | VentTime    | Venting time, delayed venting                                  |           | 1         | RW          | s    | 6     | 3600  | 3600    |
| 726 | mxPwrOutTm  | Max. time for the output voltage in power back-up mode         |           | 1         | RW          | s    | 1     | 255   | 10      |
| 728 | fanOnTemp   | Start-up temperature of the fan in temperature-controlled mode |           | 1         | RW          | °C   | 6     | 75    | 45      |
| 730 | PrsSwPt 1   | Pressure switch point 1                                        |           | 10        | RW          | hPa  |       |       | 1000    |
| 732 | PrsSwPt 2   | Pressure switch point 2                                        |           | 10        | RW          | hPa  |       |       | 1000    |
| 733 | PwrOutVolt  | Output voltage in power back-up mode                           |           | 2         | RW          | V    | 20.50 | 26.50 | 23.00   |
| 734 | PwrOutThrs  | Power threshold from which the voltage is output from P733     |           | 1         | RW          | W    | 15    | 150   | 20      |
| 739 | PrsSn1Name  | Name sensor 1                                                  |           | 4         | R           |      |       |       |         |
| 740 | Pressure 1  | Pressure value 1                                               |           | 10        | RW          | hPa  |       |       |         |
| 742 | PrsCorrPi 1 | Correction factor 1                                            |           | 2         | RW          |      | 0.1   | 8.0   | 0       |
| 749 | PrsSn2Name  | Name sensor 2                                                  |           | 4         | R           |      |       |       |         |
| 750 | Pressure 2  | Pressure value 2                                               |           | 10        | RW          | hPa  |       |       |         |
| 752 | PrsCorrPi 2 | Correction factor 2                                            |           | 2         | RW          |      | 0.1   | 8.0   | 0       |
| 777 | NomSpdConf  | Confirmation of nominal rotation speed                         |           | 1         | RW          | Hz   | 0     | 1500  | 0       |
| 797 | RS485Adr    | RS-485 Interface address                                       |           | 1         | RW          |      | 1     | 255   | 1       |

## 6.5 Additional Parameters for Control Unit

| #   | Indicator    | Description                        | Functions                                      | Data type | Access type | Unit | min         | max     | default |
| --- | ------------ | ---------------------------------- | ---------------------------------------------- | --------- | ----------- | ---- | ----------- | ------- | ------- |
| 340 | Pressure     | Actual pressure value (ActiveLine) |                                                | 7         | R           | hPa  | 1 \* 10^-10 | 1\*10^3 |         |
| 350 | Ctr Name     | Control unit type                  |                                                | 4         | R           |      |             |         |         |
| 351 | Ctr Software | Control unit software version      |                                                | 4         | R           |      |             |         |         |
| 738 | Gauge type   | Type of pressure gauge             |                                                | 4         | RW          |      |             |         |         |
| 794 | Param set    | Parameter set                      | 0=Basic parameter set 1=Extended parameter set | 7         | RW          |      | 0           | 1       | 0       |
| 795 | Servicelin   | Insert service line                |                                                | 7         | RW          |      |             |         | 795     |
