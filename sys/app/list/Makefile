ifeq ($(OS),Windows_NT)
ifdef WINE
	ASM = $(WINE) ../../../bin/tools/asm.exe
else
	ASM = ../../../bin/tools/asm.exe
endif
else
	ASM = ../../../bin/tools/asm
endif

NAME = list

.PHONY: all
all: $(NAME)

$(NAME): src/*.asm
	cd .; \
	$(ASM) app src/$(NAME).asm $(NAME)

.PHONY: clean
clean:
	rm -f $(NAME)
