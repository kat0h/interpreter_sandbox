%{
#include <stdio.h>
#include "parse.tab.h"

int yylex();
int yyerror(char const *str);
%}

%union {
  double num;
}

%type<num> expr primary
%token CR
%token<num> NUMBER
%left '+' '-'
%left '*' '/'

%%

program : CR 
        | expr CR {
          printf("%f\n", $1);
        }
        ;

expr  : expr '+' expr { $$ = $1 + $3; }
      | expr '-' expr { $$ = $1 - $3; }
      | expr '*' expr { $$ = $1 * $3; }
      | expr '/' expr { $$ = $1 / $3; }
      | primary
      ;

primary : NUMBER { $$ = yylval.num; }
        | '(' expr ')' { $$ = $2; }
        ;

%%

int yyerror(char const *str) { 
    fprintf(stderr, "parser error\n"); 
    return 0; 
}
