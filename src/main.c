#include <main.h>
#include <timer.h>
#include <bufferIO.h>
#include <ui.h>
#include <ringBuffer.h>
#include <trackControl.h>

int main(int argc, char* argv[]) {
    globalsStruct globals;
    timerStruct timer = {0, 0xffffffff, 0, 0};
    V char outputBufferContainer[20000];
    ringBuffer outputBuffer = {(char*) &outputBufferContainer, 20000, 0, 0};

    V char inputBufferContainer[1024];
    ringBuffer inputBuffer = {(char*) &inputBufferContainer, 1024, 0, 0};

    V char trackSendContainer[1024];
    ringBuffer trackSendBuffer = {(char*) &trackSendContainer, 1024, 0, 0};

    globals.timer = &timer;
    globals.outputBuffer = &outputBuffer;
    globals.inputBuffer = &inputBuffer;
    globals.trackSendBuffer = &trackSendBuffer;

    drawFrame(&globals);
    printPrompt(&globals);
    startTimer();
    setFIFO(COM2, OFF);
    initTrack(&globals);

    V int status;
    FOREVER {
        processTime(&globals);
        processOutputBuffer(&globals, COM2);
        status = processInputBuffer(&globals, COM2);
        if(status == -1) break;
    }
    return 0;
}
