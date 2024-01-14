%code requires {
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <stdbool.h>
    #include <stdio.h>
    #include "types.h"
    extern Query q;
	int yylex();
	void yyerror(const char *s);
}

%union{
    int bool_value;
    int int_value;
    double double_value;
    char *string;

    FunctionType func_type;
    Filter *filter;
    Element *el;
}

%{
Query q = {};
int tab_level = 0;
%}


/* symbols */
%token LPAREN RPAREN LBRACKET RBRACKET PIPE SLASHSLASH SLASH AT

/* functions */
%token <string> UPDATE DELETE CREATE ASTERISK
%token EOL

%token <int_value> INT_T
%token <bool_value> BOOL_T
%token <string> WORD_T
%token <double_value> DOUBLE_T

/* filter tokens */
%token <int_value> EQUALS_T NOT_EQUALS_T LESS_THAN_T GREATER_THAN_T

%type <string> function_call
%type <string> function_name
%type <string> attribute
%type <filter> filter_expr
%type <filter> filters
%type <filter> filter
%type <string> node
%type <int_value> compare_op
%type <el> node_value

%%

query
    : %empty  /* empty */
    | query EOL
    | query node
    | query node filters
    | function_call EOL
    ;

node
    : SLASH WORD_T {
        add_node(&q, $2);
        $$ = $2;
    }
    | WORD_T {
        add_node(&q, $1);
        $$ = $1;
    }
    ;

function_call
    : function_name LPAREN query RPAREN {
        q.func = get_function_type($1);
    }
    ;

function_name : CREATE | UPDATE | DELETE

filters
    : filter {
        add_filter(&q, $1);
    }
    | filters filter {
        add_filter(&q, $2);
    }
    ;

filter
    : LBRACKET filter_expr RBRACKET {
        $$ = $2;
    }
    | attribute {
        $$ = create_filter_by_var_name($1);
    }
    ;

filter_expr
    : node_value {
        $$ = create_filter_single_value($1);
    }
    | attribute compare_op node_value {
        $$ = create_filter($1, $2, $3);
    }
    ;

node_value
    : BOOL_T { $$ = create_boolean($1);  }
    | INT_T { $$ = create_number($1); }
    | DOUBLE_T { $$ = create_double($1); }
    | WORD_T { $$ = create_string($1); }

attribute
    : AT WORD_T { $$ = $2; }
    ;

compare_op: EQUALS_T | NOT_EQUALS_T | LESS_THAN_T | GREATER_THAN_T;

%%

void yyerror(const char *s) {
    fprintf(stderr, "error: %s\n", s);
}

