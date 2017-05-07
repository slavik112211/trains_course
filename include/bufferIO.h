#ifndef _IO_INCL_GUARD
#define _IO_INCL_GUARD

#include <main.h>

void processOutputBuffer(globalsStruct* globals, int channel);
int processInputBuffer(globalsStruct* globals, int channel);
int processInputChar(globalsStruct* globals, char c);
int putc(globalsStruct* globals, int channel, char c);
void copyActiveCommandFromInputBuffer(globalsStruct* globals);

// COM ports control functions:
int* getUARTHighControl(int channel);
int setFIFO( int channel, int state );
int setCOMBaudRate( int channel, int speed );
int* getUARTFlags(int channel);
int* getUARTData(int channel);

#endif
