%{
#include "../Compiler/bison/bison.h"
#include "../Compiler/targetCode/targetCode.h"

#include "stdio.h"
#include "stdlib.h"
#include <math.h>

int yylex(void);
int yyerror(char* yaccProvidedMessage);

extern FILE* yyin;
extern FILE* yyout;
extern char* yytext;
extern int yylineno;

enum ScopeAction { INCREASE , DECREASE };

char* opcodeToString[] = {
	"assign",         "add",            "sub",
	"mul",            "div",            "mod",
	"jump",           "if_eq",          "if_noteq",
	"if_lesseq",      "if_greatereq",   "if_less",
	"if_greater",     "call",           "param",
	"funcstart",      "funcend",        "tablecreate",
	"tablegetelem",   "tablesetelem",   "getretval",
	"no_op",          "ret"
};

int getCurrentLine();
void print(char *message);

char *genName();
void scopeOps(enum ScopeAction action);

char* getStringValueQuad(struct expr* e, int isResult);
void printAllQuads();

int funcdefStart = 0;
int scope = 0;

int noNameFunctionNum = 0;
int activeFunctions = 0;
int activeLoops = 0;


int globalRuleManage = 0;
int localRuleManage = 0;

int globalBeforeEqual = 0;
int localBeforeEqual = 0;

int memberRuleManage = 0;
int memberManageRule = 0;

int memberBeforeEqual = 0;
int equalFound = 0;

int accessIsValid = 0;
int notRulePassed = 0;
int totalFuncArgs = 0;
%}

%union{
    int intVal;
    char* strValue;
    double numVal;

    struct SymbolTable *symbolTable;

    struct expr *exprValue;
    struct MethodCall *methodCall;
    struct ForLoop_S *forLoopVal;
    struct LoopStack *loopStack;
}

%start program

%token<numVal> NUMBER
%token<strValue> IF ELSE WHILE FOR FUNCTION RETURN BREAK CONTINUE AND NOT OR local TRUE FALSE NIL STRING ID
%token<strValue> equal plus minus MUL DIV MOD eequal nequal pplus mminus greater lesser
%token<strValue> gequal lequal openBlock closeBlock openBracket closeBracket openPar closePar semicolon comma
%token<strValue> colon ccolon dot ddot UMINUS

%type<strValue> funcName
%type<strValue> block whilestmt forstmt

%type<methodCall> callsuffix normcall  methodcall
%type<forLoopVal> forPrefix
%type<symbolTable> funcdef funcPreArgs funcPre
%type<intVal> ifPrefix funcBody elsePrefix ifstmt whileStart whileCond idlist N M
%type<exprValue> expr const primary term assignexpr lvalue member indexedelem indexed objectdef elist call returnstmt stmt
%type<loopStack> loopstmt

/* Grammar rules
*/

%right      equal
%left       OR
%left       AND
%nonassoc   eequal nequal
%nonassoc   greater gequal lesser lequal
%left       plus minus
%left       MUL DIV MOD
%right      NOT pplus mminus UMINUS
%left       dot ddot
%left       openBracket closeBracket
%left       openPar closePar

%%

program:	stmtlist				{ print("program"); }
			;

stmtlist:	stmtlist stmt			{ print("stmtlist"); }
			|
			;

stmt:		expr semicolon			{
										print("stmt");
										$$ = $1;

										if($1->type == boolexpr_e || $1->cameFromNot == 1) {
											$1->sym = newTmp(scope);

											backPatchList($1->trueList, nextQuadLabel());
											emit(assign, newExprConstBool('1'), NULL, $1, nextQuadLabel(), getCurrentLine());
											emit(jump, NULL, NULL, newExprConstNumber(nextQuadLabel() + 2), nextQuadLabel(), getCurrentLine());

											backPatchList($1->falseList, nextQuadLabel());
											emit(assign, newExprConstBool('0'), NULL, $1, nextQuadLabel(), getCurrentLine());
										}

										resetTmp();
										print("stmt expr;");
							 		}
			|ifstmt					{
			 							print("stmt ifstmt");

			 							$$ = $<exprValue>1;
			 							resetTmp();

			 							print("stmt ifstmt");
									 }
			|whilestmt				{
		 								print("stmt whilestmt");

		 								$$ = $<exprValue>1;
		 								resetTmp();

		 								print("stmt whilestmt");
									 }
			|forstmt				{
			 							print("stmt forstmt");

			 							$$ = $<exprValue>1;
			 							resetTmp();

			 							print("stmt forstmt");
			 						}
			|returnstmt				{
			 							print("stmt returnstmt");

			 							$$ = $1;
			 							resetTmp();

			 							print("stmt returnstmt");
			 						}
			|BREAK semicolon		{
										print("stmt break;");

										if(activeLoops == 0) {
											encounteredError = 1;
											printf("Error: break in not in a loop. Line: %d", getCurrentLine());
										} else {
											breakPush(nextQuadLabel());
											emit(jump, NULL, NULL, NULL, nextQuadLabel(), getCurrentLine());
										}

										resetTmp();

										print("stmt break;");
									}
			|CONTINUE semicolon		{
										print("stmt continue");

										if(activeLoops == 0) {
											encounteredError = 1;
											printf("Error: continue in not in a loop. Line: %d", getCurrentLine());
										} else {
											continuePush(nextQuadLabel());
											emit(jump, NULL, NULL, NULL, nextQuadLabel(), getCurrentLine());
										}

										resetTmp();

										print("stmt continue;");
									}
			|block					{
			 							print("stmt block");

			 							$$ = $<exprValue>1;
			 							resetTmp();

			 							print("stmt block");
								 	}
			|funcdef				{
			 							print("stmt funcdef");

			 							$$ = $<exprValue>1;
			 							resetTmp();

			 							print("stmt funcdef");
			 						}
			|semicolon				{
			 							print("stmt semicolon");

			 							resetTmp();

			 							print("stmt semicolon");
			 						}
			;

expr:		assignexpr				{
 										print("expr assignexpr");
 										$<exprValue>$ = $1;
 										print("expr assignexpr");
								 	}
			|expr plus expr			{
										print("expr expt+expr");
										$<exprValue>$ = lvalueExpr(newTmp(scope));
										emit(add, $1, $3, $$, nextQuadLabel(), getCurrentLine());
										print("expr expt+expr");
									}
			|expr minus expr		{
										print("expr expr-expr");
										$<exprValue>$ = lvalueExpr(newTmp(scope));
										emit(sub, $1, $3, $$, nextQuadLabel(), getCurrentLine());
										print("expr expr-expr");
									}
			|expr MUL expr			{
										print("expr expr*expr");
										$<exprValue>$ = lvalueExpr(newTmp(scope));
										emit(mul, $1, $3, $$, nextQuadLabel(), getCurrentLine());
										print("expr expr*expr");
									}
			|expr DIV expr			{
										print("expr expr/expr");
										$<exprValue>$ = lvalueExpr(newTmp(scope));
										emit(div_, $1, $3, $$, nextQuadLabel(), getCurrentLine());
										print("expr expr/expr");
									}
			|expr MOD expr			{
										print("expr expr MOD expr");
										$<exprValue>$ = lvalueExpr(newTmp(scope));
										emit(mod, $1, $3, $$, nextQuadLabel(), getCurrentLine());
										print("expr expr MOD expr");
									}

			/* Meriki apotimhsh */

			|expr greater expr	{
										print("expr exp>expr");
										$<exprValue>$ = newExpr(boolexpr_e);
										$$->trueList = makeList(nextQuadLabel());
										$$->falseList = makeList(nextQuadLabel()+1);

										emit(if_greater, $1, $3, NULL, nextQuadLabel(), getCurrentLine());
										emit(jump, NULL, NULL, NULL, nextQuadLabel(), getCurrentLine());
										print("expr exp>expr");
									}
			|expr gequal expr	{
										print("expr expr>=expr");
										$<exprValue>$ = newExpr(boolexpr_e);
										$$->trueList = makeList(nextQuadLabel());
										$$->falseList = makeList(nextQuadLabel()+1);

										emit(if_greatereq, $1, $3, NULL, nextQuadLabel(), getCurrentLine());
										emit(jump, NULL, NULL, NULL, nextQuadLabel(), getCurrentLine());
										print("expr expr>=expr");
									}
			|expr lesser expr	{
										print("expr expr<expr");
										$<exprValue>$ = newExpr(boolexpr_e);
										$$->trueList = makeList(nextQuadLabel());
										$$->falseList = makeList(nextQuadLabel()+1);

										emit(if_less, $1, $3, NULL, nextQuadLabel(), getCurrentLine());
										emit(jump, NULL, NULL, NULL, nextQuadLabel(), getCurrentLine());
										print("expr expr<expr");
									}
			|expr lequal expr	{
										print("expr expr<=expr");
										$<exprValue>$ = newExpr(boolexpr_e);
										$$->trueList = makeList(nextQuadLabel());
										$$->falseList = makeList(nextQuadLabel()+1);

										emit(if_lesseq, $1, $3, NULL, nextQuadLabel(), getCurrentLine());
										emit(jump, NULL, NULL, NULL, nextQuadLabel(), getCurrentLine());
										print("expr expr<=expr");
									}
			|expr eequal 		{ checkForEmitEqEq($1, getCurrentLine(), scope); } expr {
									print("expr expr==expr");

									if($4->type == boolexpr_e && $4->cameFromNot == 1) {
										$4->sym = newTmp(scope);

										backPatchList($4->trueList, nextQuadLabel());
										emit(assign, newExprConstBool('1'), NULL, $4, nextQuadLabel(), getCurrentLine());
										emit(jump, NULL, NULL, newExprConstNumber(nextQuadLabel() + 2), nextQuadLabel(), getCurrentLine());

										backPatchList($4->falseList, nextQuadLabel());
										emit(assign, newExprConstBool('0'), NULL, $4, nextQuadLabel(), getCurrentLine());
									}

									$<exprValue>$ = newExpr(boolexpr_e);
									$$->trueList = makeList(nextQuadLabel());
									$$->falseList = makeList(nextQuadLabel() + 1);
									emit(if_eq, $1, $4, NULL, nextQuadLabel(), getCurrentLine());
									emit(jump, NULL, NULL, NULL, nextQuadLabel(), getCurrentLine());

									print("expr expr==expr");
								}
			|expr nequal 		{ checkForEmitNotEq($1, getCurrentLine(), scope); } expr {
									print("expr expr!=expr");

									if($4->type == boolexpr_e && $4->cameFromNot == 1) {
										$4->sym = newTmp(scope);

										backPatchList($4->trueList, nextQuadLabel());
										emit(assign, newExprConstBool('1'), NULL, $4, nextQuadLabel(), getCurrentLine());
										emit(jump, NULL, NULL, newExprConstNumber(nextQuadLabel() + 2), nextQuadLabel(), getCurrentLine());

										backPatchList($4->falseList, nextQuadLabel());
										emit(assign, newExprConstBool('0'), NULL, $4, nextQuadLabel(), getCurrentLine());
									}

									$<exprValue>$ = newExpr(boolexpr_e);
									$$->trueList = makeList(nextQuadLabel());
									$$->falseList = makeList(nextQuadLabel() + 1);
									emit(if_noteq, $1, $4, NULL, nextQuadLabel(), getCurrentLine());
									emit(jump, NULL, NULL, NULL, nextQuadLabel(), getCurrentLine());

									print("expr expr!=expr");
			 					}
			|expr AND 			{ checkForEmitAND($1, getCurrentLine(), notRulePassed); } M expr {
									print("expr expr&&expr");

									notRulePassed = 0;
									$<exprValue>$ = newExpr(boolexpr_e);

									// TypeCheck for arguments that are not boolean and create logic lists for them
									if($5->type != boolexpr_e) {
										$5->trueList = makeList(nextQuadLabel());
										$5->falseList = makeList(nextQuadLabel() + 1);

										emit(if_eq, newExprConstBool('1'), $5, NULL, nextQuadLabel(), getCurrentLine());
										emit(jump, NULL, NULL, NULL, nextQuadLabel(), getCurrentLine());
									}

									if($1->type == boolexpr_e)
										backPatchList($1->trueList, $4);

									$$->trueList = $5->trueList;
									$$->falseList = mergeLLs($1->falseList, $5->falseList);

									print("expr expr&&expr");
								}
			|expr OR	 		{ checkForEmitOR($1, getCurrentLine(), notRulePassed); } M expr {
                                	print("expr expr||expr");

                                	$<exprValue>$ = newExpr(boolexpr_e);

									// TypeCheck for arguments that are not boolean and create logic lists for them
									if($5->type != boolexpr_e) {
										$5->trueList = makeList(nextQuadLabel());
										$5->falseList = makeList(nextQuadLabel() + 1);

										emit(if_eq, newExprConstBool('1'), $5, NULL, nextQuadLabel(), getCurrentLine());
										emit(jump, NULL, NULL, NULL, nextQuadLabel(), getCurrentLine());
									}

									if($1->type == boolexpr_e)
										backPatchList($1->falseList, $4);

									$$->trueList = mergeLLs($1->trueList, $5->trueList);
									$$->falseList = $5->falseList;
									notRulePassed = 0;

									print("expr expr||expr");
                                }
			|term				{
									print("expr term");
									$<exprValue>$ = $1;
			 						print("expr term");
			 					}
			;

term:		openPar expr closePar		{
 											print("term (expr)");
 											$<exprValue>$ = $2;
 											 print("term (expr)");
 										}
			|minus expr %prec UMINUS	{
											print("term -expr prec uminus");

											checkUminus($2);
											$<exprValue>$ = newExpr(arithexpr_e);
											$$->sym = newTmp(scope);

											emit(mul, newExprConstNumber(-1), $2, $$, nextQuadLabel(), getCurrentLine());

											print("term -expr prec uminus");
										}
			|NOT expr					{
											print("term !expr");

											/*Meriki apotimisi*/
											$<exprValue>$ = newExpr(boolexpr_e);
											$$->sym = $2->sym;
											$$->cameFromNot = 1;

											if($2->type != boolexpr_e){
												$$->trueList = makeList(nextQuadLabel() + 1);
												$$->falseList = makeList(nextQuadLabel());

												emit(if_eq, newExprConstBool('1'), $2, NULL, nextQuadLabel(), getCurrentLine());
												emit(jump, NULL, NULL, NULL, nextQuadLabel(), getCurrentLine());
											} else {
												$$->trueList = $2->falseList;
												$$->falseList = $2->trueList;
											}

											print("term !expr");
										}
			|pplus lvalue				{
											print("term ++lvalue");

											char* name = (char*) $2->sym->value.varVal->name;

											// Check for redeclarations
											if(!isLibFunc(name, getCurrentLine())) {
												// Check inside to avoid duplicate errors
												isAlreadyDeclared(name, scope, getCurrentLine());
											}

											memberRuleManage = equalFound;

											if($2->type == tableitem_e) {
												$<exprValue>$ = emitIfTableItem($2, nextQuadLabel(), getCurrentLine(), scope);

												emit(add, $<exprValue>$, newExprConstNumber(1), $<exprValue>$ ,nextQuadLabel(), getCurrentLine());
												emit(tablesetelem, $2, $2->index, $<exprValue>$, nextQuadLabel(), getCurrentLine());
											} else {
												emit(add, $2, newExprConstNumber(1), $2, nextQuadLabel(), getCurrentLine());

												$<exprValue>$ = newExpr(arithexpr_e);
												$<exprValue>$->sym = newTmp(scope);

												emit(assign, $2, NULL, $$, nextQuadLabel(), getCurrentLine());
											}

											print("term ++lvalue");
										}
			|lvalue pplus				{
											print("term lvalue++");

											char* name = (char*) $1->sym->value.varVal->name;

											// Check for redeclarations
											if(!isLibFunc(name, getCurrentLine())) {
												// Check inside to avoid duplicate errors
												isAlreadyDeclared(name, scope, getCurrentLine());
											}

											memberRuleManage = equalFound;

											$<exprValue>$ = newExpr(var_e);
											$<exprValue>$->sym = newTmp(scope);

											if($1->type == tableitem_e) {
												struct expr *value = emitIfTableItem($1, nextQuadLabel(), getCurrentLine(), scope);

												emit(assign, value, NULL, $$, nextQuadLabel(), getCurrentLine());
												emit(add, value, newExprConstNumber(1), value, nextQuadLabel(), getCurrentLine());
												emit(tablesetelem, $1, $1->index, value, nextQuadLabel(), getCurrentLine());
											} else {
												emit(assign, $1, NULL, $$, nextQuadLabel(), getCurrentLine());
												emit(add, $1, newExprConstNumber(1), $1, nextQuadLabel(), getCurrentLine());
											}

											print("term lvalue++");
										}
			|mminus lvalue				{
											print("term --lvalue");

											char* name = (char*) $2->sym->value.varVal->name;

											// Check for redeclarations
											if(!isLibFunc(name, getCurrentLine())) {
												// Check inside to avoid duplicate errors
												isAlreadyDeclared(name, scope, getCurrentLine());
											}

											memberRuleManage = equalFound;

											if($2->type == tableitem_e) {
												$<exprValue>$ = emitIfTableItem($2, nextQuadLabel(), getCurrentLine(), scope);

												emit(sub, $<exprValue>$, newExprConstNumber(1), $<exprValue>$, nextQuadLabel(), getCurrentLine());
												emit(tablesetelem, $2, $2->index, $<exprValue>$, nextQuadLabel(), getCurrentLine());
											} else {
												emit(sub, $2, newExprConstNumber(1), $2, nextQuadLabel(), getCurrentLine());

												$<exprValue>$ = newExpr(arithexpr_e);
												$<exprValue>$->sym = newTmp(scope);

												emit(assign, $2, NULL, $$, nextQuadLabel(), getCurrentLine());
											}

											print("term --lvalue");
										}
			|lvalue mminus				{
											print("term lvalue--");

											char* name = (char*) $1->sym->value.varVal->name;

											// Check for redeclarations
											if(!isLibFunc(name, getCurrentLine())) {
												// Check inside to avoid duplicate errors
												isAlreadyDeclared(name, scope, getCurrentLine());
											}

											memberRuleManage = equalFound;

											$<exprValue>$ = newExpr(var_e);
											$<exprValue>$->sym = newTmp(scope);

											if($1->type == tableitem_e) {
												struct expr *value = emitIfTableItem($1, nextQuadLabel(), getCurrentLine(), scope);

												emit(assign, value, NULL, $$, nextQuadLabel(), getCurrentLine());
												emit(sub, value, newExprConstNumber(1), value, nextQuadLabel(), getCurrentLine());
												emit(tablesetelem, $1, $1->index, value, nextQuadLabel(), getCurrentLine());
											} else {
												emit(assign, $1, NULL, $$, nextQuadLabel(), getCurrentLine());
												emit(sub, $1, newExprConstNumber(1), $1, nextQuadLabel(), getCurrentLine());
											}

											print("term lvalue--");
										}
			|primary					{
			 								print("term primary");
			 								$<exprValue>$ = $1;
			 								print("term primary");
			 							}
			;

assignexpr:	lvalue equal { equalFound = 1; } expr 	{
				print("assignexpr lvalue=expr");

				char *var = NULL;

				if($4->type == boolexpr_e) {
					$4->sym = newTmp(scope);

					backPatchList($4->trueList, nextQuadLabel());
					emit(assign, newExprConstBool('1'), NULL, $4, nextQuadLabel(), getCurrentLine());
					emit(jump, NULL, NULL, newExprConstNumber(nextQuadLabel() + 2), nextQuadLabel(), getCurrentLine());

					backPatchList($4->falseList, nextQuadLabel());
					emit(assign, newExprConstBool('0'), NULL, $4, nextQuadLabel(), getCurrentLine());
				}

				if($<exprValue>1->type == tableitem_e) {
					emit(tablesetelem, $1, $1->index, $4, nextQuadLabel(), getCurrentLine());

					$<exprValue>$ = emitIfTableItem($1, nextQuadLabel(), getCurrentLine(), scope);
					$<exprValue>$->type = assignexpr_e;
				} else {
					char* symName = (char*) $1->sym->value.varVal->name;
					struct SymbolTable *sym = $1->sym;

					$1 = lvalueExpr(sym);
					emit(assign, $4, NULL, $1, nextQuadLabel(), getCurrentLine());

					$<exprValue>$ = newExpr(assignexpr_e);
					$<exprValue>$->sym = newTmp(scope);
					emit(assign, $1, NULL, $<exprValue>$, nextQuadLabel(), getCurrentLine());
				}

				if(memberRuleManage == 1) {
					if(globalBeforeEqual == 0 && localBeforeEqual == 0 && memberBeforeEqual == 0)
						var = (char*) $1->sym->value.varVal->name;
				} else if(globalBeforeEqual == 0 && localBeforeEqual == 0 && memberManageRule == 0)
						var = (char*) $1->sym->value.varVal->name;


				if(var != NULL) {
					if(!isLibFunc(var, getCurrentLine())) {
						if(!lookUpScopeRedeclaration(var, variable, scope, getCurrentLine(), AssignExpr))
						   if(accessIsValid == 1 && !isAlreadyDeclared(var, scope, getCurrentLine()));
					}
				}

				globalRuleManage = 0;
				localRuleManage = 0;
				globalBeforeEqual = 0;
				localBeforeEqual = 0;
				memberRuleManage = 0;
				memberManageRule = 0;
				equalFound = 0;
				accessIsValid = 0;

				print("assignexpr lvalue=expr");
			}
			;

primary:	lvalue						{
											print("primary lvalue");

											if(memberManageRule == 1 && $1->type == tableitem_e)
												$<exprValue>$ = emitIfTableItem($1, nextQuadLabel(), getCurrentLine(), scope);

											char* name = (char*) $1->sym->value.varVal->name;

											if ((globalRuleManage == 1 && globalBeforeEqual == 1 && equalFound == 1)
											 || (localRuleManage == 1 && localBeforeEqual == 1 && equalFound == 1)
											 || (memberManageRule == 1 && memberBeforeEqual == 1 && equalFound == 1)
											 || (globalRuleManage == 0 && localRuleManage == 0 && memberManageRule == 0))
												$<exprValue>$ = lvalueExpr($1->sym);

											if(equalFound == 1)
												memberRuleManage = 1;
											else {
												 /* e.x.
												  		local x;
												  		function f{ x = 2; }
												  	The local x makes primary = 1, so assignment rule will see primary = 1 and won't
												  	check the x = 2. So if we have not found '=' primary is stand alone.
												*/
												memberManageRule = 0;
												memberRuleManage = 0;
												localRuleManage = 0;
												globalRuleManage = 0;
												globalBeforeEqual = 0;
												localBeforeEqual = 0;
											}

											print("primary lvalue");
										}
			|call						{ print("primary call"); }
			|objectdef					{
			 								print("primary objectdef");
			 								$<exprValue>$ = $1;
			 								print("primary objectdef");
			 							}
			|openPar funcdef closePar	{
											print("primary (funcdef)");
											$<exprValue>$ = newExpr(programfunc_e);
											$$->sym = $2;
											print("primary (funcdef)");
										}
			|const						{
											print("primary const");
											$<exprValue>$ = $1;
											print("primary const");
										}
			;

lvalue:		ID							{
											print("lvalue id");

											struct SymbolTable *sym = NULL;
                                            int symbolExists = 1;
                                            int isFuncCall = 0;

                                            if(!invalidAccess($1, scope, getCurrentLine())) {
                                                accessIsValid = 1;
                                                if(lvalueIsFunc($1, scope) == NULL) {
                                                    if(existsInSameBucket(yylval.strValue, scope) == NULL)
                                                        symbolExists = 0;

                                                    if(scope > 0)
                                                        sym = insert(yylval.strValue, variable, LOCAL, scope, getCurrentLine(), 0, currentScopeSpace(), currentScopeOffset());
                                                    else if(scope == 0)
                                                        sym = insert(yylval.strValue, variable, GLOBAL, scope, getCurrentLine(), 0, currentScopeSpace(), currentScopeOffset());

                                                    if(!symbolExists)
                                                        increaseCurrentScopeOffset();
												}
												else
													isFuncCall = 1;
                                            }
                                            else
                                            	accessIsValid = 0;

                                            if(sym == NULL) {
                                                if(isFuncCall) {
                                                    sym = lvalueIsFunc($1, scope);
                                                    $<exprValue>$ = lvalueExpr(sym);
                                                }
                                                else
                                                	$<exprValue>$ = lvalueExpr(
                                                		createBucket(yylval.strValue, variable, GLOBAL, scope, getCurrentLine(), currentScopeSpace(), currentScopeOffset())
													);
                                            } else {
                                                switch(sym->type){
                                                    case LOCAL       : $<exprValue>$ = lvalueExpr(sym); break;
                                                    case GLOBAL      : $<exprValue>$ = lvalueExpr(sym); break;
                                                    case LIB_FUNC    : $<exprValue>$ = lvalueExpr(sym); break;
                                                    case USER_FUNC   : $<exprValue>$ = lvalueExpr(sym); break;
                                                    case FORMAL      : $<exprValue>$ = lvalueExpr(sym); break;
                                                    default          : assert(0);
                                                }
                                            }

                                            print("lvalue id");
										}
			|local ID					{
											print("lvalue local id");

											localRuleManage = 1;

											if(equalFound == 0)
												localBeforeEqual = 1;
											else localBeforeEqual = 0;

											int symbolExists = 1;
											struct SymbolTable *sym = NULL;

											if(scope > 0) {
												if (!isLibFunc(yylval.strValue, getCurrentLine())) {
													if(findBucketDuplicateLocal(yylval.strValue, scope) == NULL)
														symbolExists = 0;

													if(scope > 0)
														sym = insert(yylval.strValue, variable, LOCAL, scope, getCurrentLine(), 1, currentScopeSpace(), currentScopeOffset());
													else if(scope == 0)
														sym = insert(yylval.strValue, variable, GLOBAL, scope, getCurrentLine(), 1, currentScopeSpace(), currentScopeOffset());

													if(!symbolExists)
														increaseCurrentScopeOffset();

													$<exprValue>$ = lvalueExpr(sym);
												}
											} else {
												if (!existsLibFunc(yylval.strValue)) {
														if(existsInSameBucket(yylval.strValue, scope) == NULL)
															symbolExists = 0;

														sym = insert(yylval.strValue, variable, GLOBAL, scope, getCurrentLine(), 1, currentScopeSpace(), currentScopeOffset());

														if(!symbolExists)
															increaseCurrentScopeOffset();

														$<exprValue>$ = lvalueExpr(sym);
													}
											}

											if(sym == NULL)
												$<exprValue>$ = lvalueExpr(
													createBucket(yylval.strValue, variable, GLOBAL, scope, getCurrentLine(), currentScopeSpace(), currentScopeOffset())
												);
											else {
												switch(sym->type){
													case LOCAL       : $<exprValue>$ = lvalueExpr(sym); break;
													case GLOBAL      : $<exprValue>$ = lvalueExpr(sym); break;
													case LIB_FUNC    : $<exprValue>$ = lvalueExpr(sym); break;
													case USER_FUNC   : $<exprValue>$ = lvalueExpr(sym); break;
													case FORMAL      : $<exprValue>$ = lvalueExpr(sym); break;
													default          : assert(0);
												}
											}

											print("lvalue local id");
										}
			|ccolon ID					{
											print("lvalue ::id");

											globalBeforeEqual = equalFound == 0;
											globalRuleManage = 1;

											struct SymbolTable *entry = checkGlobalScope(yylval.strValue, getCurrentLine());
											if(entry == NULL)
												$<exprValue>$ = lvalueExpr(
													createBucket(yylval.strValue, variable, GLOBAL, scope, getCurrentLine(), currentScopeSpace(), currentScopeOffset())
												);
											else
												$<exprValue>$ = lvalueExpr(entry);

											print("lvalue ::id");
										}
			|member						{
											$<exprValue>$ = $1;
											memberManageRule = 1;
											memberBeforeEqual = equalFound == 0;

											print("lvalue member");
										}
			;

member:		lvalue dot ID							{
														print("member lvalue.id");

														struct SymbolTable *sym;
														char* name = (char*) $1->sym->value.varVal->name;
														$<exprValue>$ = memberItem($1, nextQuadLabel(), getCurrentLine(), scope, $3);

														print("member lvalue.id");
													}
			|lvalue openBracket expr closeBracket 	{
														print("member lvalue[expr]");

														if($3->type == boolexpr_e) {
															$3->sym = newTmp(scope);

															backPatchList($3->trueList, nextQuadLabel());
															emit(assign, newExprConstBool('1'), NULL, $3, nextQuadLabel(), getCurrentLine());
															emit(jump, NULL, NULL, newExprConstNumber(nextQuadLabel() + 2), nextQuadLabel(), getCurrentLine());

															backPatchList($3->falseList, nextQuadLabel());
															emit(assign, newExprConstBool('0'), NULL, $3, nextQuadLabel(), getCurrentLine());
														}

														char* name = (char*) $1->sym->value.varVal->name;
														$1 = emitIfTableItem($1, nextQuadLabel(), getCurrentLine(), scope);

														$<exprValue>$ = newExpr(tableitem_e);
														$<exprValue>$->sym = $1->sym;
														$<exprValue>$->index = $3;

														print("member lvalue[expr]");
													}
			|call dot ID							{ print("member call.id"); }
			|call openBracket expr closeBracket		{ print("member call[expr]"); }
			;

call: 		call openPar elist closePar							{
 																	print("call call(elist)");
 																	$<exprValue>$ = makeCall($$, $3, scope, getCurrentLine());
 																	print("call call(elist)");
 																}
			|lvalue callsuffix									{
                                                                    print("call lvalue callsuffix");

																	char *name = NULL;
                                                            		if($1->sym->what == function)
                                                            			name = (char*) $1->sym->value.funcVal->name;
																	else
																		name = (char*) $1->sym->value.varVal->name;

                                                                    if(existsLibFunc(name))
																		$1->type = libraryfunc_e;

																	if($2->method) {
																		struct expr *self = $1;

																		struct expr *mi = memberItem(self, nextQuadLabel(), getCurrentLine(), scope, $2->name);
																		$1 = emitIfTableItem(mi, nextQuadLabel(), getCurrentLine(), scope);

																		struct expr *tmp = $2->elist;
																		tmp->prev = self; // TODO - Error at methodCall
																		self->next = tmp;
																		$2->elist = self;
																	}

																	$<exprValue>$ = makeCall($1, $2->elist, scope, getCurrentLine());

																	print("call lvalue callsuffix");
																}
			|openPar funcdef closePar openPar elist closePar	{
																	print("call (funcdef)(elist)");

																	struct expr *func = newExpr(programfunc_e);
																	func->sym = $2;
																	$<exprValue>$ = makeCall(func, $5, scope, getCurrentLine());

																	print("call (funcdef)(elist)");
																}
			;

callsuffix:	normcall		{
 								print("callsuffix normcall");
 								$<methodCall>$ = $1;
 								print("callsuffix normcall");
 							}
			|methodcall		{
			 					print("callsuffix methodcall");
			 					$<methodCall>$ = $1;
			 					print("callsuffix methodcall");
			 				}
			;

normcall: 	openPar elist closePar			{
												print("normcall (elist)");

												struct MethodCall* mc = malloc(sizeof(struct MethodCall));
												memset(mc, 0, sizeof(struct MethodCall));
												$<methodCall>$ = mc;
												$<methodCall>$->elist = $2;
												$<methodCall>$->method = 0;
												$<methodCall>$->name = NULL;

												print("normcall (elist)");
											}
			;

methodcall:	ddot ID openPar elist closePar	{
												print("methodcall ..id(elist)");

												struct MethodCall* mc = malloc(sizeof(struct MethodCall));
												memset(mc, 0, sizeof(struct MethodCall));
												$<methodCall>$ = mc;
												$<methodCall>$->elist = $4;
												$<methodCall>$->method = 1;
												$<methodCall>$->name = $<strValue>2;

												print("methodcall ..id(elist)");
											}
			;

elist:		expr					{
										print("elist expr");

										if($1->type == boolexpr_e) {
											$1->sym = newTmp(scope);

											backPatchList($1->trueList, nextQuadLabel());
											emit(assign, newExprConstBool('1'), NULL, $1, nextQuadLabel(), getCurrentLine());
											emit(jump, NULL, NULL, newExprConstNumber(nextQuadLabel() + 2), nextQuadLabel(), getCurrentLine());

											backPatchList($1->falseList, nextQuadLabel());
											emit(assign, newExprConstBool('0'), NULL, $1, nextQuadLabel(), getCurrentLine());
										}

										$<exprValue>$ = $1;
										$<exprValue>$->voidParam = 0;

										print("elist expr");
									}
			|elist comma expr		{
										print("elist elist,expr");

										if($3->type == boolexpr_e) {
											$3->sym = newTmp(scope);

											backPatchList($3->trueList, nextQuadLabel());
											emit(assign, newExprConstBool('1'), NULL, $3, nextQuadLabel(), getCurrentLine());
											emit(jump, NULL, NULL, newExprConstNumber(nextQuadLabel() + 2), nextQuadLabel(), getCurrentLine());

											backPatchList($3->falseList, nextQuadLabel());
											emit(assign, newExprConstBool('0'), NULL, $3, nextQuadLabel(), getCurrentLine());
										}

										struct expr *tmp = $1;
										while(tmp->next != NULL)
											tmp = tmp->next;

										tmp->next = $3;
										$3->prev = tmp;
										$<exprValue>$ = $1;
										$<exprValue>$->voidParam = 0;

										print("elist elist,expr");
									}
			|/* empty */			{
										print("elist /*empty*/");

										$$ = NULL;
										//$$ = newExprNil();
										//$$->voidParam = 1;

										print("elist /*empty*/");
									}
			;

objectdef:	openBracket elist closeBracket			{
														print("objectdef [elist]");

														struct expr *t = newExpr(newtable_e);
														t->sym  = newTmp(scope);
														emit(tablecreate, NULL, NULL, t, nextQuadLabel(), getCurrentLine());

														struct expr *tmp = $2;
														int i = 0;

														while(tmp != NULL) {
															emit(tablesetelem, t, newExprConstNumber(i++), tmp, nextQuadLabel(), getCurrentLine());
															tmp = tmp->next;
														}
														$<exprValue>$ = t;

														print("objectdef [elist]");
													}
			|openBracket indexed closeBracket		{
														print("objectdef [indexed]");

														struct expr *t = newExpr(newtable_e);
														t->sym = newTmp(scope);

														emit(tablecreate, NULL, NULL, t, nextQuadLabel(), getCurrentLine());

														struct expr *tmp = $2;
														while(tmp != NULL) {
															emit(tablesetelem, t, tmp->index, tmp->indexedVal, nextQuadLabel(), getCurrentLine());
															tmp = tmp->next;
														}

														$<exprValue>$ = t;

														print("objectdef [indexed]");
													}
			;

indexed:	indexedelem									{
 															print("indexed indexedelem");
 															$<exprValue>$ = $1;
 														}
			|indexed comma indexedelem					{
															print("indexed indexed,indexedelem");

															struct expr *tmp = $1;
															while(tmp->next != NULL)
																tmp = tmp->next;

															tmp->next = $3;
															$<exprValue>$ = $1;

															print("indexed indexed,indexedelem");
														 }
			;

indexedelem: openBlock expr colon expr closeBlock		{
															print("indexedelem {expr:expr}");

															if($4->type == boolexpr_e) {
																$4->sym = newTmp(scope);

																backPatchList($4->trueList, nextQuadLabel());
																emit(assign, newExprConstBool('1'), NULL, $4, nextQuadLabel(), getCurrentLine());
																emit(jump, NULL, NULL, newExprConstNumber(nextQuadLabel() + 2), nextQuadLabel(), getCurrentLine());

																backPatchList($4->falseList, nextQuadLabel());
																emit(assign, newExprConstBool('0'), NULL, $4, nextQuadLabel(), getCurrentLine());
															}

															$<exprValue>$ = newExpr(tableitem_e);
															$$->index = $2;
															$$->indexedVal = $4;

															print("indexedelem {expr:expr}");
														}
			;

block:		openBlock { scopeOps(INCREASE); } stmtlist closeBlock 	{
																		print("block {stmtlist}");
																		toggleActive(scope, 0);
																		scopeOps(DECREASE);
																		print("block {stmtlist}");
																	}
			;

/* ----------------- START - funcdef --------------------- */

funcArgs:	openPar idlist closePar	{
										print("funcArgs (idlist)");
										enterScopeSpace();
										resetFunctionLocalOffset();
										print("funcArgs (idlist)");
									}
			;

funcName: 	ID						{
										print("funcName ID");

										activeFunctions++;
										if(!isLibFunc(yylval.strValue, getCurrentLine())) {
											if(!lookUpScopeRedeclaration(yylval.strValue, function, scope, getCurrentLine(), Funcdef));
										}

										$<strValue>$ = $1;

										print("funcName ID");
									}
			|/* empty */			{
										print("funcName /*empty*/");

										activeFunctions++;
									 	$<strValue>$ = genName();

									 	print("funcName /*empty*/");
									}
			;

funcPre:	FUNCTION funcName	{
									print("funcPre FUNCTION funcName");

									$<symbolTable>$ = insert($<strValue>2, function, USER_FUNC, scope, getCurrentLine(), 1, -1, -1);
									$<symbolTable>$->value.funcVal->iaddress = nextQuadLabel();

									// jump before function so it doesnt run on declaration
									pushFuncJump(nextQuadLabel());
									emit(jump, NULL, NULL, NULL, nextQuadLabel(), getCurrentLine());
									emit(funcstart, NULL, NULL, lvalueExpr($<symbolTable>$), nextQuadLabel(), getCurrentLine());

									push(functionLocalOffset);

									resetFormalArgOffset();
									enterScopeSpace();

									scopeOps(INCREASE);
									funcdefStart = 1 ;

									print("funcPre FUNCTION funcName");
								}
			;

funcPreArgs: funcPre funcArgs	{
									print("funcPreArgs funcPre funcArgs");

									$1->value.funcVal->totalargs = totalFuncArgs;
									totalFuncArgs = 0;

									print("funcPreArgs funcPre funcArgs");
								}
			;

funcBlockStart:	{
					print("funcBlockStart");

					pushLoopCounter(activeLoops);
					activeLoops = 0;

					print("funcBlockStart");
				}
			;

funcBlockEnd:	{
 					print("funcBlockEnd");
 					activeLoops = popLoopCounter();
 					print("funcBlockEnd");
 				}
			;

funcBody:	block	{
 						print("funcBody block");
 						exitScopeSpace();
 						print("funcBody block");
 					}
			;

funcdef:	funcPreArgs funcBlockStart funcBody funcBlockEnd	{
																	print("funcdef funcPreArgs funcBlockStart funcBody funcBlockEnd");

																	// Here we will put the default value for non defined functions
																	patchLabel(popFuncJump(), nextQuadLabel() + 1);

																	activeFunctions--;
																	exitScopeSpace();
																	$<symbolTable>1->value.funcVal->totallocals = functionLocalOffset;

																	functionLocalOffset = pop();

																	$<symbolTable>$ = $<symbolTable>1;
																	emit(funcend, NULL, NULL, lvalueExpr($<symbolTable>$), nextQuadLabel(), getCurrentLine());

																	print("funcdef funcPreArgs funcBlockStart funcBody funcBlockEnd");
																}
			;

/* ----------------- END - funcdef --------------------- */

const:		NUMBER				{
 									print("const NUMBER");
 									$<exprValue>$ = newExprConstNumber($1);
 									print("const NUMBER");
 								}
			|STRING				{
			 						print("const STRING");

			 						char* pure = strtok($1, "\"");
								  	$<exprValue>$ = newExprConstString(pure);

								  	print("const STRING");
								}
			|NIL				{
			 						print("const NIL");
			 						$<exprValue>$ = newExprNil();
			 						print("const NIL");
			 					}
			|TRUE				{
			 						print("const TRUE");
			 						$<exprValue>$ = newExprConstBool('1');
			 						print("const TRUE");
			 					}
			|FALSE				{
									print("const FALSE");
									$<exprValue>$ = newExprConstBool('0');
									print("const FALSE");
								}
			;

idlist:		ID					{
									print("idlist ID");

									if(!formalShadow(yylval.strValue, getCurrentLine()))
										insert(yylval.strValue, variable, FORMAL, scope, getCurrentLine(), 1, currentScopeSpace(), currentScopeOffset());

									increaseCurrentScopeOffset();
									totalFuncArgs++;

									print("idlist ID");
								}
			|idlist comma ID	{
									print("idlist idlist,ID");

									totalFuncArgs++;

									if(!formalShadow(yylval.strValue, getCurrentLine()) && !formalRedeclaration(yylval.strValue, scope, getCurrentLine())) {
										insert(yylval.strValue, variable, FORMAL, scope, getCurrentLine(), 1, currentScopeSpace(), currentScopeOffset());
										increaseCurrentScopeOffset();
									}

									print("idlist idlist,ID");
								}
			|/* empty */		{ print("idlist /*empty*/"); }
			;

/* ----------------- START - ifstmt --------------------- */

ifPrefix:	IF openPar expr closePar	{
											print("ifPrefix IF(expr)");

											if($3->type == boolexpr_e) {
												$3->sym = newTmp(scope);

												backPatchList($3->trueList, nextQuadLabel());
												emit(assign, newExprConstBool('1'), NULL, $3, nextQuadLabel(), getCurrentLine());
												emit(jump, NULL, NULL, newExprConstNumber(nextQuadLabel() + 2), nextQuadLabel(), getCurrentLine());
												backPatchList($3->falseList, nextQuadLabel());
												emit(assign, newExprConstBool('0'), NULL, $3, nextQuadLabel(), getCurrentLine());
											}

											emit(if_eq, newExprConstBool('1'), $3, newExprConstNumber(nextQuadLabel() + 2), nextQuadLabel(), getCurrentLine());
											$<intVal>$ = nextQuadLabel();
											emit(jump, NULL, NULL, NULL, nextQuadLabel(), getCurrentLine());

											print("ifPrefix IF(expr)");
										}
			;

elsePrefix:	ELSE 	{
						print("elsePrefix ELSE");

						$<intVal>$ = nextQuadLabel();
						emit(jump, NULL, NULL, NULL, nextQuadLabel(), getCurrentLine());

						print("elsePrefix ELSE");
					}
			;

ifstmt:		ifPrefix stmt				{
											print("ifstmt ifPrefix stmt");

											patchLabel($1, nextQuadLabel());
											$$ = $1;

											print("ifstmt ifPrefix stmt");
										}
			|ifstmt elsePrefix stmt		{
											print("ifstmt ifstmt elseprefix stmt");

											patchLabel($1, $2 + 1);
											patchLabel($2, nextQuadLabel());

											print("ifstmt ifstmt elseprefix stmt");
										}
			;

/* ----------------- END - ifstmt --------------------- */

/* ----------------- START - loopstmt --------------------- */

loopStart:	{
				print("loopStart");

				activeLoops++;
				loopPush();

				print("loopStart");
			}
			;

loopEnd:	{
 				print("loopEnd");

 				activeLoops--;
 				// loopPop will happen below at stmt

 				print("loopEnd");
			}
			;

loopstmt:	loopStart stmt loopEnd	{
										print("loopstmt");

										struct LoopStack *tmp = loopPop();
										assert(tmp);
										$<loopStack>$ = tmp;

										print("loopstmt");
									}
			;

/* ----------------- END - loopstmt --------------------- */


/* ----------------- START - whilestmt --------------------- */

whileStart:	WHILE 	{
 						print("whileStart WHILE");
 						$<intVal>$ = nextQuadLabel();
 						print("whileStart WHILE");
 					}
			;

whileCond:	openPar expr closePar			{
												print("whileCond (expr)");

												if($2->type == boolexpr_e) {
													$2->sym = newTmp(scope);

													backPatchList($2->trueList, nextQuadLabel());
													emit(assign, newExprConstBool('1'), NULL, $2, nextQuadLabel(), getCurrentLine());
													emit(jump, NULL, NULL, newExprConstNumber(nextQuadLabel() + 2), nextQuadLabel(), getCurrentLine());

													backPatchList($2->falseList, nextQuadLabel());
													emit(assign, newExprConstBool('0'), NULL, $2, nextQuadLabel(), getCurrentLine());
												}

												emit(if_eq, $2, newExprConstBool('1'), newExprConstNumber(nextQuadLabel() + 2), nextQuadLabel(), getCurrentLine());
												$<intVal>$ = nextQuadLabel();
												emit(jump, NULL, NULL, NULL, nextQuadLabel(), getCurrentLine());

												print("whileCond (expr)");
											}
			;

whilestmt:	whileStart whileCond loopstmt	{
												print("whilestmt whileStartWhileCond loopstmt");

												emit(jump, NULL, NULL, newExprConstNumber($1), nextQuadLabel(), getCurrentLine());
												patchLabel($2, nextQuadLabel());

												struct BCList *blist = $3->breakList;
												while(blist != NULL) {
													patchLabel(blist->QuadNo, nextQuadLabel());
													blist = blist->next;
												}

												struct BCList *clist = $3->contList;
												while(clist!=NULL) {
													patchLabel(clist->QuadNo, $1);
													clist = clist->next;
												}

												print("whilestmt whileStartWhileCond loopstmt");
											}
			;

/* ----------------- END - whilestmt --------------------- */

/* ----------------- START - forstmt --------------------- */

N:		{
			print("N");

			$<intVal>$ = nextQuadLabel();
			emit(jump, NULL, NULL, NULL, nextQuadLabel(), getCurrentLine());

			print("N");
		}
M:		{
 			print("M");
 			$<intVal>$ = nextQuadLabel();
 			print("M");
 		}

forPrefix:	FOR openPar elist M semicolon expr semicolon	{
				print("forPrefix FOR(elist M; expr;");

				if($6->type == boolexpr_e) {
					$6->sym = newTmp(scope);

					backPatchList($6->trueList, nextQuadLabel());
					emit(assign, newExprConstBool('1'), NULL, $6, nextQuadLabel(), getCurrentLine());
					emit(jump, NULL, NULL, newExprConstNumber(nextQuadLabel() + 2), nextQuadLabel(), getCurrentLine());

					backPatchList($6->falseList, nextQuadLabel());
					emit(assign, newExprConstBool('0'), NULL, $6, nextQuadLabel(), getCurrentLine());
				}

				struct ForLoop_S *flv = malloc(sizeof(struct ForLoop_S));
				$<forLoopVal>$ = flv;
				$<forLoopVal>$->test = $4;
				$<forLoopVal>$->enter = nextQuadLabel();
				emit(if_eq, $<exprValue>6, newExprConstBool('1'), NULL, nextQuadLabel(), getCurrentLine());

				print("forPrefix FOR(elist M; expr;");
			}
			;

forstmt:	forPrefix N elist closePar N loopstmt N		{
				print("forstmt forPrefix N elist) N loopstmt N");

				patchLabel($1->enter, $5 + 1);
				patchLabel($2, nextQuadLabel());
				patchLabel($5, $1->test);
				patchLabel($7, $2 + 1);

				struct BCList *blist = $6->breakList;
				while(blist != NULL) {
					patchLabel(blist->QuadNo, nextQuadLabel());
					blist = blist->next;
				}

				struct BCList *clist = $6->contList;
				while(clist != NULL) {
					patchLabel(clist->QuadNo, $2 + 1);
					clist = clist->next;
				}

				print("forstmt forPrefix N elist) N loopstmt N");
			}
			;

/* ----------------- END - forstmt --------------------- */

returnstmt:	RETURN semicolon		{
										print("returnstmt RETURN;");

										if(activeFunctions == 0){
											printf("Error at line %d, return outside of function.\n", getCurrentLine());
											encounteredError = 1;
										}

										emit(ret, NULL, NULL, NULL, nextQuadLabel(), getCurrentLine());
										emit(jump, NULL, NULL, newExprConstNumber(nextQuadLabel() + 2), nextQuadLabel(), getCurrentLine());

										print("returnstmt RETURN;");
									}
			|RETURN expr semicolon	{
										print("returnstmt RETURN expr;");

										if($2->type == boolexpr_e) {
											$2->sym = newTmp(scope);

											backPatchList($2->trueList, nextQuadLabel());
											emit(assign, newExprConstBool('1'), NULL, $2, nextQuadLabel(), getCurrentLine());
											emit(jump, NULL, NULL, newExprConstNumber(nextQuadLabel() + 2), nextQuadLabel(), getCurrentLine());

											backPatchList($2->falseList, nextQuadLabel());
											emit(assign, newExprConstBool('0'), NULL, $2, nextQuadLabel(), getCurrentLine());
										}

										if(activeFunctions == 0) {
											printf("Error at line %d, return outside of function.\n", getCurrentLine());
											encounteredError = 1;
										}

										emit(ret, NULL, NULL, $<exprValue>2, nextQuadLabel(), getCurrentLine());
										emit(jump, NULL, NULL, newExprConstNumber(nextQuadLabel() + 2), nextQuadLabel(), getCurrentLine());
										$<exprValue>$ = $2;

										print("returnstmt RETURN expr;");
									}
			;

%%

int yyerror(char* yaccProvidedMessage){
    fprintf(stderr, "%s", yaccProvidedMessage);
    fprintf(stderr, "at line %d,  before token: %s\n", yylineno, yytext);
    fprintf(stderr, "INPUT NOT VALID\n");
    encounteredError = 1;
    return -1 ;
}

char *genName() {
	return giveName("$$func_def_", noNameFunctionNum++);
}

void scopeOps(enum ScopeAction action) {
	if(action == INCREASE) {
		if(!funcdefStart)
			scope++;
		else
			funcdefStart = 0;
	} else
		scope--;
}

char* getStringValueQuad(struct expr *e, int isResult) {
	if(e == NULL)
		return "";
	else if (e->type == constnum_e) {
		char *r = malloc(sizeof(numberOfDigits(e->numConst)) + 1);

		// The quad we are going to jump is better to be represented as int in print.
		if(isResult)
			sprintf(r, "%d", (int) e->numConst);
		else {
			double decPart;
			decPart = modf(e->numConst, &decPart);

			if(decPart == 0)
				sprintf(r, "%d", (int) e->numConst);
			else
				sprintf(r, "%lf", e->numConst);
		}
		return r;
	}
	else if(e->type == conststring_e)
		return strdup(e->strConst);
	else if(e->type == constbool_e) {
		if(e->boolConst == '1')
			return "TRUE";
		else
			return "FALSE";
	}
	else if(e->type == nil_e)
		return "nil";

	return strdup(e->sym->value.varVal->name);
}

void printAllQuads() {
	if(encounteredError == 1) {
		print("Finsihed with errors, no quad file will be generated");
		return;
	}

	FILE *file = fopen("quads.txt", "w");
	if(!file) {
		printf("Cannot open quads.txt\n");
		return;
	}

	fprintf(file,"\n%-s%-16s%-15s%-15s%-s\n",
				"#|\t", "OP|", "ARG1|", "ARG2|", "LBL|");
	fprintf(file, "----------------------------------------------------------\n");

	for(int i = 0; i < nextQuadLabel(); i++) {
		fprintf(file, "%-d%-s%-12s\t", quads[i].label, ":\t", opcodeToString[quads[i].op]);
		fprintf(file, "%-15s%-15s%-15s\n",
					getStringValueQuad(quads[i].arg1,0), getStringValueQuad(quads[i].arg2, 0), getStringValueQuad(quads[i].result, 1));
	}

	fclose(file);
}

void print(char *message) {
	printf("Message (line %d): %s\n", getCurrentLine(), message);
}

void makeBinaryFile(char *name) {
	if(encounteredError == 1) {
		print("Finsihed with errors, no binary file will be generated");
		return;
	}

	createBinaryFile(name);
}

int main(int argc, char **argv) {
    FILE *f_in = NULL;
    FILE *f_out = NULL;

    if (argc > 1) {
        if (!(f_in = fopen(argv[1], "r"))) {
            fprintf(stderr, "Cannot open input file: %s\n", argv[1]);
            return 1;
        }
    }

    if (argc > 2) {
        if (!(f_out = fopen(argv[2], "w"))) {
            fprintf(stderr, "Cannot open output file: %s\n", argv[2]);
            return 1;
        }
    }

    yyin = f_in != NULL ? f_in : stdin;
	yyout = f_out != NULL ? f_out : stdout;

	initIntermediate();
	initTable();
    addBuiltinFunctionsTo();

	yyparse();

	emit(no_op, NULL, NULL, NULL, nextQuadLabel(), getCurrentLine());
	printAllQuads();

	generateTcode(nextQuadLabel());
	makeBinaryFile("a.bin");

    if (argc > 1) fclose(f_in);
    if (argc > 2) fclose(f_out);

    return 0;
}
