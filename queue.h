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
    size_t  head;       // Byte offset for the head of the queue.
    size_t  tail;       // Byte offset for the tail of the queue.
    size_t  count;      // Number of items in the queue, +1 per enqueue and -1 per dequeue.
    bool    dynamic;    // Whether the queue will dynamically reallocate to ensure enough capacity.
} Queue;

// Create queue with `capacity` bytes total capacity. If `dynamic` is set to true, the queue will reallocate on overflow.
Queue queue_create(size_t capacity, bool dynamic);

// Destroy `queue`, freeing all resources.
void queue_destroy(Queue* queue);

// Ensure `queue` has at least `size` bytes of capacity.
void queue_ensure(Queue* queue, size_t size);

// Add `size` bytes of `data` to the tail of `queue`.
void queue_enqueue(Queue* queue, size_t size, const void* data);

// Remove `size` bytes from the head of `queue` and move them into `data`.
void queue_dequeue(Queue* queue, size_t size, void* data);

#ifdef QUEUE_IMPLEMENTATION

Queue queue_create(size_t capacity, bool dynamic) {
    return (Queue) {
        .data = malloc(capacity),
        .capacity = capacity,
        .dynamic = dynamic,
        .count = 0,
        .size = 0,
        .head = 0,
        .tail = 0,
    };
}

void queue_destroy(Queue* queue) {
    free(queue->data);
    queue->data = NULL;
    queue->dynamic = false;
    queue->capacity = 0;
    queue->size = 0;
    queue->count = 0;
    queue->head = 0;
    queue->tail = 0;
}

void queue_ensure(Queue* queue, size_t size) {
    while (queue->capacity < size) {
        queue->capacity *= 2;
    }
    queue->data = realloc(queue->data, queue->capacity);
}

void queue_enqueue(Queue* queue, size_t size, const void* data) {
    if (queue->dynamic && queue->size + size >= queue->capacity) {
        queue_ensure(queue, queue->size + size);
    }
    if (queue->tail + size < queue->capacity) {
        memmove((uint8_t*)queue->data + queue->tail, data, size);
        queue->tail += size;
    } else {
        size_t excess_size = queue->tail + size - queue->capacity;
        size_t base_size = size - excess_size;
        memmove((uint8_t*)queue->data + queue->tail, data, base_size);
        queue->tail = 0;
        memmove((uint8_t*)queue->data + queue->tail, (uint8_t*)data + base_size, excess_size);
        queue->tail += excess_size;
    }
    queue->size += size;
    queue->count++;
}

void queue_dequeue(Queue* queue, size_t size, void* data) {
    if (queue->head + size < queue->capacity) {
        memmove(data, (uint8_t*)queue->data + queue->head, size);
        queue->head += size;
    } else {
        size_t excess_size = queue->head + size - queue->capacity;
        size_t base_size = size - excess_size;
        memmove(data, (uint8_t*)queue->data + queue->head, base_size);
        queue->head = 0;
        memmove((uint8_t*)data + base_size, (uint8_t*)queue->data + queue->head, excess_size);
        queue->head += excess_size;
    }
    queue->size -= size;
    queue->count--;
}

#endif

#endif
