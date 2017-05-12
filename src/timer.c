#include <stringFormat.h>
#include <ts7200.h>
#include <ui.h>
#include <timer.h>
#include <trackControl.h>
#include <sensors.h>

void initTimer(timerStruct *timer) {
    timer->ticksCounter = 0;
    timer->previousTicksCounter = 0xffffffff;
    timer->msFromEpoch = 0;
    timer->timePrinted = 0;
}

void startTimer(globalsStruct* globals) {
    initTimer(globals->timer);
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
        processTrackSendDelayedBuffer(globals);
        processTrackReceiveBuffer(globals);
    }
    if (timer->msFromEpoch % 100 == 0 && !timer->timePrinted) {
        printTimer(globals);
        printActiveCommand(globals);
    }
    if (timer->msFromEpoch % 300 == 0) {
        sendSensorsDumpRequest(globals);
    }
}

void printTimer(globalsStruct* globals) {
    V timerStruct *timer = globals->timer;
    V int tenthOfSeconds = (int) (timer->msFromEpoch / 100) % 10;
    V int seconds        = (int) (timer->msFromEpoch / 1000) % 60;
    V int minutes        = (int) (timer->msFromEpoch / (1000*60));

    moveCursorToTimerPosition(globals);
    // putr(globals, COM2, timer->msFromEpoch);
    bfprintf(globals, COM2, "Time: %0d:%0d.%d", minutes, seconds, tenthOfSeconds);
    timer->timePrinted = 1;
}
