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
all: os.bin

os.bin: src/*.asm
	cd .; \
	$(ASM) os src/os.asm os.bin os.map

.PHONY: clean
clean:
	rm -f os.bin
	rm -f os.map
