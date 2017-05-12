#ifndef _SENSORS_INCL_GUARD
#define _SENSORS_INCL_GUARD

#include <main.h>

#define SINGLE_SENSOR_DUMP 0xC0 // 192 decimal
#define MULTIPLE_SENSORS_DUMP 0x80 // 128 decimal

/*
 * The s88 units can be told to reset its memory after a dump to the computer,
 * or it can remain as it was and continue adding data to the byte. 
 * A reset would make each position in memory a "0" again. 
 * If you want it to reset, send the single byte 0xC0 without adding any numbers to it. 
 * If the reset mode is to be "off", then send the code 0x80.
 */
#define RESET_SENSORS_AFTER_DUMP 0xC0
#define NO_SENSORS_RESET_AFTER_DUMP 0x80

#define SENSOR_A 1
#define SENSOR_B 2
#define SENSOR_C 3
#define SENSOR_D 4
#define SENSOR_E 5

#define SENSOR_A_LABEL 'A'
#define SENSOR_B_LABEL 'B'
#define SENSOR_C_LABEL 'C'
#define SENSOR_D_LABEL 'D'
#define SENSOR_E_LABEL 'E'

#define SENSOR_LABEL_ROW 4
#define SENSOR_LABEL_COLUMN 50 

void initSensors(globalsStruct *globals);
void printSensorLabels(globalsStruct *globals);
void sendSensorsDumpRequest(globalsStruct *globals);
void printSensorData(globalsStruct* globals, char* c);
int getTriggeredSensorFromDataByte(char* c, int byteId);
int isBitSet(int bitIndex, char* c);
int getSensorIdFromDataByte(globalsStruct* globals);
char getSensorLabel(int* sensorId);

#endif
