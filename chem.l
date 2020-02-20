%option noyywrap
%{
	#include <stdio.h>
	#define YY_DECL int yylex()

	#include "chem.tab.h"

%}

%%

[ \t] ;
[0-9]+  {yylval.ival = atof(yytext); return T_NUMBER;}
[a-zA-Z]+  {yylval.sval = strdup(yytext); return T_NAME;}
"->"  {return T_RIGHTARROW;}
"<->"  {return T_BIARROW;}
"("  {return T_PARENSTART;}
")"  {return T_PARENEND;}
":="  {return T_SET;}
[\n]  {return T_NEWLINE;}

%%
