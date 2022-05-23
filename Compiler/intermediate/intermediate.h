#ifndef PROJECT_INTERMEDIATE_H
#define PROJECT_INTERMEDIATE_H

#include "../bison/bison.h"
#include <stddef.h>
#include <assert.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

extern int numberOfDigits(int tempcounter);

extern char *giveName(char *prefix, int num);

enum IOPCode{
    assign,
    add, sub, mul, div_, mod,
    jump,
    if_eq, if_noteq,
    if_lesseq, if_greatereq,
    if_less, if_greater,
    call,
    param,
    funcstart, funcend,
    tablecreate, tablegetelem, tablesetelem,
    getretval,
    no_op,
    ret
};

enum ExprType {
    var_e,
    tableitem_e,

    programfunc_e,
    libraryfunc_e,

    arithexpr_e,
    boolexpr_e,
    assignexpr_e,
    newtable_e,

    constnum_e,
    constbool_e,
    conststring_e,

    nil_e
};

struct ForLoop_S {
    int test;
    int enter;
};

struct LogicList{
    int QuadNo;
    struct LogicList *next;
};

struct expr {
    enum ExprType type;
    struct SymbolTable *sym;
    struct expr *index;
    struct expr *indexedVal;
    double numConst;
    char *strConst;
    unsigned char boolConst;
    struct expr *next;
    struct expr *prev;

    struct LogicList *trueList;
    struct LogicList *falseList;
    int cameFromNot;
    int voidParam;
};

struct MethodCall{
    struct expr *elist;
    int method;
    char *name;
};

struct Quad {
    enum IOPCode op;
    struct expr *result;
    struct expr *arg1;
    struct expr *arg2;
    unsigned int label;
    unsigned int line;
    unsigned int tAddress;
};

struct MyStack {
    int info;
    struct MyStack *next;
};

struct LoopStack {
    struct BCList *breakList;
    struct BCList *contList;
    struct LoopStack *next;
};

struct BCList{
    int QuadNo;
    struct BCList *next;
};

extern struct Quad *quads;
extern unsigned int functionLocalOffset;

#define EXPAND_SIZE 1024
#define CURR_SIZE 	(total * sizeof(struct Quad))
#define NEW_SIZE 	(EXPAND_SIZE * sizeof(struct Quad) + CURR_SIZE)


void emit (
        enum IOPCode op,
        struct expr *arg1,
        struct expr *arg2,
        struct expr *result,
        unsigned int label,
        unsigned int line
);

char *newTempName();

struct SymbolTable *newTmp(int scope);
void resetTmp();

enum ScopeSpace currentScopeSpace();

unsigned int currentScopeOffset();
void increaseCurrentScopeOffset();

void enterScopeSpace();
void exitScopeSpace();

void initIntermediate();

void resetFormalArgOffset();
void resetFunctionLocalOffset();

void patchLabel(unsigned int quadNo, unsigned int lbl);
int nextQuadLabel();

void checkUminus(struct expr *e);

struct expr *newExpr(enum ExprType type);
struct expr *newExprConstString(char *s);
struct expr *newExprConstNumber(double d);
struct expr *newExprConstBool(unsigned char c);
struct expr *newExprNil();

struct expr *lvalueExpr(struct SymbolTable *sym);

struct expr *emitIfTableItem(struct expr *e, int quadNo, int line, int scope);
struct expr *memberItem(struct expr *lvalue, int quadNo, int line, int scope, char* name);
struct expr *makeCall(struct expr *lvalue, struct expr *elist, int scope, int line);

/* START - Meriki apotimisi
*/

struct LogicList *makeList(int quadNo);
struct LogicList *mergeLLs(struct LogicList *l1, struct LogicList *l2);

void backPatchList(struct LogicList *list, int quadNo);
void checkForEmitAND(struct expr *e, int line, int notRulePassed);
void checkForEmitOR(struct expr *e, int line, int notRulePassed);
void checkForEmitNotEq(struct expr *e, int line, int scope);
void checkForEmitEqEq(struct expr *e, int line, int scope);

/* END - Meriki apotimisi
*/

/* START - Stacks
*/

void push(int mem);
int pop();

void pushLoopCounter(int mem);
int popLoopCounter();

void pushFuncJump(int mem);
int popFuncJump();

/* END - Stacks
*/


/* Break & Continue
*/
void loopPush();
void breakPush(int quadNo);
void continuePush(int quadNo);

struct LoopStack *loopPop();

#endif //PROJECT_INTERMEDIATE_H
