#ifndef _MAIN_INCL_GUARD
#define _MAIN_INCL_GUARD

#include <ringBuffer.h>

#define NULL (void *)0

#define FOREVER for( ; ; )
#define V volatile

#define COM1	0
#define COM2	1

#define ON	1
#define	OFF	0

#define SWITCHES_ON_TRACK 22 // catch 22! ;)
#define TOTAL_SENSORS 5

typedef struct {
    unsigned int ticksCounter;
    unsigned int previousTicksCounter;
    int msFromEpoch;
    int timePrinted;
} timerStruct;

typedef struct {
    unsigned int requestSent;
    unsigned int bytesReceived;
} sensorsParamsStruct;

typedef struct {
    timerStruct* timer;
    
    // COM2 buffers
    ringBuffer* outputBuffer;
    ringBuffer* inputBuffer;

    // COM1 buffers
    trackSendBufferStruct* trackSendBuffer;
    delayedRingBuffer* trackSendDelayedBuffer;

    char* inputCommand;
    int switches[SWITCHES_ON_TRACK];
    int counterCOM1;
    sensorsParamsStruct* sensorsParams;
} globalsStruct;

#endif
