/*
 * File: parse_table.h
 * Author: Matthew Nappo
 * A parse table for LL(k) table-driven parsers. Implemented by a Hashmap with
 * linear probing. Maps syntactic category-terminal pairs
 */

#ifndef PARSE_TABLE_H
#define PARSE_TABLE_H

#include <stdbool.h>
#include <inttypes.h>
#include "grammar.h"

// Maximum number of entries in the hashmap
#define HASHMAP_SIZE 128

// Probing interval for linear probing resolution
#define PROBE_INTERVAL 1

#define EPSILON 0x65

struct KEY_T {
    char *category;
    char terminal;
};

typedef struct KEY_T *Key;

struct PARSETABLE_T {
    Key         *keys;
    Production  *map;
    unsigned int load_factor;
};

typedef struct PARSETABLE_T *ParseTable;

extern ParseTable init_parsetable();
extern void parsetable_put(ParseTable pt, char *category, char terminal, Production p);
extern Production parsetable_lookup(ParseTable pt, char *category, char terminal);
extern void parsetable_print(ParseTable pt);
extern void free_parsetable(ParseTable pt);

extern ParseTable build_set_algebra_parsetable(Grammar g);

#endif
