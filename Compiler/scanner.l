%{
#include "string.h"

#include "parser.h"
#include "../Compiler/lex/lexer.h"

%}

%option noyywrap
%option yylineno
/*%option nounput*/

%%
"if"		{ return IF; }
"else"		{ return ELSE; }
"while"		{ return WHILE; }
"for"		{ return FOR; }
"function"	{ return FUNCTION; }
"return"	{ return RETURN; }
"break"		{ return BREAK; }
"continue"	{ return CONTINUE; }
"and"		{ return AND; }
"not"		{ return NOT; }
"or"		{ return OR; }
"local"		{ return local; }
"true"		{ return TRUE; }
"false"		{ return FALSE; }
"nil"		{ return NIL; }

"=="		{ return eequal; }
"!="		{ return nequal; }
"++"		{ return pplus; }
"--"		{ return mminus; }

"="			{ return equal; }
"+"			{ return plus; }
"-"			{ return minus; }
"*"			{ return MUL; }
"/"			{ return DIV; }
"%"			{ return MOD; }

">"			{ return greater; }
"<"			{ return lesser; }
">="		{ return gequal; }
"<="		{ return lequal; }

"{"			{ return openBlock; }
"}"			{ return closeBlock; }
"["			{ return openBracket; }
"]"			{ return closeBracket; }
"("			{ return openPar; }
")"			{ return closePar; }

";"			{ return semicolon; }
","			{ return comma; }
"::"		{ return ccolon; }
":"			{ return colon; }
".."		{ return ddot; }
"."			{ return dot; }


"/*"		{
				alpha_token_t yyval;
				multiline_comments_check(yyval,"/*",COMMENTS);
			}
"//"		{
				int c;
				while((c = input()) != '\n' && c != EOF && c != 0) ; // ignore all chars till you find a new line char.
					line_number_glb++;
			}
"\n"		{ line_number_glb++; }

[0-9]+\.[0-9]* {
                  double d ;
                  sscanf(yytext,"%lf",&d);
                  yylval.numVal = d;
                  return NUMBER;
               }
[0-9]+ {
            double d ;
            sscanf(yytext,"%lf",&d);
            yylval.numVal = d;
            return NUMBER;
        }

[a-zA-Z][a-zA-Z0-9_]* {
                         yylval.strValue = strdup(yytext);
                         return ID;
                      }
[\"]([^"]|\\.)*[\"] {
						/* Accepts " and then or \(escape char) or anything that is not ", and closes with " */
						alpha_token_t yyval;
						string_check(yyval, yytext, STRINGCONST);
						yylval.strValue = strdup(yytext);
						return STRING;
					}
[\"][^"]* { printf("Non closing string at line: %d\n", line_number_glb); }
[ \t]* /* ignores whitespaces */;

.	{ printf("Invalid type '%s' in line: %d\n", yytext, line_number_glb); }

%%

int getCurrentLine() {
	return line_number_glb;
}