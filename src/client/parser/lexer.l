%{
#include <stdio.h>
#include <stdlib.h>
#include "y.tab.h"
#include "types.h"
%}

%%
"(" { return LPAREN; }
")" { return RPAREN; }
"[" { return LBRACKET; }
"]" { return RBRACKET; }
"|" { return PIPE; }
"//" { return SLASHSLASH; }
"/" { return SLASH; }
"@" { return AT; }

"="  {yylval.int_value = 0; return EQUALS_T;}
"!=" {yylval.int_value = 1; return NOT_EQUALS_T;}
"<"  {yylval.int_value = 2; return LESS_THAN_T;}
">"  {yylval.int_value = 3; return GREATER_THAN_T;}

"update"       {yylval.string = strdup(yytext); return UPDATE;}
"delete"       {yylval.string = strdup(yytext); return DELETE;}
"create"       {yylval.string = strdup(yytext); return CREATE;}
"select_all"   {yylval.string = strdup(yytext); return ASTERISK;}

\n { return EOL; }

[a-zA-Z_]* { yylval.string = strdup(yytext); return WORD_T; }
([+-]?[0-9])+           {yylval.int_value = atoi(yytext); return INT_T;}
[+-]?([0-9]*[.])?[0-9]+ {yylval.double_value = atof(yytext); return DOUBLE_T;}

. { printf("Unrecognized character: %s\n", yytext); exit(1); }

%%

int yywrap(void){
	return 1;
}
