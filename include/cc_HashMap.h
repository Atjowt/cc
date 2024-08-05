#ifndef CC_HASHMAP_H
#define CC_HASHMAP_H

#include "cc_core.h"

#define cc_HashMap(T1, T2) _cc_HashMap_##T1##_##T2
#define cc_HashMap_new(T1, T2) _cc_HashMap_##T1##_##T2##_new
#define cc_HashMap_insert(T1, T2) _cc_HashMap_##T1##_##T2##_insert
#define cc_HashMap_remove(T1, T2) _cc_HashMap_##T1##_##T2##_remove

#define CC_HASHMAP_DEFINE(T1, T2) \
\
typedef struct cc_HashMap(T1, T2) { \
    T2* items; \
    size_t capacity; \
    size_t count; \
} cc_HashMap(T1, T2); \
\
CC_API cc_HashMap(T1, T2) cc_HashMap_new(T1, T2) (T2* items, size_t capacity) { \
    return (cc_HashMap(T1, T2)) { \
        .items = items, \
        .capacity = capacity, \
        .count = 0, \
    }; \
} \
\
CC_API bool cc_HashMap_insert(T1, T2) (cc_HashMap(T1, T2)* hashmap, T1 key, T2 value) { \
    /* TODO */ \
    return false; \
} \
\
CC_API bool cc_HashMap_remove(T1, T2) (cc_HashMap(T1, T2)* hashmap, T1 key) { \
    /* TODO */ \
    return false; \
} \

#endif