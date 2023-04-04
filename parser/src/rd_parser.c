/*
 * File: rd_parser.c
 * Author: Matthew Nappo
 * A recursive descent parser for the grammar of Set Algebra Expressions
 * Code is modeled from FOCS ch. 11
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "rd_parser.h"

ParseTree recursive_descent_parse(char *s) { return expr(&s); }

ParseTree expr(char **next_term)
{
    ParseTree atom;
    ParseTree expr_t;

    atom = atomic(next_term);
    if (atom == FAILED) {
        return FAILED;
    }
    else {
        (*next_term)++;
        expr_t = expr_tail(next_term);
        if (expr_t == FAILED)
            return FAILED;
        else
            return make_2child("Expr", atom, expr_t);
    }
}

ParseTree expr_tail(char **next_term)
{
    ParseTree ex;

    if (**next_term == 'U') { // Union production
        (*next_term)++;
        ex = expr(next_term);
        if (ex == FAILED)
            return FAILED;
        else
            return make_2child("ExprT", make_leaf("U"), ex);
    }
    else if (**next_term == '^') { // Intersection production
        (*next_term)++;
        ex = expr(next_term);
        if (ex == FAILED)
            return FAILED;
        else
            return make_2child("ExprT", make_leaf("^"), ex);
    }
    else { // Empty string production
        return make_1child("ExprT", make_leaf("e"));
    }
}

ParseTree atomic(char **next_term)
{
    ParseTree s;
    ParseTree ex;

    if (**next_term == '{') { // Production 2 (an atomic can be a set)
        s = set(next_term);
        if (s == FAILED)
            return FAILED;
        else
            return make_1child("Atomic", s);
    }
    else if (**next_term == '(') { // Production 1; <Atomic> -> (<Expr>)
        (*next_term)++;
        ex = expr(next_term);
        if (ex == FAILED)
            return FAILED;
        else if (**next_term == ')')
            return make_3child("Atomic", make_leaf("("), ex, make_leaf(")"));
    }
    return FAILED;
}

ParseTree set(char **next_term)
{
    ParseTree tail;

    if (**next_term == '{') {
        (*next_term)++;
        tail = set_tail(next_term);
        if (tail == FAILED)
            return FAILED;
        else
            return make_2child("Set", make_leaf("{"), tail);
    }
    else {
        return FAILED;
    }

    return NULL;
}

ParseTree set_tail(char **next_term)
{
    ParseTree elmts; // For the second production Tail -> Elements }

    elmts = elements(next_term); // Production 2
    if (elmts != FAILED) {
        if (**next_term == '}')
            return make_2child("SetT", elmts, make_leaf("}"));
        else
            return FAILED;
    }
    else if (**next_term == '}') { // Production 1: Tail -> }
        return make_1child("SetT", make_leaf("}"));
    }
    return FAILED;
}

ParseTree elements(char **next_term)
{
    // <Elements> --> <Element> <ElementsTail>
    ParseTree elmt;    // <Element>
    ParseTree elmts_t; // <ElementsTail>

    elmt = element(next_term);
    if (elmt == FAILED) {
        return FAILED;
    }
    else {
        elmts_t = elements_tail(next_term);
        if (elmts_t == FAILED)
            return FAILED;
        else
            return make_2child("Elmts", elmt, elmts_t);
    }
}

ParseTree elements_tail(char **next_term)
{
    ParseTree elmts;

    if (**next_term == ',') { // <E> -> , <E> production
        (*next_term)++;
        elmts = elements(next_term);
        if (elmts == FAILED)
            return FAILED;
        else
            return make_2child("ElmtsT", make_leaf(","), elmts);
    }
    else { // <E> --> e production
        return make_1child("ElmtsT", make_leaf("e"));
    }
}

ParseTree element(char **next_term)
{
    // An element is just a number
    ParseTree num;

    num = number(next_term);
    if (num == FAILED)
        return FAILED;
    else
        return make_1child("Elmt", num);
}

ParseTree number(char **next_term)
{
    // The two elements of the production
    // <N> --> <D> <Nt>
    ParseTree dig;
    ParseTree num_tail;

    dig = digit(next_term);
    if (dig == FAILED) {
        return FAILED;
    }
    else {
        (*next_term)++;
        num_tail = number_tail(next_term);
        if (num_tail == FAILED)
            return FAILED;
        else
            return make_2child("Num", dig, num_tail);
    }
}

ParseTree number_tail(char **next_term)
{
    // Body
    ParseTree num;

    num = number(next_term);
    if (num != FAILED) // Use production 1 if it starts with a number
        return make_1child("NumT", num);
    else // Use production 2 otherwise
        return make_1child("NumT", make_leaf("e"));
    return FAILED;
}

ParseTree digit(char **next_term)
{
    for (int i = 0; i < 10; i++) {
        if (**next_term == (char)(48+i)) {
            char *s = calloc(2, 1);
            char c = 48+i;
            strncat(s, &c, 1);
            return make_1child("Dig", make_leaf(s));
        }
    }
    return FAILED;
}

