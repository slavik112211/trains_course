#include <main.h>
#include <io.h>
#include <ui.h>

void drawFrame(globalsStruct* globals) {
    printNewlines(globals, FRAME_HEIGHT*2);
    V int i=1, j=1;
    moveCursorToPosition(globals, i, j);
    for(i=1; i<=FRAME_HEIGHT; i++) {
        for(j=1; j<=FRAME_WIDTH; j++) {
            if(j==1) {
                printFrameElement(globals);
            } else if(j==FRAME_WIDTH) {
                printFrameElement(globals);
                moveCursorDown(globals, 1);
                moveCursorLeft(globals, FRAME_WIDTH);
            } else if(i==1 || i==FRAME_HEIGHT) {
                printFrameElement(globals);
            } else {
                moveCursorRight(globals, 1);
            }
        }
    }
    printNewlines(globals, 1);
}

void printNewlines(globalsStruct* globals, int rows) {
    V int i=0;
    for (i; i<rows; i++) {
        putc(globals, COM2, '\n');
    }
}

void printFrameElement(globalsStruct* globals) {
    putc(globals, COM2, '#');
}

void moveCursorToPosition(globalsStruct* globals, int row, int column) {
    bfprintf(globals, COM2, "\033[%d;%dH", row, column);
}

void moveCursorUp(globalsStruct* globals, int rows) {
    bfprintf(globals, COM2, "\033[%dA", rows);
}

void moveCursorDown(globalsStruct* globals, int rows) {
    bfprintf(globals, COM2, "\033[%dB", rows);
}

void moveCursorRight(globalsStruct* globals, int columns) {
    bfprintf(globals, COM2, "\033[%dC", columns);
}

void moveCursorLeft(globalsStruct* globals, int columns) {
    bfprintf(globals, COM2, "\033[%dD", columns);
}

void moveCursorToTimerPosition(globalsStruct* globals) {
    moveCursorToPosition(globals, 3, FRAME_WIDTH-15);
}