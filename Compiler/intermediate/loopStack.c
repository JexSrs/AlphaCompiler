#include "intermediate.h"

struct LoopStack *loopTop;

void loopPush() {
    struct LoopStack *new = malloc(sizeof(struct LoopStack));
    new->breakList = NULL;
    new->contList = NULL;

    if(loopTop == NULL) {
        new->next = NULL;
        loopTop = new;
    } else {
        new->next = loopTop;
        loopTop = new;
    }
}

void breakPush(int quadNo) {
    struct BCList *new = malloc(sizeof(struct BCList));
    new->QuadNo = quadNo;

    if(loopTop->breakList == NULL) {
        new->next = NULL;
        loopTop->breakList = new;
    } else {
        new->next = loopTop->breakList;
        loopTop->breakList = new;
    }
}

void continuePush(int quadNo) {
    struct BCList *new = malloc(sizeof(struct BCList));
    new->QuadNo = quadNo;

    if(loopTop->contList == NULL) {
        new->next = NULL;
        loopTop->contList = new;
    } else {
        new->next = loopTop->contList;
        loopTop->contList = new;
    }
}

struct LoopStack *loopPop() {
    if(loopTop == NULL) {
        printf("Error, trying to pop from empty loop stack.");
        return NULL;
    }

    struct LoopStack *tmp = loopTop;
    loopTop = loopTop->next;
    tmp->next = NULL;

    return tmp;
}