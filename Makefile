SRCDIR = src
BINDIR = bin
TESTDIR = test
CP = cp -f
RM = rm -f
EXE = pslang
all: $(EXE) test

pslang:
	$(MAKE) -C $(SRCDIR)
	mkdir -p $(BINDIR)
	$(CP) $(SRCDIR)/$@ $(BINDIR)/$@

clean:
	$(MAKE) -C $(SRCDIR) clean
	$(RM) bin/*

rebuild:
	$(MAKE) clean
	$(MAKE) all

test:
	$(MAKE) -C $(TESTDIR) all

.PHONY: pslang clean rebuild test