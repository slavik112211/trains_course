#ifndef _IO_INCL_GUARD
#define _IO_INCL_GUARD

#include <main.h>
#include <ringBuffer.h>

typedef char *va_list;

#define __va_argsiz(t)	\
		(((sizeof(t) + sizeof(int) - 1) / sizeof(int)) * sizeof(int))

#define va_start(ap, pN) ((ap) = ((va_list) __builtin_next_arg(pN)))

#define va_end(ap)	((void)0)

#define va_arg(ap, t)	\
		 (((ap) = (ap) + __va_argsiz(t)), *((t*) (void*) ((ap) - __va_argsiz(t))))

int setfifo( int channel, int state );

int putc( globalsStruct* globals, int channel, char c );

int getc( globalsStruct* globals, int channel );

int putx( globalsStruct* globals, int channel, char c );

int putstr( globalsStruct* globals, int channel, char *str );

int putr( globalsStruct* globals, int channel, unsigned int reg );

void putw( globalsStruct* globals, int channel, int n, char fc, char *bf );

void bfprintf( globalsStruct* globals, int channel, char *fmt, ... );

int* getUARTFlags(int channel);
int* getUARTData(int channel);
int processInputBuffer(globalsStruct* globals, int channel);
void processOutputBuffer(globalsStruct* globals, int channel);

#endif
