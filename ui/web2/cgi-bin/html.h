#ifndef _HTML_H
#define _HTML_H
extern void print_header();
extern void print_noscript();
extern void print_action(int pid, char *action, char *state);
extern void print_script(int pid, char *action, int one_line);
extern void print_resource(int pid, char *action);
extern void print_process_button();
#endif
