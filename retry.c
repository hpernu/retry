/*
 ============================================================================
 Name        : retry.c
 Author      : Heikki Pernu
 Version     :
 Copyright   :
 Description : Retry ptog in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>


void usage() {
	fprintf(stderr,"%s [-hv] [-t timeout] times delay cmd [cmdarg] [cmdarg] [...]\n","retry");
	exit(EXIT_FAILURE);
}

/**
 * Convert a time in arg string to seconds in integer
 */
int totime(char *arg) {
	if (strlen(arg)>0) {
		int number=atoi(arg);

		switch (*(arg+strlen(arg)-1)) {
		case 'h': // Hours
			return number*60*60;
		case 'm': // Minutes
			return number*60;
		default:
			return number;
		}
	}
	return 0 ;
}

extern char *optarg;
extern int optind;

int main(int argc,char * const argv[]) {
	int opt=0 ;
	int verbosity=0;
	int timeout=0;
	int count=0;
	int delay=0;
	int expect=0; // What value to expect

	while ((opt=getopt(argc,argv,"+hvt:"))!= -1) {
		switch (opt) {
			case 'h':
				usage();
				break;
			case 'v':
				verbosity++;
				break;
			case 't':
				timeout=totime(optarg);
				break;
			default:
				fprintf(stderr,"Unknown option %c\n",opt);
				usage();
				break;
		}
	}
	argv+=optind;
	argc-=optind;

	if (argc<3) {
		fprintf(stderr,"At least three arguments expected\n");
		usage();
		/* NOT REACHED */
	}
	count=totime(argv[0]); argv++;
	delay=totime(argv[0]); argv++;

	// Do fork, run the actual command in the background, possibly with timeout
	pid_t pid=0;
	int status;
	FILE *out=NULL;
	FILE *err=NULL;

	do {
		if (out!=NULL) fclose(out);
		if (err!=NULL) fclose(err);
		if ((out=tmpfile())==NULL) {
			perror("Unable to create temp file for stdout buffer");
			exit(-1);
		}
		if ((err=tmpfile())==NULL) {
			perror("Unable to create temp file for stderr buffer");
			exit(-1);
		}

		if ((pid=fork())==0) {
			/* CHILD
			 * Run the actual program */
			// Should not go outside of execvp
			alarm(timeout); // Timeout with signal if timeout reached
			if (dup2(fileno(out),STDOUT_FILENO)==-1) {
				perror("Unable to redirecto stdout in subprocess");
				exit(-1);
			}
			fclose(out);
			if (dup2(fileno(err),STDERR_FILENO)==-1) {
				perror("Unable to redirecto stderr in subprocess");
				exit(-1);
			}
			fclose(err);
			exit(execvp(argv[0],argv));
		}
		/* PARENT
		 * Wait for child termination */
		wait(&status);
		if (WEXITSTATUS(status)!=expect) sleep(delay);
	} while (--count>0 && WEXITSTATUS(status)!=expect ) ;
	fflush(out);
	fflush(err);
	struct stat st ;

	// Copy saved stdout from subprocess to real stdout
	if (fstat(fileno(out),&st)!=0) {
		perror("Unable to check the stdout of subprocess");
		exit(-1);
	} else if (st.st_size>0) {
		char buf[4096];
		size_t n;
		fseek(out,0,SEEK_SET);
		while (!feof(out)) {
			n=fread(buf,1,sizeof(buf),out);
			fwrite(buf,1,n,stdout);
		}
	}
	fflush(stdout);

	// Copy saved stderr from subprocess to real stderr
	if (fstat(fileno(err),&st)!=0) {
		perror("Unable to check the stdout of subprocess");
		exit(-1);
	} else if (st.st_size>0) {
		char buf[4096];
		size_t n;
		fseek(err,0,SEEK_SET);
		while (!feof(err)) {
			n=fread(buf,1,sizeof(buf),err);
			fwrite(buf,1,n,stderr);
		}
	}
	fflush(stderr);

	return WEXITSTATUS(status);
}
