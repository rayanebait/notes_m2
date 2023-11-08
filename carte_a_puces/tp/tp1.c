#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<inttypes.h>

#define GET_INSTR_TYPE(instr) (instr & 7)
#define GET_INSTR(instr) (instr & 7<<3)

#define MAX_LABEL_CHAR 32
#define MAX_LABEL 100
#define MAX_SIZE_INSTR 100


typedef struct{
    char name[MAX_LABEL_CHAR];
    int line;
}label_t;

typedef int32_t instr_t


static int32_t parse_asm(char *, size_t);





int main(int argc, char *argv[]){
    if(argc != 2){
        fprintf(stderr, "Usage: %s <asm file>\n", argv[0]);
        exit(1);
    }

    FILE *asm_file = fopen(argv[1], "r");
    if(!asm_file){
        fprintf(stderr, "fopen\n");
        exit(1);
    }

    fseek(asm_file, 0L, SEEK_END);
    size_t len_file = ftell(asm_file);
    fseek(asm_file, 0L, SEEK_SET);

    FILE *coded = fopen("coded.txt", "w");
    if(!coded){
        fprintf(stderr, "fopen2\n");
        exit(1);
    }

    int line_counter = 0;

    char line[MAX_SIZE_INSTR];
    memset(line, 0, MAX_SIZE_INSTR);

    label_t labels[MAX_LABEL];

    int ind = 0; 

    while(line == fgets(line, MAX_SIZE_INSTR, asm_file) && !feof(asm_file)){
        if(line[0] != 9){
            memcpy(labels[ind++].name, line, MAX_SIZE_INSTR);
            line_counter++;
        }
        else{
            instr_t instr = parse_asm_instr(line, MAX_SIZE_INSTR, labels);
            if(!instr){
                fclose(asm_file);
                fclose(coded);
            }
        }


    }
    



    exit(0);
}



static instr_t parse_asm(char *buf, size_t len_buf, label_t *labels){
    instr_t coded_line = 0;

    const char* separators = " ,"
    char *token = strtok(buf, separators);

    char *token2 = token;

    char instr[10];
    int instr_number = 1;

    while ( token != NULL ) {
        token = strtok ( NULL, separators );
        
        memcpy(instr, buf + token2, (size_t) token - token2 );

        if(instr_number == 1 && instr[2] == 'w'){
            code_instr_wlabel(&coded_line, )
        }
        (void) code_instr(&coded_line, instr, instr_number);

        instr_number++;
        token2 = token;
    }


}

static void code_instr(instr_t *to_code, char *instr, int instr_number){
    switch(instr_number){
        case 1 :
            code_OP(to_code, instr);
            break;
        case 2:
            code_r1(to_code, instr);
            break;
        case 3:
            code_r2(to_code, instr);
            break;
        case 4:
            code_cst_label()
    }

}