CXX = g++
CXXFLAGS = -g -Wall -std=c++11
LDLIBS = -lfl
LEX = flex
BISON = bison
EXE = pslang
RM = rm -f
OBJS = lexer.o main.o Driver.o CodeGen.o  parser.o Instruction.o 
all: pslang

pslang: $(OBJS)
	$(CXX) $(CXXFLAGS) $^ $(LDLIBS) -o $(EXE)

lexer.o: lexer.l parser.hh Scanner.hpp
	$(LEX) --outfile=lexer.cc $<
	$(CXX) $(CXXFLAGS) -c -o $@ lexer.cc

parser.o parser.hh: parser.y Node.h
	bison -v $<
	$(CXX) $(CXXFLAGS) -c parser.cc -o parser.o

main.o: main.cpp

Driver.o: Driver.cpp Driver.h

CodeGen.o :CodeGen.cpp Node.h

Instruction.o: Instruction.cpp Instruction.h

.PHONY: clean
clean:
	$(RM) *.hh 
	$(RM) *.cc
	$(RM) parser.output
	$(RM) *.o
	$(RM) $(EXE)


