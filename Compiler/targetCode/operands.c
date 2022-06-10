#include "targetCode.h"

static unsigned int currentProcessedQuad(void);

struct IncompleteJump *ij_head = NULL;
unsigned int ij_total = 0;

unsigned int ij_size = 0;
unsigned int currQuadNo = 0;

#define EXPAND_SIZE_IJ        1024
#define CURR_SIZE_IJ        (ij_total * sizeof(struct IncompleteJump))
#define NEW_SIZE_IJ        (EXPAND_SIZE_IJ * sizeof(struct IncompleteJump) + CURR_SIZE_IJ)

static void make_operand(struct expr *e, struct VmArg *arg) {
    if (e == NULL) {
        arg->val = -1;
        arg->type = -1;
        return;
    }

    switch (e->type) {
        case var_e:
        case tableitem_e:
        case arithexpr_e:
        case boolexpr_e:
        case assignexpr_e:
        case newtable_e:
            if(e->sym->scopeSpace == PROGRAM_VAR)
                arg->type = global_a;
            else if(e->sym->scopeSpace == FUNC_LOCAL)
                arg->type = local_a;
            else if(e->sym->scopeSpace == FORMAL_ARG)
                arg->type = formal_a;
            break;
        case constbool_e:
            if (e->boolConst == '0')
                arg->val = 0;
            else
                arg->val = 1;

            arg->type = bool_a;
            break;
        case conststring_e:
            arg->val = insertString(e->strConst);
            arg->type = string_a;
            break;
        case constnum_e:
            arg->val = insertNumber(e->numConst);
            arg->type = number_a;
            break;
        case nil_e: arg->type = nil_a; break;
        case programfunc_e:
            arg->type = userfunc_a;
            arg->val = insertUserFunc(e->sym->value.funcVal->taddress, e->sym->value.funcVal->totallocals,
                                      e->sym->value.funcVal->totalargs, (char *) e->sym->value.funcVal->name);
            break;
        case libraryfunc_e:
            arg->type = libfunc_a;
            arg->val = insertLibraryDunc((char *) e->sym->value.funcVal->name);
            break;
        default: assert(0);
    }
}

static void make_retvalOperand(struct VmArg *arg) {
    arg->type = retval_a;
}

static void EXPANDER_IJ(void) {
    assert(ij_size == ij_total);
    struct IncompleteJump *new = malloc(NEW_SIZE_IJ);
    if (ij_head) {
        memcpy(new, ij_head, CURR_SIZE_IJ);
        free(ij_head);
    }

    ij_head = new;
    ij_size += EXPAND_SIZE_IJ;
}

static void add_incomplete_jump(unsigned int instrNo, unsigned int iaddress) {
    if (ij_total == ij_size)
        EXPANDER_IJ();

    struct IncompleteJump *p = ij_head + ij_total++;

    p->instrNo = instrNo;
    p->iaddress = iaddress;
}

static void patchInstrLabel(unsigned int instrNo, unsigned int taddress) {
    instructions[instrNo].result.val = taddress;
}

static void patchIncompleteJumps(unsigned int totalQuads) {
    for (int i = 0; i < ij_total; i++) {
        if (ij_head[i].iaddress == totalQuads)
            patchInstrLabel(ij_head[i].instrNo, totalInstructions);
        else
            patchInstrLabel(ij_head[i].instrNo, quads[ij_head[i].iaddress].tAddress);
    }
}

static void reset_operand(struct VmArg *arg) {
    arg->val = -1;
}

static void setArgsNull(struct Instruction *t) {
    t->arg1.val = -1;
    t->arg1.type = 11;
    t->arg2.val = -1;
    t->arg2.type = 11;
    t->result.val = -1;
    t->result.type = 11;
}

static void generate(enum VmOpcode op, struct Quad *quadInput) {
    struct Instruction t;
    setArgsNull(&t);
    t.opcode = op;
    t.srcLine = nextInstructionLabel();

    make_operand(quadInput->arg1, &t.arg1);
    make_operand(quadInput->arg2, &t.arg2);
    make_operand(quadInput->result, &t.result);
    quadInput->tAddress = nextInstructionLabel();

    emitInstr(t);
}

static void generate_ADD(struct Quad *quadInput) {
    generate(add_v, quadInput);
}

static void generate_SUB(struct Quad *quadInput) {
    generate(sub_v, quadInput);
}

static void generate_MUL(struct Quad *quadInput) {
    generate(mul_v, quadInput);
}

static void generate_DIV(struct Quad *quadInput) {
    generate(div_v, quadInput);
}

static void generate_MOD(struct Quad *quadInput) {
    generate(mod_v, quadInput);
}

static void generate_NEWTABLE(struct Quad *quadInput) {
    generate(newtable_v, quadInput);
}

static void generate_TABLEGETELM(struct Quad *quadInput) {
    generate(tablegetelem_v, quadInput);
}

static void generate_TABLESETELEM(struct Quad *quadInput) {
    generate(tablesetelem_v, quadInput);
}

static void generate_ASSIGN(struct Quad *quadInput) {
    generate(assign_v, quadInput);
}

static void generate_NOP(struct Quad *quadInput) {
    struct Instruction t;
    setArgsNull(&t);
    t.opcode = nop_v;
    t.srcLine = nextInstructionLabel();
    quadInput->tAddress = nextInstructionLabel();

    emitInstr(t);
}

static void generate_relational(enum VmOpcode op, struct Quad *quadInput) {
    struct Instruction t;
    setArgsNull(&t);
    t.opcode = op;
    t.srcLine = nextInstructionLabel();

    make_operand(quadInput->arg1, &t.arg1);
    make_operand(quadInput->arg2, &t.arg2);

    t.result.type = label_a;

    if ((int) quadInput->result->numConst < currentProcessedQuad())
        t.result.val = quads[(int) quadInput->result->numConst].tAddress;
    else
        add_incomplete_jump(nextInstructionLabel(), (unsigned int)quadInput->result->numConst);

    quadInput->tAddress = nextInstructionLabel();
    emitInstr(t);
}

static void generate_JUMP(struct Quad *quadInput) {
    generate_relational(jump_v, quadInput);
}

static void generate_IF_EQ(struct Quad *quadInput) {
    generate_relational(jeq_v, quadInput);
}

static void generate_IF_NOTEQ(struct Quad *quadInput) {
    generate_relational(jne_v, quadInput);
}

static void generate_IF_GREATER(struct Quad *quadInput) {
    generate_relational(jgt_v, quadInput);
}

static void generate_IF_GREATEREQ(struct Quad *quadInput) {
    generate_relational(jge_v, quadInput);
}

static void generate_IF_LESS(struct Quad *quadInput) {
    generate_relational(jlt_v, quadInput);
}

static void generate_IF_LESSEQ(struct Quad *quadInput) {
    generate_relational(jle_v, quadInput);
}

static void generate_PARAM(struct Quad *quadInput) {
    quadInput->tAddress = nextInstructionLabel();

    struct Instruction t;
    setArgsNull(&t);
    t.opcode = pusharg_v;
    t.srcLine = nextInstructionLabel();

    make_operand(quadInput->result, &t.result);
    emitInstr(t);
}

static void generate_CALL(struct Quad *quadInput) {
    quadInput->tAddress = nextInstructionLabel();

    struct Instruction t;
    setArgsNull(&t);
    t.opcode = call_v;
    t.srcLine = nextInstructionLabel();

    make_operand(quadInput->result, &t.result);
    emitInstr(t);
}

static void generate_GETRETVAL(struct Quad *quadInput) {
    quadInput->tAddress = nextInstructionLabel();

    struct Instruction t;
    setArgsNull(&t);
    t.opcode = assign_v;
    t.srcLine = nextInstructionLabel();

    make_operand(quadInput->result, &t.result);
    make_retvalOperand(&t.arg1);
    emitInstr(t);
}

static void generate_FUNCSTART(struct Quad *quadInput) {
    quadInput->tAddress = nextInstructionLabel();

    struct SymbolTable *f = quadInput->result->sym;
    f->value.funcVal->taddress = nextInstructionLabel();

    pushFuncStackTarget(f);

    struct Instruction t;
    setArgsNull(&t);
    t.opcode = funcenter_v;
    t.srcLine = nextInstructionLabel();

    make_operand(quadInput->result, &t.result);
    emitInstr(t);

}

static void generate_RETURN(struct Quad *quadInput) {
    quadInput->tAddress = nextInstructionLabel();

    struct Instruction t;
    setArgsNull(&t);
    t.opcode = assign_v;
    t.srcLine = nextInstructionLabel();

    make_retvalOperand(&t.result);
    make_operand(quadInput->result, &t.arg1);
    emitInstr(t);

    struct SymbolTable *f = topFuncStackTarget();
    appendFuncStackTarget(f, nextInstructionLabel());
    assert(f->value.funcVal->returnList);

    setArgsNull(&t);
    t.srcLine = nextInstructionLabel();
    t.opcode = jump_v;
    reset_operand(&t.arg1);
    reset_operand(&t.arg2);
    t.result.type = label_a;
    emitInstr(t);
}

static void generate_FUNCEND(struct Quad *quadInput) {
    struct SymbolTable *f = popFuncStackTarget();

    //Back patch return list
    struct ReturnList *rl = f->value.funcVal->returnList;
    while (rl != NULL) {
        patchInstrLabel(rl->instrLabel, nextInstructionLabel());
        rl = rl->next;
    }

    quadInput->tAddress = nextInstructionLabel();
    struct Instruction t;
    setArgsNull(&t);
    t.opcode = funcexit_v;
    t.srcLine = nextInstructionLabel();

    make_operand(quadInput->result, &t.result);
    emitInstr(t);
}

unsigned int nextInstructionLabel(void) {
    return totalInstructions;
}

static unsigned int currentProcessedQuad(void) {
    return currQuadNo;
}

generatorFunc generators[] = {
        generate_ASSIGN,
        generate_ADD,
        generate_SUB,
        generate_MUL,
        generate_DIV,
        generate_MOD,
        generate_JUMP,
        generate_IF_EQ,
        generate_IF_NOTEQ,
        generate_IF_LESSEQ,
        generate_IF_GREATEREQ,
        generate_IF_LESS,
        generate_IF_GREATER,
        generate_CALL,
        generate_PARAM,
        generate_FUNCSTART,
        generate_FUNCEND,
        generate_NEWTABLE,
        generate_TABLEGETELM,
        generate_TABLESETELEM,
        generate_GETRETVAL,
        generate_NOP,
        generate_RETURN
};

void generateTcode(unsigned int totalQuads) {
    for (int i = 0; i < totalQuads; ++i) {
        currQuadNo = i;
        (*generators[quads[i].op])(quads + i);
    }

    patchIncompleteJumps(totalQuads);
}
