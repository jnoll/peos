#include "tclinterp.h"
#include <stdio.h>


int peos_tcl_start(peos_tcl** ptcl)
{
    *ptcl = (peos_tcl*) malloc (sizeof(peos_tcl));
    if (!*ptcl){
      fprintf(stderr, "malloc failed in peos_tcl_start");
      return TCL_ERROR;
    }
    (*ptcl)->interp=Tcl_CreateInterp();
    if (Tcl_Init((*ptcl)->interp) == TCL_ERROR) {
      return TCL_ERROR;
    }
    return TCL_OK;
}

int peos_tcl_exec_cmd TCL_VARARGS_DEF(Tcl_Interp*, arg1)
{
   va_list arg_list;
   char *cmd;
   char *arg;
   int i, max;
   Tcl_CmdInfo info;
   
   Tcl_Interp* interp;
   Tcl_Obj **objv;	
   int objc;
   max=20;
   interp = TCL_VARARGS_START(Tcl_Interp *, arg1, arg_list);
   Tcl_ResetResult(interp);
   cmd = va_arg(arg_list, char *);
   if(cmd!=NULL){
      if(!Tcl_GetCommandInfo(interp, cmd, &(info) )){
       system("echo Command NOT found");
       Tcl_AppendResult(interp, "unknown command \"", cmd, "\"", NULL);
       va_end(arg_list);
       return TCL_ERROR;
    }
   }
   else return TCL_ERROR;
   objv = (Tcl_Obj **) ckalloc(max * sizeof(Tcl_Obj *));
   objv[0] = Tcl_NewStringObj(cmd, strlen(cmd));
   Tcl_IncrRefCount(objv[0]); 
   objc = 1;
   while(1){
     arg=va_arg(arg_list, char *);
     if (arg==(char *)NULL){
        objv[objc] = (Tcl_Obj *)NULL;
        break;
     }
   
   Tcl_IncrRefCount(objv[objc++]=Tcl_NewStringObj(arg,strlen(arg)));
   if(objc >= max){
       Tcl_Obj **old_obj_list=objv;
       max*=2;
       objv=(Tcl_Obj **) ckalloc(max*sizeof(Tcl_Obj *));
       for (i = 0 ; i < objc ; i++){
          objv[i] = old_obj_list[i];
       }
       Tcl_Free((char*)old_obj_list);
    }
    va_end(arg_list);
  }
#ifdef TCLINTERP_DEBUG 
  for( i = 0;i < objc;++i)
       fprintf(stderr, "-->%s<--\n", Tcl_GetStringFromObj(objv[i],NULL) );
#endif
  if((*(info).objProc)((info).objClientData, interp, objc, objv) != TCL_OK){
    fprintf(stderr,"result is error");
    fflush(0);
    return TCL_ERROR;
  }
#ifdef TCLINTERP_DEBUG 
  fprintf(stderr,"Tcl_GetStringResult(interp)==%s\n",Tcl_GetStringResult(interp));
#else
  Tcl_GetStringResult(interp);
#endif
  return TCL_OK;
}

char* peos_tcl_get_var(peos_tcl* ptcl, char* var_name)
{
     return (char*)Tcl_GetVar(ptcl->interp,var_name,TCL_GLOBAL_ONLY);
}
int peos_tcl_set_var(peos_tcl* ptcl, char* var_name, char* var_value)
{
     return (int)Tcl_SetVar(ptcl->interp,var_name,var_value,0);
}
int peos_tcl_link_var(peos_tcl* ptcl, char* var_name, char* var_addr, int type)
{
     return (int)Tcl_LinkVar(ptcl->interp,var_name,var_addr,type);    
}
int peos_tcl_script(peos_tcl* ptcl, char* file_name)
{
    int status;
    status = Tcl_EvalFile(ptcl->interp, file_name);
    if (*ptcl->interp->result != 0){
        fprintf(stderr,"Issue Running Script: %s\n", ptcl->interp->result); 
    }
#ifdef TCLINTERP_DEBUG
    fprintf(stderr,"Script ran, result: %s\n", status == TCL_OK ? "TCL_OK" : "TCL_ERROR");
#endif
    return status;
}

int peos_tcl_eval(peos_tcl* ptcl, char* name_str, char* eval_str, char* result_str )
{
    int result=-1;
    char* str=NULL;

    if(!(str=(char*)malloc(sizeof(char)*(strlen(eval_str)+strlen(name_str)+50)))){
    	fprintf(stderr,"ERROR: Insufficient memory, malloc failed in peos_tcl_eval\n");
    	return TCL_ERROR;
    }else{
        if(!strcmp(eval_str,"$$")){
	   sprintf(str,"set %s \\$\\$", name_str);
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
       fprintf(stderr,"ERROR: failed with TCL statement: %s\n", str);
       fprintf(stderr,"ERROR: Tcl returned TCL_ERROR in peos_tcl_eval %s\n",ptcl->interp->result);
       strcpy(result_str,eval_str);
    }else if(result == TCL_OK){
       strcpy(result_str,ptcl->interp->result);//peos_tcl_get_var(ptcl, name_str));
#ifdef TCLINTERP_DEBUG
       fprintf(stderr,"MESSAGE: successfully evaluated TCL statement: %s\n", str);
#endif
      // if(debug)fprintf(stderr,"MESSAGE: variable set: %s %s \n", name_str, peos_tcl_get_var(ptcl, name_str));
    }else{
       fprintf(stderr,"OTHER Tcl MESSAGE: Result %d in peos_tcl_eval\n", result);
       strcpy(result_str,eval_str);
    }
    free(str);
    return result;
}
#ifdef UNIT_TEST
#include "test_tclinterp.c"
#endif



