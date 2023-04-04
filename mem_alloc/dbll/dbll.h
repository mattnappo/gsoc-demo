#pragma once

/* structure that holds each node of a doubly-linked list */
/* Must satisfy the following invariants at all times */

/* Invariant: The first node in the linked list will have prev = NULL */
/* Invariant: The last node in the linked list will have next = NULL */

struct llnode {
  void *user_data;      /* pointer to user data */
  struct llnode *next;  /* next node in linked list, NULL if this is the last node */
  struct llnode *prev;  /* prev node in linked list, NULL if this is the first node */
};

/* structure for the doubly-linked list */
/* Invariant: first and last are both NULL in an empty list */
struct dbll {
  struct llnode *first;
  struct llnode *last;
};

struct dbll *dbll_create();

struct llnode *dbll_append(struct dbll *list, void *user_data);

void dbll_remove(struct dbll *list, struct llnode *node);

struct llnode *dbll_insert_after(struct dbll *list, struct llnode *node, void *user_data);
struct llnode *dbll_insert_before(struct dbll *list, struct llnode *node, void *user_data);

void dbll_free(struct dbll *list);

int dbll_iterate(struct dbll *list,
				 struct llnode *start,
				 struct llnode *end,
				 void *ctx,
				 int (*f)(struct dbll *, struct llnode *, void *));

int dbll_iterate_reverse(struct dbll *list,
						 struct llnode *start,
						 struct llnode *end,
						 void *ctx,
						 int (*f)(struct dbll *, struct llnode *, void *));
