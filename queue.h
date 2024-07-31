#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct Queue {
    void*   data;       // Pointer to the start of data.
    size_t  size;       // Current size of the queue in bytes.
    size_t  capacity;   // Total capacity of the queue in bytes.
    size_t  front;      // Byte offset for the front of the queue.
    size_t  back;       // Byte offset for the back of the queue.
    size_t  count;      // Number of items in the queue, +1 per enqueue and -1 per dequeue.
    bool    dynamic;    // Whether the queue will dynamically reallocate to ensure enough capacity.
} Queue;

// Create queue with `capacity` bytes total capacity. If `dynamic` is set to true, the queue will reallocate on overflow.
Queue queue_create(size_t capacity, bool dynamic);

// Destroy `queue`, freeing all resources.
void queue_destroy(Queue* queue);

// Ensure `queue` has at least `size` bytes of capacity.
void queue_ensure(Queue* queue, size_t size);

// Add `size` bytes of `data` to the back of `queue`.
void queue_enqueue(Queue* queue, size_t size, const void* restrict data);

// Remove `size` bytes from the front of `queue` and move them into `data`.
void queue_dequeue(Queue* queue, size_t size, void* restrict data);

#ifdef QUEUE_IMPLEMENTATION

Queue queue_create(size_t capacity, bool dynamic) {
    return (Queue) {
        .data = malloc(capacity),
        .capacity = capacity,
        .dynamic = dynamic,
        .count = 0,
        .size = 0,
        .front = 0,
        .back = 0,
    };
}

void queue_destroy(Queue* queue) {
    free(queue->data);
    queue->data = NULL;
    queue->dynamic = false;
    queue->capacity = 0;
    queue->size = 0;
    queue->count = 0;
    queue->front = 0;
    queue->back = 0;
}

void queue_ensure(Queue* queue, size_t size) {
    if (queue->capacity >= size) return;
    while (queue->capacity < size) {
        queue->capacity *= 2;
    }
    queue->data = realloc(queue->data, queue->capacity);
}

void queue_enqueue(Queue* queue, size_t size, const void* restrict data) {
    if (queue->dynamic) { queue_ensure(queue, queue->size + size); }
    if (queue->back + size <= queue->capacity) {
        memcpy((uint8_t*)queue->data + queue->back, data, size);
        queue->back += size;
    } else {
        size_t right_chunk = queue->back + size - queue->capacity;
        size_t left_chunk = size - right_chunk;
        memcpy((uint8_t*)queue->data + queue->back, data, left_chunk);
        queue->back = 0;
        memcpy((uint8_t*)queue->data + queue->back, (uint8_t*)data + left_chunk, right_chunk);
        queue->back += right_chunk;
    }
    queue->size += size;
    queue->count++;
}

void queue_dequeue(Queue* queue, size_t size, void* restrict data) {
    if (queue->front + size <= queue->capacity) {
        memcpy(data, (uint8_t*)queue->data + queue->front, size);
        queue->front += size;
    } else {
        size_t right_chunk = queue->front + size - queue->capacity;
        size_t left_chunk = size - right_chunk;
        memcpy(data, (uint8_t*)queue->data + queue->front, left_chunk);
        queue->front = 0;
        memcpy((uint8_t*)data + left_chunk, (uint8_t*)queue->data + queue->front, right_chunk);
        queue->front += right_chunk;
    }
    queue->size -= size;
    queue->count--;
}

#endif

#endif
