/*
 * File: nfa.c
 * Creator: Matthew Nappo
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "nfa.h"
#include "Set.h"
#include "LinkedList.h"
#include "strdup.h"

/**
 * Allocate and return a new NFA containing the given number of states.
 */
NFA new_NFA(int nstates)
{
    NFA nfa = calloc(sizeof(struct NFA_T), 1);
    nfa->n = nstates;
    nfa->s0 = -1;
    Set final_states = new_IntHashSet(nstates);
    nfa->F = final_states;

    // Construct the transition table
    nfa->d = calloc(nfa->n * sizeof(Set *), 1);
    for (int i = 0; i < nfa->n; i++) {
        nfa->d[i] = calloc(128 * sizeof(Set), 1);
        for (int j = 0; j < 128; j++) {
            Set set = new_Set(nstates);
            nfa->d[i][j] = set;
        }
    }
    return nfa;
}

/**
 * Free the given NFA.
 */
void NFA_free(NFA nfa)
{
    Set_free(nfa->F);
    for (int i = 0; i < nfa->n; i++) {
        for (int j = 0; j < 128; j++) {
            Set_free(nfa->d[i][j]);
        }
        free(nfa->d[i]);
    }
    free(nfa->d);
    if (nfa->label != NULL) {
        free(nfa->label);
    }
    free(nfa);
}

/**
 * Return the number of states in the given NFA.
 */
int NFA_get_size(NFA nfa)
{
    return nfa->n;
}

/**
 * Set the English description label of the NFA.
 */
void NFA_set_label(NFA nfa, char *label)
{
    nfa->label = strdup(label);
}

/**
 * Return the set of next states specified by the given NFA's transition
 * function from the given state on input symbol sym.
 */
Set NFA_get_transitions(NFA nfa, int state, char sym)
{
    if (state < 0 || state >= nfa->n) {
        printf("cannot read transitions from state %d (invalid state)\n", state);
        return NULL;
    }
    return *(*(nfa->d+state)+sym);
}

/**
 * For the given NFA, add the state dst to the set of next states from
 * state src on input symbol sym.
 */
void NFA_add_transition(NFA nfa, int src, char sym, int dst)
{
    if (dst >= nfa->n) {
        printf("cannot set d[%d]['%c'] = %d (invalid state %d)\n", src, sym, dst, dst);
        return;
    }
    if (src >= nfa->n) {
        printf("cannot set d[%d]['%c'] = %d (invalid state %d)\n", src, sym, dst, src);
        return;
    }
    Set_insert(*(*(nfa->d+src)+sym), dst);
}

/**
 * Add a transition for the given NFA for each symbol in the given str.
 */
void NFA_add_transition_str(NFA nfa, int src, char *str, int dst)
{
    while (*str) NFA_add_transition(nfa, src, *str++, dst);
}

/**
 * Add a transition for the given NFA for each input symbol.
 */
void NFA_add_transition_all(NFA nfa, int src, int dst)
{
    for (int i = 0; i < 128; i++) {
        NFA_add_transition(nfa, src, i, dst);
    }
}

/**
 * Set whether the given NFA's state is accepting or not.
 */
void NFA_set_accepting(NFA nfa, int state)
{
    Set_insert(nfa->F, state);
}

/**
 * Set the initial state of the NFA.
 */
void NFA_set_initial(NFA nfa, int state)
{
    if (state >= nfa->n) {
        printf("cannot set s0 = %d (invalid state %d)\n", state, state);
        return;
    }
    nfa->s0 = state;
}

/**
 * Return true if the given NFA's state is an accepting state.
 */
bool NFA_get_accepting(NFA nfa, int state)
{
    return Set_contains(nfa->F, state);
}

static bool NFA_execute_(NFA nfa, char *input)
{
    Set states = new_Set(nfa->n);
    Set_insert(states, nfa->s0);
    // For each input char
    while (*input) {
        Set next = new_Set(nfa->n);

        // For each state the NFA is in
        IntHashSetIterator it = IntHashSet_iterator(states);
        while (IntHashSetIterator_hasNext(it)) {
            int s = IntHashSetIterator_next(it);
            Set t = NFA_get_transitions(nfa, s, *input);
            Set_union(next, t);
        }
        free(it);

        // Update the current states to the next states
        Set_free(states); // only one error
        states = next;
        ++input; // Move to the next char
    }
    
    // Check that currentstates intersect finalstates != Nullset
    IntHashSetIterator it = IntHashSet_iterator(states);
    while (IntHashSetIterator_hasNext(it)) {
        int s = IntHashSetIterator_next(it);
        if (Set_contains(nfa->F, s)) {
            free(it);
            Set_free(states);
            return true;
        }
    }
    free(it);
    Set_free(states);
    return false;
}

bool NFA_execute(NFA nfa, char *input)
{
    bool accepts = NFA_execute_(nfa, input);
    if (accepts) printf("NFA accepts '%s'\n", input);
    else printf("NFA rejects '%s'\n", input);
    return accepts;
}

/**
 * Lanuch a REPL on the given NFA
 */
void NFA_repl(NFA nfa)
{
    printf("\n\nEntering REPL for the following NFA:");
    NFA_print(nfa);
    //NFA_print_withtable(nfa);

    while (1) {
        // Read string
        char s[128];
        printf("Enter string (or 'quit'): ");
        fgets(s, 128, stdin);
        s[strcspn(s, "\n")] = 0;
        if (strcmp(s, "quit") == 0) break;

        NFA_execute(nfa, s);
    }
    printf("\n");
}

/**
 * Print the given NFA to System.out along with the transition table.
 */
void NFA_print_withtable(NFA nfa)
{
    int s = 97; int v = 122;
    // int s = 32; int v = 127;

    printf("\n========== NFA PRINT ==========\n");
    printf("      purpose: %s\n", nfa->label);
    printf("     n states: %d\n", nfa->n);
    printf("initial state: %d\n", nfa->s0);
    printf("accepting states: ");
    Set_print(nfa->F);
    printf("\n");

    printf("abbreviated transition table:\n");
    // Print column headers
    printf("    ");
    for (int i = s; i <= v; i++) {
        printf("%2c ", i);
    }
    printf("\n");

    // Print rows of data
    for (int i = 0; i < nfa->n; i++) {
        printf("%2d| ", i); // Print state label
        for (int j = s; j <= v; j++) {
            Set_print(*(*(nfa->d+i)+j));
            printf(" ");
        }
        printf("\n");
    }
    printf("===============================\n");
}

/**
 * Print the given NFA to System.out without the transition table
 */
void NFA_print(NFA nfa)
{
    printf("\n========== NFA PRINT ==========\n");
    printf("      purpose: %s\n", nfa->label);
    printf("     n states: %d\n", nfa->n);
    printf("initial state: %d\n", nfa->s0);
    printf("accepting states: ");
    Set_print(nfa->F);
    printf("\n");
    printf("===============================\n");
}
