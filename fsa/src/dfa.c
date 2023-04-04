/*
 * File: dfa.c
 * Creator: Matthew Nappo
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "dfa.h"
#include "Set.h"
#include "strdup.h"

/**
 * Allocate and return a new DFA containing the given number of states.
 */
DFA new_DFA(int nstates)
{
    DFA dfa = malloc(sizeof(struct DFA_T));
    dfa->n = nstates;
    dfa->s0 = -1; // Undefined initial state so far

    // Size of set can be at most nstates, so let's be safe here
    Set final_states = new_IntHashSet(nstates);
    dfa->F = final_states;

    // Construct a blank transition table
    dfa->d = malloc(dfa->n * sizeof(int *));
    for (int i = 0; i < dfa->n; i++) {
        size_t s = 128*sizeof(int);
        dfa->d[i] = malloc(s);
        memset(dfa->d[i], -1, s);
    }

    return dfa;
}

/**
 * Free the given DFA.
 */
void DFA_free(DFA dfa)
{
    IntHashSet_free(dfa->F);
    for (int i = 0; i < dfa->n; i++) {
        free(dfa->d[i]);
    }
    free(dfa->d);
    free(dfa->label);
    free(dfa);
}

/**
 * Return the number of states in the given DFA.
 */
int DFA_get_size(DFA dfa)
{
    return dfa->n;
}

/**
 * Set the English description label of the DFA.
 */
void DFA_set_label(DFA dfa, char *label)
{
    dfa->label = strdup(label);
}

/**
 * Return the state specified by the given DFA's transition function from
 * state src on input symbol sym.
 */
int DFA_get_transition(DFA dfa, int src, char sym)
{
    if (src >= dfa->n || src < 0) {
        printf("cannot read transitions from state %d (invalid state)\n", src);
        return -1;
    }

    return *(*(dfa->d+src)+sym);
}

/**
 * For the given DFA, set the transition from state src on input symbol
 * sym to be the state dst.
 */
void DFA_set_transition(DFA dfa, int src, char sym, int dst)
{
    if (dst >= dfa->n) {
        printf("cannot set d[%d]['%c'] = %d (invalid state %d)\n", src, sym, dst, dst);
        return;
    }
    if (src >= dfa->n) {
        printf("cannot set d[%d]['%c'] = %d (invalid state %d)\n", src, sym, dst, src);
        return;
    }
    *(*(dfa->d+src)+sym) = dst;
}

/**
 * Set the transitions of the given DFA for each symbol in the given str.
 * This is a nice shortcut when you have multiple labels on an edge between
 * two states.
 */
void DFA_set_transition_str(DFA dfa, int src, char *str, int dst)
{
    while (*str) DFA_set_transition(dfa, src, *str++, dst);
}

/**
 * Set the transitions of the given DFA for all input symbols.
 * Another shortcut method.
 */
void DFA_set_transition_all(DFA dfa, int src, int dst) 
{
    for (int i = 0; i < 128; i++) {
        DFA_set_transition(dfa, src, i, dst);
    }
}

/**
 * Set whether the given DFA's state is accepting or not.
 */
void DFA_set_accepting(DFA dfa, int state)
{
    Set_insert(dfa->F, state);
}

/**
 * Set the initial state of the DFA.
 */
void DFA_set_initial(DFA dfa, int state)
{
    if (state >= dfa->n) {
        printf("cannot set s0 = %d (invalid state %d)\n", state, state);
        return;
    }
    dfa->s0 = state;
}

/**
 * Return true if the given DFA's state is an accepting state.
 */
bool DFA_get_accepting(DFA dfa, int state)
{
    return Set_contains(dfa->F, state);
}

/**
 * Run the given DFA on the given input string, and return true if it accepts
 * the input, otherwise false.
 */
static bool DFA_execute_(DFA dfa, char *input)
{
    int state = dfa->s0; // The current state
    while (*input) {
        int n_state = DFA_get_transition(dfa, state, *input++);
        // Immediately reject upon reading symbol which does not have a transition
        // From the current state
        if (n_state == -1) return false;
        else state = n_state;
    }
    return Set_contains(dfa->F, state);
}

bool DFA_execute(DFA dfa, char *input)
{
    bool accepts = DFA_execute_(dfa, input);
    if (accepts) printf("DFA accepts '%s'\n", input);
    else printf("DFA rejects '%s'\n", input);
    return accepts;
}

/**
 * Lanuch a REPL on the given DFA
 */
void DFA_repl(DFA dfa)
{
    printf("\n\nEntering REPL for the following DFA:");
    DFA_print(dfa);
    //DFA_print_withtable(dfa);

    while (1) {
        // Read string
        char s[128];
        printf("Enter string (or 'quit'): ");
        fgets(s, 128, stdin);
        s[strcspn(s, "\n")] = 0;
        if (strcmp(s, "quit") == 0) break;

        DFA_execute(dfa, s);
    }
    printf("\n");
}

/**
 * Print the given DFA to System.out along with the transition table.
 */
void DFA_print_withtable(DFA dfa)
{
    int s = 97; int v = 122;
    // int s = 32; int v = 128;

    printf("\n========== DFA PRINT ==========\n");
    printf("      purpose: %s\n", dfa->label);
    printf("     n states: %d\n", dfa->n);
    printf("initial state: %d\n", dfa->s0);
    printf("accepting states: ");
    Set_print(dfa->F);
    printf("\n");

    printf("abbreviated transition table:\n");
    // Print column headers
    printf("    ");
    for (int i = s; i <= v; i++) {
        printf("%2c ", i);
    }
    printf("\n");

    // Print rows of data
    for (int i = 0; i < dfa->n; i++) {
        printf("%2d| ", i); // Print state label
        for (int j = s; j <= v; j++) {
            printf("%2d ", *(*(dfa->d+i)+j));
        }
        printf("\n");
    }
    printf("===============================\n");
}

/*
 * Print the given DFA to System.out without the transition table
 */
void DFA_print(DFA dfa)
{
    printf("\n========== DFA PRINT ==========\n");
    printf("      purpose: %s\n", dfa->label);
    printf("     n states: %d\n", dfa->n);
    printf("initial state: %d\n", dfa->s0);
    printf("accepting states: ");
    Set_print(dfa->F);
    printf("\n");
    printf("===============================\n");
}
