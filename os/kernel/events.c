/*****************************************************************/
/* File:        events.c
/* Author:      Tingjun Wen
/* Date:        7/19/99
/* Description: Event handling functions.
/*****************************************************************/   

#include <stdio.h>
#include <stdlib.h>
#include <gdbm.h>
#include <sys/stat.h>
#include <sys/types.h>
#define __USE_XOPEN
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <string.h>
#include <dirent.h>
#include <pwd.h>
#include <errno.h>
#include "pmlengine.h"
#include "../vm/vm.h"

/* pmlLogin() always return with success. */
void pmlLogin()
{
  char in_buf[4096] = {0};
  int ret = -1;
  char cmd[17] = {0};
  char msg[1024];
  
  /* login - what first shows up when you run PMLServer */
  sendHelp();
  sendUI("\nPlease login to use the system:\n");
  do {
    memset(in_buf, 0, sizeof(in_buf));
    readUI(in_buf, sizeof(in_buf));
    if (sscanf(in_buf, "%16s", cmd) != 1)
      {
	printf("sscanf error\n");
	continue;
      }
    if (strcasecmp(cmd, "LOGIN") == 0) {
      if (sscanf(in_buf+5, "%9s%14s", uname, passwd) != 2) {
	sendUI("500 please use correct format! <'login' username pwd>\n");
	continue;
      } 
      ret = 0;
      sprintf(msg, "100 login successful.  Welcome, %s!\n", uname);
      sendUI(msg);
    }
    
    else if (strcasecmp(cmd, "EXIT") == 0) {
      sendUI("Goodbye.\n");
      close(newsockfd);
      exit(0);
    }
    
    else if (strcasecmp(cmd, "HELP") == 0) {
      sendHelp();
    }
    
    else {
      sendUI("500 please login first!\n");
    }
  } while (ret != 0);
} /* pmlLogin */

/* Opens the directory specified by the environment variable 
 * COMPILER_DIR, and displays all the available models (all 
 * the .txt files, stripped of their extensions). 
 * The user can then create instances
 * of one or more of the available process models.
 */
int listModel()
{
  DIR * dir;
  struct dirent * ent;
  char * ext;
  char msg[4096];
  char * COMP_DIR = getenv("COMPILER_DIR");
  
  if (COMP_DIR == NULL) {
    sendUI("500 COMPILER_DIR not set on PMLServer's machine. ");
    sendUI("COMPILER_DIR must be set to location of compiled .txt files...\n");
    return -1;
  }
  dir = opendir(COMP_DIR);
  if (dir == NULL) {
    sendUI("500 model directory does not exist\n");
    sendUI("Check COMPILER_DIR set to location of compiled .txt files...\n");
    return -1;
  }
  ent = readdir(dir);
  while (ent) {
    ext = strrchr(ent->d_name, '.');
    /* no extension */
    if (ext == NULL) {
      ent = readdir(dir);
      continue;
    }
    if (strcmp(ext, ".txt") != 0) {
      ent = readdir(dir);
      continue;
    }
    ext[0] = 0;
    sprintf(msg, "100-%s\n", ent->d_name);
    sendUI(msg);
    ent = readdir(dir);
  };
  sendUI("100 \n");
  
  return 0;
} /* new vm-version (.txt) listModel */


/* sendHelp() prints out, in menu form, a list of all valid commands...
 */
void sendHelp()
{
  sendUI("100- ********************* Help *************************************\n");
  sendUI("100-'login' <username> <password>\n");
  sendUI("100-'sample' \t\t- see a sample process instantiated, run.\n");
  sendUI("100-'list' \t\t- see all process models.\n");
  sendUI("100-'create' <modelname>- start a process.\n");
  sendUI("100-'available' \t- see next action(s) for current process(es).\n");
  sendUI("100-'run' <proc> <act> \t- run an action in 'available'.\n");
  sendUI("100-'running' \t\t- see what actions you have running.\n");
  sendUI("100-'done' <proc> <act> - finish a currently-running action.\n");
  sendUI("100-'exit' \t\t- close the connection.\n");
  sendUI("100-'help' \t\t- see this menu again.\n");
  sendUI("100- ****************************************************************\n");
} /* sendHelp */           

/* Prints out, step by step, a sample stepping-through of the
 * process "sample".  Provided to assist the user in understanding
 * the process of using this command-line-IF to run a process...
 */
void sampleRun()
{
  sendUI("100- ************** Running the process \"sample\" *******************\n");
  sendUI("100- 'list' \t\t\t-- after login, shows available processes.\n");
  sendUI("100- 'create sample' \t\t-- we'll choose the process called \"sample\".\n");
  sendUI("100- 'available' \t\t-- to see what the first action in \"sample\" is.\n");
  sendUI("100- 'run sample.001 edit' \t-- to begin that first action.\n");
  sendUI("100- 'running' \t\t\t-- should show \"sample.001 edit\".\n");
  sendUI("100- 'done sample.001 edit' \t-- when you've finished the action.\n");
  sendUI("100- 'run..', 'done..' \t\t-- for actions \"compile\", \"test\", and \"debug\".\n");
  sendUI("100- 'available' \t\t-- no more actions for \"sample.001\" - done!\n");
  sendUI("100- ****************************************************************\n");
} /* sampleRun */
