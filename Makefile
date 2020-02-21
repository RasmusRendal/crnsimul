CXX = g++

all: test chem

chem.tab.c chem.tab.h:	chem.y
	bison -d chem.y

lex.yy.c: chem.l chem.tab.h
	flex chem.l

chem: lex.yy.c chem.tab.c chem.tab.h
	gcc -o chem chem.tab.c lex.yy.c

clean:
	rm chem chem.tab.c lex.yy.c chem.tab.h test

test:
	$(CXX) tests/* -lgtest -o test && ./test
