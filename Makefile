# This Makefile is designed to be simple and readable.  It does not
# aim at portability.  It requires GNU Make.

BASE = main
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

$(BASE): $(BASE).o driver.o GCRNParse.o GCRNScan.o
	$(CXX) -o $@ $^

$(BASE).o: GCRNParse.hh
GCRNParse.o: GCRNParse.hh
GCRNScan.o: GCRNParse.hh

run: $(BASE)
	@echo "Type arithmetic expressions.  Quit with ctrl-d."
	./$< -

html: GCRNParse.html
%.html: %.xml
	$(XSLTPROC) $(XSLTPROCFLAGS) -o $@ $$($(BISON) --print-datadir)/xslt/xml2xhtml.xsl $<

CLEANFILES =										\
  $(BASE) *.o										\
  GCRNParse.hh GCRNParse.cc GCRNParse.output GCRNParse.xml GCRNParse.html GCRNParse.gv location.hh	\
  GCRNScan.cc
clean:
	rm -f $(CLEANFILES)
