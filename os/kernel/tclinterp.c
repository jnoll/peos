//#define DEBUG
#undef DEBUG
#undef MORE

#include "tclinterp.h"
#include <stdio.h>

void peos_tcl_start(peos_tcl* ptcl)
{
  if(!started){
     started=1;
     ptcl->interp=Tcl_CreateInterp();
     ptcl->argc=0;
     ptcl->objv = (Tcl_Obj**) ckalloc(DEFAULT_CAPACITY*sizeof(Tcl_Obj*));
     ptcl->objv_size = DEFAULT_CAPACITY;
     ptcl->is_cmd_cached=0;
  }

}

int peos_tcl_exec_cmd_from_string(peos_tcl* ptcl, char* cmd)
{
  int i;
  char* sub_str;
  char* command = (char*)malloc(sizeof(char)*strlen(cmd));
  strcpy(command,cmd);
  
  if (Tcl_Init(ptcl->interp) == TCL_ERROR) {
    return TCL_ERROR;
  }
  sub_str=strtok(command," ");

  if(sub_str!=NULL){
    ptcl->objv[ptcl->argc++]=Tcl_NewStringObj(sub_str,strlen(sub_str));
    ptcl->is_cmd_cached=1;
    if(Tcl_GetCommandInfo(ptcl->interp, sub_str, &(ptcl->cmd_info) ))
    {
      #ifdef DEBUG 
      system("echo Command found");
      #endif
    }else{
        system("echo Command NOT found");
        return TCL_ERROR;
    }
  }
  while((sub_str=strtok(NULL," "))!=NULL){
    Tcl_IncrRefCount(ptcl->objv[ptcl->argc++]=Tcl_NewStringObj(sub_str,strlen(sub_str)));
  }

  for( i = 0;i < ptcl->argc;++i){
    #ifdef DEBUG
    printf( "--> %s\n", Tcl_GetStringFromObj(ptcl->objv[i],NULL) );
    #else
    Tcl_GetStringFromObj(ptcl->objv[i],NULL);
    #endif
  }

  if((*(ptcl->cmd_info).objProc)((ptcl->cmd_info).objClientData, ptcl->interp, ptcl->argc, ptcl->objv) != TCL_OK){
    fprintf(stderr,"result is error");
    fflush(0);
    return TCL_ERROR;
  }
  #ifdef DEBUG 
  system("echo Passed");
  printf("Tcl_GetStringResult(interp)==%s\n",Tcl_GetStringResult(ptcl->interp));
  #else
  Tcl_GetStringResult(ptcl->interp);
  #endif
  
  #ifdef DEBUG
  if(*Tcl_GetStringResult(ptcl->interp)=='1'){
    printf("result is 1\n");
  }
  else printf("result is 0\n");
  #endif
  return 0;
}
int peos_tcl_release(peos_tcl* ptcl)
{
  int i;
  system("echo A");
  for (i = 0 ; i < ptcl->argc ; i++) {
    Tcl_DecrRefCount(ptcl->objv[i]);
  }
  system("echo B");
  //Tcl_Free((char *) ptcl->objv);
  system("echo C");
  ptcl->argc=0;
  //Tcl_Free((char *) ptcl-objv);
}

char* peos_tcl_get_var(peos_tcl* ptcl, char* var_name)
{
     return (char*)Tcl_GetVar(ptcl->interp,var_name,0);
}
int peos_tcl_set_var(peos_tcl* ptcl, char* var_name, char* var_value)
{
     return Tcl_SetVar(ptcl->interp,var_name,var_value,0) == TCL_OK  ? 1:0;
}
int peos_tcl_link_var(peos_tcl* ptcl, char* var_name, char* var_addr, int type)
{
     return Tcl_LinkVar(ptcl->interp,var_name,var_addr,type) == TCL_OK  ? 1:0;
}

/*
int main()
{
  //char com[]="puts stdout bob";
  int var1 =5;
  char* var2 = "hello tcl";
  peos_tcl p;
  peos_tcl_start(&p);
  peos_tcl_exec_cmd_from_string(&p,"puts stdout bob");
  peos_tcl_release(&p);
  peos_tcl_set_var(&p,"foo","9999");
  //peos_tcl_exec_cmd_from_string(&p,"set foo SOMEVALUE");
  peos_tcl_release(&p);
  //peos_tcl_exec_cmd_from_string(&p,"puts stdout $foo");
  //peos_tcl_release(&p);
  printf("value: [%s]\n", peos_tcl_get_var(&p,"foo"));

  if(!peos_tcl_link_var(&p, "var1", (char*)&var1, TCL_LINK_INT)) printf("couldnt link\n");
  printf("value: [%s]\n", peos_tcl_get_var(&p,"var1"));
  peos_tcl_set_var(&p,"var1","333");
  printf("value changed from TCL: [%s]\n", peos_tcl_get_var(&p,"var1"));
  var1 = 2004;
  printf("value changed from C: [%s]\n", peos_tcl_get_var(&p,"var1"));
  
  return 0;
}
*/
/*
int peos_tcl_TEST(peos_tcl* ptcl)
{
  int i;
  if (Tcl_Init(ptcl->interp) == TCL_ERROR) {
  return TCL_ERROR;
  }
  ptcl->objv[ptcl->argc++]=Tcl_NewStringObj("set",3);//);


  ptcl->is_cmd_cached=1;
  Tcl_IncrRefCount(ptcl->objv[ptcl->argc++]=Tcl_NewStringObj("foo",3));
  Tcl_IncrRefCount(ptcl->objv[ptcl->argc++]=Tcl_NewStringObj("itsavarvalue",12));
  system("echo B");
      if(Tcl_GetCommandInfo(ptcl->interp, "set", &(ptcl->cmd_info) ))
     {
         system("echo Command found");
         printf("command found\n");
     }else{
        system("echo Command NOT found");
        return TCL_ERROR;
     }

     for( i = 0;i < ptcl->argc;++i)
       printf( "--> %s\n", Tcl_GetStringFromObj(ptcl->objv[i],NULL) );

     if((*(ptcl->cmd_info).objProc)((ptcl->cmd_info).objClientData, ptcl->interp, ptcl->argc, ptcl->objv)
			   != TCL_OK)
     {
        system("echo Result is ERROR");
        printf("result is error");

         return TCL_ERROR;
     }
     system("echo Passed");
     printf("Tcl_GetStringResult(interp)==%s\n",Tcl_GetStringResult(ptcl->interp));
     if(*Tcl_GetStringResult(ptcl->interp)=='1')
     {
	      printf("result is 1\n");
     }
     else printf("result is 0\n");
     return 0;
}
*/


