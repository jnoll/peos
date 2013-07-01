#ifndef _HTML_H
#define _HTML_H
extern void print_header(char *title);
extern void print_banner(char* text, int pid);
extern void print_footer();
extern void print_action_name(char* action_name);
extern char* get_process_filename();
#endif
