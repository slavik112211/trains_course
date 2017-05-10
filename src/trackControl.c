#include <main.h>
#include <bufferIO.h>
#include <ui.h>
#include <ringBuffer.h>
#include <trackControl.h>
#include <ts7200.h>
#include <commandParser.h>
#include <stringFormat.h>

/*
 * https://www.student.cs.uwaterloo.ca/~cs452/manual/trains.html
 * http://www.cgl.uwaterloo.ca/wmcowan/teaching/cs452/timeless/marklin/commands.html
 */

void initTrack(globalsStruct* globals) {
    setCOMBaudRate(COM1, 2400);
    initUART(COM1);
    startTrack(globals);
    initSwitches(globals);
}

void startTrack(globalsStruct* globals) {
    ringBuffer_push(globals->trackSendBuffer, 0x60);
}

void stopTrack(globalsStruct* globals) {
    ringBuffer_push(globals->trackSendBuffer, 0x61);
}

void initUART(int channel) {
    int *line, buf;
    line = getUARTHighControl(channel);
    buf = *line;
    buf = buf & ~FEN_MASK; // FIFO off
    buf = buf | WLEN_MASK; // 8 bit per frame
    buf = buf | STP2_MASK; // Two stop bits are transmitted at the end of the frame
    buf = buf & ~PEN_MASK; // No parity bit is added to the data frame
    *line = buf;
}

// Iterate over all switches two times:
// 1st round: set all switches to Straight (0x9a and 0x9c to Curved);
// 2nd round: set all switches to Curved   (0x9a and 0x9c to Straight);
// 
// COM1 2400 baud rate: 4ms to transmit 1 byte, 8ms to transmit 1 switch command (2 bytes).
// Total time to run: 22 switches * 2 rounds * 8ms = 352 ms.
// Deactivating solenoids should be delayed by at least 450ms.
void initSwitches(globalsStruct* globals) {
    V int i = 0, round = 0, switchPosition = SWITCH_CURVED;

    for(round=0; round<2; round++) {
        for(i=0; i<SWITCHES_ON_TRACK; i++) {
            if(i < SWITCHES_ON_TRACK-4 || i == switchIdToIndex(0x99) || i == switchIdToIndex(0x9b)) {
                switchPosition = (round == 0) ? SWITCH_STRAIGHT : SWITCH_CURVED;
            } else if (i == switchIdToIndex(0x9a) || i == switchIdToIndex(0x9c)) {
                switchPosition = (round == 0) ? SWITCH_CURVED : SWITCH_STRAIGHT;
            }
            setSwitchPosition(globals, switchIndexToId(i), switchPosition, 0);
        }
    }
    deactivateSolenoids(globals);
}

// 0=stop, 1=slowest, 14=fastest, 15=reverse direction
void runTrain(globalsStruct* globals, int train, int speed) {
    if(speed < 0 || speed > 14) speed = 5;
    if(train < 0 || train > 100) train = 58;
    ringBuffer_push(globals->trackSendBuffer, speed);
    ringBuffer_push(globals->trackSendBuffer, train);
}

void stopTrain(globalsStruct* globals, int train) {
    runTrain(globals, train, 0);
}

void reverseTrain(globalsStruct* globals, int trainId) {
    stopTrain(globals, trainId);

    int timestamp = globals->timer->msFromEpoch + 2000; //2 seconds from now
    delayedCommand reverseCommand1, reverseCommand2, runCommand1, runCommand2;

    // speed=15 sets the train in reverse direction. This setting is just a flag,
    // the actual speed has to be communicated as a separate command.
    reverseCommand1.value = 15;
    reverseCommand1.timestamp = timestamp;

    reverseCommand2.value = trainId;
    reverseCommand2.timestamp = timestamp;

    timestamp += 150;
    runCommand1.value = 10; //speed=10 (range: 0 to 14)
    runCommand1.timestamp = timestamp;
    
    runCommand2.value = trainId;
    runCommand2.timestamp = timestamp;

    delayedRingBuffer_push(globals->trackSendDelayedBuffer, reverseCommand1);
    delayedRingBuffer_push(globals->trackSendDelayedBuffer, reverseCommand2);
    delayedRingBuffer_push(globals->trackSendDelayedBuffer, runCommand1);
    delayedRingBuffer_push(globals->trackSendDelayedBuffer, runCommand2);
}

// Switches can have two possible configurations: straight (S=33), and curved (C=34)
// Switch Ids are in ranges 1..18 and 0x99..0x9c (153..156).
void setSwitchPosition(globalsStruct* globals, int switchId, int position, int setSolenoidsOff) {
    if(!(1 <= switchId && switchId <= 18) && !(0x99 <= switchId && switchId <= 0x9c)) {
        switchId = 1;
    }
    globals->switches[switchIdToIndex(switchId)] = position;
    printSwitch(globals, switchIdToIndex(switchId));

    ringBuffer_push(globals->trackSendBuffer, position);
    ringBuffer_push(globals->trackSendBuffer, switchId);

    if (setSolenoidsOff != 0) deactivateSolenoids(globals);
}

// Each switch has a solenoid spiral that is activated on switching.
// Spiral MUST be deactivated, otherwise it will burn out (with 150ms-500ms delay after activating the switch).
// In case multiple switches are activated, the solenoid command needs to be sent ONLY once,
// as activating next switch deactivates the previous solenoid.
void deactivateSolenoids(globalsStruct* globals){
    delayedCommand turnSolenoidOff;
    turnSolenoidOff.value = SOLENOID_OFF;
    turnSolenoidOff.timestamp = globals->timer->msFromEpoch + 450; //0.45 seconds from now
    delayedRingBuffer_push(globals->trackSendDelayedBuffer, turnSolenoidOff);
    // printDebug(globals, 7, 40, turnSolenoidOff.value);
    // moveCursorToPosition(globals, 8, 40);
    // bfprintf(globals, COM2, "current time %x", globals->timer->msFromEpoch);
    // moveCursorToPosition(globals, 9, 40);
    // bfprintf(globals, COM2, "solenoid off %x", turnSolenoidOff.timestamp);
}

void processTrackSendBuffer(globalsStruct* globals) {
    int *flags, *data, value;
    flags = getUARTFlags(COM1);
    data = getUARTData(COM1);
    if (ringBuffer_hasElements(globals->trackSendBuffer) == 1 &&
            (*flags & TXFE_MASK) && (*flags & CTS_MASK)) {
        value = ringBuffer_pop(globals->trackSendBuffer);
        *data = value;
        // printDebug(globals, globals->counterCOM1 + 4, 30, value);
        // globals->counterCOM1++;
    }
}

void processTrackSendDelayedBuffer(globalsStruct* globals) {
    delayedRingBuffer *delayedBuffer = globals->trackSendDelayedBuffer;

    if(delayedRingBuffer_hasElements(delayedBuffer)) {
        delayedCommand nextCommand = delayedRingBuffer_peek(delayedBuffer);
        if(globals->timer->msFromEpoch > nextCommand.timestamp) {
            nextCommand = delayedRingBuffer_pop(delayedBuffer);
            ringBuffer_push(globals->trackSendBuffer, nextCommand.value);
        }
    }
}

int switchIndexToId(V int switchIndex) {
    V int switchId = 0;
    if(switchIndex >= 0 && switchIndex <= 17) switchId = switchIndex + 1;
    else if (switchIndex == 18) switchId = 0x99;
    else if (switchIndex == 19) switchId = 0x9a;
    else if (switchIndex == 20) switchId = 0x9b;
    else if (switchIndex == 21) switchId = 0x9c;
    return switchId;
}

int switchIdToIndex(V int switchId) {
    V int switchIndex = 0;
    if(switchId >= 1 && switchId <= 18) switchIndex = switchId - 1;
    else if (switchId == 0x99) switchIndex = 18;
    else if (switchId == 0x9a) switchIndex = 19;
    else if (switchId == 0x9b) switchIndex = 20;
    else if (switchId == 0x9c) switchIndex = 21;
    return switchIndex;
}

// Switches can have two possible configurations: straight (S=33), and curved (C=34)
int switchCommandNameToCommandId(char* commandName) {
    V int switchCommandId = 33;
    if (     strCompare(commandName, "S") == 0) switchCommandId = 33;
    else if (strCompare(commandName, "C") == 0) switchCommandId = 34;
    return switchCommandId;
}

char switchCommandIdToCommandName(int commandId) {
    V char commandName = 'C';
    if      (commandId == 33) commandName = 'S';
    else if (commandId == 34) commandName = 'C';
    return commandName;
}
