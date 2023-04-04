/*
 * File: grammar.h
 * Author: Matthew Nappo
 * Formally define a generic context-free-grammar by defining context-free
 * productions and constituents of those productions.
 */

#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <stdbool.h>

// All productions in the grammar of Set Algebra have no more than
// 3 constituents. For different grammars, this value must be changed.
#define BODY_MAX 3

// This grammar, for set algebra, has 26 productions.
#define GRAMMAR_MAX 26

// A constituent of a production. Also the type that is stored on the stack.
struct CONSTITUENT_T {
    char terminal;
    char *category;
    bool is_terminal; // True if and only if this constituent is a terminal
};

// A production in a CFG.
struct PRODUCTION_T {
    // The head of the production, which is also the label of the syntactic
    // category. This is the same as the "char *category" field in a 
    // constituent
    char *head; 

    // The body of a production is an (ordered) collection of constituents
    struct CONSTITUENT_T **body;

    // Number of constituents in this production
    unsigned int len;
};

// A context free grammar: a set of context-free productions. This type isn't
// really necessary. It is just a nice abstraction to have.
struct GRAMMAR_T {
    struct PRODUCTION_T **productions;
    unsigned int len;
};

typedef struct CONSTITUENT_T *Constituent;
typedef struct PRODUCTION_T  *Production;
typedef struct GRAMMAR_T     *Grammar;

/* Constituents */
extern Constituent init_constituent_terminal(char terminal);
extern Constituent init_constituent_category(char *category);
extern void        print_constituent(Constituent c);
extern void        free_constituent(Constituent c);

/* Productions */
extern Production init_production(char *category);
extern void       production_add_constituent(Production p, Constituent c);
extern void       print_production(Production p);
extern void       free_production(Production p);

/* Grammars */
extern Grammar init_grammar();
extern void    grammar_add_production(Grammar g, Production p);
extern void    print_grammar(Grammar g);
extern void    grammar_free(Grammar g);
extern Grammar build_set_algebra_grammar();

#endif
