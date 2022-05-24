#include <stdlib.h>
#include "linked_list.h"
#include "err_code.h"

int linked_list_initialize(struct LinkedList *ll) {
    if (!ll) {
        return ERR_NULL_PTR;
    }
    ll->size = 0;
    ll->front = 0;
    ll->back = 0;
    return 0;
}

int linked_list_push_back(struct LinkedList *ll, void *data) {
    if (!ll || !data) {
        return ERR_NULL_PTR;
    }
    struct LinkedListNode *temp = malloc(sizeof(struct LinkedListNode));
    if (!temp) {
        return ERR_OUT_OF_MEMORY;
    }
    temp->data = data;
    temp->next = 0;
    temp->prev = ll->back;
    if (ll->back) {
        ll->back->next = temp;
    } else {
        ll->front = temp;
    }
    ll->back = temp;
    ++ll->size;
    return 0;
}

int linked_list_pop_front(struct LinkedList *ll) {
    if (!ll) {
        return ERR_NULL_PTR;
    }
    if (!ll->size) {
        return ERR_LINKED_LIST_EMPTY;
    }
    free(ll->front->data);
    struct LinkedListNode *temp = ll->front;
    ll->front = temp->next;
    free(temp);
    if (!ll->front) {
        ll->back = 0;
    }
    --ll->size;
    return 0;
}

int linked_list_finalize(struct LinkedList *ll) {
    if (!ll) {
        return ERR_NULL_PTR;
    }
    for (; ll->size; linked_list_pop_front(ll));
    return 0;
}

int linked_list_log(
    struct LinkedList *ll ,
    void (*print_func)(void*, FILE*),
    FILE *fout
) {
    if (!ll) {
        return ERR_NULL_PTR;
    }
    fprintf(fout, "size: %lu\n", ll->size);
    for (struct LinkedListNode *i = ll->front; i; i = i->next) {
        print_func(i->data, fout);
    }
    return 0;
}