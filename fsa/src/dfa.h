/*
 * File: dfa.h
 * Creator by Matthew Nappo
 */

#ifndef _dfa_h
#define _dfa_h

#include <stdbool.h>
#include "Set.h"
#include "strdup.h"

/**
 * The data structure used to represent a deterministic finite automaton.
 */

struct DFA_T {
    int n; // Number of states
    int s0; // Initial state
    Set F; // Set of final states
    int **d; // The transition table

    char *label; // Not part of the formal model. Just an English description of the automata
};

typedef struct DFA_T *DFA;

/**
 * Allocate and return a new DFA containing the given number of states.
 */
extern DFA new_DFA(int nstates);

/**
 * Free the given DFA.
 */
extern void DFA_free(DFA dfa);

/**
 * Return the number of states in the given DFA.
 */
extern int DFA_get_size(DFA dfa);

/**
 * Set the English description label of the DFA.
 */
extern void DFA_set_label(DFA dfa, char *label);

/**
 * Return the state specified by the given DFA's transition function from
 * state src on input symbol sym.
 */
extern int DFA_get_transition(DFA dfa, int src, char sym);

/**
 * For the given DFA, set the transition from state src on input symbol
 * sym to be the state dst.
 */
extern void DFA_set_transition(DFA dfa, int src, char sym, int dst);

/**
 * Set the transitions of the given DFA for each symbol in the given str.
 * This is a nice shortcut when you have multiple labels on an edge between
 * two states.
 */
extern void DFA_set_transition_str(DFA dfa, int src, char *str, int dst);

/**
 * Set the transitions of the given DFA for all input symbols.
 * Another shortcut method.
 */
extern void DFA_set_transition_all(DFA dfa, int src, int dst);

/**
 * Set whether the given DFA's state is accepting or not.
 */
extern void DFA_set_accepting(DFA dfa, int state);

/**
 * Set the initial state of the DFA.
 */
extern void DFA_set_initial(DFA dfa, int state);

/**
 * Return true if the given DFA's state is an accepting state.
 */
extern bool DFA_get_accepting(DFA dfa, int state);

/**
 * Run the given DFA on the given input string, and return true if it accepts
 * the input, otherwise false.
 */
extern bool DFA_execute(DFA dfa, char *input);

/**
 * Lanuch a REPL on the given DFA
 */
extern void DFA_repl(DFA dfa);

/**
 * Print the given DFA to System.out along with the transition table.
 */
extern void DFA_print_withtable(DFA dfa);
/**
 * Print the given DFA to System.out without the transition table.
 */
extern void DFA_print(DFA dfa);

#endif
