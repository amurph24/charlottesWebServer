CC=gcc
FLAGS=-Wall -Wextra -Wpedantic
SOURCEDIR=source
BUILDDIR=build
INCLUDEDIR=include
TESTDIR=test




server:
	$(CC) $(FLAGS) $(SOURCEDIR)/*.c -o $(BUILDDIR)/$@

test: FORCE
	./build/server
quickstart:
	mkdir -p $(BUILDDIR) $(SOURCEDIR) $(INCLUDEDIR) $(TESTDIR)
	touch $(SOURCEDIR)/main.c
FORCE:
