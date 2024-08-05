#ifndef CC_STACK_H
#define CC_STACK_H

#include "cc_core.h"

#define cc_Stack(T) _cc_Stack_##T
#define cc_Stack_new(T) _cc_Stack_##T##_new
#define cc_Stack_push(T) _cc_Stack_##T##_push
#define cc_Stack_pop(T) _cc_Stack_##T##_pop

#define CC_STACK_DEFINE(T) \
\
typedef struct cc_Stack(T) { \
    T* items; \
    size_t capacity; \
    size_t count; \
} cc_Stack(T); \
\
CC_API cc_Stack(T) cc_Stack_new(T) (T* items, size_t capacity) { \
    return (cc_Stack(T)) { \
        .items = items, \
        .capacity = capacity, \
        .count = 0, \
    }; \
} \
\
CC_API void cc_Stack_push(T) (cc_Stack(T)* stack, T item) { \
    stack->items[stack->count++] = item; \
} \
\
CC_API T cc_Stack_pop(T) (cc_Stack(T)* stack) { \
    return stack->items[--stack->count]; \
} \

#endif