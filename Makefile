
# These might (and should) be defined by RPM build
INSTALL=install
BINDIR=$(DESTDIR)/usr/bin
CFLAGS=-g -Wall

# Rely on default rule
retry: retry.c

test: .testrun

.testrun: retry test.pl testretry.pl
	perl ./test.pl
	touch .testrun

disttest:
	perl ./test.pl /usr/bin/retry

install: retry
	$(INSTALL) -D -s retry $(BINDIR)/retry

clean:
	rm -f .testrun retry
