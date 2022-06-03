#include "tools.h"

int innerTablePrint = 0;


static void avm_tableBucketsInit(struct AvmTableBucket** p){
    for (unsigned int i = 0; i < AVM_TABLE_HASHSIZE; ++i)
        p[i] = NULL;
}

static void avm_tableBucketsDestroy(struct AvmTableBucket** p){
    struct AvmTableBucket* b;

    for(unsigned int i = 0; i<AVM_TABLE_HASHSIZE; ++i, ++p){

        for (b = *p; b; b = b->next) {
            struct AvmTableBucket *del = b;
            avm_memcellClear(&del->key);
            avm_memcellClear(&del->value);
        }

        p[i] = NULL;
    }

}

static void avm_tableDestroy(struct AvmTable* t){
    avm_tableBucketsDestroy(t->strIndexed);
    avm_tableBucketsDestroy(t->numIndexed);
}

static void avm_push_envValue(unsigned int val) {
    stack[top].type			= number_m;
    stack[top].data.numVal	= val;
    avm_dec_top();
}

static struct AvmMemCell *avm_getactual(unsigned int i) {
    assert(i < avm_totalactuals());
    return &stack[topsp + AVM_STACKENV_SIZE + 1 + i];
}


struct AvmTable *avm_tableNew(void){
    struct AvmTable* t = malloc(sizeof(struct AvmTable));
    memset(&(*t), 0, sizeof(*t));

    t->refCounter = t->total = 0;
    avm_tableBucketsInit(t->numIndexed);
    avm_tableBucketsInit(t->strIndexed);

    return t;
}

struct AvmMemCell *avm_tablegetelem(struct AvmTable *table, struct AvmMemCell *index) {
    assert(table);

    if (index->type == string_m) {
        int indexOfTable = (int) strlen(index->data.strVal) % AVM_TABLE_HASHSIZE;

        if (table->strIndexed[indexOfTable] == NULL)
            return NULL;

        struct AvmTableBucket *tmp = table->strIndexed[indexOfTable];
        while (tmp != NULL) {
            if (strcmp(tmp->key.data.strVal, index->data.strVal) == 0)
                return &tmp->value;
            tmp = tmp->next;
        }
    } else if (index->type == number_m) {
        int indexOfTable = (int) index->data.numVal % AVM_TABLE_HASHSIZE;

        if (table->numIndexed[indexOfTable] == NULL)
            return NULL;

        struct AvmTableBucket *tmp = table->numIndexed[indexOfTable];
        while (tmp != NULL) {
            if (tmp->key.data.numVal == index->data.numVal)
                return &tmp->value;

            tmp = tmp->next;
        }
    }

    return NULL;
}

void avm_tablesetelem(struct AvmTable *table, struct AvmMemCell *index, struct AvmMemCell *value) {
    assert(table);

    int indexOfTable;
    struct AvmMemCell *gotit = avm_tablegetelem(table, index);
    if (index->type == string_m) {
        indexOfTable = (int) strlen(index->data.strVal) % AVM_TABLE_HASHSIZE;
        if (gotit != NULL) {
            avm_assign(gotit, value);
            return;
        }

        if (table->strIndexed[indexOfTable] == NULL) {
            table->strIndexed[indexOfTable] = malloc(sizeof(struct AvmTableBucket));

            struct AvmTableBucket *newBucket = malloc(sizeof(struct AvmTableBucket));
            newBucket->key = *index;

            if (value->type == table_m)
                avm_tableIncRefCounter(value->data.tableVal);

            newBucket->value = *value;

            table->strIndexed[indexOfTable] = newBucket;

            table->strIndexed[indexOfTable]->next = NULL;
            table->total++;

            return;
        }

        struct AvmTableBucket *tmp = table->strIndexed[indexOfTable];
        while (tmp->next != NULL) { tmp = tmp->next; }

        struct AvmTableBucket *newBucket = malloc(sizeof(struct AvmTableBucket));
        newBucket->key = *index;

        if (value->type == table_m)
            avm_tableIncRefCounter(value->data.tableVal);

        newBucket->value = *value;
        tmp->next = newBucket;
        newBucket->next = NULL;
        table->total++;

    } else if (index->type == number_m) {
        indexOfTable = (int) index->data.numVal % AVM_TABLE_HASHSIZE;

        if (gotit != NULL) {
            avm_assign(gotit, value);
            return;
        }

        if (table->numIndexed[indexOfTable] == NULL) {
            table->numIndexed[indexOfTable] = malloc(sizeof(struct AvmTableBucket));

            struct AvmTableBucket *newBucket = malloc(sizeof(struct AvmTableBucket));
            newBucket->key.data.numVal = index->data.numVal;

            if (value->type == table_m)
                avm_tableIncRefCounter(value->data.tableVal);

            newBucket->value = *value;

            table->numIndexed[indexOfTable] = newBucket;

            table->numIndexed[indexOfTable]->next = NULL;
            table->total++;

            return;
        }

        struct AvmTableBucket *tmp = table->numIndexed[indexOfTable];
        while (tmp->next != NULL)
            tmp = tmp->next;

        struct AvmTableBucket *newBucket = malloc(sizeof(struct AvmTableBucket));
        newBucket->key = *index;

        if (value->type == table_m)
            avm_tableIncRefCounter(value->data.tableVal);

        newBucket->value = *value;
        tmp->next = newBucket;
        newBucket->next = NULL;
        table->total++;
    }
}

void avm_tableIncRefCounter(struct AvmTable *t) {
    (t->refCounter)++;
}

static void avm_tableDecRefCounter(struct AvmTable* t){
    assert(t->refCounter > 0);
    if(! --t->refCounter)
        avm_tableDestroy(t);
}

static void memclear_string(struct AvmMemCell* m){
    assert(m->data.strVal);
    free(m->data.strVal);
}

static void memclear_table(struct AvmMemCell* m){
    assert(m->data.tableVal);
    avm_tableDecRefCounter(m->data.tableVal);
}

// If 0 it will to clear in memory
static memclear_func_t memclearFuncs[] = {
        0,					//number
        memclear_string,
        0,					//bool
        memclear_table,
        0,					//userfunc
        0,					//libfunc
        0,					//nil
        0					//undef
};

void avm_memcellClear(struct AvmMemCell* m) {
    if(m->type == undef_m)
        return;

    memclear_func_t f = memclearFuncs[m->type];
    if (f)
        (*f)(m);
    m->type = undef_m;
}

void avm_assign(struct AvmMemCell *lv, struct AvmMemCell *rv) {
    if (lv == rv) return;

    if (lv->type == table_m
        && rv->type == table_m
        && lv->data.tableVal == rv->data.tableVal
        ) return;

    if (rv->type == undef_m)
        avm_warning("assigning from 'undef' content!", NULL, NULL);

    avm_memcellClear(lv);
    memcpy(lv, rv, sizeof(struct AvmMemCell));

    if (lv->type == string_m)
        lv->data.strVal = strdup(rv->data.strVal);
    else if (lv->type == table_m)
        avm_tableIncRefCounter(lv->data.tableVal);
}

void avm_warning(char *msg1, char *id, char *msg2) {
    printf("WARN: %s", msg1);
    if (id != NULL) printf("%s ", id);
    if (msg2 != NULL) printf("%s", msg2);
    printf("\n");
}

void avm_error(char *msg1, char *id, char *msg2) {
    printf("ERROR: %s ", msg1);
    if (id != NULL) printf("%s ", id);
    if (msg2 != NULL) printf("%s", msg2);
    printf("\n");
}

static char *number_tostring(struct AvmMemCell *m) {
    assert(m->type == number_m);

    char *convertedStr = malloc(128 * sizeof(char));
    double decPart, zero = 0;

    decPart = modf(m->data.numVal, &decPart);

    if (zero == decPart)
        sprintf(convertedStr, "%d", (int) m->data.numVal);
    else
        sprintf(convertedStr, "%lf", m->data.numVal);

    return convertedStr;
}

static char *string_tostring(struct AvmMemCell *m) {
    assert(m->type == string_m);
    return m->data.strVal;
}

static char *bool_tostring(struct AvmMemCell *m) {
    assert(m->type == bool_m);
    if (m->data.boolVal == '0')
        return "false";
    else
        return "true";
}

static void printTable(struct AvmMemCell* t){
    if (!innerTablePrint)
        printf("[\n");
    else
        printf("[ ");

    for (int k = 0; k < AVM_TABLE_HASHSIZE; k++) {
        struct AvmMemCell *memCell = t;
        struct AvmTableBucket *tableBucket = memCell->data.tableVal->strIndexed[k];

        while (tableBucket != NULL) {
            if (!innerTablePrint)
                printf("\t{ ");
            else
                printf(" { ");

            if (tableBucket->key.type == string_m)
                printf("\"%s\" : ", tableBucket->key.data.strVal);
            else if (tableBucket->key.type == number_m)
                printf("%d : ", (int) tableBucket->key.data.numVal);

            if (tableBucket->value.type == table_m)
                innerTablePrint = 1;

            printf("%s", avm_tostring(&tableBucket->value));

            if (!innerTablePrint)
                printf(" } ,\n");
            else
                printf(" } , ");

            tableBucket = tableBucket->next;
        }

        tableBucket = memCell->data.tableVal->numIndexed[k];
        while (tableBucket != NULL) {
            if (!innerTablePrint)
                printf("\t{ ");
            else
                printf(" { ");

            if (tableBucket->key.type == string_m)
                printf("\"%s\" : ", tableBucket->key.data.strVal);
            else if (tableBucket->key.type == number_m)
                printf("%d : ", (int) tableBucket->key.data.numVal);

            if (tableBucket->value.type == table_m)
                innerTablePrint = 1;

            printf("%s", avm_tostring(&tableBucket->value));

            tableBucket = tableBucket->next;

            if (!innerTablePrint)
                printf(" } ,\n");
            else
                printf(" } , ");
        }
    }

    if (!innerTablePrint)
        printf(" \t ] \n\n");
    else
        printf(" ]");

    innerTablePrint = 0;
}

static char *table_tostring(struct AvmMemCell *m) {
    assert(m->type == table_m);
    printTable(m);
    return "";
}

static char *userfunc_tostring(struct AvmMemCell *m) {
    assert(m->type == userfunc_m);
    char *convertedStr = "Func";
    return convertedStr;
}

static char *libfunc_tostring(struct AvmMemCell *m) {
    char *convertedStr = "LibFunc";
    return convertedStr;
}

static char *nil_tostring(struct AvmMemCell *m) {
    char *convertedStr = "Nil";
    return convertedStr;
}

static char *undef_tostring(struct AvmMemCell *m) {
    char *convertedStr = "Undef";
    return convertedStr;
}

tostring_func_t tostringFuncs[] = {
        number_tostring,
        string_tostring,
        bool_tostring,
        table_tostring,
        userfunc_tostring,
        libfunc_tostring,
        nil_tostring,
        undef_tostring
};

char *avm_tostring(struct AvmMemCell *m) {
    assert(m->type >= 0 && m->type <= undef_m);
    return (*tostringFuncs[m->type])(m);
}

static void libfunc_print(void) {
    unsigned int n = avm_totalactuals();
    char *s;
    for (unsigned int i = 0; i < n; ++i) {
        s = avm_tostring(avm_getactual(i));

        for (int j = 0; j < strlen(s); j++) {
            if (s[j] == '\\' && s[j + 1] == 'n') {
                printf("\n");
                j++;
            }
            else printf("%c", s[j]);
        }
    }
}

static void libfunc_input(void) {
    char* userInput = calloc (1, sizeof(char*));
    scanf("%s", userInput);
    char* strPure = strtok(userInput,"\"");

    retval.type = string_m;
    retval.data.strVal = strdup(strPure);
}

static void libfunc_objectmemberkeys(void) {
    unsigned int n = avm_totalactuals();
    if (n != 1) {
        char nName[ENOUGH_NUM(n)];
        sprintf(nName, "%d", n);
        avm_error("One argument and not(", nName, ") expected in 'libfunc objecttotalmembers' !");

        executionFinished = 1;
        return;
    }

    if (avm_getactual(0)->type != table_m) {
        avm_error("libfunc objecttotalmembers ", 0, ", gets only arguments, of type table");
        executionFinished = 1;
        return;
    }

    if (avm_getactual(0)->data.tableVal != NULL) {
        avm_memcellClear(&retval);
        retval.type = table_m;
        retval.data.tableVal = avm_getactual(0)->data.tableVal;
    }
}

static void libfunc_objecttotalmembers(void) {
    unsigned int n = avm_totalactuals();
    if (n != 1) {
        char nName[ENOUGH_NUM(n)];
        sprintf(nName, "%d", n);
        avm_error("One argument and not(", nName, ") expected in 'libfunc objecttotalmembers' !");
        executionFinished = 1;
        return;
    }

    if (avm_getactual(0)->type != table_m) {
        avm_error("libfunc objecttotalmembers ", 0, ", gets only arguments, of type table");
        executionFinished = 1;
        return;
    }

    if (avm_getactual(0)->data.tableVal != NULL) {
        avm_memcellClear(&retval);
        retval.type = number_m;
        retval.data.numVal = avm_getactual(0)->data.tableVal->total;
    }
}

static void libfunc_objectcopy(void) {
    // TODO
}

static void libfunc_totalarguments(void) {
    unsigned int p_topsp = avm_get_envvalue(topsp + AVM_SAVEDTOPSP_OFFSET);
    avm_memcellClear(&retval);
    if (!p_topsp) {
        avm_error("'totalarguments' called outside a function!", NULL, NULL);
        retval.type = nil_m;
        executionFinished = 1;
    } else {
        retval.type = number_m;
        retval.data.numVal = avm_get_envvalue(p_topsp + AVM_NUMACTUALS_OFFSET);
    }
}

static void libfunc_argument(void) {
    unsigned int n = avm_totalactuals();
    if(n != 1) {
        char nName[ENOUGH_NUM(n)];
        sprintf(nName, "%d", n);
        avm_error("One argument and not(", nName, ") expected in 'libfunc argument' !");
        executionFinished = 1;

    } else {
        if (avm_getactual(0)->type != number_m) {
            avm_error("libfunc argument ", 0, ", gets only arguments, of type number");
            executionFinished = 1;
        } else {
            avm_memcellClear(&retval);
            memcpy(&retval, avm_getactual(0), sizeof(struct AvmMemCell));
        }
    }
}

static void libfunc_typeof(void) {
    char* typeStrings[] = {"number","string","bool","table","userfunc","libfunc","nil","undef"};
    unsigned int n = avm_totalactuals();
    if(n != 1){
        char nName[ENOUGH_NUM(n)];
        sprintf(nName, "%d", n);
        avm_error( "One argument and not(", nName , ") expected in 'typeof' !" );
        executionFinished = 1;
    }else{
        avm_memcellClear(&retval);
        retval.type = string_m;
        retval.data.strVal = strdup(typeStrings[avm_getactual(0)->type]);
    }

}

static void libfunc_strtonum(void) {
    unsigned int n = avm_totalactuals();
    if (n != 1) {
        char nName[ENOUGH_NUM(n)];
        sprintf(nName, "%d", n);
        avm_error("One argument and not(", nName, ") expected in 'strtonum' !");
        executionFinished = 1;
    } else {
        if (avm_getactual(0)->type != string_m) {
            avm_error("strtonum libfunc", 0, ", gets only arguments, of type string");
            executionFinished = 1;
        } else {
            avm_memcellClear(&retval);
            char *numPure = strtok(avm_getactual(0)->data.strVal, "\"");
            retval.type = number_m;
            retval.data.numVal = atoi(numPure);
        }
    }
}

static void libfunc_sqrt(void) {
    unsigned int n = avm_totalactuals();
    if(n != 1) {
        char nName[ENOUGH_NUM(n)];
        sprintf(nName, "%d", n);
        avm_error("One argument and not(", nName, ") expected in 'sqrt' !");
        executionFinished = 1;
    } else {
        if (avm_getactual(0)->type != number_m) {
            avm_error("sqrt libfunc", 0, ", gets only arguments, of type number");
            executionFinished = 1;
        } else {
            avm_memcellClear(&retval);
            retval.type = number_m;
            retval.data.numVal = sqrt(avm_getactual(0)->data.numVal);
        }
    }
}

void libfunc_cos(void) {
    unsigned int n = avm_totalactuals();
    if (n != 1) {
        char nName[ENOUGH_NUM(n)];
        sprintf(nName, "%d", n);
        avm_error("One argument and not(", nName, ") expected in 'cos' !");
        executionFinished = 1;
    } else {
        if (avm_getactual(0)->type != number_m) {
            avm_error("cos libfunc", 0, ", gets only arguments, of type number");
            executionFinished = 1;
        } else {
            avm_memcellClear(&retval);
            retval.type = number_m;
            retval.data.numVal = cos(avm_getactual(0)->data.numVal);
        }
    }
}

void libfunc_sin(void) {
    unsigned int n = avm_totalactuals();
    if (n != 1) {
        char nName[ENOUGH_NUM(n)];
        sprintf(nName, "%d", n);
        avm_error("One argument and not(", nName, ") expected in 'sin' !");
        executionFinished = 1;
    } else {
        if (avm_getactual(0)->type != number_m) {
            avm_error("sin libfunc", 0, ", gets only arguments, of type number");
            executionFinished = 1;
        } else {
            avm_memcellClear(&retval);
            retval.type = number_m;
            retval.data.numVal = sin(avm_getactual(0)->data.numVal);
        }
    }
}

static library_func_t avm_getlibraryfunc(char *id) {
    if (strcmp("print", id) == 0)
        return libfunc_print;
    else if (strcmp("input", id) == 0)
        return libfunc_input;
    else if (strcmp("objectmemberkeys", id) == 0)
        return libfunc_objectmemberkeys;
    else if (strcmp("objecttotalmembers", id) == 0)
        return libfunc_objecttotalmembers;
    else if (strcmp("objectcopy", id) == 0)
        return libfunc_objectcopy;
    else if (strcmp("totalarguments", id) == 0)
        return libfunc_totalarguments;
    else if (strcmp("argument", id) == 0)
        return libfunc_argument;
    else if (strcmp("typeof", id) == 0)
        return libfunc_typeof;
    else if (strcmp("strtonum", id) == 0)
        return libfunc_strtonum;
    else if (strcmp("sqrt", id) == 0)
        return libfunc_sqrt;
    else if (strcmp("cos", id) == 0)
        return libfunc_cos;
    else if (strcmp("sin", id) == 0)
        return libfunc_sin;
    return 0;
}

void avm_callLibFunc(char* id) {
    library_func_t f = avm_getlibraryfunc(id);
    if (!f) {
        avm_error("Unsupported libfunc '", id, "' called!");
        executionFinished = 1;
    } else {
        topsp = top;
        totalActuals = 0;
        (*f)();

        if (!executionFinished)
            execute_funcexit(NULL);
    }
}

void avm_callSaveEnvironment() {
    avm_push_envValue(totalActuals);
    avm_push_envValue(pc + 1);
    avm_push_envValue(top + totalActuals + 2);
    avm_push_envValue(topsp);
}

void avm_dec_top() {
    if(top) {
        top--;
        return;
    }

    avm_error("Stack Overflow!\n", NULL, NULL);
    executionFinished = 1;
    exit(0);
}

struct UserFunc* avm_getfuncinfo(unsigned int address){
    assert(address >= 0 && address < tables->totalUserFuncs);
    return &tables->userFuncsTable[address];
}

unsigned int avm_get_envvalue(unsigned int i){
    assert(stack[i].type == number_m);

    unsigned int val = (unsigned int) stack[i].data.numVal;
    assert(stack[i].data.numVal == (double) val);

    return val;
}

unsigned int avm_totalactuals(void) {
    return avm_get_envvalue(topsp + AVM_NUMACTUALS_OFFSET);
}

static double add_impl(double x, double y) {
    return x + y;
}

static double sub_impl(double x, double y) {
    return x - y;
}

static double mul_impl(double x, double y) {
    return x * y;
}

static double div_impl(double x, double y) {
    if(y != 0)
        return x / y;

    avm_error("Devision by 0 is forbidden",NULL,NULL);
    executionFinished = 1;
    return -1.0;
}

static double mod_impl(double x , double y) {
    if(y != 0)
        return (unsigned int)x % (unsigned int)y;

    avm_error("Devision by 0 is forbidden",NULL,NULL);
    executionFinished = 1;
    return -1.0;
}

arithmetic_func_t arithmeticFuncs[] = {
        add_impl,
        sub_impl,
        mul_impl,
        div_impl,
        mod_impl
};

void execute_arithmetic(struct Instruction* instr) {
    struct AvmMemCell *lv = avm_translate_operand(&instr->result, NULL);
    struct AvmMemCell *rv1 = avm_translate_operand(&instr->arg1, &ax);
    struct AvmMemCell *rv2 = avm_translate_operand(&instr->arg2, &bx);

    assert(lv && (&stack[AVM_STACKSIZE - 1] >= lv && lv > &stack[top] || lv == &retval));
    assert(rv1 && rv2);

    if (rv1->type != number_m || rv2->type != number_m) {
        avm_error("Not a number in arithmetic!", NULL, NULL);
        executionFinished = 1;
    } else {
        arithmetic_func_t op = arithmeticFuncs[instr->opcode - add_v];
        avm_memcellClear(lv);
        lv->type = number_m;
        lv->data.numVal = (*op)(rv1->data.numVal, rv2->data.numVal);
    }
}

static unsigned char jle_impl(double x, double y) {
    return x <= y;
}

static unsigned char jge_impl(double x, double y) {
    return x >= y;
}

static unsigned char jlt_impl(double x, double y) {
    return x < y;
}

static unsigned char jgt_impl(double x, double y) {
    return x > y;
}

cmp_func_t 	comparisonFuncs[] = {
        jle_impl,
        jge_impl,
        jlt_impl,
        jgt_impl
};

void execute_comparison(struct Instruction* instr) {
    struct AvmMemCell* rv1 = avm_translate_operand(&instr->arg1, &ax);
    struct AvmMemCell* rv2 = avm_translate_operand(&instr->arg2, &bx);

    assert(rv1 && rv2);

    if(rv1->type != number_m || rv2->type != number_m) {
        avm_error("Not a number in comparison!", NULL, NULL);
        executionFinished = 1;
    } else {
        cmp_func_t cmp = comparisonFuncs[instr->opcode - jle_v];
        unsigned char result = (*cmp)(rv1->data.numVal, rv2->data.numVal);

        if (!executionFinished && result)
            pc = instr->result.val;
    }
}

static unsigned char number_tobool(struct AvmMemCell *m) {
    return m->data.numVal != 0;
}

static unsigned char string_tobool(struct AvmMemCell *m) {
    return m->data.strVal[0] != 0;
}

static unsigned char bool_tobool(struct AvmMemCell *m) {
    return m->data.boolVal;
}

static unsigned char table_tobool(struct AvmMemCell *m) {
    return 1;
}

static unsigned char userfunc_tobool(struct AvmMemCell *m) {
    return 1;
}

static unsigned char libfunc_tobool(struct AvmMemCell *m) {
    return 1;
}

static unsigned char nil_tobool(struct AvmMemCell *m) {
    return 0;
}

static unsigned char undef_tobool(struct AvmMemCell *m) {
    assert(0);
    return 0;
}

tobool_func_t toboolFuncs[] = {
        number_tobool,
        string_tobool,
        bool_tobool,
        table_tobool,
        userfunc_tobool,
        libfunc_tobool,
        nil_tobool,
        undef_tobool
};

unsigned char avm_tobool(struct AvmMemCell *m) {
    assert(m->type >= 0 && m->type < undef_m);
    return (*toboolFuncs[m->type])(m);
}

static double consts_getnumber(unsigned int index) {
    assert(index >= 0 && index < tables->totalNumConsts);
    return tables->numConstsTable[index];
}

static char *consts_getstring(unsigned int index) {
    assert(index >= 0 && index < tables->totalStrConsts);
    return tables->strConstsTable[index];
}

static char *libfuncs_getused(unsigned int index) {
    assert(index >= 0 && index < tables->totalLibFuncs);
    return tables->libFuncsTable[index];
}

struct AvmMemCell* avm_translate_operand(struct VmArg* arg, struct AvmMemCell* reg) {
    if(arg->type == -1 || arg->type == 11)
        return reg;

    switch(arg->type){
        case global_a :	return &stack[AVM_STACKSIZE - 1 - arg->val];
        case local_a  :	return &stack[topsp - arg->val];
        case formal_a :	return &stack[topsp + AVM_STACKENV_SIZE + 1 + arg->val];
        case retval_a:	return &retval;
        case number_a:	{
            reg->type = number_m;
            reg->data.numVal = consts_getnumber(arg->val);
            return reg;
        }
        case string_a:	{
            reg->type = string_m;
            reg->data.strVal = strdup(consts_getstring(arg->val));
            return reg;
        }
        case bool_a:	{
            reg->type = bool_m;
            reg->data.boolVal = arg->val;
            return reg;
        }
        case nil_a:		reg->type = nil_m; return reg;
        case userfunc_a:	{

            reg->type = userfunc_m;
            struct UserFunc* func = avm_getfuncinfo(arg->val);
            reg->data.funcVal = func->address;
            return reg;
        }
        case libfunc_a:		{
            reg->type = libfunc_m;
            reg->data.libfuncVal = libfuncs_getused(arg->val);
            return reg;
        }
        default:
            assert(0);
    }
}

void avm_initialize(void) {
    for (unsigned int i = 0; i < AVM_STACKSIZE; i++) {
        memset(&(stack[i]), 0, sizeof(stack[i]));
        stack[i].type = undef_m;
    }
}
