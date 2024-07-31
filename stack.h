#ifndef STACK_H
#define STACK_H

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct Stack {
    void*   data;       // Pointer to the start of data.
    size_t  size;       // Current size of the stack in bytes.
    size_t  capacity;   // Total capacity of the stack in bytes.
    size_t  count;      // Number of items in the stack, +1 per push and -1 per pop. 
    bool    dynamic;    // Whether the stack will dynamically reallocate to ensure enough capacity.
} Stack;

// Create stack with `capacity` bytes total capacity. If `dynamic` is set to true, the stack will reallocate on overflow.
Stack stack_create(size_t capacity, bool dynamic);

// Destroy `stack`, freeing all resources.
void stack_destroy(Stack* stack);

// Ensure `stack` has at least `size` bytes of capacity.
void stack_ensure(Stack* stack, size_t size);

// Add `size` bytes of `data` to the top of the `stack`.
void stack_push(Stack* stack, size_t size, const void* restrict data);

// Remove `size` bytes from the top of the `stack` and move them into `data`.
void stack_pop(Stack* stack, size_t size, void* restrict data);

#ifdef STACK_IMPLEMENTATION

Stack stack_create(size_t capacity, bool dynamic) {
    return (Stack) {
        .data = malloc(capacity),
        .capacity = capacity,
        .dynamic = dynamic,
        .size = 0,
        .count = 0,
    };
}

void stack_destroy(Stack* stack) {
    free(stack->data);
    stack->data = NULL;
    stack->count = 0;
    stack->size = 0;
    stack->dynamic = false;
    stack->capacity = 0;
}

void stack_ensure(Stack* stack, size_t size) {
    if (stack->capacity >= size) return;
    while (stack->capacity < size) {
        stack->capacity *= 2;
    }
    stack->data = realloc(stack->data, stack->capacity);
}

void stack_push(Stack* stack, size_t size, const void* restrict data) {
    if (stack->dynamic) { stack_ensure(stack, stack->size + size); }
    memcpy((uint8_t*)stack->data + stack->size, data, size);
    stack->size += size;
    stack->count++;
}

void stack_pop(Stack* stack, size_t size, void* restrict data) {
    stack->size -= size;
    memcpy(data, (uint8_t*)stack->data + stack->size, size);
    stack->count--;
}

#endif

#endif
