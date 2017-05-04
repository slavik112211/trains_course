#include <main.h>

#ifndef _TIMER_INCL_GUARD
#define _TIMER_INCL_GUARD

#define TICKS_PER_MS 508

typedef struct {
    int ticksCounter;
    int previousTicksCounter;
    int msFromEpoch;
    int timePrinted;
} timerStruct;

void startTimer();
int readTimer();
void printTimer(V timerStruct*);
void processTime(V timerStruct*);

#endif
