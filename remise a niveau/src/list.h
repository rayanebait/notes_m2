#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>


typedef enum{
    ITEM_TYPE_INT=1,
    ITEM_TYPE_STR
}item_type_t;


/*La le __item est défini par le compilateur avant item_t pour ca*/
/*qu'on def __item avant __item_t*/
typedef struct __item{
    item_type_t type;
    void *data;
   struct __item *next;
}item_t;

typedef struct{
    item_t *first;
    item_t *last;
    size_t length;
}list;

typedef list list_t[1];

#define LIST_EMPTY {{.first = NULL,.last = NULL,.length = 0}}

void list_free(list_t);

size_t list_length(const list_t);

int list_insert(list_t, item_type_t, void *);

void list_print(const list_t, char, FILE*);

#endif /*LIST_H*/