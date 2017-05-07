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

LDFLAGS = -init main -Map main.map -N  -T orex.ld -L/u/wbcowan/gnuarm-4.0.2/lib/gcc/arm-elf/4.0.2

all: timer.s io.s ringBuffer.s trackControl.s main.s main.elf

main.s: main.c
	$(XCC) -S $(CFLAGS) main.c

main.o: main.s
	$(AS) $(ASFLAGS) -o main.o main.s

main.elf: main.o timer.o ringBuffer.o io.o ui.o trackControl.o
	$(LD) $(LDFLAGS) -o $@ main.o timer.o ringBuffer.o io.o ui.o trackControl.o -lgcc

timer.s: timer.c
	$(XCC) -S $(CFLAGS) timer.c

timer.o: timer.s
	$(AS) $(ASFLAGS) -o timer.o timer.s

io.s: io.c
	$(XCC) -S $(CFLAGS) io.c

io.o: io.s
	$(AS) $(ASFLAGS) -o io.o io.s

ringBuffer.s: ringBuffer.c
	$(XCC) -S $(CFLAGS) ringBuffer.c

ringBuffer.o: ringBuffer.s
	$(AS) $(ASFLAGS) -o ringBuffer.o ringBuffer.s

ui.s: ui.c
	$(XCC) -S $(CFLAGS) ui.c

ui.o: ui.s
	$(AS) $(ASFLAGS) -o ui.o ui.s

trackControl.s: trackControl.c
	$(XCC) -S $(CFLAGS) trackControl.c

trackControl.o: trackControl.s
	$(AS) $(ASFLAGS) -o trackControl.o trackControl.s

clean:
	-rm -f main.elf *.s *.o main.map
