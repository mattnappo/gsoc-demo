/*
 * File: LinkedList.h
 * Singly-linked list
 */

#ifndef _LinkedList_h_gf
#define _LinkedList_h_gf

#include <stdbool.h>

typedef struct LinkedList* LinkedList;

extern LinkedList new_LinkedList();
extern void LinkedList_free(LinkedList list, bool free_data_also);

extern bool LinkedList_isEmpty(const LinkedList this);
extern void LinkedList_add_at_front(LinkedList this, void *value);
extern void LinkedList_add_at_end(LinkedList this, void *value);
extern bool LinkedList_contains(const LinkedList this, void *value, bool (*equality)(void*, void*));
extern void LinkedList_remove(LinkedList this, void *data);
extern void *LinkedList_elementAt(LinkedList this, int index);
extern void *LinkedList_pop(LinkedList this);
extern void LinkedList_iterate(const LinkedList this, void (*func)(void*));
typedef struct LinkedListIterator *LinkedListIterator;
extern LinkedListIterator LinkedList_iterator(const LinkedList this);
extern bool LinkedListIterator_hasNext(const LinkedListIterator iterator);
extern void *LinkedListIterator_next(LinkedListIterator iterator);
extern void LinkedList_print_set(LinkedList this);

#endif
