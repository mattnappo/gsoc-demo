/*
 * Part 2 - hardcoded NFAs
 * Author: Matthew Nappo
 */

#include "nfas.h"
#include "nfa.h"
#include <stdlib.h>
#include <stdio.h>

// Part (a)
NFA NFA_for_ending_head()
{
    NFA nfa = new_NFA(5);
    NFA_set_label(nfa, "Accepts strings ending in 'head'");
    NFA_set_initial(nfa, 0);
    NFA_set_accepting(nfa, 4);
    NFA_add_transition_all(nfa, 0, 0);
    NFA_add_transition(nfa, 0, 'h', 1);
    NFA_add_transition(nfa, 1, 'e', 2);
    NFA_add_transition(nfa, 2, 'a', 3);
    NFA_add_transition(nfa, 3, 'd', 4);
    return nfa;
}

// Part (b)
NFA NFA_for_contains_zz()
{
    NFA nfa = new_NFA(3);
    NFA_set_label(nfa, "Accepts strings containing 'zz'");
    NFA_set_initial(nfa, 0);
    NFA_set_accepting(nfa, 2);
    NFA_add_transition_all(nfa, 0, 0);
    NFA_add_transition(nfa, 0, 'z', 1);
    NFA_add_transition(nfa, 1, 'z', 2);
    NFA_add_transition_all(nfa, 2, 2);
    return nfa;
}

// Part (c)
NFA NFA_for_not_anagram_woodpecker()
{
    NFA nfa = new_NFA(13);
    NFA_set_label(nfa, "Accepts strings that cannot be an anagram of 'woodpecker'");
    NFA_set_initial(nfa, 0);

    // First 11 all point to itself
    for (int i = 0; i <= 12; i++) {
        NFA_add_transition_all(nfa, i, i);
    }

    char doubles[] = { 'c', 'd', 'k', 'p', 'r', 'w' };
    for (int i = 0; i < 6; i++) {
        NFA_add_transition(nfa, 0,   doubles[i], i+1);
        NFA_add_transition(nfa, i+1, doubles[i], 12);
    }

    NFA_add_transition(nfa, 0, 'e', 8);
    NFA_add_transition(nfa, 8, 'e', 9);
    NFA_add_transition(nfa, 9, 'e', 12);

    NFA_add_transition(nfa, 0, 'o', 10);
    NFA_add_transition(nfa, 10, 'o', 11);
    NFA_add_transition(nfa, 11, 'o', 12);

    NFA_set_accepting(nfa, 12);
    return nfa;
}

// Part (d)
NFA NFA_for_phone_number()
{
    NFA nfa = new_NFA(13);
    NFA_set_label(nfa, "Accepts strings that are a phone number in the form 'XXX-XXX-XXXX' (first digit is not a 0)");
    NFA_set_initial(nfa, 0);
    NFA_set_accepting(nfa,  12);
    char *digits = "0123456789";

    NFA_add_transition_str(nfa, 0, "123456789", 1);
    NFA_add_transition_str(nfa, 1, digits, 2);
    NFA_add_transition_str(nfa, 2, digits, 3);
    NFA_add_transition(nfa, 3, '-', 4);

    NFA_add_transition_str(nfa, 4, digits, 5);
    NFA_add_transition_str(nfa, 5, digits, 6);
    NFA_add_transition_str(nfa, 6, digits, 7);
    NFA_add_transition(nfa, 7, '-', 8);

    NFA_add_transition_str(nfa, 8, digits, 9);
    NFA_add_transition_str(nfa, 9, digits, 10);
    NFA_add_transition_str(nfa, 10, digits, 11);
    NFA_add_transition_str(nfa, 11, digits, 12);

    return nfa;
}

