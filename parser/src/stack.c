/*
 * File: stack.c
 * Author: Matthew Nappo
 * A generic stack implementation for "void*" data. Constant time push and pop
 * guaranteed.
 */

#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

Element init_element(Element next, void *data)
{
    Element e = malloc(sizeof(struct ELEMENT_T));
    e->data = data;
    e->next = next;
    return e;
}

Stack init_stack()
{
    Stack stack = malloc(sizeof(struct STACK_T));
    stack->head = NULL;
    stack->count = 0;
    return stack;
}

void push(Stack stack, void *data)
{
    Element e = init_element(stack->head, data);
    stack->head = e;
    stack->count++;
}

void *pop(Stack stack)
{
    if (stack->head == NULL) {
        printf("stack underflow\n");
        return NULL;
    }

    void *tmp = stack->head->data;
    if (stack->head->next == NULL)
        stack->head = NULL;
    else
        stack->head = stack->head->next;
    stack->count--;
    return tmp;
}

bool is_empty(Stack stack) { return stack->count == 0; }

void print_stack(Stack stack, void (*printer)(void *))
{
    Element e = stack->head;
    while (e) {
        printer(e->data);
        e = e->next;
    }
}

void free_stack(Stack stack, void (*destructor)(void *))
{
    Element old_head;

    // Shift over by 1 each time
    while (stack->head) {
        old_head = stack->head;
        stack->head = stack->head->next;
        destructor(old_head->data);
        free(old_head);
    }
    free(stack);
}

