/*
 * File: parse_tree.h
 * Author: Matthew Nappo
 * Header file for helper methods of a parse tree.
 * This code was adapted from the textbook FOCS chapter 11.
 */

#ifndef PARSE_TREE_H
#define PARSE_TREE_H

#include <stdbool.h>

typedef struct NODE_T *ParseTree;

// An LMC-RS tree
struct NODE_T {
    char *label;
    ParseTree lmc, rs;
    ParseTree parent;
};

extern ParseTree make_leaf(char *x);
extern ParseTree make_1child(char *x, ParseTree t1);
extern ParseTree make_2child(char *x, ParseTree t1, ParseTree t2);
extern ParseTree make_3child(char *x, ParseTree t1, ParseTree t2, ParseTree t3);
extern void      add_child(ParseTree t, ParseTree child);
extern ParseTree right_most_child(ParseTree t);
extern ParseTree left_most_category_leaf(ParseTree t, ParseTree c, bool add);
extern void      yield();
extern void      tree_print(ParseTree t);
extern void      tree_free(ParseTree t);

#endif

