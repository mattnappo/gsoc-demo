/**
 * File: main.c
 * Author: Matthew Nappo
 * The main method for running all three parts of project 1
 */

#include <stdio.h>
#include <stdlib.h>
#include "dfa.h"
#include "dfas.h"

#include "nfa.h"
#include "nfas.h"

#include "translator.h"
#include "Set.h"

void launch_dfa(DFA (*hardcoded_dfa)(void))
{
    DFA dfa = hardcoded_dfa();
    DFA_repl(dfa);
    DFA_free(dfa);
    printf("------------------------------------------------\n");
}

void launch_nfa(NFA (*hardcoded_nfa)(void))
{
    NFA nfa = hardcoded_nfa();
    NFA_repl(nfa);
    NFA_free(nfa);
    printf("------------------------------------------------\n");
}

void launch_converted_dfa(NFA (*hardcoded_nfa)(void))
{
    printf("NFA before subset construction:");
    NFA nfa = hardcoded_nfa();
    NFA_print(nfa);

    DFA dfa = NFA_to_DFA(nfa);

    printf("Running REPL on the converted DFA");
    DFA_repl(dfa);

    DFA_free(dfa);
    NFA_free(nfa);
    printf("------------------------------------------------\n");
}

// Run the hard-coded DFA REPLs
void part1()
{
    launch_dfa(DFA_for_automaton);
    launch_dfa(DFA_for_ending_ed);
    launch_dfa(DFA_for_two_2s);
    launch_dfa(DFA_for_odd_1s_0s);
    launch_dfa(DFA_for_strictly_increasing_digits);
}

// Run the hard-coded NFA REPLs
void part2()
{
    launch_nfa(NFA_for_ending_head);
    launch_nfa(NFA_for_contains_zz);
    launch_nfa(NFA_for_not_anagram_woodpecker);
    launch_nfa(NFA_for_phone_number);
}

void part3()
{
    launch_converted_dfa(NFA_for_ending_head);
    launch_converted_dfa(NFA_for_contains_zz);
    launch_converted_dfa(NFA_for_not_anagram_woodpecker);
    launch_converted_dfa(NFA_for_phone_number);
}

/* 
 * This is the coolest program I've ever written!!!
 */
int main(int argc, char *args[])
{
    printf("\n==========================\n");
    printf("CSC 173 PROJECT 1\n");
    printf("By Matthew Nappo\n");
    printf("Winter 2022\n");
    printf("==========================\n\n");

    printf("Entering part 1\n===============\n"); part1();
    printf("Entering part 2\n===============\n"); part2();
    printf("Entering part 3\n===============\n"); part3();

    return 0;
}

