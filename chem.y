%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int yylex();
extern int yyparse();
extern FILE* yyin;

void yyerror(const char* s);

struct node {
	char type[100];
	char name[100];
	int value;
	struct node *child;
	struct node *child2;
};

char *print_node(struct node *n) {
	printf("%s:", n->type);
	if (strlen(n->name) > 0) {
		printf(" Node name: %s", n->name);
	}
	if (n->value > 0) {
		printf(" Node value: %d", n->value);
	}

	if (n->child != NULL || n->child2 != NULL) {
		printf(" Children (");
		if (n->child != NULL) {
			printf("(");
			print_node(n->child);
			printf(")");
		}
		if (n->child2 != NULL) {
			printf("(");
			print_node(n->child2);
			printf(")");
		}
		printf(")");
	}
}

struct node *make_node(char *type, char *name, int value, struct node *child, struct node *child2) {
	struct node *n = malloc(sizeof(struct node));
	strcpy(n->type, type);
	strcpy(n->name, name);
	n->value = value;
	n->child = child;
	n->child2 = child2;
	return n;
}

%}

%union {
	int ival;
	char *sval;
	struct node *nval;
}

%token<ival> T_NUMBER
%token<sval> T_NAME
%token T_BIARROW T_PARENSTART T_PARENEND T_SET
%left T_PLUS T_RIGHTARROW T_NEWLINE

%type<nval> concentration
%type<nval> reaction

%type<nval> species

%start program

%%

program: concentration T_NEWLINE reaction T_NEWLINE { struct node *n = make_node("Program", "", 0, $1, $3); printf("\n\n"); print_node(n); printf("\n\n"); };

concentration: concentration T_NEWLINE concentration { struct node *n = make_node( "conclist", "", 0, $1, $3); $$ = n; }
			 | T_NAME T_SET T_NUMBER { struct node *n = make_node( "conc", $1, $3, NULL, NULL ); $$ = n; }
			 ;

reaction: reaction T_NEWLINE reaction { struct node *n = make_node("reactionlist", "", 0, $1, $3); $$ = n; }
		| species T_RIGHTARROW species { struct node *n = make_node("reaction", "", 0, $1, $3); $$ = n; }
		;

species: species T_PLUS species { struct node *n = make_node("specieslist", "", 0, $1, $3); $$ = n; }
       | T_NUMBER T_NAME { struct node *n = make_node("species", $2, $1, NULL, NULL); $$ = n; }
       | T_NAME { struct node *n = make_node("species", $1, 1, NULL, NULL); $$ = n; }
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
