#include "tools.h"

static void execute_assign(struct Instruction *instr) {
    struct AvmMemCell *lv = avm_translate_operand(&instr->result, NULL);
    struct AvmMemCell *rv = avm_translate_operand(&instr->arg1, &ax);

    assert(lv);
    assert(lv && (&stack[AVM_STACKSIZE - 1] >= lv && lv > &stack[top] || lv == &retval));
    assert(rv);

    avm_assign(lv, rv);
}

static void execute_jump(struct Instruction *instr) {
    assert(instr->result.type == label_a);

    if (!executionFinished)
        pc = instr->result.val;
}

static void execute_jeq(struct Instruction *instr) {
    assert(instr->result.type == label_a);

    struct AvmMemCell *rv1 = avm_translate_operand(&instr->arg1, &ax);
    struct AvmMemCell *rv2 = avm_translate_operand(&instr->arg2, &bx);

    unsigned char result = 0;

    if (rv1->type == undef_m || rv2->type == undef_m) {
        avm_error("'undef' involved in equality", NULL, NULL);
        executionFinished = 1;
    } else if (rv1->type == nil_m || rv2->type == nil_m)
        result = (rv1->type == nil_m) && (rv2->type == nil_m);
    else if (rv1->type == bool_m || rv2->type == bool_m)
        result = (avm_tobool(rv1) == avm_tobool(rv2));
    else if (rv1->type != rv2->type) {
        avm_error("Equality check between different types is illegal", NULL, NULL);
        executionFinished = 1;
    } else {
        if (rv1->type == number_m)
            result = rv1->data.numVal == rv2->data.numVal;
        else if (rv1->type == string_m)
            result = !strcmp(rv1->data.strVal, rv2->data.strVal);
        else
            result = (avm_tobool(rv1) == avm_tobool(rv2));
    }

    if (!executionFinished && result)
        pc = instr->result.val;
}

static void execute_jne(struct Instruction *instr) {
    assert(instr->result.type == label_a);

    struct AvmMemCell *rv1 = avm_translate_operand(&instr->arg1, &ax);
    struct AvmMemCell *rv2 = avm_translate_operand(&instr->arg2, &bx);

    unsigned char result = 0;

    if (rv1->type == undef_m || rv2->type == undef_m) {
        avm_error("'undef' involved in equality", NULL, NULL);
        executionFinished = 1;
    } else if (rv1->type == nil_m || rv2->type == nil_m)
        result = (rv1->type == nil_m) && (rv2->type == nil_m);
    else if (rv1->type == bool_m || rv2->type == bool_m)
        result = (avm_tobool(rv1) == avm_tobool(rv2));
    else if (rv1->type != rv2->type) {
        avm_error("Equality check between different types is illegal", NULL, NULL);
        executionFinished = 1;
    } else {
        if (rv1->type == number_m)
            result = rv1->data.numVal == rv2->data.numVal;
        else if (rv1->type == string_m)
            result = !strcmp(rv1->data.strVal, rv2->data.strVal);
        else
            result = (avm_tobool(rv1) == avm_tobool(rv2));
    }

    if (!executionFinished && !result)
        pc = instr->result.val;
}

static void execute_call(struct Instruction *instr) {
    struct AvmMemCell *func = avm_translate_operand(&instr->result, &ax);
    assert(func);

    avm_callSaveEnvironment();
    switch (func->type) {
        case userfunc_m: {
            pc = func->data.funcVal;
            assert(pc < tables->codeSize);
            assert(tables->code[pc].opcode == funcenter_v);
            break;
        }
        case string_m:
            avm_callLibFunc(func->data.strVal);
            break;
        case libfunc_m:
            avm_callLibFunc(func->data.libfuncVal);
            break;
        default: {
            char *s = avm_tostring(func);
            avm_error("Call : cannot bind '", s, "' to function!");
            executionFinished = 1;
        }
    }
}

static void execute_pusharg(struct Instruction *instr) {
    struct AvmMemCell *arg = avm_translate_operand(&instr->result, &ax);
    assert(arg);

    avm_assign(&stack[top], arg);
    totalActuals++;
    avm_dec_top();
}

static void execute_funcenter(struct Instruction *instr) {
    struct AvmMemCell *func = avm_translate_operand(&instr->result, &ax);
    assert(func);
    assert(pc == func->data.funcVal);

    totalActuals = 0;

    struct UserFunc *funcInfo = avm_getfuncinfo(tables->code[pc].result.val);
    topsp = top;
    top = top - funcInfo->localSize;

    if (avm_totalactuals() != 0 && funcInfo->totalargs != 0 && (avm_totalactuals() < funcInfo->totalargs)) {
        avm_error("Too few arguments for function ", funcInfo->id, NULL);
        executionFinished = 1;
        exit(0);
    }
}

void execute_funcexit(struct Instruction *instr) {
    unsigned int oldTop = top;

    top = avm_get_envvalue(topsp + AVM_SAVEDTOP_OFFSET);
    pc = avm_get_envvalue(topsp + AVM_SAVEDPC_OFFSET);
    topsp = avm_get_envvalue(topsp + AVM_SAVEDTOPSP_OFFSET);

    while (++oldTop <= top)
        avm_memcellClear(&stack[oldTop]);
}

static void execute_newtable(struct Instruction *instr) {
    struct AvmMemCell *lv = avm_translate_operand(&instr->result, NULL);
    assert(lv && (&stack[AVM_STACKSIZE - 1] >= lv && lv > &stack[top] || lv == &retval));

    avm_memcellClear(lv);

    lv->type = table_m;
    lv->data.tableVal = avm_tableNew();
    avm_tableIncRefCounter(lv->data.tableVal);
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

static void execute_tablegetelem(struct Instruction *instr) {
    struct AvmMemCell *lv = avm_translate_operand(&instr->result, NULL);
    struct AvmMemCell *t = avm_translate_operand(&instr->arg1, NULL);
    struct AvmMemCell *i = avm_translate_operand(&instr->arg2, &ax);

    assert(lv && (&stack[AVM_STACKSIZE - 1] >= lv && lv > &stack[top] || lv == &retval));
    assert(t && (&stack[AVM_STACKSIZE - 1] >= t && t > &stack[top]));
    assert(i);

    avm_memcellClear(lv);
    lv->type = nil_m;
    if (t->type != table_m) {
        avm_error("Illegal use of type ", typeStringsT[t->type], " as table!");
        executionFinished = 1;
    } else {
        struct AvmMemCell *content = avm_tablegetelem(t->data.tableVal, i);

        // NEED FIXING PHASE 5 TABLEVAL NOT INSERTED
        if (content)
            avm_assign(lv, content);
        else {
            char *ts = avm_tostring(t);
            char *is = avm_tostring(i);
            avm_warning("Index not found! Table: ", ts, is);
        }
    }
}

static void execute_tablesetelem(struct Instruction *instr) {
    struct AvmMemCell *t = avm_translate_operand(&instr->arg1, NULL);
    struct AvmMemCell *i = avm_translate_operand(&instr->arg2, &ax);
    struct AvmMemCell *c = avm_translate_operand(&instr->result, &bx);

    assert(t && (&stack[AVM_STACKSIZE - 1] >= t && t > &stack[top]));
    assert(i && c);

    if (t->type != table_m) {
        avm_error("Illegal use of type ", typeStringsT[t->type], " as table!");
        executionFinished = 1;
    } else avm_tablesetelem(t->data.tableVal, i, c);
}

static void execute_nop(struct Instruction *instr) {
    executionFinished = 1;
}

execute_func_t executeFuncs[] = {
        execute_assign,
        execute_arithmetic, // add
        execute_arithmetic, // sub
        execute_arithmetic, // mul
        execute_arithmetic, // div
        execute_arithmetic, //mod
        execute_jump,
        execute_jeq,
        execute_jne,
        execute_comparison, // jle
        execute_comparison, // jge
        execute_comparison, // jlt
        execute_comparison, // jgt
        execute_call,
        execute_pusharg,
        execute_funcenter,
        execute_funcexit,
        execute_newtable,
        execute_tablegetelem,
        execute_tablesetelem,
        execute_nop
};

void execute_cycle(void) {
    if (executionFinished)
        return;

    if (pc == tables->codeSize) {
        executionFinished = 1;
        return;
    }

    assert(pc < tables->codeSize);
    struct Instruction *instr = tables->code + pc;
    assert(instr->opcode >= 0 && instr->opcode <= AVM_MAX_INSTRUCTIONS);

    unsigned int oldPC = pc;

    (*executeFuncs[instr->opcode])(instr);
    if (pc == oldPC)
        ++pc;
}