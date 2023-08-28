#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>

#define END 0
#define START 1
#define ADD 2
#define SUB 3
#define BOX 4
#define GO 5
#define IF 6
#define DRAW 7

// first 3 bits are operation identifiers
// the remaining 29 bits are the operand's
#define OP_BITS 3
#define OP_MASK 7

#define PUTOP(op) program[*counter] = op; is_operation = true;

static char* ops[8] = {
    "end",
    "start",
    "add",
    "sub",
    "box",
    "go",
    "if",
    "draw"
};

static int start; // program starting index
static int box; // the buffer value
static int data[1024];
static int program[1024];

int is_number(char* str) {
    for (int i=0; str[i]!='\0'; i++) {
        if (!isdigit(str[i])) return 0;
    }
    return 1;
}

void print_program() {
    int i = 0;
    while (program[i] != END) {
        printf("%d\n", program[i]);
        i++;
    }
}

int op_code(char* str) {
    // returns OP code, returns -1 on failure
    for (int i=0; i < OP_MASK+1; i++) {
        if (!strcmp(str, ops[i])) {
            return i;
        } 
    }  
    return -1;
}

int operation(int value) {
    return value & OP_MASK;
}

int operand(int value) {
    return value >> OP_BITS;
}

int parse_jawa_line(char* line, uint* counter) {
    /* returns 0 in case of success
    returns 1 in case of syntax error*/
    char *str;
    bool is_operation = false; 
    while ( (str = strsep(&line, " \t\n")) != NULL ) {
        if (*str == '\0') continue;
        if (str[0] == '#') {
            *counter += 1;
            return 0;
        }// commentary reached
        if (is_number(str)) { 
            // if this line does not contain an operator
            if (!is_operation) {
                // direct writing number to memory
                program[*counter] = atoi(str);
                
            } else {
                // place numerical value after operation bits
                program[*counter] += (atoi(str)<<OP_BITS);
            }
            *counter += 1;
            return 0;
        } else { // Operator and operand syntax
            int operation = op_code(str);
            if (operation != -1) {
                // found operation
                program[*counter] = operation;
                if (operation == START || operation == END) {
                    if (operation == START) start = *counter;
                    *counter += 1;
                    return 0;
                } else {
                    is_operation = true;
                }
            } else {
                // found unknown operation
                return 1;
            } 
        }
    }
    *counter += 1; // empty line
    return 0;
}

int parse_jawa(FILE* fp) {
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    uint counter = 1;
    while ( (read = getline(&line, &len, fp)) != -1 ) {
        // read strings split by whitespaced in this line 
        if (parse_jawa_line(line, &counter) == 1) {
            return 1;
        }
    }
    free (line);
    return 0;
}

int execute_jawa() {
    int ip = start; // instruction pointer
    while (operation(program[ip]) != END ) {
        switch (operation(program[ip])) {
            case BOX:
                box = program[operand(program[ip])];
                ip++;
                break;
            case ADD:
                program[operand(program[ip])] += box;
                ip++;
                break;
            case SUB:
                program[operand(program[ip])] -= box;
                ip++;
                break;
            case GO:
                ip = operand(program[ip]);
                break;
            case IF:
                if (box == 0) ip = operand(program[ip]);
                else ip++;
                break;
            case DRAW:
                printf("%c", program[operand(program[ip])] );
                ip++;
                break;
            case END:
                return 0;
                break;
            case START:
                ip++;
                break;
        }
    }
    return 0;
}

int main(int argc, const char * argv[]) {
    FILE* fp;
    if (argc != 2 || (fp = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "Jawas can't read %s", argv[1]);
        return 1;
    }
    if (parse_jawa(fp) == 1){
        fprintf(stderr, "Syntax error!");
        return 1;
    }
    execute_jawa();
    fclose(fp);
    return 0;
}
