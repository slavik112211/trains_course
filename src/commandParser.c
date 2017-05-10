#include <main.h>
#include <commandParser.h>
#include <trackControl.h>

#define COMMAND_PARTS 3

int parseCommand(globalsStruct* globals) {
    int resultStatus = 0, trainId = 0, trainSpeed = 0, switchId = 0;
    char* delim = " ";
    char* trainForwardCommand = "tr";
    char* trainReverseCommand = "rv";
    char* switchControlCommand = "sw";
    char* quitCommand = "q";
    
    char command[COMMAND_PARTS][10]; //three command parts, 10 char max length

    V int i = 0;
    for (i; i<COMMAND_PARTS; i++) {
        getStringPart(globals->inputCommand, i, delim, command[i]);
    }

    if (strCompare(command[0], trainForwardCommand) == 0) {
        trainId = atoi(command[1]);
        trainSpeed = atoi(command[2]);
        runTrain(globals, trainId, trainSpeed);
    } else if (strCompare(command[0], trainReverseCommand) == 0) {
        trainId = atoi(command[1]);
        reverseTrain(globals, trainId);
    } else if (strCompare(command[0], switchControlCommand) == 0) {
        switchId = atoi(command[1]);
        setSwitchPosition(globals, switchId, switchCommandNameToCommandId(command[2]), 1);
    } else if (strCompare(command[0], quitCommand) == 0) {
        resultStatus = -1;
    }
    return resultStatus;
}

int findDelimIndex(V char* string, V char* delim, V int occurence) {
    V int delimOccurence = 0; //for ex., find a second occurence of " "
    V int positionIndex = 0;
    while(*string) {
        if(*string == *delim) {
            if(delimOccurence == occurence) return positionIndex;
            delimOccurence++;
        }
        positionIndex++;
        string++;
    }
    return positionIndex; //not found, return this string's null-termination char position
}

void getStringPart(char* inputString, V int part, char* delim, char* stringPart) {
    V int startIndex = 0;
    V int endIndex = 0;

    if(!part == 0)
        startIndex = findDelimIndex(inputString, delim, part-1) + 1;
    endIndex = findDelimIndex(inputString, delim, part);

    memcopy(stringPart, &inputString[startIndex], endIndex-startIndex, 1);
}

// http://www.geeksforgeeks.org/write-memcpy/
void memcopy(void *dest, void *src, short int n, int addNullByte) {
    char *csrc = (char *) src;
    char *cdest = (char *) dest;
 
    V int i = 0;
    for (i; i<n; i++) {
        cdest[i] = csrc[i];
    }
    if(addNullByte == 1) {
        cdest[i] = 0x00;
    }
}

// http://www.geeksforgeeks.org/write-your-own-atoi/
// 
// Does ASCII math. For ex. to convert '4' into 4,
// ASCII code of '0' (48) is subtracted from ASCII code of '4' (52).  
int atoi(char *str) {
    V int number = 0;
    V int i = 0;

    for (i; str[i] != '\0'; ++i) {
        number = number*10 + str[i] - '0';
    }

    return number;
}

// http://stackoverflow.com/questions/14232990/comparing-two-strings-in-c
int strCompare(char *str1, char *str2) {
    while (*str1 == *str2) {
        if (*str1 == '\0' || *str2 == '\0') break;
        str1++; str2++;
    }

    if (*str1 == '\0' && *str2 == '\0') return 0;
    else return -1;
}
