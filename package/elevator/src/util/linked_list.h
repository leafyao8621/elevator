#ifndef _LINKED_LIST_H_
#define _LINKED_LIST_H_

#include <stdint.h>
#include <stdio.h>

struct LinkedListNode {
    struct LinkedListNode *prev, *next;
    void *data;
};

struct LinkedList {
    uint64_t size;
    struct LinkedListNode *front, *back;
};

int linked_list_initialize(struct LinkedList *ll);
int linked_list_push_back(struct LinkedList *ll, void *data);
int linked_list_pop_front(struct LinkedList *ll);
int linked_list_finalize(struct LinkedList *ll);
int linked_list_log(
    struct LinkedList *ll ,
    void (*print_func)(void*, FILE*),
    FILE *fout
);

#endif