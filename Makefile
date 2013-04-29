SRCDIR = src
BINDIR = bin
TESTDIR = test
DOCDIR = doc
CP = cp -f
RM = rm -f
EXE = pslang
all: $(EXE) test

pslang:
	$(MAKE) -C $(SRCDIR)
	mkdir -p $(BINDIR)
	$(CP) $(SRCDIR)/$@ $(BINDIR)/$@

doc:
	$(MAKE) -C $(DOCDIR)

clean:
	$(MAKE) -C $(SRCDIR) clean
	$(RM) bin/*

rebuild:
	$(MAKE) clean
	$(MAKE) all

test:
	$(MAKE) -C $(TESTDIR) all
	$(MAKE) -C $(TESTDIR) llvm

.PHONY: pslang clean rebuild test doc