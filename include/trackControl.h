#ifndef _TRACK_INCL_GUARD
#define _TRACK_INCL_GUARD

#include <main.h>

#define SWITCH_STRAIGHT 33
#define SWITCH_CURVED 34
#define SOLENOID_OFF 32

void initTrack(globalsStruct* globals);
void startTrack(globalsStruct* globals);
void stopTrack(globalsStruct* globals);
void initUART(int channel);
void initSwitches(globalsStruct* globals);
void runTrain(globalsStruct* globals, int train, int speed);
void stopTrain(globalsStruct* globals, int train);
void reverseTrain(globalsStruct* globals, int trainId);
void setSwitchPosition(globalsStruct* globals, int switchId, int position, int setSolenoidsOff);
void deactivateSolenoids(globalsStruct* globals);
void processTrackSendBuffer(globalsStruct* globals);
void processTrackSendDelayedBuffer(globalsStruct* globals);

int switchIndexToId(V int switchIndex);
int switchIdToIndex(V int switchId);
int switchCommandNameToCommandId(char* commandName);
char switchCommandIdToCommandName(int commandId);

#endif
