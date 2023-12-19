#ifndef AUTHENTICATOR_H
#define AUTHENTICATOR_H

#include "error.h"

typedef enum {
    COMMAND_LIST_CREDENTIALS=48,
    COMMAND_MAKE_CREDENTIAL,
    COMMAND_GET_ASSERTION,
    COMMAND_RESET
}command_t;


status_t parse_request(void);



#endif /*AUTHENTICATOR_H*/