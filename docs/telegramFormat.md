## 5.2 Pfeiffer Vacuum protocol for RS-485 interface

### 5.2.1 Telegram frame

The telegram frame of the Pfeiffer Vacuum protocol contains only ASCII code characters [32; 127], with the exception of the end character of the telegram, CR. Basically, a host (e.g., a PC) sends a telegram, which a device (e.g., an electronic drive unit or gauge) responds to.

| Field | Description                                                                                                                                                                                                             |
| :---- | :---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| a2-a0 | **Device address**<br> • **Individual address of the unit:** ["001", "255"]<br> • **Group address:** "9xx" for all identical units (no response)<br> • **Global address:** "000" for all units on the bus (no response) |
| \*    | Action according to telegram description                                                                                                                                                                                |
| n2-n0 | **Pfeiffer Vacuum parameter numbers**                                                                                                                                                                                   |
| dnd0  | **Data length**                                                                                                                                                                                                         |
| dn-d0 | **Data in the respective data type** (see chapter "Data types," page 24)                                                                                                                        |
| c2-c0 | **Checksum** (sum of ASCII values of cells a2 to d0) modulo 256                                                                                                                                 |
| CR    | **Carriage return** (ASCII 13)                                                                                                                                                                  |

---

### 5.2.2 Telegram description

- **Data query:** `-->? a2 a1 a0 0 0 n2 n1 n0 0 2 = ? c2 c1 co CR`
- **Control command:** `--> ! a2 a1 a0 1 0 n2 n1 n0 11 10 dn d0 c2 c1 co CR`
- **Data response / Control command understood:** `--> a2 a1 a0 1 0 n2 n1 n0 11 10 dn d0 c2 c1 co CR`
- **Error message:** `--> a2 a1 a0 1 0 n2 n1 n0 0 6 N O D E F c2 c1 co CR`

| Error   | Description                              |
| :------ | :--------------------------------------- |
| NO_DEF  | Parameter number n2-n0 no longer exists  |
| \_RANGE | Data dn-d0 outside the permissible range |
| \_LOGIC | Logical access error                     |

---

### 5.2.3 Telegram example 1

**Data query: Current rotation speed (parameter [P:309], device address: "123")**

|           | 1   | 2   | 3   | 0   | 0   | 3   | 0   | 9   | 0   | 2   | =   | ?   | 1   | 1   | 2   | CR  |
| :-------- | :-- | :-- | :-- | :-- | :-- | :-- | :-- | :-- | :-- | :-- | :-- | :-- | :-- | :-- | :-- | :-- |
| **ASCII** | 49  | 50  | 51  | 48  | 48  | 51  | 48  | 57  | 48  | 50  | 61  | 63  | 49  | 49  | 50  | 13  |

**Data response: 633 Hz**

**Current rotation speed (parameter [P:309], device address: "123")**

| --\>      | 1   | 2   | 3   | 1   | 0   | 3   | 0   | 9   | 0   | 6   | 0   | 0   | 0   | 6   | 3   | 3   | 0   | 3   | 7   | CR  |
| :-------- | :-- | :-- | :-- | :-- | :-- | :-- | :-- | :-- | :-- | :-- | :-- | :-- | :-- | :-- | :-- | :-- | :-- | :-- | :-- | :-- |
| **ASCII** | 49  | 50  | 51  | 49  | 48  | 51  | 48  | 57  | 48  | 54  | 48  | 48  | 48  | 54  | 51  | 51  | 48  | 51  | 55  | 13  |

---

### 5.2.4 Telegram example 2

**Control command: Switch on the pumping station (parameter [P:010], device address: "042")**

|           | 0   | 4   | 2   | 1   | 0   | 1   | 0   | 0   | 6   | 1   | 1   | 1   | 1   | 1   | 1   | 0   | 2   | 0   | CR  |
| :-------- | :-- | :-- | :-- | :-- | :-- | :-- | :-- | :-- | :-- | :-- | :-- | :-- | :-- | :-- | :-- | :-- | :-- | :-- | :-- |
| **ASCII** | 48  | 52  | 50  | 49  | 48  | 49  | 48  | 48  | 54  | 49  | 49  | 49  | 49  | 49  | 49  | 48  | 50  | 48  | 13  |

**Control command understood: Switch on the pumping station (parameter [P:010], device address: "042")**

| --\>      | 0   | 4   | 2   | 1   | 0   | 1   | 0   | 0   | 6   | 1   | 1   | 1   | 1   | 1   | 1   | 0   | 2   | 0   | CR  |
| :-------- | :-- | :-- | :-- | :-- | :-- | :-- | :-- | :-- | :-- | :-- | :-- | :-- | :-- | :-- | :-- | :-- | :-- | :-- | :-- |
| **ASCII** | 48  | 52  | 50  | 49  | 48  | 49  | 48  | 48  | 54  | 49  | 49  | 49  | 49  | 49  | 49  | 48  | 50  | 48  | 13  |
