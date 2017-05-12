#ifndef _RINGBUFFER_INCL_GUARD
#define _RINGBUFFER_INCL_GUARD

typedef struct {
    char *buffer;
    int size;
    int readIndex;
    int writeIndex;
} ringBuffer;

int ringBuffer_hasElements(ringBuffer *);
void ringBuffer_push(ringBuffer *, char);
char ringBuffer_pop(ringBuffer *);
char ringBuffer_peek(ringBuffer *);
void ringBuffer_advanceIndex(int*, int*);

// Implementation of the ringBuffer for storing delayedCommands.
// TODO: make a generic implementation to accept ANY elements.

typedef struct {
    unsigned int timestamp;
    char value;
} delayedCommand;

typedef struct {
    delayedCommand *buffer;
    int size;
    int readIndex;
    int writeIndex;
} delayedRingBuffer;

// track commands pushed to a trackSendBufferStruct types:
// #define TRACK_CONTROL_COMMAND 0
// #define SENSORS_DUMP_REQUEST 1
typedef struct {
    unsigned int type;
    char data;
} trackCommand;

typedef struct {
    trackCommand *buffer;
    int size;
    int readIndex;
    int writeIndex;
} trackSendBufferStruct;


int delayedRingBuffer_hasElements(delayedRingBuffer *buf);
void delayedRingBuffer_push(delayedRingBuffer *buf, delayedCommand c);
delayedCommand delayedRingBuffer_pop(delayedRingBuffer *buf);
delayedCommand delayedRingBuffer_peek(delayedRingBuffer *buf);

int trackSendBuffer_hasElements(trackSendBufferStruct *buf);
void trackSendBuffer_push(trackSendBufferStruct *buf, trackCommand c);
trackCommand trackSendBuffer_pop(trackSendBufferStruct *buf);
trackCommand trackSendBuffer_peek(trackSendBufferStruct *buf);

#endif
