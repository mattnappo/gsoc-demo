/* File: translator.c
 * Author: Matthew Nappo
 * Convert a well-defined NFA to a DFA using the subset construction
 * algorithm.
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "translator.h"
#include "Set.h"
#include "LinkedList.h"
#include "bitvec.h"
#include "nfa.h"
#include "dfa.h"

bool set_equality(void *a, void *b)
{
    Set s1 = (Set)a;
    Set s2 = (Set)b;
    return Set_equals(s1, s2);
}

/* Map from the powerset of the DFA to the indicies of the powerset.
 * Since DFA state labels are subsets of NFA's states, such a
 * mapping was necessary to go from subset-labels to number-labels.
 * A subset is really just a bit vector, since at each element
 * you can either choose it (1) or not choose it (0). This method
 * computes this bitvector, which is the index in the powerset.
 * This value is the state of the NFA.
 */
int get_index(NFA nfa, Set subset)
{
    BitVec v = BitVec_new(nfa->n);
    IntHashSetIterator it = IntHashSet_iterator(subset);
    while (IntHashSetIterator_hasNext(it)) {
        int s = IntHashSetIterator_next(it);
        BitVec_set(v, s);
    } free(it);
    int index = BitVec_decimal(v);

    /*
    printf("d subset: "); Set_print(subset);
    printf(" = %d \n", index);
    BitVec_print(v);
    printf("\n");
    */

    BitVec_free(v);
    return index; // The -1 doesn't really matter
}

/* Convert a well-defined NFA to a DFA using the subset construction
 * algorithm.
 * This function makes part 3 of the project.
 */
DFA NFA_to_DFA(NFA nfa)
{
    // Initialize the DFA
    DFA dfa = new_DFA((int)pow(2, nfa->n));
    DFA_set_label(dfa, nfa->label);

    // Keep track of visited nodes and nodes left to visit & set initial state
    LinkedList successors = new_LinkedList();
    LinkedList visited = new_LinkedList();
    Set S0 = new_Set(nfa->n);
    Set_insert(S0, nfa->s0);
    DFA_set_initial(dfa, get_index(nfa, S0));
    LinkedList_add_at_front(successors, S0);

    // While there are still nodes to expand
    while (!LinkedList_isEmpty(successors)) {
        // Get the S set
        Set S = LinkedList_elementAt(successors, 0);

        // For each char in [0, 127]
        for (int c = 0; c < 128; c++) {
            // T is all of the states the NFA could be in when 'c' is read in S
            Set T = new_Set(nfa->n);

            // For each state s in S
            IntHashSetIterator it = IntHashSet_iterator(S);
            while (IntHashSetIterator_hasNext(it)) {
                int s = IntHashSetIterator_next(it);
                // Get the successor states on c
                Set t = NFA_get_transitions(nfa, s, (char)c);
                Set_union(T, t);
            } free(it);

            // If the set is not empty, add a transition from state
            // S to state T in the DFA
            if (!Set_isEmpty(T)) {
                int s_index = get_index(nfa, S);
                int t_index = get_index(nfa, T);
                DFA_set_transition(dfa, s_index, c, t_index);
                // Add this state to the list of states that need to be
                // treated (each of these T will eventually become an S)
                if (
                    !LinkedList_contains(successors, T, set_equality) &&
                    !LinkedList_contains(visited, T, set_equality)
                ) {
                    LinkedList_add_at_front(successors, T);
                }
            }
        }

        // For each member of S, check if it is accepting in NFA.
        // If any are accepting, then set S as accepting in DFA.
        IntHashSetIterator it = IntHashSet_iterator(S);
        while (IntHashSetIterator_hasNext(it)) {
            int s = IntHashSetIterator_next(it);
            if (Set_contains(nfa->F, s)) {
                DFA_set_accepting(dfa, get_index(nfa, S));
                break;
            }
        } free(it);

        LinkedList_add_at_front(visited, S);
        LinkedList_remove(successors, S);
    }
    Set_free(S0);
    LinkedList_free(successors, false);
    LinkedList_free(visited, false);
    return dfa;
}

