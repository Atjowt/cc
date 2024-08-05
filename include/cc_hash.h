#ifndef CC_HASH_H
#define CC_HASH_H

#include "cc_core.h"

/**
 * Murmurhash implementation, modified from
 * 
 * `murmurhash.h' - murmurhash
 *
 * copyright (c) 2014-2022 joseph werle <joseph.werle@gmail.com>
 */
uint32_t cc_hash_murmurhash(const void* key, uint32_t size, uint32_t seed) {

    static const uint32_t c1 = 0xcc9e2d51;
    static const uint32_t c2 = 0x1b873593;
    static const uint32_t r1 = 15;
    static const uint32_t r2 = 13;
    static const uint32_t m = 5;
    static const uint32_t n = 0xe6546b64;

    uint8_t* data = (uint8_t*)key;
    const uint32_t* chunks = NULL;
    const uint8_t* tail = NULL;

    uint32_t h = 0;
    uint32_t k = 0;
    
    int i = 0;
    int chunk_size = size / 4;

    h = seed;

    chunks = (const uint32_t*)(data + 4 * chunk_size);
    tail = (const uint8_t*)(data + 4 * chunk_size);

    for (i = -chunk_size; i != 0; ++i) {

        k = chunks[i];

        k *= c1;
        k = (k << r1) | (k >> (32 - r1));
        k *= c2;

        h ^= k;
        h = (h << r2) | (h >> (32 - r2));
        h = h * m + n;
    }

    k = 0;

    switch (size & 3) {
        case 3: k ^= (tail[2] << 16);
        case 2: k ^= (tail[1] << 8);
        case 1: {
            k ^= tail[0];
            k *= c1;
            k = (k << r1) | (k >> (32 - r1));
            k *= c2;
            h ^= k;
        }
    }

    h ^= size;

    h ^= (h >> 16);
    h *= 0x85ebca6b;
    h ^= (h >> 13);
    h *= 0xc2b2ae35;
    h ^= (h >> 16);

    return h;
}

#endif