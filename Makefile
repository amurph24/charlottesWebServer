CC=gcc
CCFLAGS=-Wall -Wextra -Wpedantic -Werror
LIBS=-lpthread
SOURCEDIR=source
BUILDDIR=build
INCLUDEDIR=include
TESTDIR=test

# all source *.c files should be compiled into .o files
SOURCEFILES = $(shell find $(SOURCEDIR) -name '*.c')
OBJFILES = $(patsubst %.c, $(BUILDDIR)/%.o, $(notdir $(SOURCEFILES)))
EXE=cwserver

# all test *.c files should be compiled into .o files
TESTFILES = $(shell find $(TESTDIR) -name '*.c')
TESTOBJFILES = $(patsubst %.c, $(BUILDDIR)/%.o, $(notdir $(TESTFILES)))


$(EXE): $(BUILDDIR)/$(EXE)
$(BUILDDIR)/$(EXE): $(OBJFILES)
	$(CC) $(CCFLAGS) -o $@ $^ $(LIBS)

$(OBJFILES): $(BUILDDIR)/%.o: $(SOURCEDIR)/%.c
	$(CC) $(CCFLAGS) -I $(INCLUDEDIR) -c $< -o $@


$(BUILDDIR)/test_$(EXE): $(TESTOBJFILES) $(filter-out $(BUILDDIR)/main.o, $(OBJFILES))
	$(CC) $(CCFLAGS) -o $@ $^ $(LIBS)

$(TESTOBJFILES): $(BUILDDIR)/%.o: $(TESTDIR)/%.c
	$(CC) $(CCFLAGS) -I $(INCLUDEDIR) -c $< -o $@

test: $(BUILDDIR)/test_$(EXE)
	./$(BUILDDIR)/test_$(EXE)


run: $(EXE)
	valgrind -s ./$(BUILDDIR)/$(EXE) 8000 &

FORCE:

clean:
	-rm $(BUILDDIR)/*.o
	-rm $(BUILDDIR)/$(EXE)
	-rm $(BUILDDIR)/test_$(EXE)

kill:
	-pkill -f "./$(BUILDDIR)/$(EXE) 8000"
	-pkill -f "./$(TESTDIR)/$(EXE) 8000"
