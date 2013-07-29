
#define YYDEBUG

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include "getcgi.h"
#include "util.h"
int
main (int argc, char **argv)
{
    int pid, i, c;
    int input_pipe[2], result_pipe[2];
    FILE *input_stream, *result_stream;
    char *editedText;
    char *login = "proc_table"; /* default login name */
    char *request_method = getenv("REQUEST_METHOD") ;
    char **cgivars = getcgivars();


    if (strcmp(request_method, "POST") == 0) {
	set_login_name(login);
	editedText = getvalue("editedText", cgivars);

	/* Create a pipe so parent can write the pml code to child
	   process running pmlcheck. */
	if (pipe(input_pipe) == -1) {
	    perror("error creating read pipe");
	    exit(EXIT_FAILURE);
	}

	if (pipe(result_pipe) == -1) {
	    perror("error creating write pipe");
	    exit(EXIT_FAILURE);
	}

	/* 
	 * Now, create a child process to run pmlcheck.  In the Unix
	 * world, to run a program one must first create a new
	 * process, which is done by cloning an existing process.
	 * This is done with the fork() system call, which simply
	 * makes a copy of the running program, then returns a value
	 * to both copies.  The parent gets the process id of the
	 * forked child copy as return value, while the child gets 0.
	 */
	if ((pid = fork()) < 0) {
	    /* A negative return value indicates an error. */
	    perror("error forking child");
	    exit(EXIT_FAILURE);
	} else if (pid == 0) {
	    /* 
	     * This is the child (clone) process.  It will communicate
	     * with the parent via its stdout; the parent will then
	     * write the result to the server, which will send it it
	     * the browser.
	     */

	    /* 
	     * First, connect the stdin of this child process to read
	     * end of the read pipe.  The unfortunately named dup2() makes
	     * its second argument be a copy of its first argument,
	     * after first closing the second argument if it's open.
	     * So, the following will make stdin a copy of the read
	     * end of the pipe.  This is important, because after
	     * calling exec() the executed program will read from the
	     * pipe rather than some other stream.
	     */
	    close(input_pipe[1]);	/* Close write end; child is reading.  */
	    dup2(input_pipe[0], STDIN_FILENO); /* Bind read end to stdin. */

	    /* Do the inverse for stdout */
	    close(result_pipe[0]);	/* Close read end; child is writing.  */
	    dup2(result_pipe[1], STDOUT_FILENO); /* Bind write end to stdout. */

	    /*
	     * At last, run pmlcheck.  It will read from stdin, which
	     * is bound to the read end of the pipe, and write to
	     * stdout, which is bound to the write end.
	     */

	    execl("pmlcheck", "pmlcheck", (char *)NULL);

	    /* If execution reaches here, execl() failed. */
	    write(STDOUT_FILENO, "exec pmlcheck failed", strlen("exec pmlcheck failed")); 
	    exit(EXIT_FAILURE);
	} else {
	    /* 
	     * This code executes in the parent, which should write
	     * the pml code to the pipe, then read pmlcheck's output
	     */

	    /* First, print the HTTP header, in this case just the
	       all-important content type. */
	    printf("Content-type: text/xml; charset=UTF-8\r\n\r\n");

            /* Make the pipe ends into stdio FILE pointers.  */
	    close(input_pipe[0]); /* Close read end; parent is writing. */
	    input_stream = fdopen(input_pipe[1], "w"); 

	    close(result_pipe[1]); /* Close write end; parent is reading. */
	    result_stream  = fdopen(result_pipe[0], "r"); 

	    /* Write the pml content and close the stream. */
	    i = 0;
	    while(editedText[i]) {
		fputc(editedText[i++], input_stream);
	    }
	    fputc('\n', input_stream);
	    fclose(input_stream);
	    while((c = fgetc(result_stream)) != EOF) {
		putchar('>');
		putchar(c);
	    }

	    /* Done; exit. */
	    exit(EXIT_SUCCESS);
	}
    }
    return(EXIT_FAILURE);
}

