#ifndef PROJECT_TOOLS_H
#define PROJECT_TOOLS_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>

#include "../main.h"

void printError(char *message, char *arg1, char *arg2);

typedef char *(*toStringFunction)(struct AvmMemCell *);
typedef void (*libraryFunction)(void);
typedef double (*arithmeticFunction)(double x, double y);
typedef unsigned char (*comparisonFunction)(double x, double y);
typedef unsigned char (*toBoolFunction)(struct AvmMemCell *);
typedef void (*execFunction)(struct Instruction *);

// memoryManager.c
struct AvmTable *newTable();
struct AvmMemCell *tableGetElem(struct AvmTable *t, struct AvmMemCell *i);
void tableSetElem(struct AvmTable *t, struct AvmMemCell *i, struct AvmMemCell *v);
void tableIncRefCounter(struct AvmTable *t);

void memClear(struct AvmMemCell *m);
void assign_(struct AvmMemCell *lv, struct AvmMemCell *rv);

char *toString(struct AvmMemCell *m);
unsigned char toBool(struct AvmMemCell *m);

void callLibraryFunction(char *name);
void callSaveEnvironment();

void decreaseTop();

struct UserFunc *getFuncInfo(unsigned int address);
unsigned int getEnvValue(unsigned int i);

unsigned int totalActuals_(void);

void doArithmeticInstr(struct Instruction* instr);
void doComparisonInstr(struct Instruction* instr);

struct AvmMemCell* parseOperand(struct VmArg* arg, struct AvmMemCell* reg);

void initialize(void);

// binaryDecoder.c
struct BinaryData* binaryDecode(char* name);

// dispatcher.c
void startCycle();
void functionExit(struct Instruction *i);

#endif //PROJECT_TOOLS_H
