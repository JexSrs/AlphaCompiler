#ifndef PROJECT_TARGETCODE_H
#define PROJECT_TARGETCODE_H

#include "../intermediate/intermediate.h"

typedef void (*generatorFunc) (struct Quad*);

struct InstructionToBinary {
    int instrOpcode;
    int instrLine;

    int resultType;
    int resultOffset;

    int arg1Type;
    int arg1Offset;

    int arg2Type;
    int arg2Offset;
};

struct IncompleteJump {
    unsigned int instrNo;
    unsigned int iaddress;
};

enum Typer {
    NUMBER_T,
    STRINGER_T,
    LIBFUNCER_T,
    USERFUNCER_T,
    INSTRUCTER_T
};

enum VmOpcode {
    assign_v, add_v, sub_v,
    mul_v, div_v, mod_v,
    jump_v, jeq_v, jne_v,
    jle_v, jge_v, jlt_v,
    jgt_v, call_v, pusharg_v,
    funcenter_v, funcexit_v, newtable_v,
    tablegetelem_v, tablesetelem_v, nop_v
};

enum VmArg_t {
    invalid = -1,
    label_a = 0,
    global_a = 1,
    formal_a = 2,
    local_a = 3,
    number_a = 4,
    string_a = 5,
    bool_a = 6,
    nil_a = 7,
    userfunc_a = 8,
    libfunc_a = 9,
    retval_a = 10,
    nop_a = 11
};

struct VmArg {
    enum VmArg_t type;
    unsigned int val;
};

struct Instruction {
    enum VmOpcode opcode;
    struct VmArg result;
    struct VmArg arg1;
    struct VmArg arg2;
    unsigned int srcLine;
};

struct UserFunc {
    unsigned int address;
    unsigned int localSize;
    unsigned int totalArgs;
    char *id;
};

struct FuncStack {
    struct SymbolTable *info;
    struct FuncStack *next;
};

// funcStartTarget.c
void pushFuncStackTarget(struct SymbolTable *mem);
struct SymbolTable *popFuncStackTarget();
struct SymbolTable *topFuncStackTarget();
void appendFuncStackTarget(struct SymbolTable *f, unsigned int instrLabel);

// constTables.c
extern double *numConsts;
extern unsigned int totalNumConsts;
extern char **stringConsts;
extern unsigned int totalStringConsts;
extern char **namedLibFuncs;
extern unsigned int totalNamedLibFuncs;
extern struct UserFunc *userFuncs;
extern unsigned int totalUserFuncs;
extern struct Instruction *instructions;
extern unsigned int totalInstructions;

int insertNumber(double val);
int insertString(char* val);
int insertLibraryDunc(char* val);
int insertUserFunc(unsigned int address, unsigned int localSize, unsigned int totalargs, char* id);

int emitInstr(struct Instruction t);

// operands.c
void generateTcode(unsigned int totalQuads);
unsigned int nextInstructionLabel(void);

// binary.c
void createBinaryFile(char *name);

#endif //PROJECT_TARGETCODE_H
