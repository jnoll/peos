#ifndef RESOURCES_H
#define RESOURCES_H
extern peos_resource_t *get_resource_list(char *model_file,int *num_resources);

extern peos_resource_t *get_resource_list_action(int pid,char *act_name,int *num_resources);

extern peos_resource_t *get_resource_list_action_requires(int pid,char *act_name,int *num_resources);

extern peos_resource_t *get_resource_list_action_provides(int pid,char *act_name,int *num_resources);
#endif
