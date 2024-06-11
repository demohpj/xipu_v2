TOOLS_UROM_DIR = ../tools/urom
TOOLS_ASM_DIR = ../tools/asm
TOOLS_EMU_DIR = ../tools/emu
TOOLS_FONTS_DIR = ../tools/fonts

ifeq ($(OS),Windows_NT)
	TOOLS_UROM_EXE = urom.exe
	TOOLS_ASM_EXE = asm.exe
	TOOLS_EMU_EXE = emu.exe
	
	TOOLS_LIB_DIR = ../tools/lib
else
	TOOLS_UROM_EXE = urom
	TOOLS_ASM_EXE = asm
	TOOLS_EMU_EXE = emu
endif

CPU_BIOS_DIR = ../cpu/bios
CPU_BIOS_BIN = bios.bin
CPU_BIOS_MAP = bios.map

SYS_OS_DIR = ../sys/os
SYS_OS_BIN = os.bin
SYS_OS_MAP = os.map

SYS_FS_DIR = ../sys/fs
SYS_FS_APP = app
SYS_FS_FILE = file

IO_FW_DIR = ../io/fw
IO_FW_HEX = Objects/io.hex

.PHONY: all
all: cpu_urom cpu_bios sys_fs io_fw tools

.PHONY: cpu_urom
cpu_urom: cpu tools
	rm -rf cpu/urom
	mkdir -p cpu/urom

ifeq ($(OS),Windows_NT)
ifdef WINE
	cd cpu/urom; \
	$(WINE) ../../tools/$(TOOLS_UROM_EXE)
else
	cd cpu/urom; \
	../../tools/$(TOOLS_UROM_EXE)
endif
else
	cd cpu/urom; \
	../../tools/$(TOOLS_UROM_EXE)
endif

.PHONY: cpu_bios
cpu_bios: cpu tools sys_map
	rm -rf cpu/bios
	$(MAKE) -C $(CPU_BIOS_DIR)
	mkdir -p cpu/bios
	cp $(CPU_BIOS_DIR)/$(CPU_BIOS_BIN) cpu/bios
	cp $(CPU_BIOS_DIR)/$(CPU_BIOS_MAP) sys/map
	
.PHONY: sys_os
sys_os: sys tools sys_fs_dirs cpu_bios sys_map
	$(MAKE) -C $(SYS_OS_DIR)
	cp $(SYS_OS_DIR)/$(SYS_OS_BIN) sys/fs
	cp $(SYS_OS_DIR)/$(SYS_OS_MAP) sys/map

.PHONY: sys_map
sys_map: sys
	rm -rf sys/map
	mkdir -p sys/map

.PHONY: sys_fs_dirs
sys_fs_dirs:
	rm -rf sys/fs
	mkdir -p sys/fs

.PHONY: sys_fs
sys_fs: sys tools sys_fs_dirs cpu_bios sys_os
	$(MAKE) -C $(SYS_FS_DIR)
	
	cp -r $(SYS_FS_DIR)/$(SYS_FS_APP) sys/fs
	cp -r $(SYS_FS_DIR)/$(SYS_FS_FILE) sys/fs

.PHONY: io_fw
io_fw: io
	rm -rf io/fw
	$(MAKE) -C $(IO_FW_DIR) src
	mkdir -p io/fw
	cp -f $(IO_FW_DIR)/src/$(IO_FW_HEX) io/fw | true

tools: tools_urom tools_asm tools_emu
	rm -rf tools
	mkdir -p tools
	cp $(TOOLS_UROM_DIR)/src/$(TOOLS_UROM_EXE) tools
	cp $(TOOLS_ASM_DIR)/src/$(TOOLS_ASM_EXE) tools
	cp $(TOOLS_EMU_DIR)/src/$(TOOLS_EMU_EXE) tools
	
	cp -r $(TOOLS_FONTS_DIR) tools
	
ifeq ($(OS),Windows_NT)
	cp -r $(TOOLS_LIB_DIR)/* tools
endif

cpu:
	rm -rf cpu
	mkdir -p cpu

sys:
	rm -rf sys
	mkdir -p sys

io:
	rm -rf io
	mkdir -p io

.PHONY: tools_asm
tools_asm:
	$(MAKE) -C $(TOOLS_ASM_DIR) src

.PHONY: tools_emu
tools_emu:
	$(MAKE) -C $(TOOLS_EMU_DIR) src

.PHONY: tools_urom
tools_urom:
	$(MAKE) -C $(TOOLS_UROM_DIR) src

.PHONY: clean
clean:
	$(MAKE) -C $(CPU_BIOS_DIR) clean
	$(MAKE) -C $(SYS_OS_DIR) clean
	$(MAKE) -C $(SYS_FS_DIR) clean
	$(MAKE) -C $(IO_FW_DIR) clean
	$(MAKE) -C $(TOOLS_UROM_DIR) clean
	$(MAKE) -C $(TOOLS_ASM_DIR) clean
	$(MAKE) -C $(TOOLS_EMU_DIR) clean
	
	rm -rf cpu
	rm -rf sys
	rm -rf io
	rm -rf tools
