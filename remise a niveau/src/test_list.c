#include "list.h"
#include "error.h"

int main(void){
    list_t l = LIST_EMPTY;

    for(int i = 0; i < 10000; i++){
        if(list_insert(l, ITEM_TYPE_INT, &i) < 0)
        {
            fprintf(stderr, "%s\n", error_sys_get());
            exit(EXIT_FAILURE);
        }
    }

    list_insert(l, ITEM_TYPE_STR, "tentative");

    list_print(l, '\n', stdout);

    list_free(l);


    exit(EXIT_SUCCESS);
}