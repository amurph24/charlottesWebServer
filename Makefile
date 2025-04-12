CC=gcc
FLAGS=-Wall -Wextra -Wpedantic
SOURCEDIR=source
BUILDDIR=build
INCLUDEDIR=include
TESTDIR=test

# all .c files should be compiled into .o files
SOURCEFILES = $(shell find $(SOURCEDIR) -name *.c)
OBJFILES = $(patsubst %.c, $(BUILDDIR)/%.o, $(notdir $(SOURCEFILES)))
EXE=server

$(EXE): $(BUILDDIR)/$(EXE)
$(BUILDDIR)/$(EXE): $(OBJFILES)
	$(CC) $(CCFLAGS) -o $@ $^

$(OBJFILES): $(BUILDDIR)/%.o: $(SOURCEDIR)/%.c
	$(CC) $(CCFLAGS) -I $(INCLUDEDIR) -c $< -o $@

test: $(EXE)
	cp $(BUILDDIR)/$(EXE) $(TESTDIR)
	./$(TESTDIR)/$(EXE) 8000

FORCE:

clean:
	-rm $(BUILDDIR)/*.o
	-rm $(BUILDDIR)/$(EXE)
kill:
	-pkill -f "./$(BUILDDIR)/$(EXE) 8000"
	-pkill -f "./$(TESTDIR)/$(EXE) 8000"
