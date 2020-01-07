
# These might (and should) be defined by RPM build
INSTALL=install
BINDIR=$(DESTDIR)/usr/bin
CFLAGS=-g -Wall

# Rely on default rule
retry: retry.c version.h

all: retry

# Build version.h on-fly
version.h: .git/ Makefile
	printf 'const char *version="%s";\n' `git tag | tail -n 1` > version.h.tmp
	mv version.h.tmp version.h

test: .testrun

.testrun: retry test.pl testretry.pl
	./retry --version
	perl ./test.pl
	touch .testrun

disttest:
	perl ./test.pl /usr/bin/retry

install: retry
	$(INSTALL) -D -s retry $(BINDIR)/retry

clean:
	rm -f .testrun retry
