#include <stdio.h>
#include <openssl/evp.h>
#include <unistd.h>
#include <fcntl.h>

#include "../src/error.h"

static int file_digest(const char*, EVP_MD_CTX *);


int
main(int argc, char **argv)
{
    const EVP_MD *md;
    EVP_MD_CTX *ctx;
    unsigned char md_value[512];
    unsigned int md_len;


    if(argc!=3){
        fprintf(stderr, "Usage: %s <digest> <file>\n", argv[0]);
        return 1;
    }

    md = EVP_get_digestbyname(argv[1]);
    if(!md){
        fprintf(stderr, "%s: %s: unknown or unsupported message digest\n",argv[0], argv[1]);
        return 1;
    }

    ctx = EVP_MD_CTX_new();
    if(!ctx){
        return 1;
    }

    if(!EVP_DigestInit_ex(ctx, md, NULL)){
        EVP_MD_CTX_free(ctx);
        return 1;
    }

    if(file_digest(argv[2], ctx) < 0){
        fprintf(stderr, "%s: %s\n", argv[0], error_sys_get());
        return 1;
    }

    if(!EVP_DigestFinal_ex(ctx, md_value, &md_len)){
        EVP_MD_CTX_free(ctx);
        return 1;
    }

    for(unsigned char *p = md_value; p - md_value < md_len ;p++)
    {
        printf("%02hhx", *p);
    }
    printf("\n");



    return 0;
}

static int
file_digest(const char *file,EVP_MD_CTX *ctx)
{
    int fd, ret = 0;
    fd = open(file, O_RDONLY);
    if(fd < 0){
        error_sys_set(__func__, "open");
        return -1;
    }

    while(1)
    {
        char buf[1024];
        ssize_t noc;

        noc = read(fd, buf, sizeof(buf));
        if(noc < 0){
            error_sys_set(__func__,"read");
            ret = -1;
            goto err;
        }

        if(noc == 0)
            break;

        if(!EVP_DigestUpdate(ctx, buf, noc)){
            error_sys_set(__func__, "EVP_DigestUpdate");
            /*error_ssl_set(__func__, "EVP_DigestUpdate");*/
            ret = -1;
            goto err;
        }
    }

    err:
    (void) close(fd);

    return ret;
}