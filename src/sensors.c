#include <main.h>
#include <sensors.h>
#include <trackControl.h>
#include <ringBuffer.h>
#include <ui.h>
#include <stringFormat.h>

void initSensors(globalsStruct *globals) {
    globals->sensorsParams->requestSent = 0;
    globals->sensorsParams->bytesReceived = 0;

    trackCommand command;
    command.type = TRACK_CONTROL_COMMAND;
    command.data = RESET_SENSORS_AFTER_DUMP;
    trackSendBuffer_push(globals->trackSendBuffer, command);
    printSensorLabels(globals);
}

void printSensorLabels(globalsStruct *globals) {
    int i;
    for(i = 1; i<=TOTAL_SENSORS; i++) {
        moveCursorToPosition(globals, SENSOR_LABEL_ROW + i, SENSOR_LABEL_COLUMN);
        bfprintf(globals, COM2, "%c", getSensorLabel(&i));
    }
}

void sendSensorsDumpRequest(globalsStruct *globals) {
    // curly braces init does not set the values in memory properly
    // command = {DUMP_ALL_SENSORS_COMMAND, MULTIPLE_SENSORS_DUMP+TOTAL_SENSORS};
    trackCommand command;
    command.type = SENSORS_DUMP_COMMAND;
    command.data = MULTIPLE_SENSORS_DUMP+TOTAL_SENSORS;
    trackSendBuffer_push(globals->trackSendBuffer, command);
}

// Each sensor returns 2 bytes
// byteId signifies whether it's interpreting the first or the second byte returned by the sensor
void printSensorData(globalsStruct* globals, char* c) {
    V int row = SENSOR_LABEL_ROW, column = SENSOR_LABEL_COLUMN + 3, byteId = 0;
    int sensorId = 0;

    sensorId = getSensorIdFromDataByte(globals);
    row += sensorId;
    byteId = (globals->sensorsParams->bytesReceived % 2 > 0) ? 1 : 2;
    if(byteId == 2) column += 3;

    moveCursorToPosition(globals, row, column);
    bfprintf(globals, COM2, "%s", "  ");

    moveCursorToPosition(globals, row, column);
    bfprintf(globals, COM2, "%u", getTriggeredSensorFromDataByte(c, byteId));
}

int getTriggeredSensorFromDataByte(char* c, int byteId) {
    V int i = 1, triggeredSensor = 0;

    for(i; i<=8; i++) {
        if(isBitSet(i, c)) {
            triggeredSensor = i + ((byteId == 2) ? 8 : 0);
            break;
        }
    }
    return triggeredSensor;
}

//bitPosition: from left to right. Ex. 0100_0000: 2nd bit is set. 
int isBitSet(int bitPosition, char* c) {
    return *c & (128 >> (bitPosition - 1));
}

int getSensorIdFromDataByte(globalsStruct* globals) {
    unsigned int* bytesReceived = &(globals->sensorsParams->bytesReceived);
    int sensorId = 0;
    if     (1 <= *bytesReceived && *bytesReceived <= 2) sensorId = SENSOR_A;
    else if(3 <= *bytesReceived && *bytesReceived <= 4) sensorId = SENSOR_B;
    else if(5 <= *bytesReceived && *bytesReceived <= 6) sensorId = SENSOR_C;
    else if(7 <= *bytesReceived && *bytesReceived <= 8) sensorId = SENSOR_D;
    else if(9 <= *bytesReceived && *bytesReceived <= 10) sensorId = SENSOR_E;
    return sensorId;
}

char getSensorLabel(int* sensorId) {
    char sensorLabel;
    if     (*sensorId == SENSOR_A) sensorLabel = SENSOR_A_LABEL;
    else if(*sensorId == SENSOR_B) sensorLabel = SENSOR_B_LABEL;
    else if(*sensorId == SENSOR_C) sensorLabel = SENSOR_C_LABEL;
    else if(*sensorId == SENSOR_D) sensorLabel = SENSOR_D_LABEL;
    else if(*sensorId == SENSOR_E) sensorLabel = SENSOR_E_LABEL;
    return sensorLabel;
}
