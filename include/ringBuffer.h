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

int delayedRingBuffer_hasElements(delayedRingBuffer *buf);
void delayedRingBuffer_push(delayedRingBuffer *buf, delayedCommand c);
delayedCommand delayedRingBuffer_pop(delayedRingBuffer *buf);
delayedCommand delayedRingBuffer_peek(delayedRingBuffer *buf);

#endif
