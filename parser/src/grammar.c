/*
 * File: grammar.c
 * Author: Matthew Nappo
 * Formally define a generic context-free-grammar by defining context-free
 * productions and constituents of those productions.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grammar.h"
#include "strdup.h"
#include "parse_table.h"

/* Constituents */

Constituent init_constituent_terminal(char terminal)
{
    Constituent c = calloc(sizeof(struct CONSTITUENT_T), 1);
    c->terminal = terminal;
    c->category = NULL;
    c->is_terminal = true;
    return c;
}

Constituent init_constituent_category(char *category)
{
    Constituent c = malloc(sizeof(struct CONSTITUENT_T));
    c->category = strdup(category);
    c->terminal = 0x0;
    c->is_terminal = false;
    return c;
}

void print_constituent(Constituent c)
{
    if (c->is_terminal) printf("%c", c->terminal);
    else printf("<%s>", c->category);
}

void free_constituent(Constituent c)
{
    if (!c->is_terminal)
        free(c->category);
    free(c);
}

/* Productions */

Production init_production(char *category)
{
    Production p = malloc(sizeof(struct PRODUCTION_T));
    p->head = strdup(category);
    p->body = malloc(sizeof(Constituent) * BODY_MAX);
    p->len = 0;
    return p;
}

void production_add_constituent(Production p, Constituent c)
{
    if (p->len < BODY_MAX) p->body[p->len++] = c;
}

void print_production(Production p)
{
    if (p == NULL) { printf("NULL\n"); return; }
    printf("<%s> --> ", p->head);
    for (int i = 0; i < p->len; i++) {
        print_constituent(p->body[i]);
        printf(" ");
    }
    printf("\n");
}

void free_production(Production p)
{
    free(p->head); // b/c malloc'd with strdup
    for (int i = 0; i < p->len; i++) {
        free_constituent(p->body[i]);
    }
    free(p->body);
    free(p);
}

/* Grammars */

Grammar init_grammar()
{
    Grammar g = malloc(sizeof(struct GRAMMAR_T));
    g->productions = malloc(sizeof(Production) * GRAMMAR_MAX);
    g->len = 0;
    return g;
}

void grammar_add_production(Grammar g, Production p)
{
    if (g->len < GRAMMAR_MAX)
        g->productions[g->len++] = p;
}

void print_grammar(Grammar g)
{
    for (int i = 0; i < g->len; i++)
        print_production(g->productions[i]);
}

void grammar_free(Grammar g)
{
    for (int i = 0; i < g->len; i++) {
        free_production(g->productions[i]);
    }
    free(g->productions);
    free(g);
}

Grammar build_set_algebra_grammar()
{
    /* BUILD THE GRAMMAR */

    Constituent cup = init_constituent_terminal('U');
    Constituent cap = init_constituent_terminal('^');
    Constituent com = init_constituent_terminal(',');
    Constituent ob  = init_constituent_terminal('{');
    Constituent cb  = init_constituent_terminal('}');
    Constituent op  = init_constituent_terminal('(');
    Constituent cp  = init_constituent_terminal(')');
    Constituent eps = init_constituent_terminal(EPSILON);

    Constituent expr   = init_constituent_category("Expr");
    Constituent exprT  = init_constituent_category("ExprT");
    Constituent atomic = init_constituent_category("Atomic");
    Constituent set    = init_constituent_category("Set");
    Constituent setT   = init_constituent_category("SetT");
    Constituent elmts  = init_constituent_category("Elmts");
    Constituent elmtsT = init_constituent_category("ElmtsT");
    Constituent elmt   = init_constituent_category("Elmt");
    Constituent num    = init_constituent_category("Num");
    Constituent numT   = init_constituent_category("NumT");
    Constituent dig    = init_constituent_category("Dig");

    Production p1 = init_production("Expr");
    production_add_constituent(p1, atomic);
    production_add_constituent(p1, exprT);

    Production p2 = init_production("ExprT");
    production_add_constituent(p2, cup);
    production_add_constituent(p2, expr);

    Production p3 = init_production("ExprT");
    production_add_constituent(p3, cap);
    production_add_constituent(p3, expr);

    Production p4 = init_production("Atomic");
    production_add_constituent(p4, op);
    production_add_constituent(p4, expr);
    production_add_constituent(p4, cp);

    Production p5 = init_production("Atomic");
    production_add_constituent(p5, set);

    Production p6 = init_production("Set");
    production_add_constituent(p6, ob);
    production_add_constituent(p6, setT);

    Production p7 = init_production("SetT");
    production_add_constituent(p7, cb);

    Production p8 = init_production("SetT");
    production_add_constituent(p8, elmts);
    production_add_constituent(p8, cb);
    
    Production p9 = init_production("Elmts");
    production_add_constituent(p9, elmt);
    production_add_constituent(p9, elmtsT);
    
    Production p10 = init_production("ElmtsT");
    production_add_constituent(p10, com);
    production_add_constituent(p10, elmts);
    
    Production p11 = init_production("Elmt");
    production_add_constituent(p11, num);
    
    Production p12 = init_production("Num");
    production_add_constituent(p12, dig);
    production_add_constituent(p12, numT);
    
    Production p13 = init_production("NumT");
    production_add_constituent(p13, num);

    // Epsilon productions
    Production e1 = init_production("ExprT");
    production_add_constituent(e1, eps);

    Production e2 = init_production("Elmts");
    production_add_constituent(e2, eps);

    Production e3 = init_production("NumT");
    production_add_constituent(e3, eps);

    Grammar g = init_grammar();
    Production productions[] = {
        p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13
    };

    for (int i = 0; i < sizeof(productions)/sizeof(productions[0]); i++) {
        grammar_add_production(g, productions[i]);
    }

    for (int i = 0; i < 10; i++) {
        Production p = init_production("Dig");
        production_add_constituent(p, init_constituent_terminal(48+i));
        grammar_add_production(g, p);
    }

    grammar_add_production(g, e1);
    grammar_add_production(g, e2);
    grammar_add_production(g, e3);

    return g;
}

