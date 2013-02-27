CXX = g++
CXXFLAGS = -g -Wall -std=c++11
LEX = flex
BISON = bison
EXE = pslang

all: pslang

pslang: parser.o lexer.o main.o Driver.o
	$(CXX) $(CXXFLAGS) $^ -lfl -o $(EXE)

lexer.o: lexer.l parser.hh
	$(LEX) --outfile=lexer.cc $<
	$(CXX) $(CXXFLAGS) -c -o lexer.o lexer.cc

parser.o parser.hh: parser.y
	bison -v $<
	$(CXX) $(CXXFLAGS) -c parser.cc -o parser.o

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o

Driver.o: Driver.cpp Driver.h
	$(CXX) $(CXXFLAGS) -c Driver.cpp -o $@

.PHONY: clean
clean:
	rm *.hh 
	rm *.cc
	rm parser.output
	rm *.o
	rm $(EXE)


