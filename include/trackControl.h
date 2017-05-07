#ifndef _TRACK_INCL_GUARD
#define _TRACK_INCL_GUARD

#include <main.h>

void initTrack(globalsStruct* globals);
void startTrack(globalsStruct* globals);
void stopTrack(globalsStruct* globals);
void initUART(int channel);
int runTrain(globalsStruct* globals, int train, int speed);
void stopTrain(globalsStruct* globals, int train);
void processTrackSendBuffer(globalsStruct* globals);

#endif
