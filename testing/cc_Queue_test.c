#include "cc_Queue.h"

CC_QUEUE_DEFINE(int)

int main(void) {
    cc_Queue(int) queue = cc_Queue_new(int)((int[3]){0}, 3);
    cc_Queue_enqueue(int)(&queue, 1);
    cc_Queue_dequeue(int)(&queue);
    cc_Queue_enqueue(int)(&queue, 1);
    cc_Queue_enqueue(int)(&queue, 2);
    cc_Queue_enqueue(int)(&queue, 3);
    if (cc_Queue_dequeue(int)(&queue) != 1) return 1;
    if (cc_Queue_dequeue(int)(&queue) != 2) return 1;
    if (cc_Queue_dequeue(int)(&queue) != 3) return 1;
    return 0;
}