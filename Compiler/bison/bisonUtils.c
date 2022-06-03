#include <stdio.h>
#include <malloc.h>
#include "./bison.h"
#include "string.h"

void addBuiltinFunctionsTo() {
    insert("print", function, LIB_FUNC, 0, 0, 0, -1, -1);
    insert("input", function, LIB_FUNC, 0, 0, 0, -1, -1);
    insert("objectmemberkeys", function, LIB_FUNC, 0, 0, 0, -1, -1);
    insert("objecttotalmembers", function, LIB_FUNC, 0, 0, 0, -1, -1);
    insert("objectcopy", function, LIB_FUNC, 0, 0, 0, -1, -1);
    insert("totalarguments", function, LIB_FUNC, 0, 0, 0, -1, -1);
    insert("argument", function, LIB_FUNC, 0, 0, 0, -1, -1);
    insert("typeof", function, LIB_FUNC, 0, 0, 0, -1, -1);
    insert("strtonum", function, LIB_FUNC, 0, 0, 0, -1, -1);
    insert("sqrt", function, LIB_FUNC, 0, 0, 0, -1, -1);
    insert("cos", function, LIB_FUNC, 0, 0, 0, -1, -1);
    insert("sin", function, LIB_FUNC, 0, 0, 0, -1, -1);
}

/* SYMTABLE FUNCTIONS
 */

struct SymbolTable *buckets[HASH_SIZE];
struct SymbolTable *collList[HASH_SIZE];

int encounteredError = 0;

struct Scope *scopeList = NULL;


void initTable() {
    for (int i = 0; i < HASH_SIZE; i++) {
        buckets[i] = NULL;
        collList[i] = NULL;
    }
}

static int hash(const char *name, int scope) {
    int output = 0, i = 0;
    unsigned long length = strlen(name);

    for (i = 0; i < length; i++) {
        output += (name[i] + i) * (scope);
    }

    if ((output % HASH_SIZE) < 0) {
        return (output % HASH_SIZE) * (-1);
    } else {
        return (output % HASH_SIZE);
    }
}

struct SymbolTable *createBucket(const char *name, enum What what, enum SymbolType type, int scope, int declarationLine,
                                 enum ScopeSpace scopeSpace, unsigned int offset) {
    struct SymbolTable *new = malloc(sizeof(struct SymbolTable));
    new->isActive = 1;
    new->type = type;
    new->what = what;
    new->scopeSpace = scopeSpace;
    new->nextScope = NULL;
    new->nextList = NULL;
    new->offset = offset;

    if (what == function) {
        new->value.varVal = malloc(sizeof(struct Variable));
        new->value.varVal->name = name;
        new->value.varVal->scope = scope;
        new->value.varVal->line = declarationLine;
    } else {
        new->value.funcVal = malloc(sizeof(struct Function));
        new->value.funcVal->name = name;
        new->value.funcVal->scope = scope;
        new->value.funcVal->line = declarationLine;
    }

    return new;
}

/**
 * Add new node to the scope list, for later use.
 * @param new
 * @param scope
 * @param first
 */
static void addToScopeList(struct SymbolTable *new, int scope, int first) {
    if (first) {
        // Scope list empty and first in bucket list
        if (!scopeList) {
            struct Scope *newScope = malloc(sizeof(struct Scope));
            newScope->scope = scope;
            newScope->firstScope = new;
            newScope->lastScope = new;
            newScope->next = NULL;

            scopeList = newScope;
        } else {
            struct Scope *s = scopeList;
            while (s && s->scope != scope)
                s = s->next;

            // Scope exists
            if (s) {
                // Replace last
                s->lastScope->nextScope = new;
                s->lastScope = new;
            } else {
                struct Scope *newScope = malloc(sizeof(struct Scope));
                newScope->scope = scope;
                newScope->firstScope = new;
                newScope->lastScope = new;
                newScope->next = NULL;

                scopeList = newScope;
            }
        }
    } else {
        if (!scopeList) {
            encounteredError = 1;
            return;
        }

        struct Scope *s = scopeList;
        while (s && s->scope != scope)
            s = s->next;

        if (s) {
            // Replace last
            s->lastScope->nextScope = new;
            s->lastScope = new;
        } else {
            struct Scope *newScope = malloc(sizeof(struct Scope));
            newScope->scope = scope;
            newScope->firstScope = new;
            newScope->lastScope = new;
            newScope->next = NULL;

            scopeList = newScope;
        }
    }
}

struct SymbolTable *insert(
        char *name,
        enum What what,
        enum SymbolType type,
        int scope,
        int declarationLine,
        int fromLocal,
        enum ScopeSpace scopeSpace,
        unsigned int offset
) {
    int index = hash(name, (int) scope);
    struct SymbolTable *symt = NULL;

    if (!buckets[index]) {
        if (fromLocal)
            symt = findBucketDuplicateLocal(name, scope);
        else
            symt = existsInSameBucket(name, scope);

        if (!symt) {
            symt = createBucket(name, what, type, scope, declarationLine, scopeSpace, offset);
            buckets[index] = symt;
            addToScopeList(symt, scope, 1);
        }
    } else {
        struct SymbolTable *tmp = buckets[index];
        while (tmp->nextList)
            tmp = tmp->nextList;

        if (fromLocal)
            symt = findBucketDuplicateLocal(name, scope);
        else
            symt = existsInSameBucket(name, scope);

        if (!symt) {
            symt = createBucket(name, what, type, scope, declarationLine, scopeSpace, offset);
            tmp->nextList = symt;
            collList[index] = symt;
            addToScopeList(symt, scope, 0);
        }
    }

    return symt;
}

int isLibFunc(const char *name, unsigned int line) {
    struct Scope *s = scopeList;
    while (s && s->scope != 0)
        s = s->next;

    if (!s)
        return 0;

    struct SymbolTable *symt = s->firstScope;
    while (symt) {
        if (symt->type != LIB_FUNC) {
            symt = symt->nextScope;
            continue;
        }

        if (symt->what == variable && !strcmp(symt->value.varVal->name, name)) {
            encounteredError = 1;
            printf("ERROR: variable shadow builtin function at line %d\n", line);
            return 1;
        } else if (symt->what == function && !strcmp(symt->value.funcVal->name, name)) {
            encounteredError = 1;
            printf("ERROR: function shadow builtin function at line %d\n", line);
            return 1;
        }

        symt = symt->nextScope;
    }
    return 0;
}

int toggleActive(int scope, int active) {
    if (scope == 0)
        return 1;

    struct Scope *s = scopeList;
    while (s && s->scope != scope)
        s = s->next;

    struct SymbolTable *symt = NULL;
    if (s)
        symt = s->firstScope;

    while (symt) {
        symt->isActive = active;
        symt = symt->nextScope;
    }
    return 0;
}

struct SymbolTable *checkGlobalScope(char *name, int line) {
    struct Scope *s = scopeList;
    while (s && s->scope != 0)
        s = s->next;

    if (s) {
        struct SymbolTable *symt = s->firstScope;
        while (symt) {
            if ((symt->what == variable && !strcmp(symt->value.varVal->name, name))
                || symt->what == function && !strcmp(symt->value.funcVal->name, name))
                return symt;

            symt = symt->nextScope;
        }
    }

    printf("ERROR: variable is not defined is global scope line %d", line);
    return NULL;
}

int isAlreadyDeclared(char *name, int scope, unsigned int line) {
    struct Scope *s = scopeList;
    int cScope = scope;

    while (cScope > 0) {
        while (s && s->scope != cScope)
            s = s->next;

        if (s && s->scope == cScope)
            break;

        s = scopeList;
        cScope -= 1;
    }

    while (s && s->scope >= 0) {
        struct SymbolTable *symt = s->firstScope;
        while (symt) {
            if (!strcmp(symt->value.varVal->name, name) && symt->isActive && symt->type == FORMAL)
                return 0;

            if (!strcmp(symt->value.varVal->name, name) && symt->isActive && symt->type == USER_FUNC) {
                encounteredError = 1;
                printf("ERROR: Value cannot be assigned to function, line %d", line);
                return 1;
            }

            symt = symt->nextScope;
        }
        s = s->next;
    }

    return 0;
}

int invalidAccess(char *name, int scope, unsigned int line) {
    struct Scope *s = scopeList;

    int fFunction = 0;
    int fEqual = 0;

    int cScope = scope; // from bottom to up

    while (cScope > 0) {
        while (s && s->scope != cScope)
            s = s->next;
        if (s && s->scope == cScope)
            break;

        cScope -= 1;
        s = scopeList;
    }

    while (s && s->scope >= 0) {
        struct SymbolTable *symt = s->firstScope;

        while (symt) {
            if (symt->type == USER_FUNC && symt->isActive && symt->value.varVal->scope != 0 &&
                symt->value.varVal->scope != scope)
                fFunction = 1;

            if (!strcmp(symt->value.varVal->name, name) && symt->isActive &&
                (symt->type == LOCAL || symt->type == FORMAL))
                fEqual = 1;

            if (fEqual && !fFunction)
                return 0;
            if (fEqual && fFunction) {
                printf("ERROR: %s is used at lesser scope line %d", name, line);
                return 1;
            }

            symt = symt->nextScope;
        }
        s = s->next;
    }

    return 0;
}

struct SymbolTable *lvalueIsFunc(char *name, int scope) {
    struct Scope *s = scopeList;
    int cScope = scope;

    while (cScope > 0) {
        while (s && s->scope != cScope)
            s = s->next;
        if (s && s->scope == cScope)
            break;

        cScope--;
        s = scopeList;
    }

    while (s && s->scope >= 0) {
        struct SymbolTable *symt = s->firstScope;
        while (symt) {
            if (!strcmp(symt->value.varVal->name, name) && symt->isActive &&
                (symt->type == USER_FUNC || symt->type == LIB_FUNC))
                return symt;
            symt = symt->nextScope;
        }
        s = s->next;
    }

    return NULL;
}

struct SymbolTable *findBucketDuplicateLocal(char *name, int scope) {
    struct Scope *s = scopeList;
    int cScope = scope; // from bottom to up

    while (s && s->scope != cScope)
        s = s->next;

    if (s) {
        struct SymbolTable *symt = s->firstScope;
        while (symt) {
            if (!strcmp(symt->value.varVal->name, name) && symt->isActive)
                return symt;
            symt = symt->nextScope;
        }
    }

    return NULL;
}

struct SymbolTable *existsInSameBucket(char *name, int scope) {
    struct Scope *s = scopeList;
    int cScope = scope;
    struct SymbolTable *symt = NULL;
    struct SymbolTable *r = NULL;
    int fFunction = 0;
    int fEqual = 0;

    while (cScope >= 0) {
        if (scope > 0) {
            while (s && s->scope != cScope)
                s = s->next;
        } else if (s && s->scope == cScope)
            break;

        s = scopeList;
        cScope -= 1;
    }

    if (!s)
        return NULL;

    cScope = scope;
    struct SymbolTable *firstCScope = findBucketDuplicateLocal(name, cScope);
    if (cScope == 0 || firstCScope)
        return firstCScope;

    while (cScope >= 0) {
        s = scopeList;
        while (s && s->scope != cScope)
            s = s->next;

        if (s) {
            symt = s->firstScope;
            while (symt) {
                if (symt->type == USER_FUNC && symt->isActive)
                    fFunction = 1;
                if (!strcmp(symt->value.varVal->name, name) && symt->isActive) {
                    if (s->scope == 0)
                        return symt;

                    r = symt;
                    fEqual = 1;
                }

                symt = symt->nextScope;
            }

            // Found function not var, so insert var
            if (fFunction && !fEqual)
                return NULL;
            // Found var but also a function (scope) in the middle, do not insert
            if (!fFunction && fEqual)
                return r;
        }

        cScope--;
    }

    return NULL;
}

int existsLibFunc(char *name) {
    struct Scope *s = scopeList;

    while (s && s->scope != 0)
        s = s->next;

    if (!s) return 0;

    struct SymbolTable *symt = s->firstScope;

    while (symt) {
        if (symt->type == LIB_FUNC) {
            if ((symt->what == variable && !strcmp(symt->value.varVal->name, name))
                || (symt->what == function && !strcmp(symt->value.funcVal->name, name)))
                return 1;
        }


        symt = symt->nextScope;
    }
    return 0;
}

int formalRedeclaration(char *name, int scope, int line) {
    struct Scope *s = scopeList;
    while (s && s->scope != scope)
        s = s->next;

    struct SymbolTable *symt = s->firstScope;
    while (symt) {
        if (symt->what == variable && !strcmp(symt->value.varVal->name, name)) {
            if (symt->type == FORMAL && symt->isActive) {
                encounteredError = 1;
                printf("ERROR: function argument redeclaration line %d", line);
                return 1;
            }
        }
        symt = symt->nextScope;
    }

    return 0;
}

int formalShadow(char *name, int line) {
    struct Scope *s = scopeList;
    while (s && s->scope != 0)
        s = s->next;

    if (!s)
        return 0;

    struct SymbolTable *symt = s->firstScope;
    while (symt) {
        if (symt->type == LIB_FUNC) {
            if ((symt->what == variable && !strcmp(symt->value.varVal->name, name))
                || ((symt->what == function && !strcmp(symt->value.funcVal->name, name)))) {
                encounteredError = 1;
                printf("ERROR: formal arg shadows libfunc line %d", line);
                return 1;
            }
        }

        symt = symt->nextScope;
    }

    return 0;
}

int lookUpScopeRedeclaration(char *name, enum What what, int scope, int yylineno, enum Rule rule) {
    struct Scope *s = scopeList;

    while (s && s->scope != scope)
        s = s->next;

    if (!s && what != function && scope - 1 >= 0) {
        s = scopeList;
        while (s && s->scope != scope - 1)
            s = s->next;
    }

    if (!s) return 0;

    struct SymbolTable *symt = s->firstScope;

    while (symt != NULL) {
        if (what == function) {
            if (symt->what == function && !strcmp(symt->value.funcVal->name, name)) {
                encounteredError = 1;
                printf("Error at line %d. Redeclaration of '%s' as function, but it has already been declared as %s at line %d.\n",
                       yylineno, name, symt->what == function ? "function" : "variable", symt->value.funcVal->line);
                return 1;
            } else if (symt->what == variable && !strcmp(symt->value.funcVal->name, name)) {
                encounteredError = 1;
                printf("Error at line %d. Redeclaration of '%s' as variable, but it has already been declared as %s at line %d.\n",
                       yylineno, name, symt->what == function ? "function" : "variable", symt->value.funcVal->line);
                return 1;
            }
        } else if (what == variable) {
            if (!strcmp(symt->value.varVal->name, name) && symt->what == function) {
                encounteredError = 1;
                if (rule == AssignExpr)
                    printf("Error at line %d. Assigning value to function '%s', which is in line %d, is forbidden.\n",
                           yylineno, name, symt->value.varVal->line);
                else if (rule == Primary)
                    printf("Error at line %d. Redeclaration of '%s' as variable, but it has already been declared as %s at line %d.\n",
                           yylineno, name, symt->what == function ? "function" : "variable", symt->value.funcVal->line);
                return 1;
            }
        }
        symt = symt->nextScope;
    }

    return 0;
}

static char *getFromSymbolType(enum SymbolType type) {
    switch (type) {
        case GLOBAL:
            return "global variable";
        case LOCAL:
            return "local variable";
        case FORMAL:
            return "formal variable";
        case USER_FUNC:
            return "user function";
        case LIB_FUNC:
            return "library function";
        default:
            return "";
    }
}

void printEverything() {
    for (int i = 0; i < HASH_SIZE; i++) {
        if (buckets[i]) {
            struct SymbolTable *symt = buckets[i];
            while (symt) {
                if (symt->what == variable) {
                    printf("\"%s\" [%s] (line %d) (scope %d)",
                           symt->value.varVal->name,
                           getFromSymbolType(symt->type),
                           symt->value.varVal->line,
                           symt->value.varVal->scope);
                } else {
                    printf("\"%s\" [%s] (line %d) (scope %d)",
                           symt->value.funcVal->name,
                           getFromSymbolType(symt->type),
                           symt->value.funcVal->line,
                           symt->value.funcVal->scope);
                }

                symt = symt->nextList;
            }
        }
    }
}

struct SymbolTable *findTemp(char *name, int scope) {
    struct Scope *s = scopeList;
    int cScope = scope;
    int fFunc = 0, fEqual = 0;
    struct SymbolTable *fBucket = NULL;

    while (cScope >= 0) {
        if (scope > 0) {
            while (s && s->scope != cScope)
                s = s->next;
        }

        if (s && s->scope == cScope)
            break;

        s = scopeList;
        cScope--;
    }

    if (!s)
        return NULL;

    struct SymbolTable *symt = findBucketDuplicateLocal(name, scope);
    if (scope == 0) return symt;
    if (symt) return symt;

    while (scope >= 0) {
        s = scopeList;
        while (s && s->scope != scope)
            s = s->next;

        if (s) {
            symt = s->firstScope;
            while (symt) {
                if (symt->type == USER_FUNC && symt->isActive)
                    fFunc = 1;

                if (!strcmp(symt->value.varVal->name, name) && symt->isActive) {
                    if (s->scope == 0)
                        return symt;
                    fBucket = symt;
                    fEqual = 1;
                }

                symt = symt->nextScope;
            }

            if (fFunc && !fEqual)
                return NULL;
            else if (!fFunc && fEqual)
                return fBucket;
        }

        scope--;
    }

    return NULL;
}

unsigned int totalGlobals() {
    unsigned int total = 0;
    int i = 0;

    for (i = 0; i < HASH_SIZE; i++) {
        if (buckets[i]) {
            struct SymbolTable *symt = buckets[i];
            while (symt != NULL) {
                if (symt->scopeSpace == PROGRAM_VAR)
                    total++;
                symt = symt->nextList;
            }
        }
    }

    return total;
}