#include <main.h>
#include <bufferIO.h>
#include <ui.h>
#include <ringBuffer.h>
#include <trackControl.h>
#include <ts7200.h>

/*
 * https://www.student.cs.uwaterloo.ca/~cs452/manual/trains.html
 * http://www.cgl.uwaterloo.ca/wmcowan/teaching/cs452/timeless/marklin/commands.html
 */

void initTrack(globalsStruct* globals) {
    setCOMBaudRate(COM1, 2400);
    initUART(COM1);
    startTrack(globals);
}

void startTrack(globalsStruct* globals) {
    ringBuffer_push(globals->trackSendBuffer, 0x60);
}

void stopTrack(globalsStruct* globals) {
    ringBuffer_push(globals->trackSendBuffer, 0x61);
}

void initUART(int channel) {
    int *line, buf;
    line = getUARTHighControl(channel);
    buf = *line;
    buf = buf & ~FEN_MASK; // FIFO off
    buf = buf | WLEN_MASK; // 8 bit per frame
    buf = buf | STP2_MASK; // Two stop bits are transmitted at the end of the frame
    buf = buf & ~PEN_MASK; // No parity bit is added to the data frame
    *line = buf;
}

void runTrain(globalsStruct* globals, int train, int speed) {
    if(speed < 0 || speed > 14) speed = 5;
    if(train < 0 || train > 100) train = 58;
    ringBuffer_push(globals->trackSendBuffer, speed);
    ringBuffer_push(globals->trackSendBuffer, train);
}

void stopTrain(globalsStruct* globals, int train) {
    if(train < 0 || train > 100) train = 58;
    ringBuffer_push(globals->trackSendBuffer, 0);
    ringBuffer_push(globals->trackSendBuffer, train);
}

void processTrackSendBuffer(globalsStruct* globals) {
    int *flags, *data;
    // V int i; char c;
    flags = getUARTFlags(COM1);
    data = getUARTData(COM1);
    if (ringBuffer_hasElements(globals->trackSendBuffer) == 1 && (*flags & TXFE_MASK)) {
        *data = ringBuffer_pop(globals->trackSendBuffer);
        // printDebug(globals, 5, 5, c);
        // for(i=0; i<5000; i++) {}
        // while( ( *flags & TXFF_MASK ) ) ;
    }
}
