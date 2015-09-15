
all: .testrun

retry: retry.c
	gcc -Wall -O2 -o retry retry.c

.testrun: retry test.pl testretry.pl
	perl ./test.pl
	touch .testrun
