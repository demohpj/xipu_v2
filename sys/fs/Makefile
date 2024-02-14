APP_SRC_DIR = ../app

SUBDIRS := $(wildcard $(APP_SRC_DIR)/*/.)

.PHONY: all
all: app file

.PHONY: app
app:
	rm -rf app
	$(MAKE) -C $(APP_SRC_DIR)
	mkdir -p app

	for dir in $(subst /.,,$(subst $(APP_SRC_DIR)/,,$(SUBDIRS))); do \
		cp $(APP_SRC_DIR)/$$dir/$$dir app; \
	done

file:
	mkdir -p file

.PHONY: clean
clean:
	$(MAKE) -C $(APP_SRC_DIR) clean
	rm -rf app

