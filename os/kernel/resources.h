#ifndef RESOURCES_H
#define RESOURCES_H
extern peos_resource_t *get_resource_list(char *model_file,int *num_resources);

#ifndef PALM
extern peos_resource_t *get_resource_list_action(int pid,char *act_name,int *num_resources);

extern peos_resource_t *get_resource_list_action_requires(int pid,char *act_name,int *num_resources);

extern peos_resource_t *get_resource_list_action_provides(int pid,char *act_name,int *num_resources);

extern int fill_resource_list_value(peos_resource_t* source, int num_source, peos_resource_t** destination, int num_destination);
#endif
#endif
