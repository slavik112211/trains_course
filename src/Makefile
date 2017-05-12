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

all: timer.s stringFormat.s bufferIO.s ringBuffer.s trackControl.s commandParser.s main.s main.elf

main.s: main.c
	$(XCC) -S $(CFLAGS) main.c

main.o: main.s
	$(AS) $(ASFLAGS) -o main.o main.s

main.elf: main.o timer.o ringBuffer.o stringFormat.o bufferIO.o ui.o trackControl.o commandParser.o sensors.o
	$(LD) $(LDFLAGS) -o $@ main.o timer.o ringBuffer.o stringFormat.o bufferIO.o ui.o trackControl.o commandParser.o sensors.o -lgcc

timer.s: timer.c
	$(XCC) -S $(CFLAGS) timer.c

timer.o: timer.s
	$(AS) $(ASFLAGS) -o timer.o timer.s

bufferIO.s: bufferIO.c
	$(XCC) -S $(CFLAGS) bufferIO.c

bufferIO.o: bufferIO.s
	$(AS) $(ASFLAGS) -o bufferIO.o bufferIO.s

stringFormat.s: stringFormat.c
	$(XCC) -S $(CFLAGS) stringFormat.c

stringFormat.o: stringFormat.s
	$(AS) $(ASFLAGS) -o stringFormat.o stringFormat.s

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

commandParser.s: commandParser.c
	$(XCC) -S $(CFLAGS) commandParser.c

commandParser.o: commandParser.s
	$(AS) $(ASFLAGS) -o commandParser.o commandParser.s

sensors.s: sensors.c
	$(XCC) -S $(CFLAGS) sensors.c

sensors.o: sensors.s
	$(AS) $(ASFLAGS) -o sensors.o sensors.s

clean:
	-rm -f main.elf *.s *.o main.map
