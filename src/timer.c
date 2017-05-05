#include <bwio.h>
#include <ts7200.h>
#include <timer.h>

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

int readTimer() {
    V int *timerValue = (int *)(TIMER3_BASE + VAL_OFFSET);
    return *timerValue;
}

void processTime(V timerStruct *timer) {
    timer->ticksCounter = readTimer();
    if ((timer->previousTicksCounter - timer->ticksCounter) >= TICKS_PER_MS) {
        timer->msFromEpoch++;
        timer->previousTicksCounter = timer->ticksCounter;
        timer->timePrinted = 0;
    }
    if (timer->msFromEpoch % 100 == 0 && !timer->timePrinted) {
        printTimer(timer);
    }
}

void printTimer(V timerStruct *timer) {
    V int tenthOfSeconds = (int) (timer->msFromEpoch / 100) % 10;
    V int seconds        = (int) (timer->msFromEpoch / 1000) % 60;
    V int minutes        = (int)  timer->msFromEpoch / (1000*60);

    bwprintf(COM2, "%d:%d.%d\n\r", minutes, seconds, tenthOfSeconds);
    timer->timePrinted = 1;
}
