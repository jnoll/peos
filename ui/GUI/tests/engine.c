#include "engine.h"
#include "glo.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define SLASH_PEOS "/peos"	/* name of peos cmd, must start with a / */

/* run the peos command with the given command */
/* XXX - should do fork/exec with the child's stdout and stderr going to a file */
int
runPeos (char *cmd)
{
  char *shell_cmd;	/* shell command to run */
  int size;		/* how much to malloc */

  /* firewall */
  if (cmd == NULL) {
	/* XXX - panic here about bogus arg passed */
	return 0;
  }

  /* form the command with >message 2>&1 on the end */
  size = strlen(cmd) + strlen(" >message 2>&1") + 1;
  shell_cmd = malloc(size+1);
  if (shell_cmd == NULL) {
	/* XXX - panic here about malloc failure */
	return 0;
  }
  shell_cmd[0] = '\0';
  snprintf(shell_cmd, size, "%s >message 2>&1", cmd);
  shell_cmd[size] = '\0';

  /* execute cmd within the shell */
  system(shell_cmd);

  /* cleanup */
  free(shell_cmd);
  shell_cmd = NULL;
  return 0;
}

/*
 * peos_in_dir - look for peos in a directory
 *
 * given:
 *	a directory
 *
 * return:
 *	malloced location of the directory/peos that was found, or
 *	NULL ==> no peos found
 */
/*@null@*/
char *
peos_in_dir(char *dirname)
{
    #define EXIT_FAIL NULL
    size_t size;	/* malloc string size */
    char *peos;		/* potential location of peos */

    /* firewall */
    if (dirname == NULL) {
	return EXIT_FAIL;
    }

    /* form $PWD/peos */
    size = strlen(dirname) + strlen(SLASH_PEOS) + 1;
    peos = (char *) malloc((size_t) size);
    if (peos == NULL) {
	perror("malloc of dirname/peos failed");
	return EXIT_FAIL;
    }
    peos[0] = '\0';
    (void) snprintf(peos, size, "%s%s", dirname, SLASH_PEOS);
    peos[size] = '\0';

    /* look for peos in the directory */
    if (access(peos, X_OK) != 0) {
	/* no peos in directory, return NULL */
	free(peos);
	peos = NULL;
        return EXIT_FAIL;
    }
    return peos;
}


/*
 * getPath - find peos
 *
 * returns:
 *	malloced peos executable location, or
 *	NULL ==> no peos found (or some other error)
 *
 * We will try to find peos in our current directory first, then
 * will will look along our $PATH and if all else fails, try ~jshah1/bin.
 */
/*@null@*/
char *
getPath(void)
{
    char *path_env;	/* value of $PATH */
    char *peos;		/* potential location of peos */

    /* look for peos in current directory */
    if (cwd != NULL) {

	/* no peos in cwd, keep looking */
	peos = peos_in_dir(cwd);
	if (peos != NULL) {
	    return peos;
	}
    }

    /* 
     * look for peos along our $PATH 
     *
     * NOTE: Normally a leading :, or a trailing :, or a :: in $PATH
     *	     means "." (the current directory).  However we just tested
     *	     cwd/peos above, so we can safely ignore this $PATH convention
     *	     and only look at the non-empty :-separated tokens of $PATH.
     */
    /* get our $PATH */
    path_env = getenv("PATH");
    if (path_env != NULL) {
	char *p;	/* current token */

	/* strtok changes the string, so work on a copy of $PATH */
	path_env = strdup(path_env);
	if (path_env == NULL) {
	    perror("strdup of $PATH failed");
	    return NULL;
	} else {
	    /* look for peos in each non-empty :-sepatared token of $PATH */
	    for (p = strtok(path_env, ":"); p != NULL; p = strtok(NULL, ":")) {
		peos = peos_in_dir(p);
		if (peos != NULL) {
		    return peos;
		}
	    }
	    free(path_env);
	    path_env = NULL;
	}
    }

    /*
     * if all else fails, look for peos in ~jshah1/bin
     */
    peos = peos_in_dir("/home/jshah1/bin");
    /* return ~jshah1/bin/peos or NULL (no peos found) */

    return peos;
}
