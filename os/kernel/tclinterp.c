//#define DEBUG
#undef DEBUG
#undef MORE
//#define MYTEST

#include "tclinterp.h"
#include <stdio.h>

#ifdef MYTEST
#include "comb.c"
#endif
/****
:::the interface code design plan:::
- have a script called processname.tcl for each
  processname.pml file. The processname.tcl file would be loaded along with the .pml
  file and initialize variables, e.g. $pwd, $rootname, $extensions etc. The user
  can then bind resources by typing $pwd/$rootname.$extension 
  in the console or gui, or perhaps if a special flag is set to true, 
  the tcl helper function would automatically suggest these values(latter is probably
  better).
  steps:
  function: load preliminary process values and flags, if the process.tcl 
  	filename is not found, TCL is OFF.
  function: inject these values into the resource table
  function: update these values if something is changed
  function(s): task-specific functions e.g. check file presense, check email notification... 
****/
int peos_tcl_start(peos_tcl* ptcl)
{

 // if(!started){
 //    started=1;
    // ptcl->argc=0;
     ptcl->interp=Tcl_CreateInterp();
     if (Tcl_Init(ptcl->interp) == TCL_ERROR) {
    return TCL_ERROR;
   }
 // }
   //  if(ptcl->argc > 0)
   //  //if(ptcl->objv != NULL)
   //      peos_tcl_release(ptcl);
   //  ptcl->objv = NULL;// (Tcl_Obj**) ckalloc(DEFAULT_CAPACITY*sizeof(Tcl_Obj*));
    // ptcl->objv_size = DEFAULT_CAPACITY;
   //  ptcl->is_cmd_cached=0;
  

  return TCL_OK;
}

int peos_tcl_exec_cmd TCL_VARARGS_DEF(Tcl_Interp*, arg1)
{
   va_list arg_list;
   char *cmd;
   char *arg;
   char **argv; 
   int argc, i, max;
   Tcl_CmdInfo info;
   
   Tcl_Interp* interp;
   Tcl_Obj **objv;	
   Tcl_Obj *resultPtr;
   int objc;
   max=20;
   //system("echo A0");
   interp = TCL_VARARGS_START(Tcl_Interp *, arg1, arg_list);
   //system("echo A1");
   Tcl_ResetResult(interp);
   //system("echo A");
   cmd = va_arg(arg_list, char *);
   //printf("cmd: [%s]\n",cmd);
   if(cmd!=NULL){
      if(!Tcl_GetCommandInfo(interp, cmd, &(info) )){
       system("echo Command NOT found");
       Tcl_AppendResult(interp, "unknown command \"", cmd, "\"", NULL);
       va_end(arg_list);
       return TCL_ERROR;
    }
   }
   else return TCL_ERROR;
   //system("echo B");
   objv = (Tcl_Obj **) ckalloc(max * sizeof(Tcl_Obj *));
   objv[0] = Tcl_NewStringObj(cmd, strlen(cmd));
   Tcl_IncrRefCount(objv[0]); 
   objc = 1;
   while(1){//(sub_str=strtok(NULL," "))!=NULL){
     arg=va_arg(arg_list, char *);
     //printf("arg: [%s]\n",arg);
     if (arg==(char *)NULL){
        objv[objc] = (Tcl_Obj *)NULL;
        break;
     }
   //system("echo C");
   
   Tcl_IncrRefCount(objv[objc++]=Tcl_NewStringObj(arg,strlen(arg)));
   //system("echo D");
    if(objc >= max){
      // system("echo De");
       Tcl_Obj **old_obj_list=objv;
       max*=2;
       objv=(Tcl_Obj **) ckalloc(max*sizeof(Tcl_Obj *));
       for (i = 0 ; i < objc ; i++){
          objv[i] = old_obj_list[i];
       }
        //system("echo E");
       Tcl_Free((char*)old_obj_list);
    }
    
    va_end(arg_list);
    // system("echo F");
  }
  for( i = 0;i < objc;++i)
       fprintf(stderr, "-->%s<--\n", Tcl_GetStringFromObj(objv[i],NULL) );
  if((*(info).objProc)((info).objClientData, interp, objc, objv) != TCL_OK){
    fprintf(stderr,"result is error");
    fflush(0);
    return TCL_ERROR;
  }
  #ifdef DEBUG 
  fprintf(stderr,"Tcl_GetStringResult(interp)==%s\n",Tcl_GetStringResult(interp));
  #else
  Tcl_GetStringResult(interp);
  #endif
  return TCL_OK;
}

char* peos_tcl_get_var(peos_tcl* ptcl, char* var_name)
{
     return (char*)
     Tcl_GetVar(ptcl->interp,var_name,TCL_GLOBAL_ONLY);
}
int peos_tcl_set_var(peos_tcl* ptcl, char* var_name, char* var_value)
{
     return Tcl_SetVar(ptcl->interp,var_name,var_value,0) == TCL_OK  ? 1:0;
}
int peos_tcl_link_var(peos_tcl* ptcl, char* var_name, char* var_addr, int type)
{
     return Tcl_LinkVar(ptcl->interp,var_name,var_addr,type);    
}
int peos_tcl_script(peos_tcl* ptcl, char* file_name)
{
    int status;
    peos_tcl_start(ptcl);
    status = Tcl_EvalFile(ptcl->interp, file_name);
    if (*ptcl->interp->result != 0){
        fprintf(stderr,"Issue Running Script: %s\n", ptcl->interp->result); 
    }
    fprintf(stderr,"Script ran, result: %s\n", status == TCL_OK ? "TCL_OK" : "TCL_ERROR");
    return status;
}
int peos_tcl_eval(peos_tcl* ptcl, char* name_str, char* eval_str, char* result_str )
{
    int result=-1;
    char* str=NULL;

    if(!(str=(char*)malloc(sizeof(char)*(strlen(eval_str)+strlen(name_str)+50)))){
    	if(debug)fprintf(stderr,"ERROR: Insufficient memory, malloc failed in peos_tcl_eval\n");
    	return TCL_ERROR;
    }else{
        if(!strcmp(eval_str,"$$")){
	sprintf(str,"set %s \\$\\$", name_str, eval_str);
	}else{
	sprintf(str,"set %s %s", name_str, eval_str);
	}
    }
    if(!result_str){
       fprintf(stderr,"ERROR: Insufficient memory, malloc failed in peos_tcl_eval\n");
       free(str);
       return TCL_ERROR;
    }
    result=Tcl_Eval(ptcl->interp, str);
   
    if( result == TCL_ERROR){
       if(debug)fprintf(stderr,"ERROR: failed with TCL statement: %s\n", str);
       if(debug)fprintf(stderr,"ERROR: Tcl reuturned TCL_ERROR in peos_tcl_eval\n");
       strcpy(result_str,eval_str);
    }else if(result == TCL_OK){
       strcpy(result_str,ptcl->interp->result);//peos_tcl_get_var(ptcl, name_str));
       if(debug)fprintf(stderr,"MESSAGE: successfully evaluated TCL statement: %s\n", str);
       if(debug)fprintf(stderr,"MESSAGE: variable set: %s %s \n", name_str, peos_tcl_get_var(ptcl, name_str));
    }else{
       if(debug)fprintf(stderr,"OTHER Tcl MESSAGE: Result %d in peos_tcl_eval\n", result);
       strcpy(result_str,eval_str);
    }
    free(str);
    return result;
}
#ifdef UNIT_TEST
#include "test_tclinterp.c"
#endif

/*
int main()
{
  //char com[]="puts stdout bob";
  int var1 =5;
  char*var2=NULL;
  char*var3=NULL;
  peos_tcl p;
  peos_tcl_start(&p);
  

  peos_tcl_set_var(&p,"foo","9999");
  printf("value: [%s]\n", peos_tcl_get_var(&p,"foo"));

  if(!peos_tcl_link_var(&p, "var1", (char*)&var1, TCL_LINK_INT)) printf("couldnt link\n");
  printf("value: [%s]\n", peos_tcl_get_var(&p,"var1"));
  
  peos_tcl_set_var(&p,"var1","333");
  Tcl_GetVar(p.interp,"var1",TCL_GLOBAL_ONLY);
  printf("value changed from TCL: [%s]\n", peos_tcl_get_var(&p,"var1"));
  var1 = 2004;
  printf("value changed from C: [%s]\n", peos_tcl_get_var(&p,"var1"));

  system("echo HI");
  peos_tcl_exec_cmd((p.interp), "set", "directory1", "/directory_name/", NULL);
  peos_tcl_exec_cmd((p.interp), "puts stdout ${directory1}");
  //peos_tcl_exec_cmd((p.interp), "set", "directory1", "/directory_name/", NULL);
 
  peos_tcl_exec_cmd((p.interp), "set", "resource1", "resource_name", NULL);
  peos_tcl_exec_cmd((p.interp), "set", "file_ext1", ".doc", NULL);
  peos_tcl_exec_cmd((p.interp), "set", "file_ext2", ".pdf", NULL);

   peos_tcl_link_var(&p, "path1", (char*)&var2, TCL_LINK_STRING);
   peos_tcl_link_var(&p, "path2", (char*)&var3, TCL_LINK_STRING);
  Tcl_Eval((p.interp), "set path1 \"${directory1}${resource1}${file_ext1}\"");
  Tcl_Eval((p.interp), "set path2 \"${directory1}${resource1}${file_ext2}\"");
  Tcl_Eval((p.interp), "puts stdout ${path1}");
  Tcl_Eval((p.interp), "puts stdout ${path2}");
 
   printf ("C knows path1 is %s\n", var2);
   printf ("C knows path2 is %s\n", var3);

  peos_tcl_script(&p,"hi.tcl");
  peos_tcl_script(&p,"/home/pasha/_tcl_book3ed/tclfromc/13_12.tcl");
  peos_tcl_script(&p,"nosuchfile");
  //Tcl_Eval((&p)->interp, "puts stdout $foo");
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


