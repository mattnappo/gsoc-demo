#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include "dbll.h"
#include "poolalloc.h"
#include "test_helper.h"

int mpool_test_is_alloc(struct memory_pool *p, void *alloc) {
  struct alloc_info *ai = NULL;
  struct llnode *n;
  ptrdiff_t x = (char *) alloc - p->start;

  for(n = p->alloc_list->first; n != NULL; n = n->next) {
    ai = n->user_data;
    if(x >= ai->offset && x < (ai->offset + ai->size)) return 1;
  }

  return 0;
}

int mpool_test_is_free(struct memory_pool *p, void *ptr) {
  struct alloc_info *ai = NULL;
  struct llnode *n;
  ptrdiff_t x = (char *) ptr - p->start;

  for(n = p->free_list->first; n != NULL; n = n->next) {
    ai = n->user_data;
    if(x >= ai->offset && x < (ai->offset + ai->size)) return 1;
  }

  return 0;
}

int test_alloc_free() {
  struct memory_pool *p;
  int i, j;
  int ret = 0;
  size_t sz[] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024};
  char *alloc[11];

  p = mpool_create(1024);

  if(!(ret = th_check(p != NULL, "mpool_create returned non-null (%p)", p)))
	return 0;

  for(i = 0; ret && i < 10; i++) {
	alloc[i] = mpool_alloc(p, sz[i]);

	ret = th_check(alloc[i] != NULL, "mpool_alloc (%p) for sz %lu is non-null", alloc[i], sz[i]) && ret;

	ret = ret && th_check((alloc[i] + sz[i]) <= (p->start + p->size), "mpool_alloc (%p) for sz %lu is inside pool", alloc[i], sz[i]);

	ret = ret && th_check(mpool_test_is_alloc(p, alloc[i]), "mpool_alloc (%p) for sz %lu is on alloc_list\n", alloc[i], sz[i]);

	if(ret) {
	  ptrdiff_t x = alloc[i] - p->start;
	  size_t align;

	  align = sz[i];
	  if(align > 16) align = 16;

	  ret = th_check(x % align == 0, "mpool_alloc return value (%p) is aligned to %lu for sz %lu", alloc[i], align, sz[i]) && ret;

	  for(j = 0; ret && j < i; j++) {
		/* we can do this comparison because alloc[j] + sz[j]
		   should be in the same pool/array */

		/* note: this only tests overlaps at allocation size
		   granularity, but the allocator could be using allocation
		   sizes of larger granularities.
		*/

		ret = th_check(alloc[j] + sz[j] <= alloc[i] || alloc[i] + sz[i] <= alloc[j] , "mpool_alloc return value (%p) must not overlap", alloc[i]) && ret;

	  }
	}
  }

  for(i = 0; ret && i < 10; i+=2) {
    mpool_free(p, alloc[i]);
    ret = ret && th_check(mpool_test_is_free(p, alloc[i]), "mpool_free placed %p on free list", alloc[i]);
  }

  for(i = 1; ret && i < 10; i+=2) {
    ret = ret && th_check(mpool_test_is_alloc(p, alloc[i]), "mpool_free left %p on alloc list", alloc[i]);
  }

  for(i = 1; ret && i < 10; i+=2) {
    mpool_free(p, alloc[i]);
    ret = ret && th_check(mpool_test_is_free(p, alloc[i]), "mpool_free placed %p on free list", alloc[i]);
  }

  if(ret) {
    alloc[0] = mpool_alloc(p, 1024);
    ret = th_check(alloc[0] != NULL, "mpool_alloc (%p) for sz %lu after freeing everything is non-null", alloc[0], 1024) && ret;

    ret = ret && th_check((alloc[0] + 1024) <= (p->start + p->size), "mpool_alloc (%p) for sz %lu is inside pool", alloc[0], 1024);
  }

  if(ret) mpool_destroy(p);

  return ret;
}

int test_create_destroy(size_t poolsize) {
  struct memory_pool *p;
  struct alloc_info *ai;
  int ret = 0;

  p = mpool_create(poolsize);

  if(!(ret = th_check(p != NULL, "mpool_create returned non-null (%p)", p)))
	return 0;

  ret = th_check(p->start != NULL, "pool->start is not null (%p)", p->start) && ret;
  ret = th_check(p->size >= poolsize, "pool->size (%lu) is >= size (%lu)", p->size, poolsize) && ret;
  ret = th_check(p->alloc_list != NULL, "pool->alloc_list  (%p) is not null", p->alloc_list) && ret;
  ret = th_check(p->free_list != NULL, "pool->free_list  (%p) is not null", p->free_list) && ret;

  ret = th_check(p->free_list->first != NULL, "pool->free_list is not empty") && ret;

  if(p->free_list->first) {
	ai = (struct alloc_info *) p->free_list->first->user_data;
	ret = th_check(ai->offset == 0, "free_list first entry offset is zero (%lu)", ai->offset) && ret;
	ret = th_check(ai->size == p->size, "free_list first entry size is same as pool size (%lu)", ai->size) && ret;
  }

  mpool_destroy(p);

  return ret;
}

int main(int argc, char *argv[]) {
  int poolsize = 1024;

  if(argc == 2) {
    poolsize = atoi(argv[1]);
    if(poolsize == 0) poolsize = 1024;
  }

  if(!test_create_destroy(poolsize))
    exit(1);

  if(!test_alloc_free(poolsize))
    exit(1);

  printf("ALL DONE\n");
  return 0;
}
