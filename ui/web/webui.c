#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "SMTP.h"
#include "config.h"
#include "smpsock.h"
#include "interface.h"
#include "strlib.h"
#include "getcgivars.h"
#include "webui.h"

#define CONFIG_LOCATION "webui.cfg"

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
char **cgivars ;
char *command;
char *action;
char *username;
char *password;
int i;
int createForm = 0;

void showuser();
int getstdinput();

int main() {

    int malformedform = 0;    
    int sessionid = 1;

	cfg = readconfig(CONFIG_LOCATION);

	if (cfg != NULL) {

                //this is weird, but if you don't have the following message in the current_msg
                //and the same length then the program doesn't behave properly
		current_msg = add_message(current_msg,"                        \n");

		getcgiinput();


		if (strcasecmp(COMMAND,"status") == 0) {
			pml_available();
		}

		if (strcasecmp(COMMAND,"run") == 0) {
			pml_run(PARA1,PARA2);
		}
		if (strcasecmp(COMMAND,"done") == 0) {
			pml_done(PARA1,PARA2);
		}
		if (strcasecmp(COMMAND,"create") == 0) {
			pml_create(PARA1);
		}
		if (strcasecmp(COMMAND,"running") == 0) {
			pml_running(PARA1,PARA2);
		}
                if (strcasecmp(COMMAND,"login") == 0) {
                        
                        //createForm = -1;
			//pml_create(PARA1);
                        //current_msg = add_message(current_msg,"<br><hr>\n");
                        /*
                        current_msg = add_message(current_msg,"<br><hr>\n");
			pml_create(PARA1);

			pml_available();
                        current_msg = add_message(current_msg,"<br><hr>\n");

			pml_running(PARA1,PARA2);
                        current_msg = add_message(current_msg,"<br><hr>\n");
                        */
                }

	}
	else {
		current_msg = add_message(current_msg,"Could not find config file.<br>\n");
	}



    /** Print the CGI response header, required for all HTML output. **/
    /** Note the extra \n, to send the blank line.                  **/
    printf("Content-type: text/html\n\n") ;
    
    /** Finally, print out the complete HTML response page.         **/
    printf("<html>\n") ;
    printf("<head><title>PML Web Interface</title>\n") ;
    //printf("<SCRIPT LANGUAGE=\"JavaScript\">\n");


    printf("<SCRIPT LANGUAGE=\"JavaScript\" SRC=\"commonHeadFunction.js\"></SCRIPT>\n");

    printf("<SCRIPT LANGUAGE=\"JavaScript\" SRC=\"menuResponse.js\"></SCRIPT>\n");

    printf("<SCRIPT LANGUAGE=\"JavaScript\" SRC=\"checkDeleteCookie.js\"></SCRIPT>\n");

    //writeHeadFunctions();
    //printf("</SCRIPT>\n");


    printf("</head>\n");

    printf("<BODY MARGINWIDTH=0 MARGINHEIGHT=0 TOPMARGIN=0 LEFTMARGIN=0 BGCOLOR=\"white\" >\n"); //onResize=\"reloadPage();\">\n");
    printf("<SCRIPT LANGUAGE=\"JavaScript\" SRC=\"myScript.js\"></SCRIPT>\n");

    printf("<SCRIPT LANGUAGE=\"JavaScript\" TYPE=\"TEXT/JAVASCRIPT\">\n");
    printf("writePeosTitle();\n");
    printf("var commandVar = getCookie(\"command\");\n");
    printf("var actionVar = getCookie(\"action\");\n");
    printf("var sessionidVar = getCookie(\"sessionid\");\n");
    printf("var userName = getCookie(\"userName\");\n");
    printf("var passwordVar = getCookie(\"password\");\n");
    printf("var userType = getCookie(\"userType\");\n");
    printf("writeMenu(userType);\n");
    printf("</SCRIPT>\n");



    //----------------------------------------------------
    //here we print the body of the page
    //----------------------------------------------------
    printf("<TD VALIGN=\"TOP\" WIDTH=\"100%\" HEIGHT=\"100%\">\n");
    printf("%s\n", current_msg);

    free(current_msg);

    //printf("COMMAND =_%s_\n", COMMAND);
    //printf("command =_%s_\n", command);
    //printf("PARA1 =_%s_\n", PARA1);
    //printf("PARA2 =_%s_\n", PARA2);
    //printf("action =_%s_\n", ACTION);

    
    //----------------------------------------------------

    //----------------------------------------------------
    //here we print the end of the page
    //----------------------------------------------------
    printf("<img src=\"invisibleBar.gif\" width=300 height=1 alt=\"\">\n");

    printf("</TD>\n");
    printf("</TR>\n");
    printf("</TABLE>\n");

    printf("</body>\n");
    printf("</html>\n");

    //----------------------------------------------------
	
    /** Free anything that needs to be freed **/
    for (i=0; cgivars[i]; i++) free(cgivars[i]) ;
    free(cgivars) ;

    return;
}


void showuser()
{
/*
	current_msg = add_message(current_msg,"username: ");
	current_msg = add_message(current_msg,NAME);
	current_msg = add_message(current_msg,"\npassword: ");
	current_msg = add_message(current_msg,PASS);
	current_msg = add_message(current_msg," -> Successful login.<br><br>\n\n");
*/
}

int getcgiinput()
{
char *temp = (char *)malloc(BUFSIZ * sizeof(char));
char *subpos;
char *tempCommand;

    /** First, get the CGI variables into a list of strings         **/
    cgivars= getcgivars() ;

    /** Get the CGI variables sent by the user. **/
    for (i=0; cgivars[i]; i+= 2) {
    
		if(strcmp(cgivars[i], "command")==0) {
                        //if the first character of the commnad is 1 it means that we are going
                        //to create list as a form.

                        if (cgivars[i+1][0] == '1') {
                            createForm = 1;
                            tempCommand = cgivars[i+1]+1;
                            strncpy(cgivars[i+1], tempCommand, 10);
                        }

			command = cgivars[i+1];


		}
		if(strcmp(cgivars[i], "action")==0) {
			action = cgivars[i+1];
		}
		if(strcmp(cgivars[i], "username")==0) {
			username = cgivars[i+1];
		}
		if(strcmp(cgivars[i], "password")==0) {
			password = cgivars[i+1];
		}

    }  
    
	strncpy(NAME,username,8);
	strncpy(PASS,password,8);
	strncpy(ACTION,action,80);
	sscanf(ACTION,"%s %s",PARA1,PARA2);
	strncpy(COMMAND,command,80);

}


simplesock contact(simplesock s,char *name,char *password)
{
	s = init_socket(cfg->server,cfg->port);
	if (strlen(s.errmessage) != 0) {

		current_msg = add_message(current_msg,"Can't establish the connection with PML Engine on ");
		current_msg = add_message(current_msg,cfg->server);
		current_msg = add_message(current_msg,".<br>\n<br>\nPlease try again later.");
		return(s);      // Unable to contact the server
	}
	if (login(s,NAME,PASS) == 0)
	{
		current_msg = add_message(current_msg,"Login incorrect. Please try again.<br>\n<br>\n");
		current_msg = add_message(current_msg,"username: (Put your user name here.)<br>\npassword: (Your password goes here.)<br>\n<br>\n");
		current_msg = add_message(current_msg,"You can reply this message with username and password to get your status.<br>\n");
		strcpy(s.errmessage,"Login incorrect.");
		logout(s);
	}
	else showuser();
	return(s);
}

int pml_available()
{
	s = contact(s,NAME,PASS);
	if (strlen(s.errmessage) == 0) {	// Login successfully
		//current_msg = add_message(current_msg,"<big><b>Available Actions</b></big><br><br>\n");
		//current_msg = add_message(current_msg,"Please enter one in the CGI form below.<br>\n<br>\n");

		//current_msg = add_message(current_msg,"<b>Task summary</b>\n");
		//current_msg = add_message(current_msg,"<hr>\n");
		//current_msg = summary(s,current_msg, createForm);
		//current_msg = add_message(current_msg,"* To get the list of available model name, use the below action.<br>\n");
		//current_msg = add_message(current_msg,"action: create<br>\n");
		//current_msg = add_message(current_msg,"<br>\n<br>\n");

		current_msg = add_message(current_msg,"<b>Task summary:</b><br><br>\n");
		current_msg = add_message(current_msg,"\n");
		current_msg = available(s,current_msg,createForm);
		current_msg = add_message(current_msg,"\n");

		//current_msg = add_message(current_msg,"You have the following task(s) running:<br>\n<br>\n");
		//current_msg = running(s,current_msg);
		//current_msg = add_message(current_msg,"--------------------------------------------------------------------------------<br>\n<br>\n");

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
			current_msg = add_message(current_msg," is now set to run.<br>\n<br>\n");
			//current_msg = add_message(current_msg,"When you finish this task, simply enter the below action into the CGI form below and it will be set to done automatically.<br>\n<br>\n");
			//current_msg = add_message(current_msg,"action: done ");
			//current_msg = add_message(current_msg,PARA1);
			//current_msg = add_message(current_msg," ");
			//current_msg = add_message(current_msg,PARA2);
			//current_msg = add_message(current_msg,"<br>\n");
		}
		else {
			current_msg = add_message(current_msg,process);
			current_msg = add_message(current_msg," ");
			current_msg = add_message(current_msg,task);
			current_msg = add_message(current_msg," can not be found or currently unavailable.<br>\n<br>\n");
		}
		logout(s);
	}
}

int pml_running(char *process,char *task)
{
	s = contact(s,NAME,PASS);
	if (strlen(s.errmessage) == 0) {	// Login successfully
		current_msg = add_message(current_msg,"<b>Running Tasks:</b><br>\n");
                current_msg = running(s,current_msg, createForm);
		current_msg = add_message(current_msg,"<br>\n<br>\n");
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
			current_msg = add_message(current_msg," is now set to done.<br>\n<br>\n");
		}
		else {
			current_msg = add_message(current_msg,process);
			current_msg = add_message(current_msg," ");
			current_msg = add_message(current_msg,task);
			current_msg = add_message(current_msg," can not be found or not currently running.<br>\n<br>\n");
		}
		//current_msg = add_message(current_msg,"To get your available tasks, try STATUS below.<br>\n");
		logout(s);
	}
}

int pml_create(char *model)
{
	s = contact(s,NAME,PASS);
	if (strlen(s.errmessage) == 0) {

		if (create(s,model)) {
			current_msg = add_message(current_msg,model);
			current_msg = add_message(current_msg," is successfully created.<br>\n<br>\n");
			//current_msg = add_message(current_msg,"To get your next available task, reply this message.<br>\n");
		}
		else {

			//current_msg = add_message(current_msg,model);
			//if (strlen(model) != 0)
			//	current_msg = add_message(current_msg," can not be found.<br>\n<br>\n");
			current_msg = add_message(current_msg,"<b>Please use the following model(s):</b>\n");
			current_msg = list(s,current_msg, createForm);

		}
		logout(s);
	}
}

