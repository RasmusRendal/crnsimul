%{
	#include <stdio.h>
%}

number [0-9]+
specieName [a-zA-Z]+
rightReactionArrow ->
bidirectionalReactionArrow <->
parenStart "("
parenEnd ")"

%%

{number} {printf("Number");}
{specieName} {printf("SpecieName");}
{rightReactionArrow} {printf("RightArrow");}
{bidirectionalReactionArrow} {printf("BidirectionalReactionArrow");}
{parenStart} {printf("parenStart");}
{parenEnd} {printf("parenEnd");}

%%
