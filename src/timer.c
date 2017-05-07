#include <stringFormat.h>
#include <ts7200.h>
#include <ui.h>
#include <timer.h>
#include <trackControl.h>

void startTimer() {
    V int *load = (int *) (TIMER3_BASE + LDR_OFFSET);
    *load = 0xffffffff;

    V int *flags = (int *) (TIMER3_BASE + CRTL_OFFSET);
    V int buf = *flags;
    buf = buf | ENABLE_MASK;
    buf = buf & ~MODE_MASK; // mode=0, free running mode
    buf = buf | CLKSEL_MASK; // 1, the 508 kHz clock
    *flags = buf;
}

unsigned int readTimer() {
    V unsigned int *timerValue = (unsigned int *)(TIMER3_BASE + VAL_OFFSET);
    return *timerValue;
}

void processTime(globalsStruct* globals) {
    V timerStruct *timer = globals->timer;
    timer->ticksCounter = readTimer();
    if ((timer->previousTicksCounter - timer->ticksCounter) >= TICKS_PER_MS) {
        timer->msFromEpoch++;
        timer->previousTicksCounter = timer->ticksCounter;
        timer->timePrinted = 0;
        processTrackSendBuffer(globals);
    }
    if (timer->msFromEpoch % 100 == 0 && !timer->timePrinted) {
        printTimer(globals);
        printActiveCommand(globals);
    }
}

void printTimer(globalsStruct* globals) {
    V timerStruct *timer = globals->timer;
    V int tenthOfSeconds = (int) (timer->msFromEpoch / 100) % 10;
    V int seconds        = (int) (timer->msFromEpoch / 1000) % 60;
    V int minutes        = (int)  timer->msFromEpoch / (1000*60);

    moveCursorToTimerPosition(globals);
    bfprintf(globals, COM2, "Time: %0d:%0d.%d\n\r", minutes, seconds, tenthOfSeconds);
    timer->timePrinted = 1;
}
