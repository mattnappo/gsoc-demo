/*
 * File: parse_table.c
 * Author: Matthew Nappo
 * A parse table for LL(k) table-driven parsers. Implemented by a Hashmap with
 * linear probing. Maps syntactic category-terminal pairs
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "parse_table.h"
#include "grammar.h"
#include "strdup.h"

static uint32_t hash(Key k)
{
    char *dest = calloc(64, 1);
    strcpy(dest, k->category);
    strncat(dest, &k->terminal, 1);

    uint32_t hash = 7;
    for (int i = 0; i < strlen(dest); i++) {
        hash = hash*31 + dest[i];
    }
    hash %= HASHMAP_SIZE;

    return hash;
}

// Compare two keys
static bool key_equality(Key k1, Key k2)
{
    if (
        strcmp(k1->category, k2->category) == 0 &&
        k1->terminal == k2->terminal
    ) { return true; }
    return false;
}

ParseTable init_parsetable() {
    ParseTable pt    = malloc(sizeof(struct PARSETABLE_T));
    Key        *keys = calloc(sizeof(struct KEY_T) * HASHMAP_SIZE, 1);
    Production *map  = calloc(sizeof(Production)   * HASHMAP_SIZE, 1);
    for (int i = 0; i < HASHMAP_SIZE; i++) {
        keys[i] = NULL;
        map[i]  = NULL;
    }
    pt->map = map;
    pt->keys = keys;
    pt->load_factor = 0;
    return pt;
}

static Key init_key(char *category, char terminal)
{
    Key k = malloc(sizeof(struct KEY_T));
    k->category = strdup(category);
    k->terminal = terminal;
    return k;
}

void parsetable_put(ParseTable pt, char *category, char terminal, Production p)
{
    Key k = init_key(category, terminal);

    uint32_t hash_code = hash(k);
    
    // If the slot is null, insert the value directly
    if (!pt->map[hash_code]) {
        pt->keys[hash_code] = k;
        pt->map [hash_code] = p;
        ++pt->load_factor;
        return;
    }

    // If the slot is not NULL, continuously try the next slot
    else if (pt->map[hash_code]) {
        for (int i = 1; i < HASHMAP_SIZE-hash_code; i+=PROBE_INTERVAL) {
            if (pt->map[hash_code+i] != NULL) {
                if (key_equality(k, pt->keys[hash_code+i])) {
                    printf("key is already in parse table\n");
                    return;
                }
            }

            // If it is a null slot, take it
            if (pt->map[hash_code+i] == NULL) {
                pt->keys[hash_code+i] = k;
                pt->map [hash_code+i]  = p;
                ++pt->load_factor;
                return;
            }
            // Else, just increment i
        }
        printf("hashmap overflow\n");
    }
}

Production parsetable_lookup(ParseTable pt, char *category, char terminal)
{
    Key k = init_key(category, terminal);
    uint32_t hash_code = hash(k);

    // Not in parse table
    if (pt->map[hash_code] == NULL) return NULL;

    for (int i = 0; i < HASHMAP_SIZE-hash_code; i++) {
        if (pt->map[hash_code+i] != NULL) {
            if (key_equality(k, pt->keys[hash_code+i]))
                return pt->map[hash_code+i];
        }
    }

    return NULL; // Not in parse table
}

void parsetable_print(ParseTable pt)
{
    printf("[\n");
    for (int i = 0; i < HASHMAP_SIZE; i++) {
        Key k = pt->keys[i];
        if (k != NULL) {
            printf("\t(%s, %c) --> ", k->category, k->terminal);
            print_production(pt->map[i]);
        }
    }
    printf("]\n");
}

void free_parsetable(ParseTable pt)
{
    // Don't free the productions themselves (unless?)
    free(pt->map);
    free(pt->keys);
    free(pt);
}


#define A_INSERT(CATG, TERM, PROD) \
    (parsetable_put(pt, CATG, TERM, g->productions[PROD]))

// Build and return the parse table for the set algebra grammar
ParseTable build_set_algebra_parsetable(Grammar g)
{
    ParseTable pt = init_parsetable();
    
    A_INSERT("Expr",    '{', 0);
    A_INSERT("Expr",    '(', 0);
    A_INSERT("ExprT",   '^', 2);
    A_INSERT("ExprT",   'U', 1);
    A_INSERT("Atomic",  '{', 4);
    A_INSERT("Atomic",  '(', 3);
    A_INSERT("Set",     '{', 5);
    A_INSERT("SetT",    '}', 6);
    A_INSERT("ElmtsT", ',', 9);
    for (int i = 0; i < 10; i++) {
        A_INSERT("SetT",  (char)(48+i), 7);
        A_INSERT("Elmts", (char)(48+i), 8);
        A_INSERT("Elmt",  (char)(48+i), 10);
        A_INSERT("Num",   (char)(48+i), 11);
        A_INSERT("NumT",  (char)(48+i), 12);
        A_INSERT("Dig",   (char)(48+i), 13+i);
    }

    A_INSERT("ExprT",  EPSILON, 23);
    A_INSERT("ElmtsT", EPSILON, 24);
    A_INSERT("NumT",   EPSILON, 25);

    return pt;
}

