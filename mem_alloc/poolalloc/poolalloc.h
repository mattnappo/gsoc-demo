#pragma once
#include "dbll.h"

struct alloc_info {
  size_t offset;     /* offset from beginning of pool */
  size_t size;       /* size of allocation */
  size_t request_size; /* size actually requested */
};

struct memory_pool {
  char *start;                /* start of pool */
  size_t size;                /* size of pool */
  struct dbll *alloc_list;    /* track allocations */
  struct dbll *free_list;     /* list of freed regions */
};

struct memory_pool *mpool_create(size_t size);
void mpool_destroy(struct memory_pool *p);
void *mpool_alloc(struct memory_pool *p, size_t size);
void mpool_free(struct memory_pool *p, void *addr);
