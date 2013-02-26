
ARMGNU ?= arm-none-eabi

COPS = -std=c99 -Wall -O2 -nostdlib -nostartfiles -ffreestanding 

gcc : blinker07.hex blinker07.bin

all : gcc 

clean :
	rm -f *.o
	rm -f *.bin
	rm -f *.hex
	rm -f *.elf
#	rm -f *.list
	rm -f *.img
	rm -f *.bc
	rm -f *.clang.opt.s

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

systimer.o : systimer.c
	$(ARMGNU)-gcc $(COPS) -c systimer.c -o systimer.o

#miniuart.o : miniuart.c
#	$(ARMGNU)-gcc $(COPS) -c miniuart.c -o miniuart.o

biguart.o : biguart.c
	$(ARMGNU)-gcc $(COPS) -c biguart.c -o biguart.o

vic.o : vic.c
	$(ARMGNU)-gcc $(COPS) -c vic.c -o vic.o

interrupt.o : interrupt.c
	$(ARMGNU)-gcc $(COPS) -c interrupt.c -o interrupt.o

blinker07.o : blinker07.c
	$(ARMGNU)-gcc $(COPS) -c blinker07.c -o blinker07.o

blinker07.elf : memmap vectors.o blinker07.o biguart.o systimer.o vic.o interrupt.o
	$(ARMGNU)-ld vectors.o blinker07.o biguart.o systimer.o vic.o interrupt.o -T memmap -o blinker07.elf
	$(ARMGNU)-objdump -D blinker07.elf > blinker07.list

blinker07.bin : blinker07.elf
	$(ARMGNU)-objcopy blinker07.elf -O binary blinker07.bin

blinker07.hex : blinker07.elf
	$(ARMGNU)-objcopy blinker07.elf -O ihex blinker07.hex






