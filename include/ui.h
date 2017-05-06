#ifndef _UI_INCL_GUARD
#define _UI_INCL_GUARD

#include <main.h>

#define FRAME_WIDTH 100
#define FRAME_HEIGHT 30

void drawFrame(globalsStruct* globals);
void printNewlines(globalsStruct* globals, int rows);
void printFrameElement(globalsStruct* globals);
void moveCursorToPosition(globalsStruct* globals, int row, int column);
void moveCursorUp(globalsStruct* globals, int rows);
void moveCursorDown(globalsStruct* globals, int rows);
void moveCursorRight(globalsStruct* globals, int columns);
void moveCursorLeft(globalsStruct* globals, int columns);
void moveCursorToTimerPosition(globalsStruct* globals);

#endif
