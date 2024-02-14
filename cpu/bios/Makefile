ifeq ($(OS),Windows_NT)
ifdef WINE
	ASM = $(WINE) ../../bin/tools/asm.exe
else
	ASM = ../../bin/tools/asm.exe
endif
else
	ASM = ../../bin/tools/asm
endif

.PHONY: all
all: bios.bin

bios.bin:
	cd .; \
	$(ASM) bios src/bios.asm bios.bin bios.map

.PHONY: clean
clean:
	rm -f bios.bin
	rm -f bios.map
