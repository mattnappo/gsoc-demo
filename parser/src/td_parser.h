/*
 * File: td_parser.h
 * Author: Matthew Nappo
 * A table-driven LL(k) parser
 */

#ifndef TD_PARSER_H
#define TD_PARSER_H

#include "parse_tree.h"
#include "parse_table.h"

extern ParseTree table_driven_parse(char *s, ParseTable pt);

#endif

