#ifndef _TIMER_INCL_GUARD
#define _TIMER_INCL_GUARD

#include <main.h>

#define TICKS_PER_MS 508

void startTimer();
unsigned int readTimer();
void printTimer(V globalsStruct*);
void processTime(V globalsStruct*);

#endif
