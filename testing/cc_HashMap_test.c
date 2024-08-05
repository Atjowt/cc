#include "cc_HashMap.h"

typedef char* string;

CC_HASHMAP_DEFINE(string, int)

/* TODO */

int main(void) {

    cc_HashMap(string, int) hashmap = cc_HashMap_new(string, int)((int[32]){0}, 32);

    cc_HashMap_insert(string, int)(&hashmap, "apple", 1);
    cc_HashMap_insert(string, int)(&hashmap, "banana", 2);
    cc_HashMap_insert(string, int)(&hashmap, "cantaloupe", 3);

    cc_HashMap_remove(string, int)(&hashmap, "banana");

    return 0;
}