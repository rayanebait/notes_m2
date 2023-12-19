#include "fq.h"
#include "error.h"

#define ODD(a) ((a)%2 == 1)

int32_t
_eucl2(int32_t a, int32_t b){
    if(ODD(a) == 1 && ODD(b) == 1){
        return 2*_eucl2(a/2, b/2);
    }
    else if(ODD(a) == 1){
        return _eucl2(a/2, b);
    }
    else if(ODD(b) == 1){
        return _eucl2(a, b/2);
    }
    else{
        if(a > b){
            return _eucl2(a-b, b);
        }
        else if(a < b){
            return _eucl2(a, b-a);
        }
        else{
            return a;
        }
    }
}

void
_extended_eucl(int32_t a, int32_t b, int32_t *u, int32_t *v){
    if(a == 0 || b == 0){
        printf("?\n");
        return;
    }
    else if(a < b){
        int32_t t = b;
        b = a;
        a = t;
    }
    int32_t u0, u1, u2;
    u0 = 1;
    u1 = 0;

    int32_t r0, r1, r2;
    r0 = a;
    r1 = b;
    r2 = 1;
    
    int32_t q;
    while(r2 != 0){
        q = r0/r1;
        r2 = r0%r1;
        r0 = r1;
        r1 = r2;

        u2 = u0 - (u1 * q);
        u0 = u1;
        u1 = u2;
    }

    *u = u0;
    if(v == NULL){
        return;
    }
    else{
        *v = (r0 - (*u)*a)/b;
    }

}

void
_int_mod_inv(int32_t a, int32_t *inv, int32_t modulus){
    _extended_eucl(a, modulus , inv, NULL);
    return;
}

/*faudra initialiser le generateur a la main*/
int
fqctx_init(fqctx_t ctx, int32_t p, int32_t d){
    ctx->p = p;
    ctx->d = d;

    if(int_poly_init(ctx->polgen, ctx->d)){
        error_sys_set(__func__,"int_poly_init");
        return 1;
    }

    return 0;
}
void
fqctx_clear(fqctx_t ctx){
    int_poly_clear(ctx->polgen);
}

int 
int_poly_init(int_poly_t f, int degree){
    f->coeffs = calloc(2*degree + 1, sizeof(int32_t));
    if(!(f->coeffs)){
        error_sys_set(__func__, "int_poly_init");
        return 1;
    }

    f->deg = degree;
    
    /*lors de la multiplication a_ib_k-i on suppose les a_i=0 pour i>deg f*/
    /*On alloue plus pour pas avoir de pb après*/
    f->length = 2*degree + 1;
    return 0;
}


int
int_poly_realloc(int_poly_t f, int degree){
    int_poly_t tmp;
    if(int_poly_init(tmp, f->deg)){
        error_sys_set(__func__,"int_poly_realloc");
        return 1;
    }

    /*On fait une copie du polynome avant de réallouer*/
    memcpy(tmp->coeffs, f->coeffs, sizeof(int32_t)*(f->length));


    free(f->coeffs);
    f->coeffs = calloc(2*degree+1, sizeof(int32_t));
    if(!(f->coeffs)){
        error_sys_set(__func__,"calloc");
        return 1;
    }

    memcpy(f->coeffs, tmp->coeffs, sizeof(int32_t)*(tmp->deg+1));

    f->deg = degree;
    f->length = 2*degree;

    int_poly_clear(tmp);
    return 0;

}

void
int_poly_clear(int_poly_t f){
    free(f->coeffs);
}

void
_int_poly_degree(int_poly_t f){
    if(f->coeffs[f->deg] != 0) return ;

    int i = f->deg;
    while(i>=0 && f->coeffs[i] == 0){
        i--;
    }

    f->deg = i;
}

void
int_poly_set_zero(int_poly_t f){
    f->deg = -1;
    f->length = 0;
    f->coeffs[0] = 0;
}

int
int_poly_set(int_poly_t f, int_poly_t g){
    f->deg = g->deg;
    if(f->length < g->deg + 1){
        if(int_poly_realloc(f, g->deg)){
            error_sys_set(__func__,"calloc");
            return 1;
        }
    }

    for(int i = 0; i < g->deg + 1; i++) f->coeffs[i] = g->coeffs[i];

    return 0;
}

int
int_poly_init_set_int32(int_poly_t f, int32_t a){
    if(int_poly_init(f, 0)){
        error_sys_set(__func__,"int_poly_init");
        return 1;
    }
    f->coeffs[0] = a;

    return 0;
}

void 
int_polymod_reduce(int_poly_t f, int32_t modulus){
    if(f->length == 0) return;

    /*réduction des coeffs, on suppose que f[f->deg]%modulus est non nul pour l'instant*/
    for(int i = 0; i < f->deg + 1; i++){
        f->coeffs[i] = f->coeffs[i]%modulus;
    }

    _int_poly_degree(f);

}

int
int_polymod_add(int_poly_t f, int_poly_t g, int_poly_t h, int32_t modulus){
    int deg = (g->deg > h->deg) ? g->deg : h->deg;
    if(f->length < g->deg + 1 || f->length < g->deg + 1){
        if(int_poly_realloc(f, deg)){
            error_sys_set(__func__,"int_poly_realloc");
            return 1;
        }
    }

    for(int i = 0; i < deg + 1; i++) f->coeffs[i] = (g->coeffs[i] + h->coeffs[i])%modulus;

    _int_poly_degree(f);
    return 0;
}



int32_t
_comp_coeffsmod_mul(int_poly_t g, int_poly_t h, int32_t modulus, int32_t index){
    int32_t sum = 0;

    for(int i = 0; i <= index; i++){
        sum += g->coeffs[i]*(h->coeffs[index-i]);
        sum = sum % modulus;
    }

    return sum;
}

/*retourne 0 si ok, 1 sinon*/
int
int_polymod_mul(int_poly_t f, int_poly_t g, int_poly_t h, int32_t modulus){
    if(g->deg == -1 || h->deg == -1){
        int_poly_set_zero(f);
        return 0;
    }
    /*_comp_coeffsmod_mul accede jusqu'a h[g->deg+h->deg]*/
    /*Pas une bonne idée*/
    if(h->length <g->deg+h->deg +1){
        if(int_poly_realloc(h, g->deg+h->deg)){
            error_sys_set(__func__,"int_poly_realloc");
            return 1;
        }
    }
    /*_comp_coeffsmod_mul accede jusqu'a g[g->deg+h->deg]*/
    /*Faudrait mettre des if a la place */
    if(g->length<g->deg+h->deg +1){
        if(int_poly_realloc(h, g->deg+h->deg)){
            error_sys_set(__func__,"int_poly_realloc");
            return 1;
        }
    }
    if(f->length < g->deg+h->deg +1){
        if(int_poly_realloc(f, g->deg+h->deg)){
            error_sys_set(__func__,"int_poly_realloc");
            return 1;
        }
    }

    int_poly_t T;
    if(int_poly_init(T, f->deg)){
        error_sys_set(__func__,"int_poly_init");
        return 1;
    }

    for(int i = 0; i <= g->deg + h->deg; i++ ){
        T->coeffs[i] = _comp_coeffsmod_mul(g, h, modulus, i);
    }

    int_poly_set(f, T);
    _int_poly_degree(f);

    int_poly_clear(T);

    return 0;

}
int int_polymod_binexp(int_poly_t modulus, int_poly_t powered, int_poly_t pol, int32_t n, int32_t p){
    if(n <= 0){
        int_poly_set_zero(powered);
    }
    else{
        int32_t nb_coeffs = modulus->deg;
        int_poly_t tmp;
        int_poly_t tmp2;
        int32_t itmp;

        int_poly_init_set_int32(tmp, 1);
        int_poly_init(tmp2, nb_coeffs - 1);
        int_poly_set(tmp2, pol);
        itmp = n;

        while(itmp != 0){
            if(itmp%2 == 1){
                if(int_polymod_mul(tmp, tmp2, tmp, p)){
                    error_sys_set(__func__, "int_poly_mul");
                    return 1;
                }
                               
                if(int_polymod_divrem(tmp, modulus, NULL, tmp, p)){
                    error_sys_set(__func__, "int_poly_divrem");
                    return 1;
                }
            }
            itmp = (int32_t) (itmp/2); //itmp=[itmp/2]
            if(itmp!=0){
                if(int_polymod_mul(tmp2, tmp2, tmp2, p)){ //itmp=itmp^2
                    error_sys_set(__func__, "int_poly_mul");
                    return 1;
                }
                if(int_polymod_divrem(tmp2, modulus, NULL, tmp2, p)){
                    error_sys_set(__func__, "int_poly_divrem");
                    return 1;
                }
            }
        }
        int_poly_set(powered, tmp);

        int_poly_clear(tmp);
        int_poly_clear(tmp2);
    }
    
    return 0;
}

int
int_polymod_mul_int32(int_poly_t f, int_poly_t g, int32_t a, int32_t modulus){
    if(f->length < g->deg + 1){
        if(int_poly_realloc(f, g->deg)){
            error_sys_set(__func__,"int_poly_realloc");
            return 1;
        }
    }

    for(int32_t i = 0; i <= g->deg; i++){
        f->coeffs[i] = (g->coeffs[i]*a)%modulus;
    }

    _int_poly_degree(f);

    return 0;
}

int
int_polymod_submul_int32(int_poly_t f, int_poly_t g, int_poly_t h, int32_t a, int32_t modulus){
    if(a == 0){
        int_poly_set(f, g);
        return 0;
    }

    int32_t mdeg = (g->deg > h->deg) ? g->deg : h->deg;
    if(f->length < mdeg){
        if(int_poly_realloc(f, mdeg)){
            error_sys_set(__func__,"int_poly_realloc");
            return 1;
        }
    }

    /*Pas besoin d'allouer un autre poly intermediaire ca se fait terme a terme*/
    int_polymod_mul_int32(f, h, -a, modulus);
    int_polymod_add(f, g, f, modulus);


    /*gere automatiquement si deg(g)=deg(h)*/
    _int_poly_degree(f);

    return 0;
}
int 
int_polymod_submul(int_poly_t f, int_poly_t g, int_poly_t h, int_poly_t i, int32_t modulus){
    int32_t mdeg = (g->deg > h->deg + i->deg) ? g->deg : h->deg + i->deg;
    if(f->length < mdeg){
        if(int_poly_realloc(f, mdeg)){
            error_sys_set(__func__,"int_poly_realloc");
            return 1;
        }
    }

    int_polymod_mul(f, h, i, modulus);
    int_polymod_mul_int32(f, f, -1, modulus);
    int_polymod_add(f, g, f, modulus);

    _int_poly_degree(f);
    return 0;
}

/*ordre lexicographie sur les coefficients, retourne le signe de f->coeff-g->coeff*/
/*je sais pas si ce sera utile*/
int
int_poly_compare_lex(int_poly_t f, int_poly_t g){
    int deg = f->deg - g->deg;
    if(deg > 0){
        return 1;
    }
    else if(deg < 0){
        return -1;
    }
    int i = f->deg;
    while(i>= 0 && f->coeffs[i] == g->coeffs[i]) i--;

    if(i == -1) return 0;

    if(f->coeffs[i]<g->coeffs[i]) return 1;

    return -1;
}


/*shift vers la droite quand shift>0*/
/*shift vers la gauche quand shift<0*/
int
int_poly_shift(int_poly_t f, int_poly_t g, int shift){
    if(shift == 0){
        int_poly_set(f, g);
        return 0;
    }
    if(f->length < g->deg - shift + 1){
        if(int_poly_realloc(f, g->deg - shift)){
            error_sys_set(__func__, "int_poly_realloc");
            return 1;
        }
    }

    int_poly_t T;
    (void) int_poly_init(T,g->deg - shift + 1);

    if(shift > 0){
        for(int i = 0; i < g->deg - shift+1; i++){
            T->coeffs[i] = g->coeffs[i + shift];
        }
    }
    else{
        for(int i = -shift; i < g->deg - shift+1; i++){
            T->coeffs[i] = g->coeffs[i + shift];
        }
    }

    _int_poly_degree(T);

    int_poly_set(f, T);
    return 0;
}


/*int_poly_truncate(f, X^2+X+1, 1, 0) met X^2+X dans f */
int
int_poly_truncate(int_poly_t f, int_poly_t g, int until, int side){
    if(until>g->deg){
        int_poly_set_zero(f);
        return 0;
    }
    else if(until <= 0){
        int_poly_set(f, g);
        return 0;
    }
    if(side == 0){
        /*on tronque par la droite*/
        if(f->length < g->deg + 1){
            if(int_poly_realloc(f, g->deg + 1)){
                error_sys_set(__func__,"int_poly_realloc");
                return 1;
            }
        }

        for(int i = until; i < g->deg + 1; i++) f->coeffs[i] = g->coeffs[i];
    }
    else{
        /*On tronque par la gauche*/
        if(f->length < g->deg + 1 - until){
            if(int_poly_realloc(f, g->deg - until + 1)){
                error_sys_set(__func__,"int_poly_realloc");
                return 1;
            }
        }

        for(int i = g->deg; i > g->deg - until; i++) f->coeffs[i] = g->coeffs[i];
    }

    _int_poly_degree(f);
    return 0;
}


/*le modulus doit etre premier et tout doit être init*/
/*sauf Q, si Q==NULL seul le reste est calculé*/
int
int_polymod_divrem(int_poly_t A, int_poly_t B, int_poly_t Q, int_poly_t R, int32_t modulus){
    if(B->deg > A->deg){
        int_poly_set_zero(Q);
        int_poly_set(R, A);
        return 0;
    }

    int32_t inv_lead_coefB = 0;
    _int_mod_inv(B->coeffs[B->deg], &inv_lead_coefB, modulus);

    int_poly_t poly_inv_lead_coefB;
    int_poly_init_set_int32(poly_inv_lead_coefB, inv_lead_coefB);

    /*On rend B unitaire*/
    int_polymod_mul(B, B, poly_inv_lead_coefB, modulus);

    if(Q!= NULL && Q->length < A->deg - B->deg + 1){
        if(int_poly_realloc(Q, A->deg - B->deg)){
            error_sys_set(__func__,"int_poly_realloc");
            return 1;
        }
    }
    if(R->deg < B->deg){
        if(int_poly_realloc(R, B->deg)){
            error_sys_set(__func__,"int_poly_realloc");
            return 1;
        }
    }

    int shift = A->deg - B->deg;
    /*La deg(R)=deg(B) normalement*/
    int_poly_shift(R, A, shift);


    int_poly_t TMP;
    int_poly_init(TMP, shift);

    /*Contient les shift premiers coeffs*/
    /*ie TMP de degré deg(A)-deg(B)*/
    int_poly_truncate(TMP, A, B->deg,1);

    int index = 0;
    while(index <= shift){
        /*Ici c'est bien le terme de plus haut degré de B qu'on regarde*/
        /*D'ou le B->deg au cas ou R->deg*/
        /*Permet de passer outre le fait qu'on ait plus de coeffs de TMP a faire descendre */
        int_polymod_submul_int32(R, R, B, R->coeffs[B->deg], modulus);
        if(Q != NULL){
            Q->coeffs[shift - index] = R->coeffs[B->deg];
        }

        int_poly_shift(R, R, -1);
        R->coeffs[0] = TMP->coeffs[shift - index];

        index++;
    }

    if(R->deg >= B->deg){
        int_polymod_submul_int32(R, R, B, R->coeffs[R->deg], modulus);
    }
    /*Normalement a ce stade R->deg < B->deg*/


    int_poly_clear(TMP);
    return 0;
}

int
int_polymod_extended_eucl(int_poly_t A, int_poly_t B, int_poly_t U, int_poly_t V, int32_t modulus){
    int_poly_t U2;
    (void) int_poly_init(U2, B->deg);
    if(A->deg < B->deg){

        (void) int_poly_set(U2, B);
        (void) int_poly_set(B, A);
        (void) int_poly_set(A, U2);
    }

    int_poly_t U0, U1;
    (void) int_poly_init_set_int32(U0, 1);
    (void) int_poly_init_set_int32(U1, 0);

    int_poly_t R0, R1, R2;
    (void) int_poly_init(R0, A->deg);
    (void) int_poly_init(R1, B->deg);
    (void) int_poly_set(R0, A);
    (void) int_poly_set(R1, B);
    (void) int_poly_init_set_int32(R2, 1);
    
    int_poly_t Q;
    (void) int_poly_init(Q, A->deg - B->deg);
    while(R2->deg != -1){
        int_polymod_divrem(R0, R1, Q, R2, modulus);
        int_poly_set(R0, R1);
        int_poly_set(R1, R2);

        int_polymod_submul(U2, U0, U1, Q, modulus);
        
        int_poly_set(U0, U1);
        int_poly_set(U1, U2);
    }

    int_poly_set(U, U0);
    if(V == NULL){
        int_poly_clear(U0);
        int_poly_clear(U1);
        int_poly_clear(U2);
        int_poly_clear(R0);
        int_poly_clear(R1);
        int_poly_clear(R2);
        return 0;
    }
    else{
        int_polymod_submul(V, R0, U, A, modulus);
        int_polymod_divrem(V, B, R0, R1, modulus);
        int_poly_set(V, R0);

        int_poly_clear(U0);
        int_poly_clear(U1);
        int_poly_clear(U2);
        int_poly_clear(R0);
        int_poly_clear(R1);
        int_poly_clear(R2);
        return 0;
    }
}

int int_poly_from_str(int_poly_t f, char *str_poly){
    char *end = str_poly;
    int32_t degree = (int32_t) strtol(str_poly, &end, 10);

    int_poly_init(f, degree);

    int ind = 0;
    while(end != NULL && ind <= degree){
        str_poly = end;
        f->coeffs[ind] = (int32_t) strtol(str_poly, &end, 10);
        ind++;
    }
    _int_poly_degree(f);

    return 0;
}
void int_poly_print_pretty(int_poly_t f){

    
    if(f->coeffs[0] != 0) printf("(%ld)+",(long)f->coeffs[0]);

    for(int i = 1; i<f->deg; i++){
        if(f->coeffs[i] != 0) printf("(%ld)*X^%ld+",(long)f->coeffs[i], (long)i);
    }

    printf("%ldX^%ld",(long)f->coeffs[f->deg], (long) f->deg);
}


short
log_2(const long a)
{
    if (a == 0) return 0;

    short log = 0;
    while ((a >> log) != 1) log++;

    return log;
}