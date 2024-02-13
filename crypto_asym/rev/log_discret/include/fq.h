#ifndef FQ_H
#define FQ_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

typedef struct {
    int32_t *coeffs;
    int deg;
    int length;
}int_poly;

typedef int_poly int_poly_t[1];

typedef struct {
    int32_t p;
    int32_t d;
    int_poly_t polgen;
}fqctx;


typedef fqctx fqctx_t[1];

short log_2(const long);
int fqctx_init(fqctx_t ctx, int32_t p, int32_t d);
void fqctx_clear(fqctx_t ctx);

int int_poly_init(int_poly_t f, int degree);
int int_poly_realloc(int_poly_t f, int degree);
void int_poly_clear(int_poly_t f);
int int_poly_set(int_poly_t, int_poly_t);
void int_poly_set_zero(int_poly_t f);
int int_poly_init_set_int32(int_poly_t f, int32_t a);

void int_polymod_reduce(int_poly_t, int32_t);
int int_polymod_add(int_poly_t f, int_poly_t g, int_poly_t h, int32_t modulus);
 int int_polymod_mul(int_poly_t, int_poly_t, int_poly_t, int32_t);
int int_polymod_mul_int32(int_poly_t f, int_poly_t g, int32_t a, int32_t modulus);
int int_polymod_submul_int32(int_poly_t f, int_poly_t g, int_poly_t h, int32_t a, int32_t modulus);
int int_polymod_submul(int_poly_t f, int_poly_t g, int_poly_t h, int_poly_t i, int32_t modulus);
int int_poly_compare_lex(int_poly_t f, int_poly_t g);
int int_poly_shift(int_poly_t f, int_poly_t g, int shift);
int int_poly_truncate(int_poly_t f, int_poly_t g, int until, int side);
int int_polymod_divrem(int_poly_t A, int_poly_t B, int_poly_t Q, int_poly_t R, int32_t modulus);
int int_polymod_extended_eucl(int_poly_t A, int_poly_t B, int_poly_t U, int_poly_t V, int32_t modulus);
int int_poly_from_str(int_poly_t, char*);
void int_poly_print_pretty(int_poly_t);

#endif /*FQ_H*/