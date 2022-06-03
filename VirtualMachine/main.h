#ifndef PROJECT_MAIN_H
#define PROJECT_MAIN_H

#define AVM_STACKSIZE           4096

#define AVM_NUMACTUALS_OFFSET   +4
#define AVM_SAVEDPC_OFFSET      +3
#define AVM_SAVEDTOP_OFFSET     +2
#define AVM_SAVEDTOPSP_OFFSET   +1
#define AVM_STACKENV_SIZE       4

#define  AVM_MAX_INSTRUCTIONS 	20

#define AVM_TABLE_HASHSIZE      211

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

struct UserFunc {
    unsigned int address;
    unsigned int localSize;
    unsigned int totalargs;
    char *id;
};

enum VmOpcode {
    assign_v,		add_v,				sub_v,
    mul_v,			div_v,				mod_v,
    jump_v,			jeq_v,				jne_v,
    jle_v,			jge_v,				jlt_v,
    jgt_v,			call_v,				pusharg_v,
    funcenter_v,	funcexit_v,			newtable_v,
    tablegetelem_v,	tablesetelem_v, 	nop_v
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

struct DataTables {
    double *numConstsTable;
    unsigned int totalNumConsts;

    char **strConstsTable;
    unsigned int totalStrConsts;

    char **libFuncsTable;
    unsigned int totalLibFuncs;

    struct UserFunc *userFuncsTable;
    unsigned int totalUserFuncs;

    struct Instruction *code;
    unsigned int codeSize;

    // This is extra
    unsigned int totalGlobals;
};

enum AvmMemCellType {
    number_m = 0,
    string_m = 1,
    bool_m = 2,
    table_m = 3,
    userfunc_m = 4,
    libfunc_m = 5,
    nil_m = 6,
    undef_m = 7
};

struct AvmMemCell {
    enum AvmMemCellType type;
    union {
        double numVal;
        char *strVal;
        unsigned char boolVal;
        struct AvmTable *tableVal;
        unsigned int funcVal;
        char *libfuncVal;
    } data;
};

struct AvmTableBucket {
    struct AvmMemCell key;
    struct AvmMemCell value;
    struct AvmTableBucket *next;
};

struct AvmTable {
    unsigned int refCounter;
    struct AvmTableBucket *strIndexed[AVM_TABLE_HASHSIZE];
    struct AvmTableBucket *numIndexed[AVM_TABLE_HASHSIZE];
    unsigned int total;
};

extern struct AvmMemCell ax, bx;
extern struct AvmMemCell retval;
extern unsigned int top;
extern unsigned int topsp;

extern unsigned char executionFinished;
extern unsigned int pc;

extern struct DataTables* tables;

extern unsigned int totalActuals;
extern struct AvmMemCell stack[AVM_STACKSIZE];


#endif //PROJECT_MAIN_H
