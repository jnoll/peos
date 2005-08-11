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
int peos_tcl_delete(peos_tcl* ptcl)
{
   if(!Tcl_InterpDeleted(ptcl->interp)){ 
   	Tcl_DeleteInterp(ptcl->interp);
   }
   return Tcl_InterpDeleted(ptcl->interp) ? TCL_OK : TCL_ERROR;
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
char* peos_tcl_get_tclf_dir(peos_tcl* ptcl, char* file_name)
{
    int max_depth = 20;
    int i;
    char * path = NULL;
    char * query = NULL;
    char * TCLF_DIR = getenv("TCLF_DIR");
    sprintf(stderr, "###Looking for %s\n", file_name);
    if(!path) path = (char*) malloc(sizeof(char)*256);
    if(!query) query = (char*) malloc(sizeof(char)*256);
    if(!path || !query) return NULL;//strcat((char*)malloc(sizeof(char)*256),"./");
    if (TCLF_DIR == NULL){
	Tcl_Eval(ptcl->interp, "exec find ./ -name tclf_*.tcl");
	path[0]='\0';
	while(!strcmp(ptcl->interp->result,"") && (max_depth-- > 0)){
		path=strcat(path,"../");
		sprintf(query,"exec find %s -name %s", path,file_name);
        	Tcl_Eval(ptcl->interp, query);
	}
	if (max_depth<=0)
		sprintf(path,"./%s", file_name);
	else{
		sprintf(path,"%s", ptcl->interp->result);
	}
    }else{
       sprintf(path,"./%s", file_name);
    }
    Tcl_EvalFile(ptcl->interp, path);
    if(query) free(query);
    return path;
}
int peos_tcl_script(peos_tcl* ptcl, char* file_name)
{
    int status;
    int max_depth = 20;
    int i;
    char * path = NULL;
    char * query = NULL;
    char * TCLF_DIR = getenv("TCLF_DIR");
    //fprintf(stderr, "## hi %s\n", file_name);
    if(!path) path = (char*) malloc(sizeof(char)*256);
    if(!query) query = (char*) malloc(sizeof(char)*256);
    if(!path || !query) return TCL_ERROR;
    
    if (TCLF_DIR == NULL){
	sprintf(query,"exec find ./ -name %s  -maxdepth 1", file_name);
	Tcl_Eval(ptcl->interp, query);
	strcpy(query,"");
	path[0]='\0';
	while(!strcmp(ptcl->interp->result,"") && (max_depth-- > 0)){
	        //fprintf(stderr, "## #%d\n", max_depth);
		path=strcat(path,"../");
		sprintf(query,"exec find %s -name %s -maxdepth 1", path,file_name);;
		//fprintf(stderr, "## %s\n", query);
        	Tcl_Eval(ptcl->interp, query);
	}
	if (max_depth<=0){
	        //fprintf (stderr, "#1 file_name %s \n", file_name);
		sprintf(path,"./%s", file_name);
	}else{
	       // fprintf (stderr, "#2 res %s \n", ptcl->interp->result);
		sprintf(path,"%s", ptcl->interp->result);
	}
	//fprintf(stderr, "## ????\n");
    }else{
      // fprintf (stderr, "#3 file_name %s \n", file_name);
       sprintf(path,"./%s", file_name);
    }
    status = Tcl_EvalFile(ptcl->interp, path);
    if (*ptcl->interp->result != 0){
        fprintf(stderr,"Issue Running Script: %s\n", ptcl->interp->result);
        fprintf(stderr,"in Directory: ");
        system("pwd");
        fprintf(stderr,"\n");
 
    }
#ifdef TCLINTERP_DEBUG
    fprintf(stderr,"Script ran, result[%s]: %s\n",*ptcl->interp->result , status == TCL_OK ? "TCL_OK" : "TCL_ERROR");
#endif
    if(path) free(path);
    if(query) free(query);
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
	   //sprintf(str,"set %s \\$\\$", name_str);
            sprintf(str, "set %s \\${%s}", name_str, name_str);
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



