#ifndef CC_QUEUE_H
#define CC_QUEUE_H

#include "cc_core.h"

#define cc_Queue(T) _cc_Queue_##T
#define cc_Queue_new(T) _cc_Queue_##T##_new
#define cc_Queue_enqueue(T) _cc_Queue_##T##_enqueue
#define cc_Queue_dequeue(T) _cc_Queue_##T##_dequeue
#define cc_Queue_foreach(T) _cc_Queue_##T##_foreach
#define cc_Queue_peek(T) _cc_Queue_##T##_peek

#define CC_QUEUE_DEFINE(T) \
\
typedef struct cc_Queue(T) { \
    T* items; \
    size_t capacity; \
    size_t head, tail; \
    size_t count; \
} cc_Queue(T); \
\
CC_API cc_Queue(T) cc_Queue_new(T) (T* items, size_t capacity) { \
    return (cc_Queue(T)) { \
        .items = items, \
        .capacity = capacity, \
        .head = 0, .tail = 0, \
        .count = 0, \
    }; \
} \
\
CC_API void cc_Queue_enqueue(T) (cc_Queue(T)* queue, T item) { \
    queue->items[queue->tail++] = item; \
    queue->tail %= queue->capacity; \
    queue->count++; \
} \
\
CC_API T cc_Queue_dequeue(T) (cc_Queue(T)* queue) { \
    T item = queue->items[queue->head++]; \
    queue->head %= queue->capacity; \
    queue->count--; \
    return item; \
} \
\
CC_API T cc_Queue_peek(T) (const cc_Queue(T)* queue) { \
    return queue->items[queue->head]; \
} \
\
CC_API void cc_Queue_foreach(T) (const cc_Queue(T)* queue, void (*action)(T)) { \
    size_t i = queue->head; \
    while (1) { \
        size_t next = (i + 1) % queue->capacity; \
        action(queue->items[i]); \
        if (next == queue->tail) break; \
        i = next; \
    } \
} \

#endif