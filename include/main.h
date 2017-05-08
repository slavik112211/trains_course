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

typedef struct {
    unsigned int ticksCounter;
    unsigned int previousTicksCounter;
    int msFromEpoch;
    int timePrinted;
} timerStruct;

typedef struct {
    timerStruct* timer;
    ringBuffer* outputBuffer;
    ringBuffer* inputBuffer;
    ringBuffer* trackSendBuffer;
    delayedRingBuffer* trackSendDelayedBuffer;
    char* inputCommand;
} globalsStruct;

#endif
