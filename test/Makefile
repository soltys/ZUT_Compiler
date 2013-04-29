EXE = ../bin/pslang
LLVM_ARGS = --llvm
all: input.psl llvm
	$(EXE) input.psl

llvm: llvm_test.psl
	$(EXE) $(LLVM_ARGS) -i llvm_test.psl 2> out.bc

.PHONY: all llvm