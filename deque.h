#ifndef DEQUE_H
#define DEQUE_H

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

// TODO: resizing will not work. disabled for now.
// Idea: copy the data over to the opposite half and move the corresponding pointer to the same relative position

typedef struct Deque {
    void*   data;       // Pointer to the start of data.
    size_t  size;       // Current size of the deque in bytes.
    size_t  capacity;   // Total capacity of the deque in bytes.
    size_t  front;      // Byte offset for the front of the deque.
    size_t  back;       // Byte offset for the back of the deque.
    size_t  count;      // Number of items in the deque, +1 per enqueue and -1 per dequeue.
    // bool    dynamic;    // Whether the deque will dynamically reallocate to ensure enough capacity.
} Deque;

// Create deque with `capacity` bytes total capacity. If `dynamic` is set to true, the deque will reallocate on overflow.

// Create deque with `capacity` bytes total capacity.
Deque deque_create(size_t capacity/*, bool dynamic*/);

// Destroy `deque`, freeing all resources.
void deque_destroy(Deque* deque);

// // Ensure `deque` has at least `size` bytes of capacity.
// void deque_ensure(Deque* deque, size_t size);

// Add `size` bytes of `data` to the front of `deque`.
void deque_enqueue_front(Deque* deque, size_t size, const void* restrict data);

// Remove `size` bytes from the front of `deque` and move them into `data`.
void deque_dequeue_front(Deque* deque, size_t size, void* restrict data);

// Add `size` bytes of `data` to the back of `deque`.
void deque_enqueue_back(Deque* deque, size_t size, const void* restrict data);

// Remove `size` bytes from the back of `deque` and move them into `data`.
void deque_dequeue_back(Deque* deque, size_t size, void* restrict data);

// Get `size` bytes starting `offset` bytes from the front of the `deque` and putting them into `data`.
void deque_get(const Deque* deque, size_t size, size_t offset, void* restrict data);

#ifdef DEQUE_IMPLEMENTATION

Deque deque_create(size_t capacity/*, bool dynamic*/) {
    return (Deque) {
        .data = malloc(capacity),
        .capacity = capacity,
        // .dynamic = dynamic,
        .count = 0,
        .size = 0,
        .front = 0,
        .back = 0,
    };
}

void deque_destroy(Deque* deque) {
    free(deque->data);
    deque->data = NULL;
    // deque->dynamic = false;
    deque->capacity = 0;
    deque->size = 0;
    deque->count = 0;
    deque->front = 0;
    deque->back = 0;
}

// void deque_ensure(Deque* deque, size_t size) {
//     size_t old_cap = deque->size;
//     if (deque->capacity >= size) return;
//     while (deque->capacity < size) {
//         deque->capacity *= 2;
//     }
//     deque->data = realloc(deque->data, deque->capacity);
// }

void deque_enqueue_back(Deque* deque, size_t size, const void* restrict data) {
    // if (deque->dynamic) { deque_ensure(deque, deque->size + size); }
    if (deque->back + size < deque->capacity) {
        memcpy((uint8_t*)deque->data + deque->back, data, size);
        deque->back += size;
    } else {
        size_t right_chunk = deque->back + size - deque->capacity;
        size_t left_chunk = size - right_chunk;
        memcpy((uint8_t*)deque->data + deque->back, data, left_chunk);
        deque->back = 0;
        memcpy((uint8_t*)deque->data + deque->back, (uint8_t*)data + left_chunk, right_chunk);
        deque->back += right_chunk;
    }
    deque->size += size;
    deque->count++;
}

void deque_dequeue_front(Deque* deque, size_t size, void* restrict data) {
    if (deque->front + size < deque->capacity) {
        memcpy(data, (uint8_t*)deque->data + deque->front, size);
        deque->front += size;
    } else {
        size_t right_chunk = deque->front + size - deque->capacity;
        size_t left_chunk = size - right_chunk;
        memcpy(data, (uint8_t*)deque->data + deque->front, left_chunk);
        deque->front = 0;
        memcpy((uint8_t*)data + left_chunk, (uint8_t*)deque->data + deque->front, right_chunk);
        deque->front += right_chunk;
    }
    deque->size -= size;
    deque->count--;
}

void deque_enqueue_front(Deque* deque, size_t size, const void* restrict data) {
    // if (deque->dynamic) { deque_ensure(deque, deque->size + size); }
    if (deque->front >= size) {
        deque->front -= size;
        memcpy((uint8_t*)deque->data + deque->front, data, size);
    } else {
        size_t left_chunk = size - deque->front;
        size_t right_chunk = size - left_chunk;
        deque->front = 0;
        memcpy((uint8_t*)deque->data + deque->front, (uint8_t*)data + left_chunk, right_chunk);
        deque->front = deque->capacity - left_chunk;
        memcpy((uint8_t*)deque->data + deque->front, data, left_chunk);
    }
    deque->size += size;
    deque->count++;
}

void deque_dequeue_back(Deque* deque, size_t size, void* restrict data) {
    if (deque->back >= size) {
        deque->back -= size;
        memcpy(data, (uint8_t*)deque->data + deque->back, size);
    } else {
        size_t left_chunk = size - deque->back;
        size_t right_chunk = size - left_chunk;
        deque->back = 0;
        memcpy((uint8_t*)data + left_chunk, (uint8_t*)deque->data + deque->front, right_chunk);
        deque->back = deque->capacity - left_chunk;
        memcpy(data, (uint8_t*)deque->data + deque->back, left_chunk);
    }
    deque->size -= size;
    deque->count--;
}

void deque_get(const Deque* deque, size_t size, size_t offset, void* restrict data) {
    size_t start = (deque->front + offset) % deque->capacity;
    if (start + size <= deque->capacity) {
        memcpy(data, (uint8_t*)deque->data + start, size);
    } else {
        size_t left_chunk = deque->capacity - start;
        size_t right_chunk = size - left_chunk;
        memcpy(data, (uint8_t*)deque->data + start, left_chunk);
        memcpy((uint8_t*)data + left_chunk, (uint8_t*)deque->data, right_chunk);
    }
}

#endif

#endif
