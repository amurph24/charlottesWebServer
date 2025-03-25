CC=gcc
FLAGS=-Wall -Wextra -Wpedantic
SOURCEDIR=source
BUILDDIR=build
INCLUDEDIR=include
TESTDIR=test
EXE=server

$(EXE):
	$(CC) $(FLAGS) $(SOURCEDIR)/*.c -I $(INCLUDEDIR) -o $(BUILDDIR)/$@

test: $(EXE) FORCE
	cp $(BUILDDIR)/$(EXE) $(TESTDIR)
	./$(TESTDIR)/$(EXE) 8000
quickstart:
	mkdir -p $(BUILDDIR) $(SOURCEDIR) $(INCLUDEDIR) $(TESTDIR)
	touch $(SOURCEDIR)/main.c
FORCE:

kill:
	pkill -f "./$(BUILDDIR)/$(EXE) 8000"
