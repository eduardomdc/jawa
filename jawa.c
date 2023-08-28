#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

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

int operation(int value) {
    return value & OP_MASK;
}

int operand(int value) {
    return value >> OP_BITS;
}

int parse_jawa(FILE* fp) {
    char str[128];
    unsigned short counter = 1;
    while (fscanf(fp, "%s", str) != EOF) {
        if (is_number(str)) {
            program[counter] = atoi(str);
        }
        else if (strcmp(str, "BOX") == 0) {
            program[counter] = BOX;
            if (fscanf(fp, "%s", str) != EOF){
                if (is_number(str)){
                    program[counter] += (atoi(str)<<OP_BITS);
                } else return 1;
            }
        }
        else if (strcmp(str, "ADD") == 0) {
            program[counter] = ADD;
            if (fscanf(fp, "%s", str) != EOF){
                if (is_number(str)){
                    program[counter] += (atoi(str)<<OP_BITS);
                } else return 1;
            }
        }
        else if (strcmp(str, "SUB") == 0) {
            program[counter] = SUB;
            if (fscanf(fp, "%s", str) != EOF){
                if (is_number(str)){
                    program[counter] += (atoi(str)<<OP_BITS);
                } else return 1;
            }
        }
        else if (strcmp(str, "GO") == 0) {
            program[counter] = GO;
            if (fscanf(fp, "%s", str) != EOF){
                if (is_number(str)){
                    program[counter] += (atoi(str)<<OP_BITS);
                } else return 1;
            }
        }
        else if (strcmp(str, "IF") == 0) {
            program[counter] = IF;
            if (fscanf(fp, "%s", str) != EOF){
                if (is_number(str)){
                    program[counter] += (atoi(str)<<OP_BITS);
                } else return 1;
            }
        }
        else if (strcmp(str, "DRAW") == 0) {
            program[counter] = DRAW;
            if (fscanf(fp, "%s", str) != EOF){
                if (is_number(str)){
                    program[counter] += (atoi(str)<<OP_BITS);
                } else return 1;
            }
        }
        else if (strcmp(str, "START") == 0) {
            program[counter] = START;
            start = counter;
        }
        else if (strcmp(str, "END") == 0) {
            program[counter] = END;
        }
        else {
            return 1;
        }
        counter++;
    }
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
