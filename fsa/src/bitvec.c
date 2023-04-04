/*
 * File: bitvec.c
 * Author: Matthew Nappo
 */

#include <stdlib.h>
#include <stdio.h>
#include "bitvec.h"

// Make a new bit vector
BitVec BitVec_new(int nbits)
{
    size_t n_bytes = (7+nbits)/8; // Round up
    uint8_t *b = calloc(n_bytes, 1);
    BitVec v = calloc(sizeof(struct BITVEC_T), 1);
    v->b = b;
    v->n = n_bytes;
    return v;
}

// Set the ith bit
void BitVec_set(BitVec v, int i)
{
    size_t byte = i / 8; // Round down to determine which byte i falls into
    size_t j = i % 8; // How far from right-most bit is i (position in the byte)

    // Start with 00000001, and bitshift left i times.
    // Then OR to set the bit in place at the calculated byte
    v->b[byte] |= 1 << j;
}

// Get the decimal representation. It will fit into a 32-bit integer (int)
// if the number of original bits < 32. So, number of states of NFA must be < 32
int BitVec_decimal(BitVec v)
{
    int d = 0;
    for (int i = 0; i < v->n; i++) d += (int) v->b[i];
    return d;
}

void BitVec_free(BitVec v)
{
    free(v->b);
    free(v);
}

