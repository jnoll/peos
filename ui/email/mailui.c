#include <stdio.h>
#include <stdlib.h>
#include "SMTP.h"
#include "config.h"
#include "smpsock.h"
#include "interface.h"

#define CONFIG_LOCATION "./mailui.cfg"
#define LOG stdout

uiconfig *cfg;
char *current_msg = NULL;
simplesock s;
char EMAIL[80];
char NAME[9];	// Limit  username and password to 8 characters
char PASS[9];
char ACTION[80] = "";
char COMMAND[80];
char PARA1[80];
char PARA2[80];
char NEWSUBJ[80];

void showuser();
int getstdinput();

int main(int argc, char *argv[])
{
recipient *head;

puts("It runs.");
	cfg = readconfig(CONFIG_LOCATION);
	if (cfg != NULL) {
		if (argc < 3) {
			puts("Command format error. Please check .procmailrc file.");
			return(1);
		}
		
		if (argc >= 2) strcpy(EMAIL,argv[1]);
		if (argc >= 3) strcpy(COMMAND,argv[2]);

		current_msg = add_message(current_msg,"Welcome to PML\n\n");
		getstdinput();

		if (strcasecmp(COMMAND,"status") == 0) {
			pml_available();
			strcpy(NEWSUBJ,"PML status");
		}

		if (strcasecmp(COMMAND,"run") == 0) {
			pml_run(PARA1,PARA2);
			strcpy(NEWSUBJ,"PML run");
		}
		if (strcasecmp(COMMAND,"done") == 0) {
			pml_done(PARA1,PARA2);
			strcpy(NEWSUBJ,"PML done");
		}
		if (strcasecmp(COMMAND,"create") == 0) {
			pml_create(PARA1);
			strcpy(NEWSUBJ,"PML create");
		}

		head = create_rcptlist();
		if (sendmailto(cfg->smtp,EMAIL,"","",cfg->pmlmail,NEWSUBJ,current_msg,head) == 0) {
			fprintf(LOG,"* Message can not be sent.\n\n");
		}
		else {
			fprintf(LOG,"* Message is successfully sent.\n\n");
		}
		clear_rcptlist(head);
	}
	else {
		fprintf(LOG,"* Configuration file not found.\n\n");
		return(0);
	}
}

void showuser()
{
	current_msg = add_message(current_msg,"username: ");
	current_msg = add_message(current_msg,NAME);
	current_msg = add_message(current_msg,"\npassword: ");
	current_msg = add_message(current_msg,PASS);
	current_msg = add_message(current_msg,"\n\n");
}

int getstdinput()
{
char *temp = (char *)malloc(BUFSIZ * sizeof(char));
char *subpos;

	while (fgets(temp,BUFSIZ,stdin) != NULL) {
		subpos = (char *)strstr(temp,"username:");	// Find username
		if (subpos != NULL) {
			subpos = subpos + 9;
			alltrim(subpos);
			strncpy(NAME,subpos,8);
		}
		subpos = (char *)strstr(temp,"password:");	// Find password
		if (subpos != NULL) {
			subpos = subpos + 9;
			alltrim(subpos);
			strncpy(PASS,subpos,8);
		}
		subpos = (char *)strstr(temp,"action:");	// Find action
		if (subpos != NULL && strlen(ACTION) == 0) {
			subpos = subpos + 7;
			alltrim(subpos);
			strncpy(ACTION,subpos,80);
			sscanf(ACTION,"%s %s %s",COMMAND,PARA1,PARA2);
		}
	}
}

simplesock contact(simplesock s,char *name,char *password)
{
	s = init_socket(cfg->server,cfg->port);
	if (strlen(s.errmessage) != 0) {

		current_msg = add_message(current_msg,"Can't establish the connection with PML Engine on ");
		current_msg = add_message(current_msg,cfg->server);
		current_msg = add_message(current_msg,".\n\nPlease try again later.");
		fprintf(LOG,"* %s\n",s.errmessage);
		return(s);      // Unable to contact the server
	}
	if (login(s,NAME,PASS) == 0)
	{
		current_msg = add_message(current_msg,"Login incorrect. Please try again.\n\n");
		current_msg = add_message(current_msg,"username: (Put your user name here.)\npassword: (Your password goes here.)\n\n");
		current_msg = add_message(current_msg,"You can reply this message with username and password to get your status.\n");
		strcpy(s.errmessage,"Login incorrect.");
		fprintf(LOG,"* %s\n",s.errmessage);
		logout(s);
	}
	else showuser();
	return(s);
}

int pml_available()
{
	s = contact(s,NAME,PASS);
	if (strlen(s.errmessage) == 0) {	// Login successfully
		current_msg = add_message(current_msg,"Delete unwanted action and reply. Read action details below.\n\n");
		current_msg = add_message(current_msg,"Note: The first action will be used if more than one action available.\n\n");

		current_msg = add_message(current_msg,"Task summary\n");
		current_msg = add_message(current_msg,"--------------------------------------------------------------------------------\n");
		current_msg = summary(s,current_msg);
		current_msg = add_message(current_msg,"* To get the list of available model name, use the below action.\n");
		current_msg = add_message(current_msg,"action: create\n");
		current_msg = add_message(current_msg,"--------------------------------------------------------------------------------\n\n");

		current_msg = add_message(current_msg,"You have the following task(s) available:\n\n");
		current_msg = available(s,current_msg);
		current_msg = add_message(current_msg,"--------------------------------------------------------------------------------\n\n");

		current_msg = add_message(current_msg,"You have the following task(s) running:\n\n");
		current_msg = running(s,current_msg);
		current_msg = add_message(current_msg,"--------------------------------------------------------------------------------\n\n");

		logout(s);
	}
}

int pml_run(char *process,char *task)
{
	s = contact(s,NAME,PASS);
	if (strlen(s.errmessage) == 0) {
		if (run(s,process,task)) {
			current_msg = add_message(current_msg,process);
			current_msg = add_message(current_msg," ");
			current_msg = add_message(current_msg,task);
			current_msg = add_message(current_msg," is now set to run.\n\n");
			current_msg = add_message(current_msg,"When you finish this task, simply reply this message and it will be set to done automatically.\n\n");
			current_msg = add_message(current_msg,"action: done ");
			current_msg = add_message(current_msg,PARA1);
			current_msg = add_message(current_msg," ");
			current_msg = add_message(current_msg,PARA2);
			current_msg = add_message(current_msg,"\n");
		}
		else {
			current_msg = add_message(current_msg,process);
			current_msg = add_message(current_msg," ");
			current_msg = add_message(current_msg,task);
			current_msg = add_message(current_msg," can not be found or currently unavailable.\n\n");
		}
		logout(s);
	}
}

int pml_done(char *process,char *task)
{
	s = contact(s,NAME,PASS);
	if (strlen(s.errmessage) == 0) {
		if (done(s,process,task)) {
			current_msg = add_message(current_msg,process);
			current_msg = add_message(current_msg," ");
			current_msg = add_message(current_msg,task);
			current_msg = add_message(current_msg," is now set to done.\n\n");
		}
		else {
			current_msg = add_message(current_msg,process);
			current_msg = add_message(current_msg," ");
			current_msg = add_message(current_msg,task);
			current_msg = add_message(current_msg," can not be found or not currently running.\n\n");
		}
		current_msg = add_message(current_msg,"To get your next available task, reply this message.\n");
		logout(s);
	}
}

int pml_create(char *model)
{
	s = contact(s,NAME,PASS);
	if (strlen(s.errmessage) == 0) {
		if (create(s,model)) {
			current_msg = add_message(current_msg,model);
			current_msg = add_message(current_msg," is successfully created.\n\n");
			current_msg = add_message(current_msg,"To get your next available task, reply this message.\n");
		}
		else {
			current_msg = add_message(current_msg,model);
			if (strlen(model) != 0)
				current_msg = add_message(current_msg," can not be found.\n\n");
			current_msg = add_message(current_msg,"Please use the following model(s).\n\n");
			current_msg = list(s,current_msg);
			current_msg = add_message(current_msg,"\naction: create (put the model name here.)\n");
		}
		logout(s);
	}
}
