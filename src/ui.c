#include <main.h>
#include <bufferIO.h>
#include <stringFormat.h>
#include <ui.h>
#include <trackControl.h>

// http://ascii-table.com/ansi-escape-sequences.php

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

void printPrompt(globalsStruct* globals) {
    moveCursorToPosition(globals, FRAME_HEIGHT-2, 3);
    bfprintf(globals, COM2, "Enter command > ");
}

void printCurrentCommand(globalsStruct* globals, int row, int column) {
    moveCursorToPosition(globals, row, column);
    V ringBuffer* inputBuffer = globals->inputBuffer;
    V int i = inputBuffer->readIndex;
    while (i < inputBuffer->writeIndex) {
        putc(globals, COM2, inputBuffer->buffer[i]);
        i++;
    }
}

void printActiveCommand(globalsStruct* globals) {
    moveCursorToPosition(globals, FRAME_HEIGHT-2, 19);
    V ringBuffer* inputBuffer = globals->inputBuffer;
    V int i = inputBuffer->readIndex;
    while (i < inputBuffer->writeIndex) {
        putc(globals, COM2, inputBuffer->buffer[i]);
        i++;
    }
}

void printPreviousCommand(globalsStruct* globals) {
    moveCursorToPosition(globals, FRAME_HEIGHT-3, 19);
    putstr(globals, COM1, globals->inputCommand);
}

void eraseCommand(globalsStruct* globals, int row) {
    moveCursorToPosition(globals, row, 19);
    V int i;
    for(i=0;i<FRAME_WIDTH-20;i++) {
        putc(globals, COM2, ' ');
    }
}

void clearCommandPrompt(globalsStruct* globals) {
    eraseCommand(globals, FRAME_HEIGHT-2);
    eraseCommand(globals, FRAME_HEIGHT-3);
}

void printDebug(globalsStruct* globals, int row, int column, char c) {
    moveCursorToPosition(globals, row, column);
    putr(globals, COM2, c);
}

void printSwitches(globalsStruct* globals) {
    V int i = 0;
    for(i; i<SWITCHES_ON_TRACK; i++) {
        printSwitch(globals, i);
    }
}

void printSwitch(globalsStruct* globals, int switchIndex) {
    V int row = 5, column = 5;
    moveCursorToPosition(globals, row + switchIndex, column);
    bfprintf(globals, COM2, "%u", switchIndexToId(switchIndex));

    moveCursorToPosition(globals, row + switchIndex, column + 6);
    putc(globals, COM2, switchCommandIdToCommandName(globals->switches[switchIndex]));
}
