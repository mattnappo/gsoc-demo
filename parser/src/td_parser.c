/*
 * File: td_parser.c
 * Author: Matthew Nappo
 * A table-driven LL(k) parser
 */

#include <stdlib.h>
#include <stdio.h>
#include "td_parser.h"
#include "stack.h"
#include "strdup.h"

ParseTree table_driven_parse(char *s, ParseTable pt)
{
    Stack stack = init_stack();
    // Push the start symbol <Expr>
    push(stack, init_constituent_category("Expr"));

    // The parse tree
    ParseTree tree = make_leaf("Expr");

    // Keep a pointer to the currrent node of the parse tree
    ParseTree curr = tree;

    int i = 0;
    char **sp = &s;

    // While there are still parsing goals and chars to read
    while(!is_empty(stack) && **sp != 0x0) {
        Constituent top = (Constituent) pop(stack);

        // If the top of the stack is a terminal
        if (top->is_terminal) {
            if (top->terminal == EPSILON || top->terminal == **sp) {
                (*sp)++;
                ++i;
                continue;
            }
            else {
                printf("failed in terminal case\n");
                return NULL;
            }
        }

        // If the top of the stack is a syntactic category
        if (!top->is_terminal) {
            Production p = parsetable_lookup(pt, top->category, **sp);
            // If there is no production on the next lookahead symbol
            if (p == NULL) {
                // Then check for an epsilon production
                p = parsetable_lookup(pt, top->category, EPSILON);
                // If there is no epsilon production either, string not parsable
                if (p == NULL) return NULL; 

                // If there is an epsilon production, just move on after popping
                else {
                    left_most_category_leaf(tree, make_leaf("e"), true);
                    ++i;
                    continue;
                }
            }

            // Get the left-most leaf that is a syntactic category
            curr = left_most_category_leaf(tree, NULL, false);
            for (int i = 0; i < p->len; i++) { // Add the constituents to the tree
                if (p->body[i]->is_terminal)
                    add_child(curr, make_leaf(&p->body[i]->terminal));
                else
                    add_child(curr, make_leaf(p->body[i]->category));
            }

            // If there is a production, push the constituents from right to left
            for (int i = p->len-1; i >= 0; i--) {
                push(stack, p->body[i]);
            }
            ++i;
        }
    }

    // Cleanup: add the last e transition for ENDM symbol
    left_most_category_leaf(tree, make_leaf("e"), true);
    return tree;
}

