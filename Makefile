CC=gcc
FLAGS=-Wall -Wextra -Wpedantic
SOURCEDIR=source
BUILDDIR=build
INCLUDEDIR=include
TESTDIR=test




server:
	$(CC) $(FLAGS) $(SOURCEDIR)/*.c -I $(INCLUDEDIR) -o $(BUILDDIR)/$@

test: server FORCE
	./build/server 8000
quickstart:
	mkdir -p $(BUILDDIR) $(SOURCEDIR) $(INCLUDEDIR) $(TESTDIR)
	touch $(SOURCEDIR)/main.c
FORCE:
