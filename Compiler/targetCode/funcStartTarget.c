#include "targetCode.h"

struct FuncStack *functionStackTarget = NULL;


static int isFuncStackTargetEmpty() {
    return functionStackTarget == NULL;
}

void pushFuncStackTarget(struct SymbolTable *mem) {
    struct FuncStack *new = malloc(sizeof(struct FuncStack));
    new->info = mem;

    if(functionStackTarget == NULL)
        new->next = NULL;
    else
        new->next = functionStackTarget;

    functionStackTarget = new;
}

struct SymbolTable *popFuncStackTarget() {
    if(isFuncStackTargetEmpty())
        return NULL;

    // Get item
    struct FuncStack *tmp = functionStackTarget;
    struct SymbolTable *sym = tmp->info;

    // Override top
    functionStackTarget = tmp->next;
    tmp->next = NULL;

    // TODO - Throws error at CSD machines
//    free(tmp);

    return sym;
}

struct SymbolTable *topFuncStackTarget() {
    if(isFuncStackTargetEmpty())
        return NULL;

    return functionStackTarget->info;
}

void appendFuncStackTarget(struct SymbolTable *f, unsigned int instrLabel) {
    struct ReturnList *new = malloc(sizeof(struct ReturnList));
    new->instrLabel = instrLabel;
    new->next = NULL;

    if(f->value.funcVal->returnList == NULL)
        f->value.funcVal->returnList = new;
    else {
        struct ReturnList *tmp = f->value.funcVal->returnList;
        while (tmp->next != NULL)
            tmp = tmp->next;

        tmp->next = new;
    }
}