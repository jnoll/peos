#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#define NODE_PROCESS 0
#define NODE_ITERATION 1
#define NODE_BRANCH 2
#define NODE_SELECTION 3

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
    _resource **reqd_resources;
    int total_reqd_resources;
    _resource **prov_resources;
    int total_prov_resources;
    char *script;
};

void get_first_active_action_details(xmlNode *node, _process_list *proct)
{
    char *action_state, *action_name;

    node = node->xmlChildrenNode;
    while (node) {
	if (!xmlStrcmp(node->name, (const xmlChar *)"action")) {
	    if (proct->first_action_name == NULL) {
		action_name = xmlGetProp(node, "name");
		proct->first_action_name = (char *) malloc((strlen(action_name) + 1) * sizeof(char));
		strcpy(proct->first_action_name, action_name);
	    }
	    if (proct->active_action_name == NULL) {
		action_state = xmlGetProp(node, "state");
		if (!xmlStrcmp(action_state, (const xmlChar *)"READY")) {
		    action_name = xmlGetProp(node, "name");
		    proct->active_action_name = (char *) malloc((strlen(action_name) + 1) * sizeof(char));
		    strcpy(proct->active_action_name, action_name);
		}
	    }
	} else if (!xmlStrcmp(node->name, (const xmlChar *)"iteration") ||
			!xmlStrcmp(node->name, (const xmlChar *)"selection") ||
			!xmlStrcmp(node->name, (const xmlChar *)"sequence") ||
			!xmlStrcmp(node->name, (const xmlChar *)"branch")) {
	    get_first_active_action_details(node, proct);
	}
	node = node->next;
    }
}

_process_list *get_proc_details(xmlNode *node)
{
    _process_list *proct;
    char *model, *tchar;
    char *action_state, *action_name;
    int lbreak = 1;
    int first = 0;
    int i;

    proct = (_process_list *) malloc(sizeof(_process_list));
    proct->pid = atoi(xmlGetProp(node, "pid"));
    model = xmlGetProp(node, "model");
    tchar = strrchr(model, '/');
    tchar++;
    proct->name = (char *) malloc((strlen(tchar) - 3) * sizeof(char));
    for (i=0; (i < strlen(tchar)) && (tchar[i] != '.'); i++)
	proct->name[i] = tchar[i];
    proct->name[i] = '\0';
    proct->first_action_name = NULL;
    proct->active_action_name = NULL;
    get_first_active_action_details(node, proct);
    proct->next = NULL;
    return proct;
}

void create_action_list(xmlDoc *doc, xmlNode *node, _action_page *apage, char *act_name, int parent)
{
    char *tname, *tstate, *tscript, *tstr;
    xmlNodePtr inner;

    node = node->xmlChildrenNode;
    while (node) {
	if (!xmlStrcmp(node->name, (const xmlChar *)"action")) {
	    tname = xmlGetProp(node, "name");
	    apage->action_list[apage->total_actions] =
		    (char *) malloc((strlen(tname) + 1) * sizeof(char));
	    strcpy(apage->action_list[apage->total_actions], tname);
	    if (!strcmp(tname, act_name)) {
		tstate = xmlGetProp(node, "state");
		apage->state = (char *) malloc((strlen(tstate) + 1) * sizeof(char));
		strcpy(apage->state, tstate);
		inner = node->xmlChildrenNode;
		apage->reqd_resources[apage->total_reqd_resources] = NULL;
		apage->prov_resources[apage->total_prov_resources] = NULL;
		while (inner) {
		    if (!xmlStrcmp(inner->name, (const xmlChar *)"script")) {
			tscript = xmlNodeListGetString(doc, inner->xmlChildrenNode, 1);
			apage->script = (char *) malloc((strlen(tscript) + 1) * sizeof(char));
			strcpy(apage->script, tscript);
		    } else if (!xmlStrcmp(inner->name, (const xmlChar *)"req_resource")) {
			apage->reqd_resources[apage->total_reqd_resources] =
					(_resource *) malloc(sizeof(_resource));
			tstr = xmlGetProp(inner, "name");
			apage->reqd_resources[apage->total_reqd_resources]->name =
				(char *) malloc((strlen(tstr) + 1) * sizeof(char));
			strcpy(apage->reqd_resources[apage->total_reqd_resources]->name, tstr);
			tstr = xmlGetProp(inner, "value");
			apage->reqd_resources[apage->total_reqd_resources]->value =
				(char *) malloc((strlen(tstr) + 1) * sizeof(char));
			strcpy(apage->reqd_resources[apage->total_reqd_resources]->value, tstr);
			tstr = xmlGetProp(inner, "qualifier");
			apage->reqd_resources[apage->total_reqd_resources]->qualifier =
				(char *) malloc((strlen(tstr) + 1) * sizeof(char));
			strcpy(apage->reqd_resources[apage->total_reqd_resources]->qualifier, tstr);
			apage->total_reqd_resources++;
		    } else if (!xmlStrcmp(inner->name, (const xmlChar *)"prov_resource")) {
			apage->prov_resources[apage->total_prov_resources] =
					(_resource *) malloc(sizeof(_resource));
			tstr = xmlGetProp(inner, "name");
			apage->prov_resources[apage->total_prov_resources]->name =
				(char *) malloc((strlen(tstr) + 1) * sizeof(char));
			strcpy(apage->prov_resources[apage->total_prov_resources]->name, tstr);
			tstr = xmlGetProp(inner, "value");
			apage->prov_resources[apage->total_prov_resources]->value =
				(char *) malloc((strlen(tstr) + 1) * sizeof(char));
			strcpy(apage->prov_resources[apage->total_prov_resources]->value, tstr);
			tstr = xmlGetProp(inner, "qualifier");
			apage->prov_resources[apage->total_prov_resources]->qualifier =
				(char *) malloc((strlen(tstr) + 1) * sizeof(char));
			strcpy(apage->prov_resources[apage->total_prov_resources]->qualifier, tstr);
			apage->total_prov_resources++;
		    }
		    inner = inner->next;
		}
		xmlFree(inner);
	    }
	    apage->total_actions++;
	} else if (!xmlStrcmp(node->name, (const xmlChar *)"iteration")) {
	    char *temp1 = "||iteration||";
	    apage->action_list[apage->total_actions] =
		    (char *) malloc((strlen(temp1) + 1) * sizeof(char));
	    strcpy(apage->action_list[apage->total_actions], temp1);
	    apage->total_actions++;
	    create_action_list(doc, node, apage, act_name, NODE_ITERATION);
	    char *temp2 = "!!iteration!!";
	    apage->action_list[apage->total_actions] =
		    (char *) malloc((strlen(temp2) + 1) * sizeof(char));
	    strcpy(apage->action_list[apage->total_actions], temp2);
	    apage->total_actions++;
	} else if (!xmlStrcmp(node->name, (const xmlChar *)"selection")) {
	    char *temp1 = "||selection||";
	    apage->action_list[apage->total_actions] =
		    (char *) malloc((strlen(temp1) + 1) * sizeof(char));
	    strcpy(apage->action_list[apage->total_actions], temp1);
	    apage->total_actions++;
	    create_action_list(doc, node, apage, act_name, NODE_SELECTION);
	    char *temp2 = "!!selection!!";
	    apage->action_list[apage->total_actions] =
		    (char *) malloc((strlen(temp2) + 1) * sizeof(char));
	    strcpy(apage->action_list[apage->total_actions], temp2);
	    apage->total_actions++;
	} else if (!xmlStrcmp(node->name, (const xmlChar *)"sequence")) {
	    char *temp1 = "||sequenceb||";
	    if (parent == NODE_SELECTION) temp1 = "||sequences||";
	    apage->action_list[apage->total_actions] =
		    (char *) malloc((strlen(temp1) + 1) * sizeof(char));
	    strcpy(apage->action_list[apage->total_actions], temp1);
	    apage->total_actions++;
	    create_action_list(doc, node, apage, act_name, parent);
	    char *temp2 = "!!sequenceb!!";
	    if (parent == NODE_SELECTION) temp1 = "!!sequences!!";
	    apage->action_list[apage->total_actions] =
		    (char *) malloc((strlen(temp2) + 1) * sizeof(char));
	    strcpy(apage->action_list[apage->total_actions], temp2);
	    apage->total_actions++;
	} else if (!xmlStrcmp(node->name, (const xmlChar *)"branch")) {
	    /*char *temp1 = "||branch||";
	    apage->action_list[apage->total_actions] =
		    (char *) malloc((strlen(temp1) + 1) * sizeof(char));
	    strcpy(apage->action_list[apage->total_actions], temp1);
	    apage->total_actions++;
	    create_action_list(doc, node, apage, act_name, NODE_BRANCH);
	    char *temp2 = "!!branch!!";
	    apage->action_list[apage->total_actions] =
		    (char *) malloc((strlen(temp2) + 1) * sizeof(char));
	    strcpy(apage->action_list[apage->total_actions], temp2);
	    apage->total_actions++;*/
	    create_action_list(doc, node, apage, act_name, NODE_BRANCH);
	}
	node = node->next;
    }
}

_action_page *get_action_page_details(char *filename, int pid, char *act_name)
{
    xmlDocPtr doc;
    xmlNodePtr cur;
    _action_page *apage = NULL;
    char *tmodel;

    doc = xmlParseFile(filename);

    if (doc == NULL)
    {
	printf("Error parsing file \"%s\"\n", filename);
	return NULL;
    }

    cur = xmlDocGetRootElement(doc);
    if (xmlStrcmp(cur->name, (const xmlChar *) "process_table"))
    {
	printf("Document of wrong type, root node must be <prosees_table>\n");
	xmlFreeDoc(doc);
	exit(1);
    }

    cur = cur->xmlChildrenNode;
    while (cur) {
	if ((!xmlStrcmp(cur->name, (const xmlChar *)"process"))) {
	    if (atoi(xmlGetProp(cur, "pid")) == pid) {
		apage = (_action_page *) malloc(sizeof(_action_page));
		apage->pid = pid;
		tmodel = strrchr(xmlGetProp(cur, "model"), '/');
		apage->total_actions = 0;
		tmodel++;
		apage->model = (char *) malloc((strlen(tmodel) + 1) * sizeof(char));
		strcpy(apage->model, tmodel);
		apage->action_list = (char **) malloc(sizeof(char *) * 50);
		apage->reqd_resources = (_resource **) malloc(sizeof(_resource *) * 10);
		apage->total_reqd_resources = 0;
		apage->prov_resources = (_resource **) malloc(sizeof(_resource *) * 10);
		apage->total_prov_resources = 0;
		create_action_list(doc, cur, apage, act_name, NODE_PROCESS);
	    }
	}
	cur = cur->next;
    }
    xmlFreeDoc(doc);

    return apage;
}

_process_list *get_processes_list(char *filename)
{
    xmlDocPtr doc;
    xmlNodePtr cur;
    _process_list *new, *plist = NULL, *ptemp;
    int num_of_processes = 0;

    doc = xmlParseFile(filename);

    if (doc == NULL)
    {
	printf("Error parsing file \"%s\"\n", filename);
	return NULL;
    }

    cur = xmlDocGetRootElement(doc);
    if (xmlStrcmp(cur->name, (const xmlChar *) "process_table"))
    {
	printf("Document of wrong type, root node must be <prosees_table>\n");
	xmlFreeDoc(doc);
	exit(1);
    }

    cur = cur->xmlChildrenNode;
    while (cur) {
	if ((!xmlStrcmp(cur->name, (const xmlChar *)"process")))
	{
	    new = get_proc_details(cur);
	    if (plist)
	    {
		ptemp->next = new;
		ptemp = new;
	    } else {
		plist = ptemp = new;
	    }
	    num_of_processes++;
	}
	cur = cur->next;
    }
    xmlFreeDoc(doc);

    return plist;
}
/*
int main()
{
    int i = 0;
    _action_page *ap = get_action_page_details("/webpages/vkhandel/PEOS/cgi-bin/dfZRuitU82fEY.dat.xml", 1, "add_ice");
    if (ap) {
        printf("Name of the model: %s (PID = %d)\n", ap->model, ap->pid);
        printf("Toral actions: %d\n", ap->total_actions);
        for (i = 0; i < ap->total_actions; i++)
        {
             printf("Action #%d: %s\n", i+1, ap->action_list[i]);
        }
        printf("State of selected action: %s\n", ap->state);
	printf("Required resources(%d): ", ap->total_reqd_resources);
	for (i = 0; i < ap->total_reqd_resources; i++) {
	    printf("%s = %s", ap->reqd_resources[i]->name, ap->reqd_resources[i]->value);
	    if (i < (ap->total_reqd_resources-1)) {
		printf(", ");
	    }
	}
	if (ap->total_reqd_resources == 0) {
	    printf("No resources required\n");
	}
	printf("\nProvided resources(%d): ", ap->total_prov_resources);
	for (i = 0; i < ap->total_prov_resources; i++) {
	    printf("%s = %s", ap->prov_resources[i]->name, ap->prov_resources[i]->value);
	    if (i < (ap->total_prov_resources-1)) {
		printf(", ");
	    }
	}
	if (ap->total_prov_resources == 0) {
	    printf("No resources provided\n");
	}
        printf("Script: %s\n", ap->script);

    }
    return 0;
}*/
