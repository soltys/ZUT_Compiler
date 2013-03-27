CXX = g++
CXXFLAGS = -g -Wall -std=c++11
LDLIBS = -lfl
LEX = flex
BISON = bison
EXE = pslang
RM = rm -f
OBJS =  main.o  Driver.o  Symbol.o Node.o CodeGen.o Instruction.o 
GENOBJS = lexer.o parser.o
ALLOBJS = $(OBJS) $(GENOBJS)
all: pslang

pslang: $(ALLOBJS)
	$(CXX) $(CXXFLAGS) $^ $(LDLIBS) -o $(EXE)

lexer: lexer.o
	$(CXX) $(CXXFLAGS) $^ $(LDLIBS) -o $@ 

lexer.o: lexer.l parser.hh Scanner.hpp
	$(LEX) --outfile=lexer.cc $<
	$(CXX) $(CXXFLAGS) -c -o $@ lexer.cc

parser.o parser.hh: parser.y Node.h
	bison -v $<
	$(CXX) $(CXXFLAGS) -c parser.cc -o parser.o

$(OBJS): %.o: %.cpp %.h $(GENOBJS)
	$(CXX) -c $(CXXFLAGS) $< -o $@


.PHONY: clean
clean:
	$(RM) *.hh 
	$(RM) *.cc
	$(RM) parser.output
	$(RM) *.o
	$(RM) *~
	$(RM) $(EXE)


