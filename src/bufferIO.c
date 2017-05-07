#include <main.h>
#include <bufferIO.h>
#include <ringBuffer.h>
#include <ts7200.h>
#include <ui.h>
#include <trackControl.h>

void processOutputBuffer(globalsStruct* globals, int channel) {
    int *flags, *data;
    flags = getUARTFlags(COM2);
    data = getUARTData(COM2);
    if (ringBuffer_hasElements(globals->outputBuffer) == 1 && (*flags & TXFE_MASK)) {
        *data = ringBuffer_pop(globals->outputBuffer);
    }
}

int processInputBuffer(globalsStruct* globals, int channel) {
    int *flags, *data, flagsBuffer;
    unsigned char c;
    flags = getUARTFlags(COM2);
    data = getUARTData(COM2);

    if(*flags & RXFF_MASK) {
        c = *data;
        flagsBuffer = *flags;
        flagsBuffer = flagsBuffer & ~RXFF_MASK; // set 'Receive register full' to OFF
        flagsBuffer = flagsBuffer | RXFE_MASK;  // set 'Receive register empty' to ON
        *flags = flagsBuffer;

        if(c == 3) return -1; // Ctrl-C
        if(c != 13) { // Carriage return
            ringBuffer_push(globals->inputBuffer, c);
        } else {
            clearCommandPrompt(globals);
            printPreviousCommand(globals);
            globals->inputBuffer->readIndex = globals->inputBuffer->writeIndex;
            runTrain(globals, 70, 12);
        }
    }
    return 0;
}

int putc(globalsStruct* globals, int channel, char c) {
    ringBuffer_push(globals->outputBuffer, c);
    return 0;
}

// Busy-wait putc()
//
// int putc(globalsStruct* globals, int channel, char c) {
//     int *flags, *data;
//     flags = getUARTFlags(channel);
//     data = getUARTData(channel);
//     while( ( *flags & TXFF_MASK ) ) ;
//     *data = c;
//     return 0;
// }

/*
 * COM ports control functions. UARTs - memory registers used to push data over COM ports. 
 * Code taken from the busy-wait IO library bwio.c.
 * 
 * Specific to the TS-7200 ARM evaluation board.
 * 
 * The UARTs are initialized by RedBoot to the following state
 *  115,200 bps, 8 bits words, no parity bits, fifos enabled.
 */

int* getUARTHighControl(int channel) {
    int *flags;
    switch( channel ) {
    case COM1:
        flags = (int *)( UART1_BASE + UART_LCRH_OFFSET );
        break;
    case COM2:
        flags = (int *)( UART2_BASE + UART_LCRH_OFFSET );
        break;
    default:
        return NULL;
        break;
    }
    return flags;
}

int setFIFO(int channel, int state) {
    int *line, buf;
    line = getUARTHighControl(channel);
    buf = *line;
    buf = state ? buf | FEN_MASK : buf & ~FEN_MASK;
    *line = buf;
    return 0;
}

int setCOMBaudRate(int channel, int rate) {
    int *high, *low;
    switch( channel ) {
    case COM1:
        high = (int *)( UART1_BASE + UART_LCRM_OFFSET );
        low = (int *)( UART1_BASE + UART_LCRL_OFFSET );
            break;
    case COM2:
        high = (int *)( UART2_BASE + UART_LCRM_OFFSET );
        low = (int *)( UART2_BASE + UART_LCRL_OFFSET );
            break;
    default:
            return -1;
            break;
    }
    switch( rate ) {
    case 115200:
        *high = 0x0;
        *low = 0x3;  // 0000_0011
        return 0;
    case 2400:
        *high = 0x0;
        *low = 0xbf; // 1001_0000 = 144; 1010_1111 = 191.
        return 0;
    default:
        return -1;
    }
}

int* getUARTFlags(int channel) {
    int *flags;
    switch( channel ) {
    case COM1:
        flags = (int *)( UART1_BASE + UART_FLAG_OFFSET );
        break;
    case COM2:
        flags = (int *)( UART2_BASE + UART_FLAG_OFFSET );
        break;
    default:
        return NULL;
        break;
    }
    return flags;
}

int* getUARTData(int channel) {
    int *data;
    switch( channel ) {
    case COM1:
        data = (int *)( UART1_BASE + UART_DATA_OFFSET );
        break;
    case COM2:
        data = (int *)( UART2_BASE + UART_DATA_OFFSET );
        break;
    default:
        return NULL;
        break;
    }
    return data;
}
