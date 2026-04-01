# Vending Machine 100-Channel Motor Driver

[!\[License\](https://img.shields.io/badge/License-Apache%202.0-blue.svg null)](LICENSE)

An open-source **100-channel vending machine motor driver control system** based on the **SWM181** microcontroller. Supports multiple motor types including spring motors, electromagnetic locks, and crawler-type motors, with RS485 communication protocol.

***

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Hardware Architecture](#hardware-architecture)
- [Communication Protocol](#communication-protocol)
- [Project Structure](#project-structure)
- [Getting Started](#getting-started)
- [Default Configuration](#default-configuration)
- [License](#license)

***

## Overview

This project provides a complete firmware solution for controlling up to 100 motors in a vending machine setup. It uses a 10x10 matrix configuration to drive various types of motors commonly found in vending machines, including spring-based dispensers, electromagnetic locks, and crawler-type mechanisms.

The system is built around the Synwit SWM181 microcontroller (ARM Cortex-M0 core) and communicates via RS485 protocol, making it suitable for industrial vending machine applications.

***

## Features

### Motor Control

- **100-channel matrix control** (10x10 matrix, supporting up to 100 motors)
- **Multiple motor type support**:
  - Spring motors with feedback (FB Motor)
  - Electromagnetic locks with feedback (FB Electromagnet)
  - Electromagnetic locks without feedback (NFB Electromagnet)
  - Crawler-type motors with feedback (FB Crawler)

### Communication & Safety

- **RS485 communication** with dual-channel support
- **Infrared detection** for anti-pinch and goods detection
- **CRC16 checksum** for reliable communication
- **Watchdog timer** support for system stability

### Monitoring & Updates

- **Ambient monitoring** (temperature & humidity via Modbus RTU)
- **Electronic lock control**
- **XMODEM protocol** support for firmware updates

***

## Hardware Architecture

| Component     | Description                   |
| ------------- | ----------------------------- |
| MCU           | SWM181 (ARM Cortex-M0, 48MHz) |
| Communication | RS485 (Dual Channel)          |
| Motor Drive   | 10x10 Matrix (X-Y control)    |
| Feedback      | 10-channel position feedback  |
| Sensors       | Infrared detection (NPN type) |
| Power         | 12V/24V DC                    |
| Debug         | SWD/J-Link support            |

### Matrix Configuration

- **X Axis (Columns)**: 10 channels for motor selection
- **Y Axis (Rows)**: 10 channels for motor activation
- **Feedback**: 10 channels for position sensing

***

## Communication Protocol

The system uses a custom frame protocol over RS485 with CRC16 error checking.

### Frame Structure

```
+------------+-------------+----------------+------+------+-----------+----------+
| Frame Head | Device Type | Device Address | CMD  | Data | CRC16     | Frame End|
| 0xDDCCBBAA | 1 byte      | 1 byte         | 1 byte| var | 2 bytes   |0xFBFCFDFE|
| (4 bytes)  |             |                |      |      |           | (4 bytes)|
+------------+-------------+----------------+------+------+-----------+----------+
```

### Supported Commands

| Command           | Value  | Description                             |
| ----------------- | ------ | --------------------------------------- |
| CMD\_OUTPUT       | `0x01` | Control motor to dispense goods         |
| CMD\_READ         | `0x02` | Read temperature, humidity, lock status |
| CMD\_LOCK         | `0x03` | Control electronic lock                 |
| CMD\_REPLY        | `0x04` | Response frame                          |
| CMD\_MODIFICATION | `0x05` | Modify device RS485 address             |

### Response Codes

| Code                                   | Value  | Description                 |
| -------------------------------------- | ------ | --------------------------- |
| RESULT\_VM\_FB\_MOTOR\_OUTPUT          | `0xC1` | Motor output result         |
| RESULT\_VM\_STATES                     | `0xC2` | Status query result         |
| RESULT\_VM\_ADDR\_MODIFICATION         | `0xC3` | Address modification result |
| RESULT\_VM\_LOCK                       | `0xC4` | Lock control result         |
| RESULT\_VM\_OUTPUT\_NFB\_ELECTROMAGNET | `0xC5` | NFB electromagnet result    |
| RESULT\_VM\_OUTPUT\_FB\_ELECTROMAGNET  | `0xC6` | FB electromagnet result     |

***

## Project Structure

```
vending-machine-100ch/
├── Code/
│   └── spring_drive_V1.6/          # Firmware source code (Keil MDK project)
│       ├── Application/             # Application layer
│       │   ├── main.c               # Main entry point
│       │   ├── vm.c / vm.h          # Vending machine logic & protocol
│       │   ├── ambient.c / ambient.h# Temperature & humidity monitoring
│       │   ├── xmodem.c / xmodem.h  # Firmware update protocol
│       │   ├── soft_timer.c/h       # Software timer implementation
│       │   ├── Drivers/             # Hardware drivers
│       │   │   └── Source/
│       │   │       ├── motor_driver.c   # Motor control (10x10 matrix)
│       │   │       ├── rs485.c          # RS485 communication driver
│       │   │       ├── infrared_ray_check.c  # IR sensor detection
│       │   │       ├── lock.c           # Electronic lock control
│       │   │       ├── led.c            # LED indicator control
│       │   │       ├── timer.c          # Hardware timer
│       │   │       ├── tick.c           # System tick
│       │   │       ├── uart.c           # UART driver
│       │   │       ├── address.c        # Device address management
│       │   │       └── flash.c          # Flash storage
│       │   ├── SEEGER/              # SEGGER RTT debug
│       │   └── MALLOC/              # Memory allocator
│       └── Project/                 # Keil project files
│           ├── spring.uvprojx       # Keil project
│           └── RTE/                 # Runtime environment
│
├── HW/
│   └── 弹簧货道电机驱动板V0816/      # Hardware design files
│       ├── SCH/                     # Schematics (Altium Designer)
│       ├── PCB/                     # PCB layout
│       ├── Project/                 # Altium project files
│       ├── Libraries/               # Component libraries
│       ├── Datasheet/               # Component datasheets
│       │   ├── 主控/                # MCU datasheet (SWM181)
│       │   └── 通信IC/              # Communication IC datasheets
│       └── Output/                  # Manufacturing outputs
│
└── LICENSE                          # Apache 2.0 License
```

***

## Getting Started

### Hardware Requirements

- **Vending machine motor driver board** (弹簧货道电机驱动板 V0816)
- **Power supply**: 12V/24V DC (depending on motor specifications)
- **RS485 master controller** (for communication)
- **Motors**: Spring motors, electromagnetic locks, or crawler-type motors
- **Debugger**: J-Link or compatible SWD debugger

### Software Requirements

- **Keil MDK-ARM** (version 5.0 or later recommended)
- **J-Link Software** (for debugging and flashing)
- **Serial terminal** (for RS485 communication testing)

### Build Instructions

1. **Open the project**:
   ```
   Code/spring_drive_V1.6/Project/spring.uvprojx
   ```
2. **Select target device**: SWM181xC or SWM181xB
3. **Build the project**:
   - Click `Build` button or press `F7`
4. **Connect debugger**:
   - Connect J-Link to the SWD interface on the board
5. **Flash firmware**:
   - Click `Download` button or press `F8`

### Communication Test

1. Connect RS485 to your master controller
2. Set baudrate to 115200
3. Send test frame:
   ```
   Frame Head: AA BB CC DD
   Device Type: 01
   Device Address: 01
   CMD: 02 (Read Status)
   Data: 00 00 00 00 00 00 00
   CRC16: [calculated]
   Frame End: FE FD FC FB
   ```

***

## Default Configuration

| Parameter         | Default Value | Description                              |
| ----------------- | ------------- | ---------------------------------------- |
| RS485 Address     | `0x01`        | Device address (modifiable via CMD 0x05) |
| Broadcast Address | `0xFF`        | Broadcast address for all devices        |
| Baudrate          | 115200        | RS485 communication speed                |
| Device Type       | `0x01`        | Motor driver device type                 |
| Frame Head        | `0xDDCCBBAA`  | Protocol frame header                    |
| Frame End         | `0xFBFCFDFE`  | Protocol frame footer                    |
| Watchdog          | Enabled       | System watchdog timer                    |
| Max Channels      | 100           | Maximum motor channels (10x10)           |

***

## License

This project is licensed under the **Apache License 2.0**.

See [LICENSE](LICENSE) file for details.

***

## Notes

- X and Y axis mapping follows the schematic design
- Infrared sensor uses NPN signal type
- Motor feedback signals are active-low
- Ensure proper power supply current rating for all connected motors

