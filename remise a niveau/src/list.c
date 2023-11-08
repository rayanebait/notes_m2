#include <string.h>

#include "error.h"
#include "list.h"

static item_t *item_new(item_type_t, void*);

size_t 
list_length(const list_t l)
{
    return l->length;
}

int 
list_insert(list_t l, item_type_t t, void *d)
{
    item_t *i;
    i = item_new(t, d);
    if(!i){
        error_sys_set(__func__, "malloc");
        return -1;
    }

    /*first devient next*/
    i->next = l->first;
    l->first = i;
    if(!l->last)
        l->last = i;

    l->length++;

    return 0;
}

void
list_print(const list_t l, char end, FILE* os)
{
    fprintf(os, "[");
    for(item_t *i=l->first; i; i = i->next){
        switch(i->type)
        {
            case ITEM_TYPE_INT:
                fprintf(os, "%d", *(int*) i->data);
                break;
            case ITEM_TYPE_STR:
                fprintf(os, "\"%s\"", (char*) i->data);
                break;
        }
        if(i->next != NULL){
            fprintf(os, ", ");
        }
    }
    fprintf(os, "]");
    if(end)
    {
        fprintf(os, "%c",end);
    }

}

static item_t *
item_new(item_type_t t, void* data)
{
    size_t dlen;

    switch (t){
        case ITEM_TYPE_INT : dlen = sizeof(int);
                                break;
        case ITEM_TYPE_STR : dlen = strlen(data) + 1;
                                break;
        default :
                return NULL;
    }

    item_t *i = malloc(sizeof *i);
    if(!i){
        /*malloc failed*/
        /*treat error*/
        return NULL;
    }

    i->type = t;

    i->data = malloc(dlen);
    if(!i->data){
        free(i);
        return NULL;
    }

    (void) memcpy(i->data, data, dlen);

    return i;
}

static void
item_free(item_t *i)
{
    free(i->data);
    free(i);
}


void
list_free(list_t l)
{
    item_t *tmp;
    for(item_t *i = l->first; i; i = tmp){
        tmp = i->next;
        item_free(i);
    }

}