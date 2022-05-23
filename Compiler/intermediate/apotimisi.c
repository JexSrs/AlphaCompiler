#include "intermediate.h"

struct LogicList *makeList(int quadNo) {
    struct LogicList *list = malloc(sizeof(struct LogicList));
    memset(list, 0, sizeof(struct LogicList));

    list->QuadNo = quadNo;
    list->next = NULL;

    return list;
}

struct LogicList *mergeLLs(struct LogicList *l1, struct LogicList *l2) {
    if (l1 == NULL && l2 == NULL)
        return NULL;
    else if (l1 == NULL)
        return l2;
    else if (l2 == NULL)
        return l1;

    struct LogicList *tmp = l1;
    while (tmp->next != NULL)
        tmp = tmp->next;
    tmp->next = l2;
    return l1;
}

void backPatchList(struct LogicList *list, int quadNo) {
    struct LogicList *tmp = list;
    while (tmp != NULL) {
        patchLabel(tmp->QuadNo, quadNo);
        tmp = tmp->next;
    }
}

void checkForEmitAND(struct expr *e, int line, int notRulePassed) {
    if (e->type == boolexpr_e) return;

    e->trueList = makeList(nextQuadLabel());
    e->falseList = makeList(nextQuadLabel() + 1);

    emit(if_eq, newExprConstBool('1'), e, NULL, nextQuadLabel(), line);
    emit(jump, NULL, NULL, NULL, nextQuadLabel(), line);

    backPatchList(e->trueList, nextQuadLabel());
}

void checkForEmitOR(struct expr *e, int line, int notRulePassed) {
    if (e->type == boolexpr_e) return;

    e->trueList = makeList(nextQuadLabel());
    e->falseList = makeList(nextQuadLabel() + 1);

    emit(if_eq, newExprConstBool('1'), e, NULL, nextQuadLabel(), line);
    emit(jump, NULL, NULL, NULL, nextQuadLabel(), line);

    backPatchList(e->falseList, nextQuadLabel());
}

void checkForEmitNotEq(struct expr *e, int line, int scope) {
    if (e->type != boolexpr_e || e->cameFromNot != 1) return;

    e->sym = newTmp(scope);

    backPatchList(e->trueList, nextQuadLabel());

    emit(assign, newExprConstBool('1'), NULL, e, nextQuadLabel(), line);
    emit(jump, NULL, NULL, newExprConstNumber(nextQuadLabel() + 2), nextQuadLabel(), line);

    backPatchList(e->falseList, nextQuadLabel());

    emit(assign, newExprConstBool('0'), NULL, e, nextQuadLabel(), line);
}

void checkForEmitEqEq(struct expr *e, int line, int scope) {
    checkForEmitNotEq(e, line, scope);
}