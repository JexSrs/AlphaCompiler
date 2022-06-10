#include "targetCode.h"

int totalNumSize = 0;
int totalStringSize = 0;
int totalLibFuncSize = 0;
int totalUserFuncSize = 0;
int totalInstrSize = 0;

#define CURR_SIZE_NUM           (totalNumConsts * sizeof(double))
#define NEW_SIZE_NUM            (EXPAND_SIZE * sizeof(double) + CURR_SIZE_NUM)

#define CURR_SIZE_STR           (totalStringConsts * sizeof(char*))
#define NEW_SIZE_STR            (EXPAND_SIZE * sizeof(char*) + CURR_SIZE_STR)

#define CURR_SIZE_LIBFUNC       (totalNamedLibFuncs * sizeof(char*))
#define NEW_SIZE_LIBFUNC        (EXPAND_SIZE * sizeof(char*) + CURR_SIZE_LIBFUNC)

#define CURR_SIZE_USERFUNC      (totalUserFuncs * sizeof(struct UserFunc))
#define NEW_SIZE_USERFUNC       (EXPAND_SIZE * sizeof(struct UserFunc) + CURR_SIZE_USERFUNC)

#define CURR_SIZE_INSTR_ARR     (totalInstrSize * sizeof(struct Instruction))
#define NEW_SIZE_INSTR_ARR      (EXPAND_SIZE * sizeof(struct Instruction) + CURR_SIZE_INSTR_ARR)

double *numConsts = NULL;
unsigned int totalNumConsts = 0;
char **stringConsts = NULL;
unsigned int totalStringConsts = 0;
char **namedLibFuncs = NULL;
unsigned int totalNamedLibFuncs = 0;
struct UserFunc *userFuncs = NULL;
unsigned int totalUserFuncs = 0;
struct Instruction *instructions = NULL;
unsigned int totalInstructions = 0;

static void expandType(enum Typer type) {
    switch (type) {
        case NUMBER_T: {
            double *a = malloc(NEW_SIZE_NUM);
            if (numConsts) {
                memcpy(a, numConsts, CURR_SIZE_NUM);
                free(numConsts);
            }
            numConsts = a;
            totalNumSize += EXPAND_SIZE;
        }
            break;
        case STRINGER_T: {
            char **a1 = malloc(NEW_SIZE_STR);
            if (stringConsts) {
                memcpy(a1, stringConsts, CURR_SIZE_STR);
                free(stringConsts);
            }
            stringConsts = a1;
            totalStringSize += EXPAND_SIZE;
        }
            break;
        case LIBFUNCER_T: {
            char **a2 = malloc(NEW_SIZE_LIBFUNC);
            if (namedLibFuncs) {
                memcpy(a2, namedLibFuncs, CURR_SIZE_LIBFUNC);
                free(namedLibFuncs);
            }
            namedLibFuncs = a2;
            totalLibFuncSize += EXPAND_SIZE;
        }
            break;
        case USERFUNCER_T: {
            struct UserFunc *a3 = malloc(NEW_SIZE_USERFUNC);
            if (userFuncs) {
                memcpy(a3, userFuncs, CURR_SIZE_USERFUNC);
                free(userFuncs);
            }
            userFuncs = a3;
            totalUserFuncSize += EXPAND_SIZE;
        }
            break;
        case INSTRUCTER_T: {
            struct Instruction *a4 = malloc(NEW_SIZE_INSTR_ARR);
            if (instructions) {
                memcpy(a4, instructions, CURR_SIZE_INSTR_ARR);
                free(instructions);
            }
            instructions = a4;
            totalInstrSize += EXPAND_SIZE;
        }
            break;
        default:
            assert(0);
    }
}

int insertNumber(double val) {
    for (int i = 0; i < totalNumConsts; i++) {
        if(numConsts[i] == val)
            return i;
    }

    if(totalNumConsts == totalNumSize)
        expandType(NUMBER_T);

    int pos = (int) totalNumConsts;
    numConsts[totalNumConsts++] = val;

    return pos;
}

int insertString(char *val) {
    for (int i = 0; i < totalStringConsts; i++) {
        if(strcmp(stringConsts[i], val) == 0)
            return i;
    }

    if(totalStringConsts == totalStringSize)
        expandType(STRINGER_T);

    int pos = (int) totalStringConsts;
    stringConsts[totalStringConsts++] = val;

    return pos;
}

int insertLibraryDunc(char *val) {
    for (int i = 0; i < totalNamedLibFuncs; i++) {
        if(strcmp(namedLibFuncs[i], val) == 0)
            return i;
    }

    if(totalNamedLibFuncs == totalLibFuncSize)
        expandType(LIBFUNCER_T);

    int pos = (int) totalNamedLibFuncs;
    namedLibFuncs[totalNamedLibFuncs++] = val;

    return pos;
}

int insertUserFunc(unsigned int address, unsigned int localSize, unsigned int totalargs, char *id) {
    for (int i = 0; i < totalUserFuncs; i++) {
        if(strcmp(userFuncs[i].id , id) == 0 && userFuncs[i].address == address)
            return i;
    }

    if(totalUserFuncs == totalUserFuncSize)
        expandType(USERFUNCER_T);

    int pos = (int) totalUserFuncs;

    struct UserFunc *new = userFuncs + totalUserFuncs++;
    new->id = id;
    new->address = address;
    new->localSize = localSize;
    new->totalArgs = totalargs;

    return pos;
}

int emitInstr(struct Instruction t) {
    if(totalInstructions == totalInstrSize)
        expandType(INSTRUCTER_T);

    struct Instruction *new = instructions + totalInstructions++;
    new->opcode = t.opcode;
    new->result = t.result;
    new->arg1 = t.arg1;
    new->arg2 = t.arg2;
    new->srcLine = t.srcLine;

    return (int) totalInstructions;
}
