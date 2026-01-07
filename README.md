Designed and implemented a complete embedded Electronic Voting Machine system, covering application logic, firmware, BSP, scheduling, and a PC-based graphical interface.
The project demonstrates full-stack embedded development, including hardware abstraction, task scheduling, mode management, and UART-based communication with a Python/HTML interface.
A. System Architecture & Core Design
    Implemented core application logic with event management and mode/state management (Configuration Mode and Normal Voting Mode).
    Startup behavior determined by a configuration pin, dynamically selecting between configuration and normal operating modes.
    Designed robust state transitions between initialization, configuration, normal operation, and shutdown.
B. Firmware & BSP Development
    1. Developed BSP drivers including 16×2 LCD driver and low-level hardware abstraction.
    2. Implemented firmware drivers:
        GPIO driver
        RX/TX (UART) driver for host communication
        Timer driver
        USART driver for HTML interface integration
    3. Ensured clean separation between BSP, firmware drivers, scheduler, and application logic.
C. Scheduler Implementation
    1. Designed and implemented two schedulers:
        Cooperative Scheduler
        Priority-based Preemptive Scheduler
    2. Scheduler managed time-critical tasks, input handling, display updates, and communication events.
D. Configuration Mode Features
    1. Menu-driven configuration interface:
        Display vote counts
        Switch between configuration and normal modes
        Reset all stored voting data
        Shutdown system
        Connect to UART-based HTML server
    2. Allowed setting and resetting candidate names and clearing persistent vote data.
    3. Enabled communication with PC-based interface for monitoring and control.
