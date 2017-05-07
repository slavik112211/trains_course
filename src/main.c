#include <main.h>
#include <timer.h>
#include <bufferIO.h>
#include <ui.h>
#include <ringBuffer.h>
#include <trackControl.h>

int main(int argc, char* argv[]) {
    globalsStruct globals;
    timerStruct timer;
    V char outputBufferContainer[20000];
    ringBuffer outputBuffer = {(char*) &outputBufferContainer, 20000, 0, 0};
    V char inputBufferContainer[1024];
    ringBuffer inputBuffer = {(char*) &inputBufferContainer, 1024, 0, 0};
    V char trackSendContainer[1024];
    ringBuffer trackSendBuffer = {(char*) &trackSendContainer, 1024, 0, 0};
    char inputCommand[100];

    globals.timer = &timer;
    globals.outputBuffer = &outputBuffer;
    globals.inputBuffer = &inputBuffer;
    globals.trackSendBuffer = &trackSendBuffer;
    globals.inputCommand = &inputCommand[0];

    drawFrame(&globals);
    startTimer(&globals);
    printPrompt(&globals);
    setFIFO(COM2, OFF);
    initTrack(&globals);

    V int resultStatus;
    FOREVER {
        processTime(&globals);
        processOutputBuffer(&globals, COM2);
        resultStatus = processInputBuffer(&globals, COM2);
        if(resultStatus == -1) break;
    }
    return 0;
}

