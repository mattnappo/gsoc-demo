#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "dbll.h"
#include "test_helper.h"

int test_dbll_insert_before() {
  struct dbll *ll;

  int N = 5;
  struct llnode *n[N];

  int ret = 0;
  int test_data[] = {0, 1, 2, 3, 4};

  ll = dbll_create();

  if(!th_check(ll != NULL, "insert_before: dbll_create return value (%p) must be non-NULL", ll))
	return 0;

  n[0] = dbll_insert_before(ll, NULL, &test_data[0]);

  if((ret = th_check(n[0] != NULL, "insert_before: dbll_insert_before return value (%p) must be non-NULL", n[0]))) {
	ret = th_check(n[0]->user_data == &test_data[0],
				   "insert_before: node user_data (%p) is same user_data as argument (%p)", n[0]->user_data, &test_data[0]) && ret;

	ret = th_check(n[0]->prev == NULL,
				   "insert_before: only node prev (%p) must be NULL ", n[0]->prev) && ret;

	ret = th_check(n[0]->next == NULL,
				   "insert_before: only node next (%p) is NULL", n[0]->next) && ret;

	ret = th_check(ll->first == n[0],
				   "insert_before: ll->first (%p) is n[0] (%p)", ll->first, n[0]) && ret;

	ret = th_check(ll->last == n[0],
				   "insert_before: ll->last (%p) is n[0] (%p)", ll->last, n[0]) && ret;
  }

  int i = 0;
  struct llnode *pprev;

  for(i = 1; ret && i < N; i++) {
	pprev = ll->last->prev;

	n[i] = dbll_insert_before(ll, ll->last, &test_data[i]);

	ret = th_check(n[i] != NULL, "insert_before: dbll_append return value (n[%d] == %p) must be non-NULL",
				   i, n[i]) && ret;

	if(ret) {
	  ret = th_check(n[i]->user_data == &test_data[i],
					 "insert_before:n[%d]: user_data (%p) is same user_data as argument (%p)",
					 i,
					 n[i]->user_data,
					 &test_data[i]) && ret;


	  ret = th_check(n[i]->next == ll->last,
					 "insert_before:n[%d]: node next (%p) must be ll->last (%p)",
					 i,
					 n[i]->next,
					 ll->last) && ret;

	  ret = th_check(ll->last->prev == n[i],
					 "insert_before:n[%d]: ll->last's prev (%p) must be current node (%p)",
					 i,
					 ll->last->prev,
					 n[i]) && ret;

	  ret = th_check(n[i]->prev == pprev,
					 "insert_before:n[%d]: new node's prev (%p) must be old ll->last->prev (%p)", i, n[i]->prev, pprev) && ret;

	  if(pprev != NULL) {
		ret = th_check(pprev->next == n[i],
					   "insert_before:n[%d]: old ll->last->next->next (%p) must be newly inserted node (%p)", i, pprev->next, n[i]) && ret;
	  }

	  ret = th_check(ll->last == n[0],
					 "insert_before:n[%d]: ll->last (%p) remains n[0] (%p)", i, ll->last, n[0]) && ret;

	}
  }

  dbll_free(ll);
  fprintf(stderr, "=== DONE\n\n");
  return ret;
}

int test_dbll_insert_after() {
  struct dbll *ll;

  int N = 5;
  struct llnode *n[N];

  int ret = 0;
  int test_data[] = {0, 1, 2, 3, 4};

  ll = dbll_create();

  if(!th_check(ll != NULL, "insert_after: dbll_create return value (%p) must be non-NULL", ll))
	return 0;

  n[0] = dbll_insert_after(ll, NULL, &test_data[0]);

  if((ret = th_check(n[0] != NULL, "insert_after: dbll_insert_after return value (%p) must be non-NULL", n[0]))) {
	ret = th_check(n[0]->user_data == &test_data[0],
				   "insert_after: node user_data (%p) is same user_data as argument (%p)", n[0]->user_data, &test_data[0]) && ret;

	ret = th_check(n[0]->prev == NULL,
				   "insert_after: only node prev (%p) must be NULL ", n[0]->prev) && ret;

	ret = th_check(n[0]->next == NULL,
				   "insert_after: only node next (%p) is NULL", n[0]->next) && ret;

	ret = th_check(ll->first == n[0],
				   "insert_after: ll->first (%p) is n[0] (%p)", ll->first, n[0]) && ret;

	ret = th_check(ll->last == n[0],
				   "insert_after: ll->last (%p) is n[0] (%p)", ll->last, n[0]) && ret;
  }

  int i = 0;
  struct llnode *pnext;

  for(i = 1; ret && i < N; i++) {
	pnext = ll->first->next;

	n[i] = dbll_insert_after(ll, ll->first, &test_data[i]);

	ret = th_check(n[i] != NULL, "insert_after: dbll_append return value (n[%d] == %p) must be non-NULL",
				   i, n[i]) && ret;

	if(ret) {
	  ret = th_check(n[i]->user_data == &test_data[i],
					 "insert_after:n[%d]: user_data (%p) is same user_data as argument (%p)",
					 i,
					 n[i]->user_data,
					 &test_data[i]) && ret;


	  ret = th_check(n[i]->prev == ll->first,
					 "insert_after:n[%d]: node prev (%p) must be ll->first (%p)",
					 i,
					 n[i]->prev,
					 ll->first) && ret;

	  ret = th_check(ll->first->next == n[i],
					 "insert_after:n[%d]: ll->first's next (%p) must be current node (%p)",
					 i,
					 ll->first->next,
					 n[i]) && ret;

	  ret = th_check(n[i]->next == pnext,
					 "insert_after:n[%d]: new node's next (%p) must be old ll->first->next (%p)", i, n[i]->next, pnext) && ret;

	  if(pnext != NULL) {
		ret = th_check(pnext->prev == n[i],
					   "insert_after:n[%d]: old ll->first->next->prev (%p) must be newly inserted node (%p)", i, pnext->prev, n[i]) && ret;
	  }

	  ret = th_check(ll->first == n[0],
					 "insert_after:n[%d]: ll->first (%p) remains n[0] (%p)", i, ll->first, n[0]) && ret;

	}
  }

  dbll_free(ll);
  fprintf(stderr, "=== DONE\n\n");
  return ret;
}

/* examples of functions passed to the dbll_iterate functions */
int find_max(struct dbll *ll, struct llnode *n, void *ctx) {
  int *vmax = (int *) ctx;
  int *val = (int *) n->user_data;

  if(*val > *vmax)
	*vmax = *val;

  return 1;
}

int compute_sum(struct dbll *ll, struct llnode *n, void *ctx) {
  int *vsum = (int *) ctx;
  int *val = (int *) n->user_data;

  *vsum = *vsum + *val;

  return 1;
}

struct node_search_query {
  int val_greater_than;
  struct llnode *n;
};

int find_first_node(struct dbll *ll, struct llnode *n, void *ctx) {
  struct node_search_query *nsq = (struct node_search_query *) ctx;
  int *val = (int *) n->user_data;

  if(*val > nsq->val_greater_than) {
	nsq->n = n;
	return 0; // stop search, we found the node
  }

  return 1;
}


int test_dbll_fwd_iteration() {
  struct dbll *ll;

  int N = 5;
  struct llnode *n[N];

  int ret = 0;
  int test_data[] = {0, 1, 2, 3, 4};

  ll = dbll_create();

  if(!(ret = th_check(ll != NULL, "fwd_iter: dbll_create return value (%p) must be non-NULL", ll)))
	return 0;

  int i = 0;

  for(i = 0;  i < N; i++) {
	n[i] = dbll_append(ll, &test_data[i]);

	ret = th_check(n[i] != NULL, "fwd_iter: dbll_append return value (n[%d] == %p) must be non-NULL",
				   i, n[i]) && ret;
  }

  if(!ret) return ret;

  int nmax = 0, sum = 0;
  int iret;

  iret = dbll_iterate(ll, NULL, NULL, &nmax, find_max);

  ret = th_check(iret == 1, "fwd_iter: iterate should return 1 as return value") && ret;
  if(iret == 1) {
	ret = th_check(nmax == 4, "fwd_iter: iterate+find_max must find %d as max value, found %d",
				   4, nmax) && ret;
  }


  iret = dbll_iterate(ll, NULL, NULL, &sum, compute_sum);

  ret = th_check(iret == 1, "fwd_iter: iterate should return 1 as return value") && ret;
  if(iret == 1) {
	ret = th_check(sum == (4*5/2), "fwd_iter: iterate+compute_sum must compute %d as sum, computed %d", (4*5/2), sum) && ret;
  }


  struct node_search_query q = { .val_greater_than = 3, .n = NULL };

  iret = dbll_iterate(ll, NULL, NULL, &q, find_first_node);

  ret = th_check(iret == 1, "fwd_iter: iterate should return 1 as return value") && ret;
  if(iret == 1) {
	ret = th_check(q.n == n[4], "fwd_iter: iterate+find_first_node must find %p, found %p", n[4], q.n) && ret;
  }


  q.n = NULL;

  iret = dbll_iterate(ll, NULL, n[3], &q, find_first_node);

  ret = th_check(iret == 1, "fwd_iter: iterate should return 1 as return value") && ret;
  if(iret == 1) {
	ret = th_check(q.n == NULL, "fwd_iter: iterate+find_first_node stopping before n[4] must be NULL, is %p", q.n) && ret;
  }


  q.val_greater_than = 5;
  q.n = NULL;

  iret = dbll_iterate(ll, n[3], n[4], &q, find_first_node);

  ret = th_check(iret == 1, "fwd_iter: iterate should return 1 as return value") && ret;
  if(iret == 1) {
	ret = th_check(q.n == NULL, "fwd_iter: iterate+find_first_node starting from n[3] must be NULL, is %p", q.n) && ret;
  }

  fprintf(stderr, "=== DONE\n\n");
  dbll_free(ll);
  return ret;
}

int test_dbll_rev_iteration() {
  struct dbll *ll;

  int N = 5;
  struct llnode *n[N];

  int ret = 0;
  int test_data[] = {0, 1, 2, 3, 4};

  ll = dbll_create();

  if(!(ret = th_check(ll != NULL, "rev_iter: dbll_create return value (%p) must be non-NULL", ll)))
	return 0;

  int i = 0;

  for(i = 0;  i < N; i++) {
	n[i] = dbll_append(ll, &test_data[i]);

	ret = th_check(n[i] != NULL, "rev_iter: dbll_append return value (n[%d] == %p) must be non-NULL",
				   i, n[i]) && ret;
  }

  if(!ret) return ret;

  int nmax = 0, sum = 0;
  int iret;

  iret = dbll_iterate_reverse(ll, NULL, NULL, &nmax, find_max);

  ret = th_check(iret == 1, "rev_iter: iterate should return 1 as return value") && ret;
  if(iret == 1) {
	ret = th_check(nmax == 4, "rev_iter: iterate+find_max must find %d as max value, found %d",
				   4, nmax) && ret;
  }


  iret = dbll_iterate_reverse(ll, NULL, NULL, &sum, compute_sum);

  ret = th_check(iret == 1, "rev_iter: iterate should return 1 as return value") && ret;
  if(iret == 1) {
	ret = th_check(sum == (4*5/2), "rev_iter: iterate+compute_sum must compute %d as sum, computed %d", (4*5/2), sum) && ret;
  }


  struct node_search_query q = { .val_greater_than = 3, .n = NULL };

  iret = dbll_iterate_reverse(ll, NULL, NULL, &q, find_first_node);

  ret = th_check(iret == 1, "rev_iter: iterate should return 1 as return value") && ret;
  if(iret == 1) {
	ret = th_check(q.n == n[4], "rev_iter: iterate+find_first_node must find %p, found %p", n[4], q.n) && ret;
  }


  q.n = NULL;

  iret = dbll_iterate_reverse(ll, n[3], NULL, &q, find_first_node);

  ret = th_check(iret == 1, "rev_iter: iterate should return 1 as return value") && ret;
  if(iret == 1) {
	ret = th_check(q.n == NULL, "rev_iter: iterate+find_first_node not including n[4] and n[5] must be NULL, is %p", q.n) && ret;
  }

  q.val_greater_than = 5;
  q.n = NULL;

  iret = dbll_iterate_reverse(ll, n[4], n[3], &q, find_first_node);

  ret = th_check(iret == 1, "rev_iter: iterate should return 1 as return value") && ret;
  if(iret == 1) {
	ret = th_check(q.n == NULL, "rev_iter: iterate+find_first_node in n[4], n[3] must be NULL, is %p", q.n) && ret;
  }

  fprintf(stderr, "=== DONE\n\n");
  dbll_free(ll);
  return ret;
}

int test_dbll_remove() {
  struct dbll *ll;

  int N = 5;
  struct llnode *n[N];

  int ret = 0;
  int test_data[] = {0, 1, 2, 3, 4};

  ll = dbll_create();

  if(!(ret = th_check(ll != NULL, "remove: dbll_create return value (%p) must be non-NULL", ll)))
	return 0;

  int i = 0;

  for(i = 0;  i < N; i++) {
	n[i] = dbll_append(ll, &test_data[i]);

	ret = th_check(n[i] != NULL, "remove: dbll_append return value (n[%d] == %p) must be non-NULL",
				   i, n[i]) && ret;
  }

  if(!ret) return ret;

  /* remove first node */
  void *prev, *next;

  assert(ll->first == n[0]); /* this should be true of the append tests passed */

  next = n[0]->next;

  dbll_remove(ll, ll->first);

  ret = th_check(ll->first != n[0], "remove: first node (%p) is not n[0] (%p) after removal",
				 ll->first,
				 n[0]) && ret;

  ret = th_check(ll->first == next, "remove: first node (%p) is n[0]->next (%p) after removal",
				 ll->first,
				 next) && ret;

  ret = th_check(ll->last == n[N-1], "remove: last node (%p) is unchanged (%p) after removal of first node",
				 ll->last,
				 n[N-1]) && ret;


  /* remove last node */
  assert(ll->last == n[N-1]); /* this should be true of the append tests passed */

  prev = n[N-1]->prev;

  dbll_remove(ll, ll->last);

  ret = th_check(ll->last != n[N-1], "remove: last node (%p) is not n[N-1] (%p) after removal",
				 ll->first,
				 n[N-1]) && ret;

  ret = th_check(ll->last == prev, "remove: last node (%p) is n[N-1]->prev (%p) after removal",
				 ll->last,
				 prev) && ret;

  ret = th_check(ll->first == n[1], "remove: first node (%p) is unchanged (%p) after removal of last node",
				 ll->first,
				 n[1]) && ret;


  /* remove a middle node */
  struct llnode *pprev, *pnext;
  assert(N > 3);

  pprev = n[N/2]->prev;
  pnext = n[N/2]->next;

  dbll_remove(ll, n[N/2]);

  ret = th_check(pprev->next != n[N/2], "remove: next of prev node (%p) is not middle node (%p) after removal", pprev->next, n[N/2]) && ret;

  ret = th_check(pnext->prev != n[N/2], "remove: prev of next node (%p) is not middle node (%p) after removal", pnext->prev, n[N/2]) && ret;

  ret = th_check(pprev->next == pnext, "remove: link between prev and next is maintained (%p == %p)", pprev->next, pnext) && ret;

  ret = th_check(pprev == pnext->prev, "remove: link between next and prev is maintained (%p == %p)", pprev, pnext->prev) && ret;

  ret = th_check(ll->first == n[1], "remove: first node (%p) is unchanged (%p) after removal of middle node", ll->first, n[1]) && ret;

  ret = th_check(ll->last == n[N-2], "remove: last node (%p) is unchanged (%p) after removal of middle node", ll->last, n[N-2]) && ret;

  for(i = 0; i < N-3; i++) {
	if(i % 2) {
	  next = ll->first->next;
	  dbll_remove(ll, ll->first);
	  ret = th_check(ll->first == next, "remove: first node is next node", ll->first, next) && ret;
	} else {
	  prev = ll->last->prev;
	  dbll_remove(ll, ll->last);
	  ret = th_check(ll->last == prev, "remove: last node is prev node", ll->last, prev) && ret;
	}
  }

  /* all nodes must be deleted by now */
  ret = th_check(ll->first == NULL, "remove: first must be null in empty list", ll->first) && ret;
  ret = th_check(ll->last == NULL, "remove: last must be null in empty list", ll->last) && ret;

  dbll_free(ll);

  fprintf(stderr, "=== DONE\n\n");
  return ret;
}

int test_dbll_append() {
  struct dbll *ll;

  int N = 5;
  struct llnode *n[N];

  int ret = 0;
  int test_data[] = {0, 1, 2, 3, 4};

  ll = dbll_create();

  if(!th_check(ll != NULL, "append: dbll_create return value (%p) must be non-NULL", ll))
	return 0;

  n[0] = dbll_append(ll, &test_data[0]);
  if((ret = th_check(n[0] != NULL, "append: dbll_append return value (%p) must be non-NULL", n[0]))) {
	ret = th_check(n[0]->user_data == &test_data[0],
				   "append: node user_data (%p) is same user_data as argument (%p)", n[0]->user_data, &test_data[0]) && ret;

	ret = th_check(n[0]->prev == NULL,
				   "append: only node prev (%p) must be NULL ", n[0]->prev) && ret;

	ret = th_check(n[0]->next == NULL,
				   "append: only node next (%p) is NULL", n[0]->next) && ret;

	ret = th_check(ll->first == n[0],
				   "append: ll->first (%p) is n[0] (%p)", ll->first, n[0]) && ret;

	ret = th_check(ll->last == n[0],
				   "append: ll->last (%p) is n[0] (%p)", ll->last, n[0]) && ret;
  }

  int i = 0;

  for(i = 1; ret && i < N; i++) {
	n[i] = dbll_append(ll, &test_data[i]);

	ret = th_check(n[i] != NULL, "append: dbll_append return value (n[%d] == %p) must be non-NULL",
				   i, n[i]) && ret;

	if(ret) {
	  ret = th_check(n[i]->user_data == &test_data[i],
					 "append:n[%d]: user_data (%p) is same user_data as argument (%p)",
					 i,
					 n[i]->user_data,
					 &test_data[i]) && ret;


	  ret = th_check(n[i]->prev == n[i-1],
					 "append:n[%d]: node prev (%p) must be previously added node (%p)",
					 i,
					 n[i]->prev,
					 n[i-1]) && ret;

	  ret = th_check(n[i-1]->next == n[i],
					 "append:n[%d]: prev node's next (%p) must be current node (%p)",
					 i,
					 n[i-1]->next,
					 n[i]) && ret;

	  ret = th_check(n[i]->next == NULL,
					 "append:n[%d]: last node next (%p) is NULL", i, n[i]->next) && ret;

	  ret = th_check(ll->first == n[0],
					 "append:n[%d]: ll->first (%p) remains n[0] (%p)", i, ll->first, n[0]) && ret;

	  ret = th_check(ll->last == n[i],
					 "append:n[%d]: ll->last (%p) is n[%d] (%p)", i, ll->last, i, n[i]) && ret;
	}
  }

  dbll_free(ll);
  fprintf(stderr, "=== DONE\n\n");
  return ret;
}

int test_dbll_create_and_free() {
  struct dbll *ll;
  int ret = 0;

  ll = dbll_create();

  if(th_check(ll != NULL, "dbll_create return value (%p) must be non-NULL", ll))
  {
	ret = th_check(ll->first == NULL, "ll->first (%p) in empty list must be NULL", ll->first);
	ret = th_check(ll->last == NULL, "ll->last (%p) in empty list must be NULL", ll->last) && ret;

	if(ret) {
	  dbll_free(ll);
	}
  }

  fprintf(stderr, "=== DONE\n\n");
  return ret;
}

int main(void) {
  if(!test_dbll_create_and_free())
	exit(1);

  if(!test_dbll_append())
	exit(1);

  if(!test_dbll_remove())
	exit(1);

  if(!test_dbll_fwd_iteration())
	exit(1);

  if(!test_dbll_rev_iteration())
	exit(1);

  if(!test_dbll_insert_after())
	exit(1);

  if(!test_dbll_insert_before())
	exit(1);

  printf("ALL DONE\n");
  return 0;
}


