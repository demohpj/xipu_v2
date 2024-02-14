QMAKE ?= qmake

QT_PRO = asm.pro

.PHONY: all
all: src doc

.PHONY: src
src: qmake
	$(MAKE) -C src

.PHONY: doc
doc:
	$(MAKE) -C doc

.PHONY: qmake
qmake:
	cd src; \
	$(QMAKE) $(QT_PRO)

.PHONY: clean
clean: clean_src clean_doc
	
.PHONY: clean_src
clean_src: qmake
	$(MAKE) -C src clean

.PHONY: clean_doc
clean_doc:
	$(MAKE) -C doc clean
