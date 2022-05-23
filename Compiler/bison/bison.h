#ifndef PROJECT_BISON_H
#define PROJECT_BISON_H

#define HASH_SIZE 100
extern int encounteredError;

enum SymbolType {
    GLOBAL, LOCAL, FORMAL,
    USER_FUNC, LIB_FUNC,
};

enum What {
    function, variable
};

enum ScopeSpace {
    PROGRAM_VAR, FUNC_LOCAL, FORMAL_ARG
};

struct ReturnList {
    unsigned int instrLabel;
    struct ReturnList *next;
};

struct SymbolTable {
    int isActive;
    enum ScopeSpace scopeSpace;
    unsigned int offset;

    enum What what;
    union {
        struct Variable {
            const char *name;
            unsigned int scope;
            unsigned int line;
        } *varVal;

        struct Function {
            const char *name;
            unsigned int scope;
            unsigned int line;

            unsigned int totalargs;
            unsigned int iaddress;
            unsigned int totallocals;

            unsigned int taddress;
            struct ReturnList *returnList;
        } *funcVal;
    } value;
    enum SymbolType type; // It is for both Variable and Function

    struct SymbolTable *nextList;
    struct SymbolTable *nextScope;
};

struct Scope {
    unsigned int scope;
    struct Scope *next;

    struct SymbolTable *firstScope;
    struct SymbolTable *lastScope;
};

enum Rule {
    AssignExpr, Primary, Funcdef
};


/* FUNCTIONS */
extern void addBuiltinFunctionsTo();


/* SYMTABLE */
extern void initTable();

// Symbol redefinition
// Allowed:     variables(same name, same scope)
// Forbidden:   Functions(same name, same scope)
//              VariableFunction(same name, same scope)
//              SymbolLIBFUNC(same name)

// Undefined symbol
// Allowed:     Variables init on found.
// Forbidden:   If asking for variable(global) and is not defined





/**
 * Insert when new variable init (ex. local x;)
 *                 function      (ex. function f(y) {})
 *                 non initialized symbol (ex. x = input();)
 *
 * Make sure not to insert ::x which a an already defined global
 */
extern struct SymbolTable *insert(
        char *name,
        enum What what,
        enum SymbolType type,
        int scope,
        int declarationLine,
        int fromLocal,
        enum ScopeSpace scopeSpace,
        unsigned int offset
);

extern struct SymbolTable *createBucket(const char *name, enum What what, enum SymbolType type, int scope,
                                        int declarationLine, enum ScopeSpace scopeSpace, unsigned int offset);

/**
 * Hide/Show
 * When exiting a block or a function
 */
extern int toggleActive(int scope, int active);

/**
 * Check if var or func declared on scope 0.
 * WILL THROW ERROR.
 */
extern struct SymbolTable *checkGlobalScope(char *name, int line);

/**
 * Check if symbol is already declared function (redeclaration).
 * Checks if symbol shadows a libfunc.
 * WILL THROW ERROR
 */
extern int isLibFunc(const char *name, unsigned int line);


/**
 * Checks if symbol is already declared in scope.
 * Checks if something tries to assign on a function.
 * WILL THROW ERROR
 */
extern int isAlreadyDeclared(char *name, int scope, unsigned int line);

/**
 * Checks whether a there is access to a symbol.
 * WILL THROW ERROR
 */
extern int invalidAccess(char *name, int scope, unsigned int line);

/**
 * Check if lvalue is a function or not.
 */
extern struct SymbolTable *lvalueIsFunc(char *name, int scope);

/**
 * Check if there is a symbol with the same name in the same bucket list.
 */
extern struct SymbolTable *existsInSameBucket(char *name, int scope);

/**
 * "Like" like exists (above) but returns the item.
 */
extern struct SymbolTable *findBucketDuplicateLocal(char *name, int scope);

/**
 * Checks if a function exists.
 */
extern int existsLibFunc(char *name);

/**
 * Checks for function arguments redeclaration.
 * WILL THROW ERROR.
 */
extern int formalRedeclaration(char *name, int scope, int line);

/**
 * Checks if function arguments shadows lib symbols.
 * WILL THROW ERROR.
 */
extern int formalShadow(char *name, int line);

/**
 * Checks whether there is already a variable or function
 * with same name, declared in the specified scope
 * WILL THROW ERROR
 * @param name
 * @param type
 * @param scope
 * @param yylineno
 * @param rule
 * @return
 */
int lookUpScopeRedeclaration(char *name, enum What what, int scope, int yylineno, enum Rule rule);

/**
 * Phase 2 - Not needed in Phase3 and later
 */
extern void printEverything();

/**
 * Checks whether there is already a variable or function
 * with same name, declared in the specified hash of the BucketList.
 * Search for temp functions.
 * @param name
 * @param scope
 * @return
 */
extern struct SymbolTable *findTemp(char *name, int scope);


unsigned int totalGlobals();

#endif //PROJECT_BISON_H
