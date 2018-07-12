.PHONY: all
.PHONY: clean

all: gc wii

.PHONY: gc
gc:
	make -f Makefile.gc

.PHONY: wii
wii:
	make -f Makefile.wii

clean:
	rm -rf build
	rm *.dol
	rm *.elf