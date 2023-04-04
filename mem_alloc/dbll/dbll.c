#include <stdlib.h>
#include <stdio.h>
#include "dbll.h"

/* Routines to create and manipulate a doubly-linked list */

/* create a doubly-linked list */
/* returns an empty list or NULL if memory allocation failed */
struct dbll *dbll_create()
{
    struct dbll *dbll = calloc(sizeof(struct dbll), 1);
    if (!dbll) return NULL;
    dbll->first = NULL;
    dbll->last = NULL;
    return dbll;
}

// Recursive function for `dbll_free`
void dbll_free_node(struct llnode *node)
{
    // Base case
    if (!node) return;

    // Recursive step
    dbll_free_node(node->next);
    free(node);

}

/* frees all memory associated with a doubly-linked list */
/* this must also free all memory associated with the linked list nodes */
/* assumes user data has already been freed */
void dbll_free(struct dbll *list)
{
    if (list->first)
        dbll_free_node(list->first);
    free(list);
}

/* iterate over items stored in a doubly-linked list */

/* begin iteration from node start, end iteration at node stop (include stop in iteration) */
/* the list is traversed using the next pointer of each node */

/* if start is NULL, then start is the first node */
/* if end is NULL, then end is the last node */

/* at each node, call the user function f with a pointer to the list,
   a pointer to the current node, and the value of ctx */

/* if f returns 0, stop iteration and return 1 */

/* return 0 if you reached the end of the list without encountering end */
/* return 1 on successful iteration */

int dbll_iterate(struct dbll *list,
				 struct llnode *start,
				 struct llnode *end,
				 void *ctx,
				 int (*f)(struct dbll *, struct llnode *, void *))
{
    // Set starting and ending points
    struct llnode *curr = start;
    if (!start) curr = list->first;
    if (!end)   end  = list->last;

    // begin iteration
    while (curr != end) {
        if (!(*f)(list, curr, ctx)) return 1;
        curr = curr->next;
    }
    (*f)(list, curr, ctx);
    return 1;
}

/* similar to dbll_iterate, except that the list is traversed using
   the prev pointer of each node (i.e. in the reverse direction).

   Adjust accordingly. For example if start is NULL, then start is the
   last node in the list.  If end is NULL, then end is the first node
   in the list.

*/

int dbll_iterate_reverse(struct dbll *list,
						 struct llnode *start,
						 struct llnode *end,
						 void *ctx,
						 int (*f)(struct dbll *, struct llnode *, void *)
						 )
{
    // Set starting and ending points
    struct llnode *curr = start;
    if (!start) curr = list->last;
    if (!end)   end  = list->first;

    // begin iteration
    while (curr != end) {
        if (!(*f)(list, curr, ctx)) return 1;
        curr = curr->prev;
    }
    (*f)(list, curr, ctx);
    return 1;
}


/* Remove `llnode` from `list` */
/* Memory associated with `node` must be freed */
/* You can assume user_data will be freed by somebody else (or has already been freed) */
void dbll_remove(struct dbll *list, struct llnode *node)
{
    // Case 1: only one node in the list
    if (list->first == list->last && list->last == node) {
        list->first = NULL;
        list->last = NULL;
        free(node);
        return;
    }

    // Case 2: remove the head
    if (list->first == node) {
        node->next->prev = NULL;
        list->first = node->next;
        free(node);
        return;
    }

    // Case 3: Remove the tail
    if (list->last == node) {
        list->last->prev->next = NULL;
        list->last = node->prev;
        free(node);
        return;
    }

    // Case 4: Default case (remove in the middle)
    node->prev->next = node->next;
    node->next->prev = node->prev;
    free(node);
}

/* Create and return a new node containing `user_data` */
/* The new node must be inserted after `node` */
/* if node is NULL, then insert the node at the end of the list */
/* return NULL if memory could not be allocated */
struct llnode *dbll_insert_after(struct dbll *list, struct llnode *node, void *user_data)
{
    // Make the new node
    struct llnode *new_node = calloc(sizeof(struct llnode), 1);
    if (!new_node) return NULL;
    new_node->user_data = user_data;

    // Insert as first element
    if (!list->last && !list->first) {
        list->first = new_node;
        list->last = new_node;
        return new_node;
    }

    // Add to end of list
    if (!node || node == list->last) {
        new_node->prev = list->last;
        new_node->next = NULL;
        list->last->next = new_node;
        list->last = new_node;
        return new_node;
    }

    // Add to middle of list
    new_node->next = node->next;
    new_node->prev = node;
    node->next->prev = new_node; // problem line
    node->next = new_node;
    return new_node;
}

/* Create and return a new node containing `user_data` */
/* The new node must be inserted before `node` */
/* if node is NULL, then insert the new node at the beginning of the list */
/* return NULL if memory could not be allocated */
struct llnode *dbll_insert_before(struct dbll *list, struct llnode *node, void *user_data)
{
    // Make the new node
    struct llnode *new_node = calloc(sizeof(struct llnode), 1);
    if (!new_node) return NULL;
    new_node->user_data = user_data;

    // Insert as first element
    if (!list->last && !list->first) {
        list->first = new_node;
        list->last = new_node;
        return new_node;
    }

    // Insert at head
    if (!node || node == list->first) {
        new_node->prev = NULL;
        new_node->next = list->first;
        list->first->prev = new_node;
        list->first = new_node;
        return new_node;
    }

    // Insert in middle
    new_node->next = node;
    new_node->prev = node->prev;
    node->prev->next = new_node;
    node->prev = new_node;
    return new_node;
}

/* create and return an `llnode` that stores `user_data` */
/* the `llnode` is added to the end of list */
/* return NULL if memory could not be allocated */
/* this function is a convenience function and can use the dbll_insert_after function */
struct llnode *dbll_append(struct dbll *list, void *user_data)
{
    return dbll_insert_after(list, NULL, user_data);
}
