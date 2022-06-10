#include "tools.h"

static void doAssign(struct Instruction *i) {
    struct AvmMemCell *a = parseOperand(&i->result, NULL);
    struct AvmMemCell *b = parseOperand(&i->arg1, &amc1);
    assign_(a, b);
}

static void doJump(struct Instruction *i) {
    if (!running) pc = i->result.val;
}

static void doJEq(struct Instruction *i) {
    struct AvmMemCell *op1 = parseOperand(&i->arg1, &amc1);
    struct AvmMemCell *op2 = parseOperand(&i->arg2, &amc2);

    unsigned char result = 0;

    if (op1->type == undef_m || op2->type == undef_m) {
        printError("Cannot receive \"undefined\" in equality", NULL, NULL);
        running = 0;
    } else if (op1->type == nil_m || op2->type == nil_m)
        result = op1->type == nil_m && op2->type == nil_m;
    else if (op1->type == bool_m || op2->type == bool_m)
        result = toBool(op1) == toBool(op2);
    else if (op1->type != op2->type)
        result = 0; // Do not exit, just say that they are not equal.
    else if (op1->type == number_m)
        result = op1->data.numVal == op2->data.numVal;
    else if (op1->type == string_m)
        result = !strcmp(op1->data.strVal, op2->data.strVal);
    else
        result = toBool(op1) == toBool(op2);

    if (!running && result)
        pc = i->result.val;
}

static void doJNE(struct Instruction *i) {
    struct AvmMemCell *rv1 = parseOperand(&i->arg1, &amc1);
    struct AvmMemCell *rv2 = parseOperand(&i->arg2, &amc2);

    unsigned char result = 0;
    if (rv1->type == undef_m || rv2->type == undef_m) {
        printError("Cannot receive \"undefined\" in equality", NULL, NULL);
        running = 0;
    } else if (rv1->type == nil_m || rv2->type == nil_m)
        result = (rv1->type == nil_m) && (rv2->type == nil_m);
    else if (rv1->type == bool_m || rv2->type == bool_m)
        result = (toBool(rv1) == toBool(rv2));
    else if (rv1->type != rv2->type)
        result = 0; // Do not exit, just say that they are not equal.
    else  if (rv1->type == number_m)
        result = rv1->data.numVal == rv2->data.numVal;
    else if (rv1->type == string_m)
        result = !strcmp(rv1->data.strVal, rv2->data.strVal);
    else
        result = (toBool(rv1) == toBool(rv2));

    if (!running && !result)
        pc = i->result.val;
}

static void doCall(struct Instruction *i) {
    struct AvmMemCell *func = parseOperand(&i->result, &amc1);
    callSaveEnvironment();

    switch (func->type) {
        case userfunc_m: pc = func->data.funcVal;                        break;
        case string_m: callLibraryFunction(func->data.strVal);     break;
        case libfunc_m: callLibraryFunction(func->data.libfuncVal); break;
        default:
            printError("cannot call\"", toString(func), "\" as function");
            running = 0;
    }
}

static void doPushArg(struct Instruction *i) {
    struct AvmMemCell *a = parseOperand(&i->result, &amc1);
    assign_(&stack[top], a);
    totalActuals++;
    decreaseTop();
}

static void doFunCenter(struct Instruction *i) {
    totalActuals = 0;

    struct UserFunc *funcInfo = getFuncInfo(binaryData->code[pc].result.val);
    top_sp = top;
    top = top - funcInfo->localSize;

    if (totalActuals_() != 0 && funcInfo->totalArgs != 0 && (totalActuals_() < funcInfo->totalArgs)) {
        printError("function ", funcInfo->id, "has insufficient arguments");
        running = 0;
        exit(0);
    }
}

void functionExit(struct Instruction *i) {
    unsigned int oldTop = top;

    top = getEnvValue(top_sp + AVM_SAVED_TOP_OFFSET);
    pc = getEnvValue(top_sp + AVM_SAVE_DPC_OFFSET);
    top_sp = getEnvValue(top_sp + AVM_SAVED_TOPSP_OFFSET);

    while (++oldTop <= top)
        memClear(&stack[oldTop]);
}

static void doNewTable(struct Instruction *i) {
    struct AvmMemCell *lv = parseOperand(&i->result, NULL);

    memClear(lv);

    lv->type = table_m;
    lv->data.tableVal = newTable();
    tableIncRefCounter(lv->data.tableVal);
}

char *typeStringsT[] = {
    "number",
    "string",
    "bool",
    "table",
    "userfunc",
    "libfunc",
    "nil",
    "undef"
};

static void doTableGetElem(struct Instruction *instr) {
    struct AvmMemCell *lv = parseOperand(&instr->result, NULL);
    struct AvmMemCell *t = parseOperand(&instr->arg1, NULL);
    struct AvmMemCell *i = parseOperand(&instr->arg2, &amc1);

    memClear(lv);

    lv->type = nil_m;
    if (t->type != table_m) {
        printError("Type ", typeStringsT[t->type], " is not a table.");
        running = 0;
        return;
    }

    struct AvmMemCell *content = tableGetElem(t->data.tableVal, i);

    if (!content) {
        printf("WARN: Index not found! Table %s %s\n", toString(t), toString(i));
        return;
    }

    assign_(lv, content);
}

static void doTableSetElem(struct Instruction *instr) {
    struct AvmMemCell *t = parseOperand(&instr->arg1, NULL);
    struct AvmMemCell *i = parseOperand(&instr->arg2, &amc1);
    struct AvmMemCell *c = parseOperand(&instr->result, &amc2);

    if (t->type != table_m) {
        printError("Type ", typeStringsT[t->type], " is not a table.");
        running = 0;
        return;
    }

    tableSetElem(t->data.tableVal, i, c);
}

static void doNOP(struct Instruction *i) {
    running = 0;
}

execFunction executeFuncs[] = {
    doAssign,
    doArithmeticInstr, // add
    doArithmeticInstr, // sub
    doArithmeticInstr, // mul
    doArithmeticInstr, // div
    doArithmeticInstr, //mod
    doJump,
    doJEq,
    doJNE,
    doComparisonInstr, // jle
    doComparisonInstr, // jge
    doComparisonInstr, // jlt
    doComparisonInstr, // jgt
    doCall,
    doPushArg,
    doFunCenter,
    functionExit,
    doNewTable,
    doTableGetElem,
    doTableSetElem,
    doNOP
};

void startCycle(void) {
    if (!running)
        return;

    if (pc == binaryData->codeSize) {
        running = 0;
        return;
    }

    struct Instruction *instr = binaryData->code + pc;
    unsigned int oldPC = pc;
    (*executeFuncs[instr->opcode])(instr);

    if (pc == oldPC)
        pc++;
}