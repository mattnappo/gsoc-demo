/*
 * File: parse_tree.c
 * Author: Matthew Nappo
 * Implementation code for helper methods of a parse tree.
 * This code was adapted from the textbook FOCS chapter 11.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "parse_tree.h"
#include "strdup.h"

bool found = false;

ParseTree make_leaf(char *x)
{
    ParseTree t = malloc(sizeof(struct NODE_T));
    t->label = strdup(x);
    t->lmc = NULL;
    t->rs = NULL;
    return t;
}

ParseTree make_1child(char *x, ParseTree t1)
{
    ParseTree t = make_leaf(x);
    t->lmc = t1;
    t1->parent = t;
    return t;
}

ParseTree make_2child(char *x, ParseTree t1, ParseTree t2)
{
    ParseTree t = make_1child(x, t1);
    t1->rs = t2;
    t2->parent = t1;
    return t;
}

ParseTree make_3child(char *x, ParseTree t1, ParseTree t2, ParseTree t3)
{
    ParseTree t = make_2child(x, t1, t2);
    t1->rs = t2;
    t2->rs = t3;

    t2->parent = t1;
    t3->parent = t1;
    return t;
}

void add_child(ParseTree t, ParseTree child)
{
    if (t->lmc == NULL) {
        t->lmc = child;
    }
    else {
        ParseTree tmp = t->lmc;
        while (tmp->rs != NULL)
            tmp = tmp->rs;
        tmp->rs = child;
    }
    child->parent = t;
}

ParseTree right_most_child(ParseTree t)
{
    if (t == NULL || t->lmc == NULL) return NULL;
    t = t->lmc;
    while (t->rs != NULL) t = t->rs;
    return t;
}

static void left_most_category_leaf_(ParseTree t, ParseTree *c, bool add)
{
    if (t == NULL) return;

    // Go all the way to the left
    if (t->lmc != NULL) {
        left_most_category_leaf_(t->lmc, c, add);
    }
    
    // Now, the LMC is null. Read out all the siblings

    // Return it if it is a leaf category
    if (t->lmc == NULL) {           // If leaf
        if (strlen(t->label) > 1) { // If category
            if (!found) {
                if (add) {
                    t->lmc = *c;
                    (*c)->parent = t;
                }
                else {
                    *c = t;
                }
                found = true;
                return;
            }
        }
    }

    // Check right siblings
    if (t->rs != NULL)
        left_most_category_leaf_(t->rs, c, add);
}

ParseTree left_most_category_leaf(ParseTree t, ParseTree c, bool add)
{
    // If head, return
    if (t->lmc == NULL) {
        if (add) {
            t->lmc = c;
            c->parent = t;
        }
        return t;
    }

    left_most_category_leaf_(t->lmc, &c, add);
    found = false;
    if (add) return c->parent;
    else     return c;
}

static void tree_print_(ParseTree t, int indent)
{
    for (int i = 0; i < indent; i++) printf(" ");
    printf("%s\n", t->label);

    if (t->lmc != NULL) {
        tree_print_(t->lmc, indent+1+strlen(t->label));
    }

    if (t->rs != NULL) {
        tree_print_(t->rs, indent);
    }
}

void yield_(ParseTree t)
{
    if (t == NULL) return;

    // Go all the way to the left
    if (t->lmc != NULL) {
        yield_(t->lmc);
    }
    
    // Now, the LMC is null. Read out all the siblings

    // Print it if it is a leaf
    if (t->lmc == NULL) {
        if (strcmp(t->label, "e") != 0)
            printf("%s", t->label);
    }

    // Move on to the next sibling
    if (t->rs != NULL)
        yield_(t->rs);
}

void yield(ParseTree t)
{
    printf("yield: ");
    yield_(t);
    printf("\n");
}

void tree_print(ParseTree t)
{
    if (t == NULL) {
        printf("<null tree>\n");
        return;
    }
    printf("\n==================== PARSE TREE ====================\n");
    tree_print_(t, 0);
    yield(t);
    printf("====================================================\n");
}

void tree_free_(ParseTree t)
{
    // ONLY REMOVE IF IT IS A LEAF
    // So, first we go deep then we remove like it is a linked list recursively
    // Therefore, lmc takes precedence

    // If it is a leaf, free it
    if (!(t->lmc || t->rs)) {
        free(t->label);
        free(t);
    }

    // Branch down the left
    if (t->lmc) {
        tree_free_(t->lmc);
    }

    // If there is no left child, go down the siblings
    else {
        while (t->rs != NULL) {
            if (t->rs->rs == NULL) {
                tree_free_(t->rs);
            }
            else {
                ParseTree old = t->rs;
                t->rs = t->rs->rs;
                tree_free_(old);
                //free(old->label);
                //free(old);
            }
        }
    }
}

void tree_free(ParseTree t) {} // Headache for another day

