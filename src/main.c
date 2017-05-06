#include <main.h>
#include <timer.h>
#include <io.h>
#include <ui.h>
#include <ringBuffer.h>

int main(int argc, char* argv[]) {
    V globalsStruct globals;
    V timerStruct timer = {0, 0xffffffff, 0, 0};
    V char outputBufferContainer[20000];
    V ringBuffer outputBuffer = {(char*) &outputBufferContainer, 20000, 0, 0};
    globals.timer = &timer;
    globals.outputBuffer = &outputBuffer;

    drawFrame(&globals);
    startTimer();
    setfifo(COM2, OFF);

    FOREVER {
        processTime(&globals);
        processOutputBuffer(&globals, COM2);
    }
    return 0;
}
