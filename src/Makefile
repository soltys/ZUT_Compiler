LEX = flex
CC = g++
CFLAGS = -Wall -g -std=c++11

all: tokenizer

tokens_gen.cpp: tokens.l 
	$(LEX) --outfile=tokens_gen.cpp tokens.l 

tokenizer: tokens_gen.cpp
	$(CC) $(CFLAGS) tokens_gen.cpp -lfl -o tokenizer

clean:
	rm tokenizer *_gen.cpp
	

