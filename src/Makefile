CXX = g++
CXFLAGS = -Wall
LEX = flex
BISON = bison
EXE = pslang

all: lexer.cc parser.cc
	$(CXX) lexer.cc parser.cc -lfl -o $(EXE)

lexer.cc: lexer.l parser.hh
	$(LEX) --outfile=lexer.cc lexer.l

parser.cc parser.hh: parser.y
	bison -v parser.y


.PHONY: clean
clean:
	rm *.hh
	rm *.cc
	rm $(EXE)
	rm parser.output