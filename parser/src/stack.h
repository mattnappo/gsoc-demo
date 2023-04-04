/*
 * File: stack.h
 * Author: Matthew Nappo
 * A generic stack implementation for "void*" data. Constant time push and pop.
 */

#ifndef STACK_H
#define STACK_H

#include <stdbool.h>

// A stack node. Not named "Node" for namespace pollution purposes
struct ELEMENT_T {
    struct ELEMENT_T *next;
    void *data;
};

struct STACK_T {
    struct ELEMENT_T *head;
    unsigned int count;
};

typedef struct ELEMENT_T *Element;
typedef struct STACK_T   *Stack;

extern Stack  init_stack();
extern void   push(Stack stack, void *data);
extern void  *pop(Stack stack);
extern bool   is_empty(Stack stack);
extern void   print_stack(Stack stack, void (*printer)(void *));
extern void   free_stack (Stack stack, void (*destructor)(void *));

extern Element init_element(Element next, void *data);

#endif
