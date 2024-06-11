UV4 ?= UV4.exe

FW_HEX = src/Objects/io.hex
FW_PRO = io.uvprojx
FW_LOG = build.log

.PHONY: all
all: src doc

.PHONY: src
src: $(FW_HEX)

$(FW_HEX): src/*.c src/*.h
ifeq ($(OS),Windows_NT)
ifndef WINE
	cd src; \
	$(UV4) -j0 -b $(FW_PRO) -o $(FW_LOG)
endif
endif

.PHONY: doc
doc:
	$(MAKE) -C doc

.PHONY: clean
clean: clean_src clean_doc
	
.PHONY: clean_src
clean_src:
	cd src; \
	rm -rf DebugConfig; \
	rm -rf Listings; \
	rm -rf Objects; \
	rm -rf $(FW_LOG)

.PHONY: clean_doc
clean_doc:
	$(MAKE) -C doc clean
