#include "cc_Stack.h"

CC_STACK_DEFINE(int)

int main(void) {
    cc_Stack(int) stack = cc_Stack_new(int)((int[3]){}, 3);
    cc_Stack_push(int)(&stack, 1);
    cc_Stack_push(int)(&stack, 2);
    cc_Stack_push(int)(&stack, 3);
    if (cc_Stack_pop(int)(&stack) != 3) return 1;
    if (cc_Stack_pop(int)(&stack) != 2) return 1;
    if (cc_Stack_pop(int)(&stack) != 1) return 1;
    return 0;
}