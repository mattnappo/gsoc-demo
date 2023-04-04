/* File: translator.h
 * Author: Matthew Nappo
 * Convert a well-defined NFA to a DFA using the subset construction
 * algorithm.
 */

#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include "nfa.h"
#include "dfa.h"

/* Convert a well-defined NFA to a DFA using the subset construction
 * algorithm.
 * This function makes part 3 of the project.
 */
extern DFA NFA_to_DFA(NFA nfa);

extern int get_index(NFA nfa, Set subset);

#endif
