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
void ringBuffer_advanceIndex(int*, int*);

#endif
