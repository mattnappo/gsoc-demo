/*
 * File: main.c
 * Author: Matthew Nappo
 * The entry point of the project. The main method runs a REPL that
 * parses the input string using both parsing methods: the recursive-
 * descent parser and the table-driven parser.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "parse_tree.h"
#include "rd_parser.h"
#include "td_parser.h"

void handle_recursive_descent(char *s)
{
    ParseTree t = recursive_descent_parse(s);   
    
    printf("\n--------------- BEGIN RECURSIVE DESCENT PARSE ---------------");
    if (t == NULL)
        printf("\nString is not parsable by the recursive-descent parser\n");
    else
        tree_print(t);
    tree_free(t);
    printf("---------------- END RECURSIVE DESCENT PARSE ----------------\n");
}

void handle_table_driven(char *s, ParseTable pt)
{
    printf("\n--------------- BEGIN TABLE-DRIVEN PARSE ----------------");
    ParseTree t = table_driven_parse(s, pt);
    
    if (t == NULL)
        printf("\nString is not parsable by the table-driven parser\n");
    else
        tree_print(t);
    tree_free(t);
    printf("---------------  END TABLE-DRIVEN PARSE -----------------\n\n");
}

void repl()
{
    printf("----- Entering REPL -----\n");

    Grammar g     = build_set_algebra_grammar();
    ParseTable pt = build_set_algebra_parsetable(g);

    while (1) {
        // Read string
        char s[256];
        printf("Enter string (or 'quit'): ");
        fgets(s, 256, stdin);
        s[strcspn(s, "\n")] = 0;
        if (strcmp(s, "quit") == 0) break;

        handle_recursive_descent(s);
        handle_table_driven(s, pt);
    }
    printf("\n");
}


#ifdef MAIN

int main(int argc, char *args[])
{
    repl();

    return 0;
}

#endif
