
#include <stdio.h>

#include "error.h"
#include "uart.h"
#include "Authenticator.h"

status_t parse_request(void){
    // char c = fgetc(stdin);
    char request[41];
    switch(fread(request, 1, 41, stdin)){
        case 21: /*MakeCredential*/
            if(request[0] != (char) COMMAND_MAKE_CREDENTIAL){
                set_error(STATUS_ERR_BAD_PARAMETER);
                print_error(err_code);

                return STATUS_ERR_BAD_PARAMETER;
            } else {
                // MakeCredential();

                return STATUS_OK;
            }
        case 41: /*GetAssertion*/
            if(request[0] != (char) COMMAND_GET_ASSERTION){
                set_error(STATUS_ERR_BAD_PARAMETER);
                print_error(err_code);

                return STATUS_ERR_BAD_PARAMETER;
            } else {
                // GetAssertion();

                return STATUS_OK;
            }
        case 1: /*Reset or list Credentials*/
            if(request[0] == (char) COMMAND_LIST_CREDENTIALS){
                //ListCredentials();

                return STATUS_OK;
            } else if(request[0] == (char) COMMAND_RESET){
                // reset();
                set_error(STATUS_OK);
                print_error(STATUS_OK);

                return STATUS_OK;
            } else {
                set_error(STATUS_ERR_BAD_PARAMETER);
                print_error(err_code);

                return STATUS_ERR_BAD_PARAMETER;
            }
        default:
            fprintf(stderr, "Unknown error, shouldn't happen\r\n");

            return STATUS_ERR_BAD_PARAMETER;
    }

}

