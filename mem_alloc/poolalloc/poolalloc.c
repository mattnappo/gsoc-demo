#include <stdlib.h>
#include <stdio.h>
#include "dbll.h"
#include "poolalloc.h"

#define CHECK(item) \
    do { if (!item) return NULL; } while(0)

void dbll_destroy_node(struct llnode *node)
{
    if (!node) return;
    free(node->user_data);
    dbll_destroy_node(node->next);
    free(node);
}
void dbll_destroy(struct dbll *list)
{
    if (list->first)
        dbll_destroy_node(list->first);
    free(list);
}

struct alloc_info *block_create(size_t offset, size_t size, size_t req_size)
{
    struct alloc_info *block = calloc(sizeof(struct alloc_info), 1);
    CHECK(block);

    block->offset = offset;
    block->size = size;
    block->request_size = req_size;

    return block;
}

/*
   a pool-based allocator that uses doubly-linked lists to track
   allocated and free blocks
 */

/* create and initialize a memory pool of the required size */
/* use malloc() or calloc() to obtain this initial pool of memory from the system */
struct memory_pool *mpool_create(size_t size)
{

    /* set start to memory obtained from malloc */
    struct memory_pool *pool = calloc(sizeof(struct memory_pool), 1);
    CHECK(pool);
    pool->start = calloc(size, 1);
    CHECK(pool->start);

    pool->size = size;
    pool->alloc_list = dbll_create();
    pool->free_list = dbll_create();

    struct alloc_info *init_block = block_create(0, size, 0);
    dbll_append(pool->free_list, init_block);

    return pool;
}

/* ``destroy'' the memory pool by freeing it and all associated data structures */
/* this includes the alloc_list and the free_list as well */
void mpool_destroy(struct memory_pool *p)
{
    free(p->start);

    dbll_destroy(p->alloc_list);
    dbll_destroy(p->free_list);
    free(p);
}

size_t calc_align(size_t size)
{
    switch (size) {
        case 1:
            return 1;
        case 2:
            return 2;
        case 3:
        case 4:
            return 4;
        case 5:
        case 6:
        case 7:
        case 8:
            return 8;
        default:
            return 16;
    }
}

size_t align_address(size_t align, size_t offset)
{
    if (offset % align)
        return offset + (align - (offset % align));
    return offset;
}

/* allocate a chunk of memory out of the free pool */
/* Return NULL if there is not enough memory in the free pool */
/* The address you return must be aligned to 1 (for size=1), 2 (for
   size=2), 4 (for size=3,4), 8 (for size=5,6,7,8). For all other
   sizes, align to 16.
*/
void *mpool_alloc(struct memory_pool *p, size_t size)
{
    if (!size) return NULL; // cannot allocate nothing

    struct llnode *node = p->free_list->first;
    int align = calc_align(size);
    int found = 0;
    int padding;
    int aligned_address;
    struct alloc_info *block;
    while (node) {
        block = (struct alloc_info *)node->user_data;
        aligned_address = align_address(align, block->offset);
        padding = aligned_address - block->offset;
        if (block->size >= size+padding) {
            found = 1;
            break;
        }
        node = node->next;
    }
    if (!found) {
        printf("ERROR: failed to allocate %d bytes: out of memory\n", size);
        return NULL;
    }

    // If we used up the entire block, remove it from the free list
    if (block->size == size+padding) {
        dbll_remove(p->free_list, node);
        free(block);
    }
    else {
        // Else, just shrink the free_list block
        block->size -= size+padding;
        block->offset += size+padding;
    }

    // Add the new block to the alloc_list
    struct alloc_info *alloc_block = block_create(aligned_address, size+padding, size);
    dbll_append(p->alloc_list, alloc_block);
    return p->start+aligned_address;
}

/* Free a chunk of memory out of the pool */
/* This moves the chunk of memory to the free list. */
/* You may want to coalesce free blocks [i.e. combine two free blocks
   that are are next to each other in the pool into one larger free
   block. Note this requires that you keep the list of free blocks in order */
void mpool_free(struct memory_pool *p, void *addr)
{
    /* search the alloc_list for the block */
    /* move it to the free_list */
    /* coalesce the free_list */
    struct llnode *node = p->alloc_list->first;
    struct alloc_info *block;
    int found = 0;
    while (node) {
        block = (struct alloc_info *) node->user_data;
        if (addr == &p->start[block->offset]) {
            found = 1;
            break;
        }
        node = node->next;
    }

    if (!found) {
        printf("ERROR: cannot free unallocated address\n");
        return;
    }

    // Move block from allocated to free
    dbll_remove(p->alloc_list, node);
    dbll_append(p->free_list, block);
    coalesce_free(p);
}

void coalesce_free(struct memory_pool *p)
{
    if (!p->free_list->first || !p->free_list->first->next) return;

    struct llnode *node1 = p->free_list->first;
    struct llnode *node2 = p->free_list->first;
    struct llnode *tmp;
    struct alloc_info *block1;
    struct alloc_info *block2;

    while(node1) {
        block1 = (struct alloc_info *) node1->user_data;
        while (node2) {
            tmp = node2->next;
            block2 = (struct alloc_info *) node2->user_data;
            if (block1->offset + block1->size == block2->offset
                    || block1->offset + block1->size + 1 == block2->offset) {
                // Merge block2 into block1
                block1->size += block2->size;
                dbll_remove(p->free_list, node2);
            }
            node2 = tmp;
        }
        node1 = node1->next;
        node2 = node1;
    }
}

void print_list(struct dbll *list)
{
    struct llnode *node = list->first;
    printf("[\n");
    while (node) {
        print_node(node);
        node = node->next;
    }
    printf("]\n");
}

void print_node(struct llnode *node)
{
    struct alloc_info block;
    block = *((struct alloc_info *) node->user_data);
    printf("\t{ offset = %lu, size = %lu, req = %lu }\n",
            block.offset, block.size, block.request_size);
}
