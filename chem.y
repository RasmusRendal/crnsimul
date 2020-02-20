%{

#include <stdio.h>
#include <stdlib.h>

extern int yylex();
extern int yyparse();
extern FILE* yyin;

void yyerror(const char* s);
%}

%union {
	int ival;
	char *sval;
}

%token<ival> T_NUMBER
%token<sval> T_NAME
%token T_BIARROW T_PARENSTART T_PARENEND T_SET
%left T_PLUS T_RIGHTARROW T_NEWLINE

%type concentration
%type reaction

%type species

%start program

%%

program:
	   | concentration T_NEWLINE reaction T_NEWLINE {printf("Program");}
;

concentration:
				  | concentration T_NEWLINE concentration {printf("Conc");}
				  | T_NAME T_SET T_NUMBER {printf("The concentration of %s is %d", $1, $3);}
				  ;

reaction:
		 | reaction T_NEWLINE reaction
		 | species T_RIGHTARROW species {printf("One reaction");}
		 ;

species:
		   | species T_PLUS species
		   | T_NUMBER T_NAME {printf("Species");}
		   | T_NAME {printf("Species");}
		   ;

%%

int main() {
	yyin = stdin;

	do {
		yyparse();
	} while(!feof(yyin));

	return 0;
}

void yyerror(const char* s) {
	fprintf(stderr, "Parse error: %s\n", s);
	exit(1);
}
