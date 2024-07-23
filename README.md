# XiPU_v2

XiPU_v2 is an 8-bit 7400 TTL CPU project. It is a second iteration of the XiPU project.

## 1. About the project

The XiPU_v2 is loosely based on MOS 6502 and early x86. It was my starting point of design my own CPU. For now, the XiPU_v2 has many differences and it is so much simpler than those CPUs.

The second version of the XiPU has extended instruction set, new flat memory structure, advanced stack and function frame operations support.

The main goal of the project is creating a fully functional 8-bit 7400 TTL CPU and a simple computer based on the one.

### 1.1. Author

- Pawel Jablonski (pj@xirx.net)

### 1.2. License

You can use this code however you like but leave information about the original author. Code is free for non-commercial and commercial use.

Please see the LICENSE.txt file.

### 1.3. Links

- WWW: [xirx.net](https://xirx.net "xirx.net")
- GIT: [git.xirx.net](https://git.xirx.net "git.xirx.net")

## 2. Useful knowledge

### 2.1. Dictionary

- XiPU - XiRX Processor Unit. The name of the 8-bit TTL CPU.
- XiPU_v2 - The second iteration of the XiPU.
- XiPC - XiRX Personal Computer. The name of the simple computer based on XiPU CPU
- TTL - Transistor-transistor logic.
- HCT - High-Speed CMOS chips available in TTL compatible form.
- RAM - Volatile random access memory.
- ROM - Non-volatile read-only memory.
- Register - Quickly accessible location for a small amount of data stored in a TTL chip.
- uROM - uROM determines which of the internal registers is used to read from and write to at the current micro step of executing instruction.
- BIOS - Basic input/output system. Provides access to basic IO API and loads the OS.
- OS - Operating System. It contains a terminal interface and basic libraries.
- Stack - High accessibly memory page where data is stored as FILO.
- Function Frame - Part of the stack where arguments of the function and local variables are stored.
- IO operation - Read or write data to pins connected to external world.
- External BUS - Data highway for IO operations.
- ALU - Arithmetic logic unit.

### 2.2. XiPU_v2

- 8-bit processor based on 7400 HCT chips compatible with TTL.
- 64 kB of RAM space. Lower 2 kB are reserved for BIOS stored at EEPROM. The 4 kB at the end of the memory are used as a stack. Rest 58 kB of memory could be used by OS or applications.
- 1 MHz clock speed. The processor uses a 2 MHz crystal to get two 1 MHz square waves shifted relative to the other about half.
- The processor uses uROM stored in two EEPROMs.
- Each instruction could be built from 2 to 16 micro steps. Each micro step takes exactly 1 clock tick.
- Four registers:
	- A, B - 8-bit general purpose registers.
	- X, Y - 8-bit auxiliary registers. Could be used mostly as general purpose registers. Together, it can be used as a 16-bit address register.
- Two hidden stack registers:
	- SP - Stack Pointer. It is used to address the head of the stack.
	- BP - Base Pointer. It is used to address the function frame on the stack to store parameters of a function and local variables.
- Three internal buses:
	- BUS A - 8-bit main BUS for registers, RAM, ROM, IO operations.
	- BUS B - 8-bit auxiliary BUS for registers used as second input for ALU.
	- BUS C - 16-bit memory address BUS for addressing RAM.
- 8-bit IN and 8-bit OUT external data BUS. It is used as a simple communication way with the external word.

### 2.3. XiPC

- Personal computer based on the XiPU.
- 40x30 characters with 16 colors support. It uses 640x480 LCD with an 8-bit color palette to render image output.
- Mono speaker with a volume knob.
- RS-232 with 1200 bps support.
- RTC with battery support.
- Built-in QWERTY keyboard.
- 4 status LEDs.
- 7.5V max 1.0A power supply.

## 3. Project overview

### 3.1. Requirements

The project supports building process on Windows and Linux.

- Geany - Multi-language IDE. Used to create Makefile, assembler code etc.
- QtCreator - IDE for C++/Qt. Used to create desktop applications.
- Qt 5.12.4 - Multiplatform Qt 5 library. It supports UI, data structures, files etc.
- GCC - C and C++ code compiler.
- Keil uVision 5.37 - Compiler and debugger for the STM32F0 MCU.
- Make - Build automation tool.
- Doxygen 1.9.7 - Documentation generator from annotated sources.
- Proteus 8.10 - Circuit simulator software. Used to simulate the XiPU.
- QCAD - DXF files editor. Used to create Plexiglas case for the XiPC.
- DipTrace 4.2.0.1 - Schematic and PCB design software.

### 3.2. Project structure

```console
.
├── LICENSE.txt    # License file.
├── Makefile       # Main Makefile file to building binaries and documentations at once.
├── README.md      # Readme file.
├── bin            # Destination directory for all generated binaries during the build process.
├── case
│   ├── 3d         # A 3D printed case project for the XiPC.
│   └── plexiglass # A simple Plexiglas case project for the XiPC.
├── cpu
│   ├── bios       # BIOS with a loader and IO API for the OS.
│   ├── pcb        # PCB design for the XiPU.
│   └── sim        # Simulation project of the XiPU.
├── doc            # Destination directory for all generated documentations during the build process.
├── ide
│   ├── geany      # IDE configuration to support assembler for the XiPU.
│   └── qtcreator  # IDE configuration for code rules.
├── io
│   ├── fw         # Firmware for the IO Board.
│   └── pcb        # PCB design for the IO Board.
├── kbd
│   └── pcb        # PCB design for the Keyboard.
├── sys
│   ├── app        # Applications written for the XiPC.
│   ├── fs         # File System generator needed by the real XiPC and the XiPC Emulator.
│   └── os         # OS with a simple terminal and basic libraries for applications.
└── tools
    ├── asm        # Assembler Compiler.
    ├── emu        # XiPC Emulator.
    ├── fonts      # Fonts used in desktop GUI applications.
    ├── lib        # Binary libraries needed to run compiled applications on Windows x64.
    └── urom       # uROM generator.
```

## 4. Compiling

### 4.1. Build all

To build all binaries and create documentations for them, please type:

```console
make
```

### 4.2. Build a single module

#### 4.2.1 uROM

To build the application and create documentation, please type:

```console
cd tools/urom
make
```

#### 4.2.2. ASM Compiler

To build the application and create documentation, please type:

```console
cd tools/asm
make
```

#### 4.2.3. BIOS

To build the BIOS image file, the map file with API handlers, and create documentation, please type:

```console
cd cpu/bios
make
```

#### 4.2.4. OS

To build the OS, the map file with API handlers, please type:

```console
cd sys/os
make
```

#### 4.2.5. IO Board firmware

To build the IO Board firmware file, and create documentation, please type:

```console
cd io/fw
make
```
To build successfully a firmware is needed the Keil uVision5 with a free license for STM32F0 MCU. UV4.exe must be added to the PATH env.

#### 4.2.6. Applications

To build the user applications, please type:

```console
cd sys/app
make
```

#### 4.2.7. File System

To generate a directory with the virtual file system, please type:

```console
cd sys/fs
make
```

#### 4.2.8. XiPC Emulator

To build the application and create documentation, please type:

```console
cd tools/emu
make
```

## 5. Using

### 5.1. uROM

To generate uROM files, please type:

```console
./urom
```

### 5.2. ASM Compiler

To compile BIOS, please type:

```console
./asm bios [input_file] [output_bin] [output_map]
```

To compile OS, please type:

```console
./asm os [input_file] [output_bin] [output_map]
```

To compile an application, please type:

```console
./asm app [input_file] [output_bin]
```

#### 5.3. XiPC Emulator

To run the XiPC Emulator application, please type:

```console
./emu
```

It will show a window of the emulator ready to work. Next step is loading uROM files, BIOS file and choose the directory with the virtual file system. After that, the application is ready to run emulation.
