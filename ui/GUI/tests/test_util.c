#include <stdio.h>
#include <getopt.h>
#include "test_util.h"


/* Test control flags. */

int fork_status = CK_NOFORK;
int verbosity   = CK_NORMAL;

void
parse_args(int argc, char *argv[])
{
  char c;

  while((c = getopt(argc, argv, "nvfq")) != -1)
  {
  	switch (c){
	case 'n':
		verbosity = CK_NORMAL;
		break;
	case 'v':
		verbosity = CK_VERBOSE;
		break;
	case 'q':
		verbosity = CK_MINIMAL;
		break;
	case 'f':
		fork_status = CK_FORK;
		break;
	default:
		fprintf(stderr, "usage: %s [-vfq] (normal, verbose, fork, minimal)", argv[0]);
	exit(1);
	}
  }
}

