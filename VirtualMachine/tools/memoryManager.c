#include "tools.h"

void printError(char *message, char *arg1, char *arg2) {
    printf("ERROR: %s ", message);
    if (arg1 != NULL) printf("%s ", arg1);
    if (arg2 != NULL) printf("%s", arg2);
    printf("\n");
}

static void initTableBuckets(struct AvmTableBucket **p) {
    for (unsigned int i = 0; i < AVM_TABLE_HASH_SIZE; ++i)
        p[i] = NULL;
}

static void deleteTableBuckets(struct AvmTableBucket **p) {
    for (unsigned int i = 0; i < AVM_TABLE_HASH_SIZE; i++, p++) {
        for (struct AvmTableBucket *tmp = *p; tmp; tmp = tmp->next) {
            memClear(&tmp->key);
            memClear(&tmp->value);
        }

        p[i] = NULL;
    }
}

static void pushEnvValueToStack(unsigned int val) {
    stack[top].type = number_m;
    stack[top].data.numVal = val;
    decreaseTop();
}

static struct AvmMemCell *getActual(unsigned int i) {
    return &stack[top_sp + AVM_STACK_ENV_SIZE + 1 + i];
}

struct AvmTable *newTable(void) {
    struct AvmTable *t = malloc(sizeof(struct AvmTable));
    memset(&(*t), 0, sizeof(*t));

    t->refCounter = t->total = 0;
    initTableBuckets(t->numIndexed);
    initTableBuckets(t->strIndexed);

    return t;
}

struct AvmMemCell *tableGetElem(struct AvmTable *t, struct AvmMemCell *i) {
    if (i->type == string_m) {
        int indexOfTable = strlen(i->data.strVal) % AVM_TABLE_HASH_SIZE;
        if (t->strIndexed[indexOfTable] == NULL)
            return NULL;

        struct AvmTableBucket *tmp = t->strIndexed[indexOfTable];
        while (tmp != NULL) {
            if (strcmp(tmp->key.data.strVal, i->data.strVal) == 0)
                return &tmp->value;
            tmp = tmp->next;
        }

        return NULL;
    }

    if (i->type == number_m) {
        int indexOfTable = (int) i->data.numVal % AVM_TABLE_HASH_SIZE;
        if (t->numIndexed[indexOfTable] == NULL)
            return NULL;

        struct AvmTableBucket *tmp = t->numIndexed[indexOfTable];
        while (tmp != NULL) {
            if (tmp->key.data.numVal == i->data.numVal)
                return &tmp->value;

            tmp = tmp->next;
        }

        return NULL;
    }

    return NULL;
}

void tableSetElem(struct AvmTable *t, struct AvmMemCell *i, struct AvmMemCell *v) {
    int index;
    struct AvmMemCell *gotElem = tableGetElem(t, i);
    if (i->type == string_m) {
        index = (int) strlen(i->data.strVal) % AVM_TABLE_HASH_SIZE;
        if (gotElem != NULL) {
            assign_(gotElem, v);
            return;
        }

        if (t->strIndexed[index] == NULL) {
            t->strIndexed[index] = malloc(sizeof(struct AvmTableBucket));

            struct AvmTableBucket *new = malloc(sizeof(struct AvmTableBucket));
            new->key = *i;

            if (v->type == table_m)
                tableIncRefCounter(v->data.tableVal);

            new->value = *v;

            t->strIndexed[index] = new;

            t->strIndexed[index]->next = NULL;
            t->total++;
        } else {
            struct AvmTableBucket *tmp = t->strIndexed[index];
            while (tmp->next != NULL) { tmp = tmp->next; }

            struct AvmTableBucket *newBucket = malloc(sizeof(struct AvmTableBucket));
            newBucket->key = *i;

            if (v->type == table_m)
                tableIncRefCounter(v->data.tableVal);

            newBucket->value = *v;
            tmp->next = newBucket;
            newBucket->next = NULL;
            t->total++;
        }
        return;
    }

    if (i->type == number_m) {
        index = (int) i->data.numVal % AVM_TABLE_HASH_SIZE;

        if (gotElem != NULL) {
            assign_(gotElem, v);
            return;
        }

        if (t->numIndexed[index] == NULL) {
            t->numIndexed[index] = malloc(sizeof(struct AvmTableBucket));

            struct AvmTableBucket *new = malloc(sizeof(struct AvmTableBucket));
            new->key.data.numVal = i->data.numVal;

            if (v->type == table_m)
                tableIncRefCounter(v->data.tableVal);

            new->value = *v;

            t->numIndexed[index] = new;

            t->numIndexed[index]->next = NULL;
            t->total++;
        } else {
            struct AvmTableBucket *tmp = t->numIndexed[index];
            while (tmp->next != NULL)
                tmp = tmp->next;

            struct AvmTableBucket *new = malloc(sizeof(struct AvmTableBucket));
            new->key = *i;

            if (v->type == table_m)
                tableIncRefCounter(v->data.tableVal);

            new->value = *v;
            tmp->next = new;
            new->next = NULL;
            t->total++;
        }
    }
}

void tableIncRefCounter(struct AvmTable *t) {
    t->refCounter++;
}

static void memClearString(struct AvmMemCell *m) {
    free(m->data.strVal);
}

static void memClearTable(struct AvmMemCell *m) {
    --m->data.tableVal->refCounter;

    if (m->data.tableVal->refCounter == 0) {
        deleteTableBuckets(m->data.tableVal->strIndexed);
        deleteTableBuckets(m->data.tableVal->numIndexed);
    }
}


void memClear(struct AvmMemCell *m) {
    if (m->type == undef_m)
        return;

    if(m->type == table_m)
        memClearTable(m);
    else if(m->type == string_m)
        memClearString(m);

    m->type = undef_m;
}

void assign_(struct AvmMemCell *lv, struct AvmMemCell *rv) {

    if (lv == rv // Same var
        || (lv->type == table_m && rv->type == table_m && lv->data.tableVal == rv->data.tableVal) // Same table
        ) return;


    memClear(lv);
    memcpy(lv, rv, sizeof(struct AvmMemCell));


    if (lv->type == string_m)
        lv->data.strVal = strdup(rv->data.strVal);
    else if (lv->type == table_m)
        tableIncRefCounter(lv->data.tableVal);
}

static char *numberToString(struct AvmMemCell *m) {
    char *convertedStr = malloc(128 * sizeof(char));
    double decPart = modf(m->data.numVal, &decPart);

    if (decPart == (double) 0)
        sprintf(convertedStr, "%d", (int) m->data.numVal);
    else
        sprintf(convertedStr, "%lf", m->data.numVal);

    return convertedStr;
}

static char *stringToString(struct AvmMemCell *m) {
    return m->data.strVal;
}

static char *boolToString(struct AvmMemCell *m) {
    if (m->data.boolVal == '0')
        return "false";
    else
        return "true";
}

static void printTable(struct AvmMemCell *t) {
    printf("[\n");

    for (int k = 0; k < AVM_TABLE_HASH_SIZE; k++) {
        struct AvmMemCell *mc = t;
        struct AvmTableBucket *tmp = mc->data.tableVal->strIndexed[k];

        while (tmp != NULL) {
            printf("\t{ ");

            if (tmp->key.type == string_m)
                printf("\"%s\" : ", tmp->key.data.strVal);
            else if (tmp->key.type == number_m)
                printf("%d : ", (int) tmp->key.data.numVal);

            printf("%s", toString(&tmp->value));
            printf(" } ,\n");

            tmp = tmp->next;
        }

        tmp = mc->data.tableVal->numIndexed[k];
        while (tmp != NULL) {
            printf("\t{ ");

            if (tmp->key.type == string_m)
                printf("\"%s\" : ", tmp->key.data.strVal);
            else if (tmp->key.type == number_m)
                printf("%d : ", (int) tmp->key.data.numVal);

            printf("%s", toString(&tmp->value));
            printf(" } ,\n");

            tmp = tmp->next;
        }
    }

    printf("\n]\n");
}

static char *table_tostring(struct AvmMemCell *m) {
    printTable(m);
    return "";
}

static char *userfunc_tostring(struct AvmMemCell *m) {
    return "[Function]";
}

static char *libfunc_tostring(struct AvmMemCell *m) {
    return "[Library Function]";
}

static char *nil_tostring(struct AvmMemCell *m) {
    return "nil";
}

static char *undef_tostring(struct AvmMemCell *m) {
    return "undefined";
}

toStringFunction tostringFuncs[] = {
        numberToString,
        stringToString,
        boolToString,
        table_tostring,
        userfunc_tostring,
        libfunc_tostring,
        nil_tostring,
        undef_tostring
};

char *toString(struct AvmMemCell *m) {
    return (*tostringFuncs[m->type])(m);
}

static void libfuncPrint() {
    for (unsigned int i = 0; i < totalActuals_(); ++i) {
        char *s = toString(getActual(i));

        for (int j = 0; j < strlen(s); j++) {
            // If \ first then always next pos.
            if (s[j] == '\\' && s[j + 1] == 'n') {
                printf("\n");
                j++;
            } else
                printf("%c", s[j]);
        }
    }
}

static void libfuncInput() {
    char *read = calloc(1, sizeof(char *));
    scanf("%s", read);
    returnValue.type = string_m;
    returnValue.data.strVal = read;
}

static void libfuncObjectmemberkeys(void) {
    unsigned int n = totalActuals_();
    if (n != 1) {
        printError("objecttotalmemberskeys: argument is invalid.", NULL, NULL);
        running = 0;
        return;
    }

    if (getActual(0)->type != table_m) {
        printError("objecttotalmemberskeys: gets only type table arguments.", NULL, NULL);
        running = 0;
        return;
    }

    if (getActual(0)->data.tableVal != NULL) {
        memClear(&returnValue);
        returnValue.type = table_m;
        returnValue.data.tableVal = getActual(0)->data.tableVal;
    }
}

static void libfuncObjecttotalmembers(void) {
    unsigned int n = totalActuals_();
    if (n != 1) {
        printError("objecttotalmembers: argument is invalid.", NULL, NULL);
        running = 0;
        return;
    }

    if (getActual(0)->type != table_m) {
        printError("objecttotalmembers: gets only type table arguments.", NULL, NULL);
        running = 0;
        return;
    }

    if (getActual(0)->data.tableVal != NULL) {
        memClear(&returnValue);
        returnValue.type = number_m;
        returnValue.data.numVal = getActual(0)->data.tableVal->total;
    }
}

static void libfuncObjectcopy(void) {
    // TODO
}

static void libfuncTotalarguments(void) {
    unsigned int p_top_sp = getEnvValue(top_sp + AVM_SAVED_TOPSP_OFFSET);
    memClear(&returnValue);

    if (!p_top_sp) {
        printError("totalarguments: cannot be called outside of a function!", NULL, NULL);
        returnValue.type = nil_m;
        running = 0;
        return;
    }

    returnValue.type = number_m;
    returnValue.data.numVal = getEnvValue(p_top_sp + AVM_NUM_ACTUALS_OFFSET);
}

static void libfuncArgument(void) {
    unsigned int n = totalActuals_();
    if (n != 1) {
        printError("argument: argument is invalid.", NULL, NULL);
        running = 0;
        return;
    }

    if (getActual(0)->type != number_m) {
        printError("argument: gets only type number arguments.", NULL, NULL);
        running = 0;
        return;
    }

    memClear(&returnValue);
    memcpy(&returnValue, getActual(0), sizeof(struct AvmMemCell));
}

static void libfuncTypeof(void) {
    char *typeStrings[] = {"number", "string", "bool", "table", "userfunc", "libfunc", "nil", "undef"};
    unsigned int n = totalActuals_();
    if (n != 1) {
        printError("typeof: invalid arguments.", NULL, NULL);
        running = 0;
        return;
    }

    memClear(&returnValue);
    returnValue.type = string_m;
    returnValue.data.strVal = strdup(typeStrings[getActual(0)->type]);
}

static void libfuncStrtonum(void) {
    unsigned int n = totalActuals_();
    if (n != 1) {
        printError("strtonum: argument is invalid.", NULL, NULL);
        running = 0;
        return;
    }

    if (getActual(0)->type != string_m) {
        printError("strtonum: gets only type string arguments.", NULL, NULL);
        running = 0;
        return;
    }

    memClear(&returnValue);
    char *numPure = strtok(getActual(0)->data.strVal, "\"");
    returnValue.type = number_m;
    returnValue.data.numVal = atoi(numPure);
}

static void libfuncSqrt(void) {
    unsigned int n = totalActuals_();
    if (n != 1) {
        printError("sqrt: argument is invalid.", NULL, NULL);
        running = 0;
        return;
    }

    if (getActual(0)->type != number_m) {
        printError("sqrt: gets only type number arguments.", NULL, NULL);
        running = 0;
        return;
    }

    memClear(&returnValue);
    returnValue.type = number_m;
    returnValue.data.numVal = sqrt(getActual(0)->data.numVal);
}

void libfuncCos(void) {
    unsigned int n = totalActuals_();
    if (n != 1) {
        printError("cos: argument is invalid.", NULL, NULL);
        running = 0;
        return;
    }

    if (getActual(0)->type != number_m) {
        printError("cos: gets only type number arguments.", NULL, NULL);
        running = 0;
        return;
    }

    memClear(&returnValue);
    returnValue.type = number_m;
    returnValue.data.numVal = cos(getActual(0)->data.numVal);
}

void libfuncSin(void) {
    unsigned int n = totalActuals_();
    if (n != 1) {
        printError("sin: argument is invalid.", NULL, NULL);
        running = 0;
        return;
    }

    if (getActual(0)->type != number_m) {
        printError("sin: gets only type number arguments.", NULL, NULL);
        running = 0;
        return;
    }

    memClear(&returnValue);
    returnValue.type = number_m;
    returnValue.data.numVal = sin(getActual(0)->data.numVal);
}

static libraryFunction getLibFunc(char *id) {
    if (!strcmp("print", id))
        return libfuncPrint;
    else if (!strcmp("input", id))
        return libfuncInput;
    else if (!strcmp("objectmemberkeys", id))
        return libfuncObjectmemberkeys;
    else if (!strcmp("objecttotalmembers", id))
        return libfuncObjecttotalmembers;
    else if (!strcmp("objectcopy", id))
        return libfuncObjectcopy;
    else if (!strcmp("totalarguments", id))
        return libfuncTotalarguments;
    else if (!strcmp("argument", id))
        return libfuncArgument;
    else if (!strcmp("typeof", id))
        return libfuncTypeof;
    else if (!strcmp("strtonum", id))
        return libfuncStrtonum;
    else if (!strcmp("sqrt", id))
        return libfuncSqrt;
    else if (!strcmp("cos", id))
        return libfuncCos;
    else if (!strcmp("sin", id))
        return libfuncSin;
    return 0;
}

void callLibraryFunction(char *name) {
    libraryFunction func = getLibFunc(name);
    if (!func) {
        printError("library function:", name, "is not supported.");
        running = 0;
        return;
    }

    top_sp = top;
    totalActuals = 0;
    (*func)();

    if (!running)
        functionExit(NULL);
}

void callSaveEnvironment() {
    pushEnvValueToStack(totalActuals);
    pushEnvValueToStack(pc + 1);
    pushEnvValueToStack(top + totalActuals + 2);
    pushEnvValueToStack(top_sp);
}

void decreaseTop() {
    if (top) {
        top--;
        return;
    }

    printError("Stack overflow", NULL, NULL);
    running = 0;
    exit(0);
}

struct UserFunc *getFuncInfo(unsigned int address) {
    return &binaryData->userFuncsTable[address];
}

unsigned int getEnvValue(unsigned int i) {
    return (unsigned int) stack[i].data.numVal;
}

unsigned int totalActuals_(void) {
    return getEnvValue(top_sp + AVM_NUM_ACTUALS_OFFSET);
}

static double doAdd(double x, double y) {
    return x + y;
}

static double doSub(double x, double y) {
    return x - y;
}

static double doMul(double x, double y) {
    return x * y;
}

static double doDiv(double x, double y) {
    if (y != 0)
        return x / y;

    printError("Division by 0 is not allowed.", NULL, NULL);
    running = 0;
    return 0;
}

static double doMod(double x, double y) {
    if (y != 0)
        return (unsigned int) x % (unsigned int) y;

    printError("Division by 0 is not allowed.", NULL, NULL);
    running = 0;
    return 0;
}

arithmeticFunction arithmeticFuncs[] = { doAdd, doSub, doMul, doDiv, doMod };
void doArithmeticInstr(struct Instruction *instr) {
    struct AvmMemCell *lv = parseOperand(&instr->result, NULL);
    struct AvmMemCell *rv1 = parseOperand(&instr->arg1, &amc1);
    struct AvmMemCell *rv2 = parseOperand(&instr->arg2, &amc2);

    if (rv1->type != number_m || rv2->type != number_m) {
        printError("Arithmetics can only be applied to numbers.", NULL, NULL);
        running = 0;
        return;
    }

    memClear(lv);
    lv->type = number_m;
    lv->data.numVal = (arithmeticFuncs[instr->opcode - add_v])(rv1->data.numVal, rv2->data.numVal);
}

static unsigned char doJLE(double x, double y) {
    return x <= y;
}

static unsigned char doJGE(double x, double y) {
    return x >= y;
}

static unsigned char doJLT(double x, double y) {
    return x < y;
}

static unsigned char doJGT(double x, double y) {
    return x > y;
}

comparisonFunction comparisonFuncs[] = { doJLE, doJGE, doJLT, doJGT };
void doComparisonInstr(struct Instruction *instr) {
    struct AvmMemCell *rv1 = parseOperand(&instr->arg1, &amc1);
    struct AvmMemCell *rv2 = parseOperand(&instr->arg2, &amc2);

    if (rv1->type != number_m || rv2->type != number_m) {
        printError("Comparison can only be applied to numbers", NULL, NULL);
        running = 0;
        return;
    }

    unsigned char result = (*comparisonFuncs[instr->opcode - jle_v])(rv1->data.numVal, rv2->data.numVal);
    if (!running && result)
        pc = instr->result.val;
}

static unsigned char numberToBool(struct AvmMemCell *m) {
    return m->data.numVal != 0;
}

static unsigned char stringToBool(struct AvmMemCell *m) {
    return m->data.strVal[0] != 0;
}

static unsigned char boolToBool(struct AvmMemCell *m) {
    return m->data.boolVal;
}

static unsigned char tableToBool(struct AvmMemCell *m) {
    return 1;
}

static unsigned char userFuncToBool(struct AvmMemCell *m) {
    return 1;
}

static unsigned char libFuncToBool(struct AvmMemCell *m) {
    return 1;
}

static unsigned char nilToBool(struct AvmMemCell *m) {
    return 0;
}

static unsigned char undefinedToBool(struct AvmMemCell *m) {
    return 0;
}

toBoolFunction toboolFuncs[] = { numberToBool, stringToBool, boolToBool, tableToBool, userFuncToBool,
                                 libFuncToBool, nilToBool, undefinedToBool };
unsigned char toBool(struct AvmMemCell *m) {
    return (*toboolFuncs[m->type])(m);
}

struct AvmMemCell *parseOperand(struct VmArg *arg, struct AvmMemCell *reg) {
    if (arg->type == invalid || arg->type == nop_a)
        return reg;

    switch (arg->type) {
        case global_a: {
            int index = AVM_STACK_SIZE - 1 - arg->val;
            printf("INDEX 1: %d <= %d\n\n", index, arg->val);
            return &stack[index];
        }
        case local_a:
//            printf("INDEX 2: %d\n\n", top_sp - arg->val);
            return &stack[top_sp - arg->val];
        case formal_a:
//            printf("INDEX 3: %d\n\n", top_sp + AVM_STACK_ENV_SIZE + 1 + arg->val);
            return &stack[top_sp + AVM_STACK_ENV_SIZE + 1 + arg->val];
        case retval_a:
            return &returnValue;
        case number_a:
            reg->type = number_m;
            reg->data.numVal = binaryData->numConstsTable[arg->val];
            return reg;
        case string_a:
            reg->type = string_m;
            reg->data.strVal = strdup(binaryData->strConstsTable[arg->val]);
            return reg;
        case bool_a:
            reg->type = bool_m;
            reg->data.boolVal = arg->val;
            return reg;
        case nil_a:
            reg->type = nil_m;
            return reg;
        case userfunc_a:
            reg->type = userfunc_m;
            reg->data.funcVal = getFuncInfo(arg->val)->address;
            return reg;
        case libfunc_a:
            reg->type = libfunc_m;
            reg->data.libfuncVal = binaryData->libFuncsTable[arg->val];
            return reg;
        default: assert(0);
    }
}

void initialize(void) {
    for (unsigned int i = 0; i < AVM_STACK_SIZE; i++) {
        memset(&(stack[i]), 0, sizeof(stack[i]));
        stack[i].type = undef_m;
    }
}
