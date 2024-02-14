SUBDIRS := $(wildcard */.)

.PHONY: all
all:
	for dir in $(SUBDIRS); do \
		($(MAKE) -C $$dir); \
	done

.PHONY: clean
clean:
	for dir in $(SUBDIRS); do \
		($(MAKE) -C $$dir clean); \
	done

