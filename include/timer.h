#ifndef _TIMER_INCL_GUARD
#define _TIMER_INCL_GUARD

#include <main.h>

#define TICKS_PER_MS 508

void initTimer(timerStruct *timer);
void startTimer(globalsStruct*);
unsigned int readTimer();
void printTimer(globalsStruct*);
void processTime(globalsStruct*);

#endif
