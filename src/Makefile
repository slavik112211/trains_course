#
# Makefile
#
XCC     = gcc
AS	= as
LD      = ld
CFLAGS  = -c -fPIC -Wall -I. -I../include -mcpu=arm920t -msoft-float
# -g: include hooks for gdb
# -c: only compile
# -mcpu=arm920t: generate code for the 920t architecture
# -fpic: emit position-independent code
# -Wall: report all warnings

ASFLAGS	= -mcpu=arm920t -mapcs-32
# -mapcs: always generate a complete stack frame

LDFLAGS = -init main -Map main.map -N  -T orex.ld -L/u/wbcowan/gnuarm-4.0.2/lib/gcc/arm-elf/4.0.2 -L../lib

all: timer.s main.s main.elf

main.s: main.c main.h
	$(XCC) -S $(CFLAGS) main.c

main.o: main.s
	$(AS) $(ASFLAGS) -o main.o main.s

main.elf: main.o timer.o
	$(LD) $(LDFLAGS) -o $@ main.o timer.o -lbwio -lgcc

timer.s: timer.c timer.h
	$(XCC) -S $(CFLAGS) timer.c

timer.o: timer.s
	$(AS) $(ASFLAGS) -o timer.o timer.s

clean:
	-rm -f main.elf *.s *.o main.map
