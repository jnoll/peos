#ifndef _XMLPARSE_H
#define _XMLPARSE_H
typedef struct _process_list _process_list;
struct _process_list {
    int pid;
    char *name;
    char *active_action_name; /* Action whose state is READY */
    char *first_action_name;
    _process_list *next;
};

typedef struct _resource _resource;
struct _resource {
    char *name;
    char *value;
    char *qualifier;
};

typedef struct _action_page _action_page;
struct _action_page {
    int pid;
    char *model;
    char **action_list;
    int total_actions;
    char *state;
    _resource *reqd_resources;
    _resource *prov_resources;
    char *script;
};

extern _process_list *get_processes_list(char *filename);
extern _action_page *get_action_page_details(char *filename, int pid, char *act_name);
#endif
