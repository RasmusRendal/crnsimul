# This Makefile is designed to be simple and readable.  It does not
# aim at portability.  It requires GNU Make.

BASE = crnparser
BISON = bison
CXX = g++
FLEX = flex
XSLTPROC = xsltproc

all: $(BASE)

%.cc %.hh %.xml %.gv: %.yy
	$(BISON) $(BISONFLAGS) --xml --graph=$*.gv -o $*.cc $<

%.cc: %.l
	$(FLEX) $(FLEXFLAGS) -o$@ $<

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c -o$@ $<

$(BASE): src/CRN/main.o src/CRN/driver.o src/CRN/CRNParse.o src/CRN/CRNScan.o
	$(CXX) -o $@ $^

$(BASE).o: src/CRN/CRNParse.hh
CRNParse.o: src/CRN/CRNParse.hh
CRNScan.o: src/CRN/CRNParse.hh

clean:
	git clean -fXd
