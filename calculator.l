%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "calculator.tab.h"
%}

%option noyywrap

%%

[ \t]+          { }
\n              { return NEWLINE; }

[0-9]+\.[0-9]+  {
    yylval.dval = atof(yytext);
    return NUMBER;
}

[0-9]+          {
    yylval.dval = atof(yytext);
    return NUMBER;
}

[a-zA-Z][a-zA-Z0-9]* {
    yylval.sval = strdup(yytext);
    return IDENTIFIER;
}

"+"             { return PLUS;   }
"-"             { return MINUS;  }
"*"             { return TIMES;  }
"/"             { return DIVIDE; }
"="             { return ASSIGN; }
"("             { return LPAREN; }
")"             { return RPAREN; }
";"             { return SEMI;   }

.               { fprintf(stderr, "Невідомий символ: %s\n", yytext); }

%%
