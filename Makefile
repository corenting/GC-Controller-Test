.PHONY: all
.PHONY: clean

all: switch gc wii

.PHONY: gc
gc:
	make -f Makefile.gc

.PHONY: wii
wii:
	make -f Makefile.wii

.PHONY: switch
switch:
	make -f Makefile.switch

clean:
	rm -rf build_gc
	rm -rf build_wii
	rm -rf build_switch
	rm *.dol
	rm *.elf
	rm *.nacp
	rm *.nro
	rm *.nso
	rm *.pfs0