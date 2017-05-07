#ifndef _IO_INCL_GUARD
#define _IO_INCL_GUARD

#include <main.h>

int processInputBuffer(globalsStruct* globals, int channel);
void processOutputBuffer(globalsStruct* globals, int channel);
int putc( globalsStruct* globals, int channel, char c );

// COM ports control functions:
int* getUARTHighControl(int channel);
int setFIFO( int channel, int state );
int setCOMBaudRate( int channel, int speed );
int* getUARTFlags(int channel);
int* getUARTData(int channel);

#endif
