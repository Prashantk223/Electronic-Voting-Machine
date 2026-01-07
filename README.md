# Electronic Voting Machine (EVM) – Embedded Systems Project

Designed and implemented a complete embedded **Electronic Voting Machine (EVM)** system covering application logic, firmware, BSP, scheduling, and a PC-based graphical interface.

This project demonstrates **full-stack embedded development**, including hardware abstraction, task scheduling, mode management, and UART-based communication with a Python/HTML interface.

---

## System Architecture & Core Design

- Implemented core application logic with **event-driven state management**
- Supported two operating modes:
  - Configuration Mode
  - Normal Voting Mode
- Startup mode selection based on a **hardware configuration pin**
- Designed robust state transitions:
  - Initialization
  - Configuration
  - Normal operation
  - Shutdown

---

## Firmware & BSP Development

### Board Support Package (BSP)
- Developed low-level hardware abstraction
- Implemented **16×2 LCD driver**

### Firmware Drivers
- GPIO driver
- UART RX/TX driver for host communication
- Timer driver
- USART driver for HTML/Python interface integration

- Maintained **clean separation** between:
  - BSP
  - Firmware drivers
  - Scheduler
  - Application logic

---

## Scheduler Implementation

- Designed and implemented two schedulers:
  - **Cooperative Scheduler**
  - **Priority-based Preemptive Scheduler**
- Scheduler handled:
  - Time-critical tasks
  - Input handling
  - Display updates
  - Communication events

---

## Configuration Mode Features

### Menu-driven Configuration Interface
- Display vote counts
- Switch between configuration and normal voting modes
- Reset all stored voting data
- Shutdown system
- Connect to UART-based HTML server

### Data Management
- Set and reset candidate names
- Clear persistent vote data

### PC-Based Interface
- Enabled monitoring and control via Python/HTML interface over UART

---

## Technologies Used

- Microcontroller: **STM32F407**
- Language: **C**
- Interfaces: GPIO, UART, USART, Timers
- PC Interface: Python + HTML
- Architecture: Bare-metal embedded system
