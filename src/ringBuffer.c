#include <ringBuffer.h>
#include <main.h>

int ringBuffer_hasElements(ringBuffer *buf) {
    return (buf->writeIndex != buf->readIndex) ? 1 : 0;
}

void ringBuffer_push(ringBuffer *buf, char c) {
    buf->buffer[buf->writeIndex] = c;
    ringBuffer_advanceIndex(&(buf->writeIndex), &(buf->size));
}

char ringBuffer_pop(ringBuffer *buf) {
    char c = buf->buffer[buf->readIndex];
    ringBuffer_advanceIndex(&(buf->readIndex), &(buf->size));
    return c;
}

char ringBuffer_peek(ringBuffer *buf) {
    return buf->buffer[buf->readIndex];
}

void ringBuffer_advanceIndex(int *index, int *size) {
    *index = (*index == *size - 1) ? 0 : (*index) + 1;
}

// Implementation of the ringBuffer for storing delayedElements.
// TODO: make a generic implementation to accept ANY elements.

int delayedRingBuffer_hasElements(delayedRingBuffer *buf) {
    return (buf->writeIndex != buf->readIndex) ? 1 : 0;
}

void delayedRingBuffer_push(delayedRingBuffer *buf, delayedCommand c) {
    buf->buffer[buf->writeIndex] = c;
    ringBuffer_advanceIndex(&(buf->writeIndex), &(buf->size));
}

delayedCommand delayedRingBuffer_pop(delayedRingBuffer *buf) {
    delayedCommand c = buf->buffer[buf->readIndex];
    ringBuffer_advanceIndex(&(buf->readIndex), &(buf->size));
    return c;
}

delayedCommand delayedRingBuffer_peek(delayedRingBuffer *buf) {
    return buf->buffer[buf->readIndex];
}

// Implementation of the ringBuffer for storing railroad commands.
// TODO: make a generic implementation to accept ANY elements.

int trackSendBuffer_hasElements(trackSendBufferStruct *buf) {
    return (buf->writeIndex != buf->readIndex) ? 1 : 0;
}

void trackSendBuffer_push(trackSendBufferStruct *buf, trackCommand c) {
    buf->buffer[buf->writeIndex] = c;
    ringBuffer_advanceIndex(&(buf->writeIndex), &(buf->size));
}

trackCommand trackSendBuffer_pop(trackSendBufferStruct *buf) {
    trackCommand c = buf->buffer[buf->readIndex];
    ringBuffer_advanceIndex(&(buf->readIndex), &(buf->size));
    return c;
}

trackCommand trackSendBuffer_peek(trackSendBufferStruct *buf) {
    return buf->buffer[buf->readIndex];
}

int test_push();
int test_pop();
int test_hasElements();

// running unit-tests: gcc -g -I ../include/ ringBuffer.c; ./a.out; echo $?
// int main(int argc, char* argv[]) {
//     if(test_push() != 0) return -1;
//     if(test_pop() != 0) return -1;
//     if(test_hasElements() != 0) return -1;
//     return 0;
// }

int test_push() {
    char bufferContainer[3];
    ringBuffer buf = {bufferContainer, 3, 0, 0};
    ringBuffer* buffer = &buf;
    ringBuffer_push(buffer, 'a');
    if(buffer->buffer[0] != 'a') return -1;

    ringBuffer_push(buffer, 'b');
    if(buffer->buffer[1] != 'b') return -1;
    if(buffer->writeIndex != 2) return -1;
    
    ringBuffer_push(buffer, 'c');
    if(buffer->buffer[2] != 'c') return -1;
    if(buffer->writeIndex != 0) return -1;
    return 0;
}

int test_pop() {
    char bufferContainer[3];
    ringBuffer buf = {bufferContainer, 3, 0, 0};
    ringBuffer* buffer = &buf;
    ringBuffer_push(buffer, 'a');
    ringBuffer_push(buffer, 'b');

    char c = ringBuffer_pop(buffer);
    if(c != 'a') return -1;
    if(buffer->readIndex != 1) return -1;
    ringBuffer_push(buffer, 'c');

    c = ringBuffer_pop(buffer);
    if(c != 'b') return -1;
    if(buffer->readIndex != 2) return -1;

    c = ringBuffer_pop(buffer);
    if(c != 'c') return -1;
    if(buffer->readIndex != 0) return -1;
    return 0;
}

int test_hasElements() {
    char bufferContainer[3];
    ringBuffer buf = {bufferContainer, 3, 0, 0};
    ringBuffer* buffer = &buf;
    if(ringBuffer_hasElements(buffer) != 0) return -1;

    ringBuffer_push(buffer, 'a');
    ringBuffer_push(buffer, 'b');
    if(ringBuffer_hasElements(buffer) != 1) return -1;

    ringBuffer_pop(buffer);
    if(ringBuffer_hasElements(buffer) != 1) return -1;

    ringBuffer_pop(buffer);
    if(ringBuffer_hasElements(buffer) != 0) return -1;
    return 0;
}
