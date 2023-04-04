/*
 * File: rd_parser.h
 * Author: Matthew Nappo
 * A recursive descent parser for the grammar of Set Algebra Expressions
 * Code is modeled from FOCS ch. 11
 */

#ifndef RD_PARSER_H
#define RD_PARSER_H

#include "parse_tree.h"

#define FAILED NULL

extern ParseTree recursive_descent_parse(char *s);
extern ParseTree expr(char **next_term);
extern ParseTree expr_tail(char **next_term);
extern ParseTree atomic(char **next_term);
extern ParseTree set(char **next_term);
extern ParseTree set_tail(char **next_term);
extern ParseTree elements(char **next_term);
extern ParseTree elements_tail(char **next_term);
extern ParseTree element(char **next_term);
extern ParseTree number(char **next_term);
extern ParseTree number_tail(char **next_term);
extern ParseTree digit(char **next_term);

#endif
