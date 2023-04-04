/*
 * Part 1 of Project 1.
 * A series of hardcoded DFAs for various languages
 * Author: Matthew Nappo
 */

#include "dfas.h"
#include <stdlib.h>
#include <stdio.h>

// Part (a)
DFA DFA_for_automaton()
{
    DFA dfa = new_DFA(10);
    DFA_set_label(dfa, "Accepts the string 'automaton'");
    DFA_set_initial(dfa, 0);
    DFA_set_accepting(dfa, 9);
    DFA_set_transition(dfa, 0, 'a', 1);
    DFA_set_transition(dfa, 1, 'u', 2);
    DFA_set_transition(dfa, 2, 't', 3);
    DFA_set_transition(dfa, 3, 'o', 4);
    DFA_set_transition(dfa, 4, 'm', 5);
    DFA_set_transition(dfa, 5, 'a', 6);
    DFA_set_transition(dfa, 6, 't', 7);
    DFA_set_transition(dfa, 7, 'o', 8);
    DFA_set_transition(dfa, 8, 'n', 9);
    return dfa;
}

// Part (b)
DFA DFA_for_ending_ed()
{
    DFA dfa = new_DFA(3);
    DFA_set_label(dfa, "Accepts strings ending in 'ed'");
    DFA_set_initial(dfa, 0);
    DFA_set_accepting(dfa, 2);

    // From 0
    DFA_set_transition_all(dfa, 0, 0);
    DFA_set_transition(dfa, 0, 'e', 1);

    // From 1
    DFA_set_transition_all(dfa, 1, 0);
    DFA_set_transition(dfa, 1, 'd', 2);
    DFA_set_transition(dfa, 1, 'e', 1); // Self e
    DFA_set_transition(dfa, 2, 'd', 1); // Back d

    // From 2
    DFA_set_transition_all(dfa, 2, 0);
    DFA_set_transition(dfa, 2, 'e', 1);

    return dfa;
}

// Part (c)
DFA DFA_for_two_2s()
{
    DFA dfa = new_DFA(3);
    DFA_set_label(dfa, "Accepts strings with exactly two '2's");
    DFA_set_initial(dfa, 0);
    DFA_set_accepting(dfa, 2);

    // From 0
    DFA_set_transition_all(dfa, 0, 0);
    DFA_set_transition(dfa, 0, '2', 1);

    // From 1
    DFA_set_transition_all(dfa, 1, 1);
    DFA_set_transition(dfa, 1, '2', 2);

    // From 2
    DFA_set_transition_all(dfa, 2, 2);
    DFA_set_transition(dfa, 2, '2', -1);

    return dfa;
}

// Part (d)
DFA DFA_for_odd_1s_0s()
{
    DFA dfa = new_DFA(4);
    DFA_set_label(dfa, "Accepts bitstrings with an odd number of 0s and 1s");
    DFA_set_initial(dfa, 0);
    DFA_set_accepting(dfa, 3);

    DFA_set_transition(dfa, 0, '0', 1);
    DFA_set_transition(dfa, 1, '0', 0);
    DFA_set_transition(dfa, 0, '1', 2);
    DFA_set_transition(dfa, 2, '1', 0);
    DFA_set_transition(dfa, 2, '0', 3);
    DFA_set_transition(dfa, 3, '0', 2);
    DFA_set_transition(dfa, 1, '1', 3);
    DFA_set_transition(dfa, 3, '1', 1);

    return dfa;
}

// Part (e)
// This DFA I devised will accept strings of strictly increasing digits.
// That is, each digit is larger than the previous digit.
DFA DFA_for_strictly_increasing_digits()
{
    DFA dfa = new_DFA(11);
    DFA_set_label(dfa, "Accepts strings of strictly increasing digits");
    DFA_set_initial(dfa, 0);
    for (int i = 1; i <= 10; i++) DFA_set_accepting(dfa, i);

    // Cancel out decreasing digits
    DFA_set_transition_str(dfa, 1, "0", -1);
    DFA_set_transition_str(dfa, 2, "01", -1);
    DFA_set_transition_str(dfa, 3, "012", -1);
    DFA_set_transition_str(dfa, 4, "0123", -1);
    DFA_set_transition_str(dfa, 5, "01234", -1);
    DFA_set_transition_str(dfa, 6, "012345", -1);
    DFA_set_transition_str(dfa, 7, "0123456", -1);
    DFA_set_transition_str(dfa, 8, "01234567", -1);
    DFA_set_transition_str(dfa, 9, "012345678", -1);

    // If in state i, only create transitions for numbers
    // larger than i
    for (int j = 0; j < 10; j++) {
        for (int i = j; i < 10; i++) {
            DFA_set_transition(dfa, j, 48+i, i+1);
        }
    }

    return dfa;
}

