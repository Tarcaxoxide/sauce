
.PHONY: default compile setup clean

default: clean setup compile

compile:
	cd kernel && make clean
	cd kernel && make buildimg
	cd gnu-efi && make bootloader

setup:
	cd kernel && make setup
	cd gnu-efi && make

clean:
	cd kernel && make clean
	cd gnu-efi && make clean

run:
	cd kernel && make run