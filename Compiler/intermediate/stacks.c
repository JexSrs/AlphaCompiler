#include "intermediate.h"

struct MyStack *functionLocalsStack = NULL,
        *loopCounterStack = NULL,
        *functionJumpStack = NULL;

void push(int mem) {
    struct MyStack *new = malloc(sizeof(struct MyStack));
    new->info = mem;

    if(functionLocalsStack == NULL) {
        new->next = NULL;
        functionLocalsStack = new;
    } else {
        new->next = functionLocalsStack;
        functionLocalsStack = new;
    }
}

int pop() {
    if(functionLocalsStack == NULL)
        return -1;

    struct MyStack *tmp = functionLocalsStack;
    int i = tmp->info;
    functionLocalsStack = functionLocalsStack->next;

    tmp->next = NULL;

    // TODO - Throws error at CSD machines
//    free(tmp);

    return i;
}

void pushLoopCounter(int mem) {
    struct MyStack *new = malloc(sizeof(struct MyStack));
    new->info = mem;

    if(loopCounterStack == NULL) {
        new->next = NULL;
        loopCounterStack = new;
    } else {
        new->next = loopCounterStack;
        loopCounterStack = new;
    }
}

int popLoopCounter() {
    if(loopCounterStack == NULL)
        return -1;

    struct MyStack *tmp = loopCounterStack;
    int i = tmp->info;
    loopCounterStack = loopCounterStack->next;

    tmp->next = NULL;

    // TODO - Throws error at CSD machines
//    free(tmp);

    return i;
}

void pushFuncJump(int mem) {
    struct MyStack *new = malloc(sizeof(struct MyStack));
    new->info = mem;

    if(functionJumpStack == NULL) {
        new->next = NULL;
        functionJumpStack = new;
    } else {
        new->next = functionJumpStack;
        functionJumpStack = new;
    }
}

int popFuncJump() {
    if(functionJumpStack == NULL)
        return -1;

    struct MyStack *tmp = functionJumpStack;
    int i = tmp->info;
    functionJumpStack = functionJumpStack->next;

    tmp->next = NULL;

    // TODO - Throws error at CSD machines
//    free(tmp);

    return i;
}