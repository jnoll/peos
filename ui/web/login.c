/*************************************************************************/
/**     by Tewei Dai, teweidai@yahoo.com SUMMER 2000
**/
/**	for Professor John Noll's Software Engineering class: CSC5728
**/
/**	See http://oslab-server.cudenver.edu for more info.
**/
/**
**/
/**     login.c-- routine to verify the user and user password
**/
/*************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "SMTP.h"
#include "config.h"
#include "strlib.h"
#include "getcgivars.h"

#define CONFIG_LOCATION "webui.cfg"

uiconfig *cfg;
char *username;
char *password;
char name[30];
char pass[30];

int hasProblem, isPasswordRight;
int sessionId = 1;

int main() {

    cfg = readconfig(CONFIG_LOCATION);
    if (cfg != NULL) {
        hasProblem = 0;
        getcgiinput();
    }
    else {
        hasProblem = 1;
    }

    // Print the CGI response header 
    printf("Content-type: text/html\n\n") ;
    printf("<html>\n") ;
    printf("<head><title>PML Web Interface</title></head>\n") ;

    isPasswordRight = checkPassword();
    //printf("user type %i", isPasswordRight);

    if(hasProblem){
        printf("<body>\n");
        printf("The system is have problem please come back at a different time\n");	
        printf("<br>\n");
        printf("<p>You can email us at <A HREF=\"mailto:5728-dl@oslab-server.cudenver.edu\">
                5728-dl@oslab-server.cudenver.edu</A> to report this problem</P>\n");
    }
    else if(!isPasswordRight){        
        printf("<body>\n");
        printf("The user name and password failed to authenticate\n");	
        printf("<br>\n");
        printf("<p>You can email us at <A HREF=\"mailto:5728-dl@oslab-server.cudenver.edu\">
                5728-dl@oslab-server.cudenver.edu</A> to report this problem</P>\n");
    }
    else if(isPasswordRight == 1 || isPasswordRight == 2 || isPasswordRight == 3){
        printf("<body onLoad=\"window.location='main.html';\">\n") ;
        setCookie();
    }  
    else{
        printf("<body>\n") ;
        printf("The user name and password failed to authenticate\n");	
        printf("<br>\n");
        printf("<p>You can email us at <A HREF=\"mailto:5728-dl@oslab-server.cudenver.edu\">
                5728-dl@oslab-server.cudenver.edu</A> to report this problem</P>\n");
    }

    printf("</body>\n") ;
    printf("</html>\n") ;

    return 0;
}

int getcgiinput() {
    char **cgivars;
    int i;
 
    // First, get the CGI variables into a list of strings
    cgivars = getcgivars();

    // Get the CGI variables sent by the user. 
    for (i=0; cgivars[i]; i+= 2) {
    
        if(strcmp(cgivars[i], "username")==0) {
            username = cgivars[i+1];
            strncpy(name, username, 30);
        }
        if(strcmp(cgivars[i], "password")==0) {
            password = cgivars[i+1];
            strncpy(pass, password, 30);
        }

    }  

    // Free anything that needs to be freed 
    for (i=0; cgivars[i]; i++) free(cgivars[i]) ;
    free(cgivars) ;
}

/*
  this function returns 0 if the password isn't right
                returns 1 for user type 1
                returns 2 for user type 2 and so forth
*/
int checkPassword() {

    // if the user did not supply the name or password then return 0 
    if(strlen(name)==0 || strlen(pass)==0){
        return 0;
    }

    // the stuff below is simply a stub test code!!! 
    // password length is not 1 greater than the user name
    if(strlen(name)+1 != strlen(pass)){
        return 0;
    }

    if(strncmp(name, pass, strlen(name)) != 0 ){
        return 0;
    }

    return atoi(password+strlen(name));
}
int setCookie(){
    printf("<SCRIPT LANGUAGE=\"JavaScript\">\n");
    printf("document.cookie=\"userName=%s\";\n", name);
    printf("document.cookie=\"userType=%i\";\n", isPasswordRight);
    printf("document.cookie=\"password=%i\";\n", pass);
    printf("document.cookie=\"sessionId=%i\";\n", sessionId);
    printf("</SCRIPT>\n");
}




