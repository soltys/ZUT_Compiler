SRCDIR = src
BINDIR = bin
CP = cp -f
RM = rm -f
all: pslang

pslang:
	$(MAKE) -C $(SRCDIR)
	mkdir -p $(BINDIR)
	$(CP) $(SRCDIR)/$@ $(BINDIR)/$@

clean:
	$(MAKE) -C $(SRCDIR) clean
	$(RM) bin/*
.PHONY: pslang clean