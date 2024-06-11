DOXYGEN = doxygen

DOC_DIR = html
DOC_TAG = doxygen.tag
DOC_LOG = warnings.log

SRC_FILES := $(wildcard ../src/*.c ../src/*.h)

.PHONY: all
all: html

html: $(SRC_FILES)
	rm -rf html
	$(DOXYGEN) Doxyfile

.PHONY: clean
clean:
	rm -rf $(DOC_DIR)
	rm -f $(DOC_TAG)
	rm -f $(DOC_LOG)
