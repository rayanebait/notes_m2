#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#include "error.h"
#include "count.h"


#define BUFSIZE 1024



static int file_parse(const char *, letter_t *);
static void process_buf(char *, size_t, letter_t*);
static void occ_print(const letter_t *);
static int compar(const void*, const void*);
static void sort(letter_t *);

int
main(int argc, char **argv){
    if(argc!=2){
        fprintf(stderr, "Usage: %s <file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    letter_t occ[26];
    memset(occ, 0, sizeof(occ));

    /*Les bits 27 à 32 indique la lettre (26 lettres sur 5 bits) les autres le nb d'occurences*/
    for(int i = 0; i < 26; i++){
        occ[i] = i << 27;
    }

    if(file_parse(argv[1], occ) < 0){
        fprintf(stderr,"%s failed: %s\n",argv[0], error_sys_get());
        exit(EXIT_FAILURE);
    }

    return 0;
}

static int
file_parse(const char *filename, letter_t *occ)
{
    int fd;

    fd = open(filename, O_RDONLY);
    if(fd == -1){
        error_sys_set(__func__, "open");
        return -1;
    }

    while(1)
    {
        char buf[BUFSIZE];
        ssize_t noc;

        noc = read(fd, buf, BUFSIZE);
        if(noc < 0){
            error_sys_set(__func__, "read");
            (void) close(fd);
            return -1;
        }

        if(noc == 0)
            break;

        process_buf(buf, noc, occ);
    }

    (void) occ_print(occ);
    sort(occ);
    printf("Sorted\n\n");
    (void) occ_print(occ);
    

    return 0;
}

static void process_buf(char *buf, size_t len, letter_t *occ)
{
    for(char *p = buf; p-buf < len; p++)
    {
        if('a' <= *p  && *p <= 'z'){
            occ[*p - 'a']++;
        }
        if('A' <= *p  && *p <= 'Z'){
            occ[*p - 'A']++;
        }
    }

}

static void
occ_print(const letter_t *occ)
{
    for(int i = 0; i < 26; i++)
    {
        printf("%c : %u\n", LETTER_GET(occ[i]), OCC_GET(occ[i]));
    }
}

static int
compar(const void *l1, const void *l2)
{
    return OCC_GET(*(const letter_t *) l2) - OCC_GET(*(const letter_t *) l1);
}

static void
sort(letter_t *occ){
    qsort((void *) occ, 26, sizeof(letter_t), compar);
}