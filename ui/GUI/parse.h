/* PARSE.H; modification can be made to the data structure -- currently static*/

#ifndef _PARSE_H
#define _PARSE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

#define MAX_ACTION	128
#define MAX_SUBTREE	32

typedef struct action_node action;
typedef struct processList process;
typedef struct actionpointer actionptr;
typedef struct sensitivity sense;
typedef struct datatree data;
typedef struct _FRAME frame;
typedef struct _resource resource;

struct _FRAME {
GtkWidget *frame1;
GtkWidget *hpanned;
GtkWidget *viewport1;
GtkWidget *tree1;
GtkWidget *text1;
GtkWidget *scrollwindow2;
GtkWidget *Previous;
GtkWidget *Next;
GtkWidget *Start;
GtkWidget *Finish;
GtkWidget *Suspend;
GtkWidget *Abort;
GtkWidget *Rebind;
GtkWidget *vbox2;
GtkWidget *scrollwindow;
GtkWidget *fixed;
xmlNode *curr;
int index;
};

struct action_node {
	xmlNode * action[MAX_ACTION]; 
	action * iterptr;
	int count;
};

struct _resource {
  char * name;
  char * value;
};

struct processList {
	xmlNode * process;
	action list;
	frame page;
	GList *res;
};

struct  actionpointer {
	xmlNode *prev;
	xmlNode *next;
	xmlNode *cur;
	GtkWidget *item;
};

struct sensitivity {
	int next;
	int prev;
	int finish;
	int start;
	int suspend;
	int abort;
	int rebind;
};

int parse ();
void destroyDocumentStuff ();
void freeAll();
void freesub(action *a);
void printProcessProp (xmlNode *process);
void printAction (action *act);
void walkaction (xmlNode *act, action * procAction, int j);
void walk (xmlNode *root);
#endif
