/*
 * A bit vector/field implementation
 * Written by Matthew Nappo
 */

#ifndef BITVEC_H
#define BITVEC_H

#include <inttypes.h>
#include <stddef.h>

struct BITVEC_T {
    uint8_t *b;
    size_t n;
};

typedef struct BITVEC_T *BitVec;

// Make a new bit vector
extern BitVec BitVec_new(int nbits);

// Set the ith bit
extern void BitVec_set();

// Get the decimal representation (it probably fits into a (4-byte) int
// on my machine at least)
extern int BitVec_decimal();

extern void BitVec_free(BitVec v);

#endif
