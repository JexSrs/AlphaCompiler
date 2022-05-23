#include "intermediate.h"

int numberOfDigits(int tempcounter) {
    int counter = 0;
    if (tempcounter == 0)
        return 1;
    while (tempcounter != 0) {
        tempcounter /= 10;
        counter++;
    }
    return counter;
}

char *giveName(char *prefix, int num) {
    char *name = malloc(strlen(prefix) + numberOfDigits(num) + 1);
    sprintf(name, "%s%d", prefix, num);
    return name;
}


int tmpCounter = 0;
unsigned int total;
unsigned int currentQuad;
unsigned int programVarOffset;
unsigned int formalArgOffset;
unsigned int scopeSpaceCounter;

struct Quad *quads;
unsigned int functionLocalOffset;

static void expand() {
    assert(total == currentQuad);

    struct Quad *q = (struct Quad *) malloc(NEW_SIZE);

    if (quads) {
        memcpy(q, quads, CURR_SIZE);
        free(quads);
    }

    quads = q;
    total += EXPAND_SIZE;
}

void emit(enum IOPCode op, struct expr *arg1, struct expr *arg2, struct expr *result, unsigned int label,
          unsigned int line) {
    if (currentQuad == total)
        expand();

    struct Quad *q = quads + currentQuad++;
    q->op = op;
    q->arg1 = arg1;
    q->arg2 = arg2;
    q->result = result;
    q->label = label;
    q->line = line;
}

char *newTempName() {
    return giveName("_t", tmpCounter);
}

struct SymbolTable *newTmp(int scope) {
    char *name = newTempName();

    struct SymbolTable *symt = findTemp(name, scope);
    if (symt) return symt;

    symt = insert(name, variable, scope == 0 ? GLOBAL : LOCAL, scope, -1,
                  0, currentScopeSpace(), currentScopeOffset());

    increaseCurrentScopeOffset();
    return symt;
}

void resetTmp() {
    tmpCounter = 0;
}

enum ScopeSpace currentScopeSpace() {
    if (scopeSpaceCounter == 1)
        return PROGRAM_VAR;
    else if (scopeSpaceCounter % 2 == 0)
        return FORMAL_ARG;
    else return FUNC_LOCAL;
}

unsigned int currentScopeOffset() {
    switch (currentScopeSpace()) {
        case PROGRAM_VAR:
            return programVarOffset;
        case FORMAL_ARG:
            return formalArgOffset;
        case FUNC_LOCAL:
            return functionLocalOffset;
        default:
            assert(NULL);
    }
}

void increaseCurrentScopeOffset() {
    switch (currentScopeSpace()) {
        case PROGRAM_VAR:
            programVarOffset++;
            break;
        case FORMAL_ARG:
            formalArgOffset++;
            break;
        case FUNC_LOCAL:
            functionLocalOffset++;
            break;
        default:
            assert(NULL);
    }
}

void enterScopeSpace() {
    scopeSpaceCounter++;
}

void exitScopeSpace() {
    assert(scopeSpaceCounter > 1);
    scopeSpaceCounter--;
}

void initIntermediate() {
    quads = (struct Quad *) 0;
    total = 0;
    currentQuad = 0;

    programVarOffset = 0;
    formalArgOffset = 0;
    functionLocalOffset = 0;

    scopeSpaceCounter = 1;
}

void resetFormalArgOffset() {
    formalArgOffset = 0;
}

void resetFunctionLocalOffset() {
    functionLocalOffset = 0;
}

void patchLabel(unsigned int quadNo, unsigned int lbl) {
    assert(quadNo < currentQuad);
    quads[quadNo].result = newExprConstNumber(lbl);
}

int nextQuadLabel() {
    return (int) currentQuad;
}

void checkUminus(struct expr *e) {
    switch (e->type) {
        case constbool_e:
        case conststring_e:
        case boolexpr_e:
        case nil_e:
        case newtable_e:
        case programfunc_e:
        case libraryfunc_e:
            printf("Illegal expression unary minus\n");
            exit(0);
    }
}

struct expr *newExpr(enum ExprType type) {
    struct expr *e = malloc(sizeof(struct expr));
    memset(e, 0, sizeof(struct expr));
    if (type != -1)
        e->type = type;
    return e;
}

struct expr *newExprConstString(char *s) {
    struct expr *e = newExpr(conststring_e);
    e->strConst = strdup(s);
    return e;
}

struct expr *newExprConstNumber(double d) {
    struct expr *e = newExpr(constnum_e);
    e->numConst = d;
    return e;
}

struct expr *newExprConstBool(unsigned char c) {
    struct expr *e = newExpr(constbool_e);
    e->boolConst = c;
    return e;
}

struct expr *newExprNil() {
    struct expr *e = newExpr(nil_e);
    e->numConst = 0;
    return e;
}

struct expr *lvalueExpr(struct SymbolTable *sym) {
    assert(sym);

    struct expr *e = newExpr(-1);
    e->next = NULL;
    e->sym = sym;

    switch (sym->type) {
        case LOCAL:
        case GLOBAL:
        case FORMAL:
            e->type = var_e;
            break;
        case LIB_FUNC:
            e->type = libraryfunc_e;
            break;
        case USER_FUNC:
            e->type = programfunc_e;
            break;
        default:
            assert(0);
    }

    return e;
}

struct expr *emitIfTableItem(struct expr *e, int quadNo, int line, int scope) {
    if (e->type != tableitem_e)
        return e;

    struct expr *result = newExpr(var_e);
    result->sym = newTmp(scope);
    emit(tablesetelem, e, e->index, result, quadNo, line);

    return result;
}

struct expr *memberItem(struct expr *lvalue, int quadNo, int line, int scope, char *name) {
    lvalue = emitIfTableItem(lvalue, quadNo, line, scope);

    struct expr *item = newExpr(tableitem_e);
    item->sym = lvalue->sym;
    item->index = newExprConstString(name);
    return item;
}

struct expr *makeCall(struct expr *lvalue, struct expr *elist, int scope, int line) {
    struct expr *func = emitIfTableItem(lvalue, nextQuadLabel(), line, scope);
    struct expr *tmp = elist;

    while (tmp->next != NULL)
        tmp = tmp->next;

    if (elist->voidParam == 0) {
        while (tmp != NULL) {
            emit(param, tmp, NULL, func, nextQuadLabel(), line);
            tmp = tmp->prev;
        }
    }

    emit(call, NULL, NULL, func, nextQuadLabel(), line);

    struct expr *result = newExpr(var_e);
    result->sym = newTmp(scope);

    emit(getretval, NULL, NULL, result, nextQuadLabel(), line);

    return result;
}
