## 6.5 Additional Parameters for Control Unit

| #   | Indicator    | Description                        | Functions                                      | Data type | Access type | Unit | min         | max     | default |
| --- | ------------ | ---------------------------------- | ---------------------------------------------- | --------- | ----------- | ---- | ----------- | ------- | ------- |
| 340 | Pressure     | Actual pressure value (ActiveLine) |                                                | 7         | R           | hPa  | 1 \* 10^-10 | 1\*10^3 |         |
| 350 | Ctr Name     | Control unit type                  |                                                | 4         | R           |      |             |         |         |
| 351 | Ctr Software | Control unit software version      |                                                | 4         | R           |      |             |         |         |
| 738 | Gauge type   | Type of pressure gauge             |                                                | 4         | RW          |      |             |         |         |
| 794 | Param set    | Parameter set                      | 0=Basic parameter set 1=Extended parameter set | 7         | RW          |      | 0           | 1       | 0       |
| 795 | Servicelin   | Insert service line                |                                                | 7         | RW          |      |             |         | 795     |
