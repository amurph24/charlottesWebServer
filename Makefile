CC=gcc
SOURCEDIR=source
BUILDDIR=build
INCLUDEDIR=include
TESTDIR=test




server:
	$(CC) $(SOURCEDIR)/*.c -o $(BUILDDIR)/$@

test: FORCE
	./build/server
quickstart:
	mkdir -p $(BUILDDIR) $(SOURCEDIR) $(INCLUDEDIR) $(TESTDIR)
	touch $(SOURCEDIR)/main.c
FORCE:
