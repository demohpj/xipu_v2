TOOLS_UROM_DOC_DIR = ../tools/urom/doc
TOOLS_ASM_DOC_DIR = ../tools/asm/doc
TOOLS_EMU_DOC_DIR = ../tools/emu/doc
IO_FW_DOC_DIR = ../io/fw/doc

.PHONY: all
all: tools_urom tools_asm tools_emu io_fw

.PHONY: tools_urom
tools_urom: tools
	rm -rf tools/urom
	$(MAKE) -C $(TOOLS_UROM_DOC_DIR)
	cp -R $(TOOLS_UROM_DOC_DIR)/html tools/urom

.PHONY: tools_asm
tools_asm: tools
	rm -rf tools/asm
	$(MAKE) -C $(TOOLS_ASM_DOC_DIR)
	cp -R $(TOOLS_ASM_DOC_DIR)/html tools/asm

.PHONY: tools_emu
tools_emu: tools
	rm -rf tools/emu
	$(MAKE) -C $(TOOLS_EMU_DOC_DIR)
	cp -R $(TOOLS_EMU_DOC_DIR)/html tools/emu

.PHONY: io_fw
io_fw: io
	rm -rf io/fw
	$(MAKE) -C $(IO_FW_DOC_DIR)
	cp -R $(IO_FW_DOC_DIR)/html io/fw

tools:
	mkdir -p tools

io:
	mkdir -p io

.PHONY: clean
clean:
	$(MAKE) -C $(TOOLS_UROM_DOC_DIR) clean
	$(MAKE) -C $(TOOLS_ASM_DOC_DIR) clean
	$(MAKE) -C $(TOOLS_EMU_DOC_DIR) clean
	$(MAKE) -C $(IO_FW_DOC_DIR) clean
	
	rm -rf tools
	rm -rf io
