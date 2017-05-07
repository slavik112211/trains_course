#ifndef _PARSER_INCL_GUARD
#define _PARSER_INCL_GUARD

#include <main.h>

int parseCommand(globalsStruct* globals);
int findDelimIndex(V char* string, V char* delim, V int occurence);
void getStringPart(char* inputString, V int part, char* delim, char* stringPart);
void memcopy(void *dest, void *src, V short int n, int addNullByte);
int atoi(char *str);
int strCompare(char *str1, char *str2);

#endif
