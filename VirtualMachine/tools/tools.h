#ifndef PROJECT_TOOLS_H
#define PROJECT_TOOLS_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>

#include "../main.h"

#define ENOUGH_NUM(n) (int)((ceil(log10(n)) + 1) * sizeof(char))

typedef void (*memclear_func_t)(struct AvmMemCell *);
typedef char *(*tostring_func_t)(struct AvmMemCell *);
typedef void (*library_func_t)(void);
typedef double (*arithmetic_func_t)(double x, double y);
typedef unsigned char (*cmp_func_t)(double x, double y);
typedef unsigned char (*tobool_func_t)(struct AvmMemCell *);
typedef void (*execute_func_t)(struct Instruction *);

// memoryManager.c
struct AvmTable *avm_tableNew();
struct AvmMemCell *avm_tablegetelem(struct AvmTable *table, struct AvmMemCell *index);
void avm_tablesetelem(struct AvmTable *table, struct AvmMemCell *index, struct AvmMemCell *value);
void avm_tableIncRefCounter(struct AvmTable *t);
void avm_memcellClear(struct AvmMemCell *m);
void avm_assign(struct AvmMemCell *lv, struct AvmMemCell *rv);

void avm_warning(char *msg1, char *id, char *msg2);
void avm_error(char *msg1, char *id, char *msg2);

char *avm_tostring(struct AvmMemCell *m);

void avm_callLibFunc(char *libfuncName);
void avm_callSaveEnvironment();

void avm_dec_top();

struct UserFunc *avm_getfuncinfo(unsigned int address);
unsigned int avm_get_envvalue(unsigned int i);

unsigned int avm_totalactuals(void);

void execute_arithmetic(struct Instruction* instr);
void execute_comparison(struct Instruction* instr);

unsigned char avm_tobool(struct AvmMemCell *m);

struct AvmMemCell* avm_translate_operand(struct VmArg* arg, struct AvmMemCell* reg);

void avm_initialize(void);

// binaryDecoder.c
struct DataTables* decode(int visualize, char* name);

// dispatcher.c
void execute_cycle();

void execute_funcexit(struct Instruction *instr);

#endif //PROJECT_TOOLS_H
