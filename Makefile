.PHONY: all
all: doc bin

.PHONY: doc
doc:
	$(MAKE) -C doc

.PHONY: bin
bin:
	$(MAKE) -C bin

.PHONY: clean
clean:
	$(MAKE) -C doc clean
	$(MAKE) -C bin clean
