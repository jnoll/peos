<<<<<<< HEAD
=======
#define _XOPEN_SOURCE
#include <unistd.h>
>>>>>>> 71f6a61cb06573d2dbf317e9ee585b978950aef8
#include <stdio.h>

void print_header(char *title)
{
    printf("%s%c%c\n","Content-Type:text/html;charset=iso-8859-1",13,10);
    printf("<html>\n");
    printf("<head>\n");
    printf("  <meta content=\"text/html; charset=ISO-8859-1\" http-equiv=\"content-type\">\n");
    printf("  <title>%s</title>\n", title);
    printf("<STYLE TYPE=\"text/css\">\n");
    printf(" TABLE { border: 1px solid white; }\n");
    printf(" H1  {  text-transform: uppercase }\n");
    printf(" H2 { text-transform: capitalize }\n");
    printf(" H3 { text-transform: capitalize }\n");
    printf("\n");
    printf(" scroll {overflow: auto; height: 100px;}\n");
    printf("\n");
    printf("\n");
    printf(" OL.index { list-style-type: decimal ; \n");
    printf("	  list-style-position: inside; \n");
    printf(" } \n");
    printf(" UL.index { list-style-type: none ; list-style-position: inside} \n");
    printf(" LI.index { text-transform: capitalize; }\n");
    printf("</STYLE>\n");
    printf("</head>\n");
    printf("<body>\n");
}

void print_banner(char *text, int pid)
{
    printf("<table style=\"text-align: left; background-color: rgb(255, 204, 51); width: 100%; \"\n");
    printf(" border=\"0\" cellspacing=\"0\" cellpadding=\"1\">\n");
    printf("  <tbody>\n");
    printf("    <tr>\n");
    printf("      <td style=\"width:100px;\">\n");
    printf("        <img style=\"width: 100px; height: 100px;\" alt=\"PEOS Logo\"\n");
    printf("        src=\"../images/peos_logo.jpg\">\n");
    printf("      </td>\n");
    printf("      <td style=\"vertical-align: middle;\">\n");
    printf("        <h3><i>COEN 286, Fall 2005</i></h3>\n");
    printf("        <h1 style=\"text-align: center;\">%s (%d)</h1>\n", text, pid);
    printf("      </td>\n");
    printf("    </tr>\n");

    printf("  </tbody>\n");
    printf("</table>\n");
}

void print_footer()
{
    printf("<table style=\"text-align: left; width: 100%;\" border=\"0\" cellspacing=\"0\"\n");
    printf(" cellpadding=\"1\">\n");
    printf("  <tbody>\n");
    printf("    <tr>\n");
    printf("      <td style=\"background-color: rgb(255, 204, 51);\">\n");
    printf("      &copy;2004 Santa Clara University. Please direct your inquiries:\n");
    printf("      <a href=\"mailto:jnoll@scu.edu\">jnoll@scu.edu</a>\n");
    printf("      </td>\n");
    printf("    </tr>\n");
    printf("  </tbody>\n");
    printf("</table>\n");
    printf("</body>\n");
    printf("</html>\n");
}

void print_action_name(char* action_name)
{
    int i, action_len;
    char* display_action = strdup(action_name);
    action_len = strlen(display_action);
    display_action[0] = toupper(display_action[0]);
    for (i = 0; i < action_len; i++) {
        if (display_action[i] == '_') {
        display_action[i] = ' ';
        if (display_action[i + 1])
            display_action[i + 1] = toupper(display_action[i + 1]);
        }
    }
    printf("%s",   display_action);
    free(display_action);       
}

<<<<<<< HEAD
void get_process_filename()
=======
char* get_process_filename()
>>>>>>> 71f6a61cb06573d2dbf317e9ee585b978950aef8
{
    int i;
    char *login_name, *enc_loginname, *result;
    login_name = (char *)getenv("REMOTE_USER");
<<<<<<< HEAD
    enc_loginname = (char *)crypt(login_name, "df");
=======
    if ((enc_loginname = (char *)crypt(login_name, "df")) == NULL || strlen(enc_loginname) == 0) {
	perror("Error encrypting login_name");
	enc_loginname = login_name;
    }


>>>>>>> 71f6a61cb06573d2dbf317e9ee585b978950aef8
    for (i = 0; i < strlen(enc_loginname); i++) {
        if (enc_loginname[i] == '/' || enc_loginname[i] == '.')
            enc_loginname[i] = '_';
    }
    result = (char *)malloc((strlen(enc_loginname) + strlen(".dat") + 1) * sizeof(char));
    strcpy(result, enc_loginname);
    strcat(result, ".dat");
<<<<<<< HEAD
    //free(enc_loginname);
    //free(login_name);
=======

    fprintf(stderr, "login_name=%s, enc_loginname=%s, result=%s\n",
        login_name, enc_loginname, result);

>>>>>>> 71f6a61cb06573d2dbf317e9ee585b978950aef8
    return result;
}
