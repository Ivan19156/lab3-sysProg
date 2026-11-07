%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

int yylex(void);
void yyerror(const char *s);

ASTNode *root = NULL;
%}

%union {
    double  dval;
    char   *sval;
    ASTNode *node;
}

%token <dval> NUMBER
%token <sval> IDENTIFIER
%token PLUS MINUS TIMES DIVIDE ASSIGN LPAREN RPAREN NEWLINE SEMI

%type <node> expr term factor unary

%left PLUS MINUS
%left TIMES DIVIDE
%right UMINUS

%%

program:
      program stmt
    | 
    ;

stmt:
      expr NEWLINE {
          printf("= %.6g\n", eval($1));
          printf("AST:\n");
          print_ast($1, 0);
          printf("JSON:\n");
          ast_to_json($1);
          free_ast($1);
      }
    | expr SEMI {
          printf("= %.6g\n", eval($1));
          printf("AST:\n");
          print_ast($1, 0);
          printf("JSON:\n");
          ast_to_json($1);
          free_ast($1);
      }
    | NEWLINE
    ;

expr:
      IDENTIFIER ASSIGN expr  { $$ = make_assign($1, $3); free($1); }
    | expr PLUS  term         { $$ = make_binop(NODE_ADD, $1, $3); }
    | expr MINUS term         { $$ = make_binop(NODE_SUB, $1, $3); }
    | term                    { $$ = $1; }
    ;

term:
      term TIMES  factor      { $$ = make_binop(NODE_MUL, $1, $3); }
    | term DIVIDE factor      { $$ = make_binop(NODE_DIV, $1, $3); }
    | factor                  { $$ = $1; }
    ;

factor:
      MINUS factor %prec UMINUS  { $$ = make_neg($2); }
    | unary                      { $$ = $1; }
    ;

unary:
      NUMBER                  { $$ = make_number($1); }
    | IDENTIFIER              { $$ = make_var($1); free($1); }
    | LPAREN expr RPAREN      { $$ = $2; }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Помилка синтаксису: %s\n", s);
}

int main(void) {
    printf("Калькулятор з AST. Введіть вираз (або Ctrl+D для виходу):\n");
    yyparse();
    return 0;
}
