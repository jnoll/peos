#include <check.h>
#include <stdlib.h>
#include <unistd.h>		/* unlink() */
#include <sys/stat.h>		/* mkdir() */
#include <sys/types.h>		/* mkdir() */

#include "test_util.h"



#include "tclinterp.h"
#include <stdio.h>

/*
int peos_tcl_start(peos_tcl* ptcl)
{
     return TCL_OK;
}

int peos_tcl_exec_cmd TCL_VARARGS_DEF(Tcl_Interp*, arg1)
{
     return TCL_OK;
}

char* peos_tcl_get_var(peos_tcl* ptcl, char* var_name)
{
     return (char*)NULL;
}
int peos_tcl_set_var(peos_tcl* ptcl, char* var_name, char* var_value)
{
     return TCL_OK;
}
int peos_tcl_link_var(peos_tcl* ptcl, char* var_name, char* var_addr, int type)
{
     return TCL_OK;    
}
*/
/*
int peos_tcl_start(peos_tcl* ptcl)
{

  if(!started){
     started=1;
    // ptcl->argc=0;
     ptcl->interp=Tcl_CreateInterp();
     if (Tcl_Init(ptcl->interp) == TCL_ERROR) {
    return TCL_ERROR;
   }
  }
  return TCL_OK;
}*/
START_TEST(test_peos_tcl_start)
{
    Tcl_Interp* p=NULL;
    int temp;
    fail_unless(started == 0 || started == 1, "Variable \"started has an unexped value");
    fail_unless((p=Tcl_CreateInterp())!=NULL, "Tcl_CreateInterp did not return a valid token");
    fail_unless((temp = Tcl_Init(p)) == TCL_OK || (temp = Tcl_Init(p)) == TCL_ERROR,
      "Tcl_Init returned neither TCL_OK, nor TCL_ERROR");
    //fail_unless(
    Tcl_DeleteInterp(p);//==TCL_OK, "Tcl_DeleteInterp could not delete the interpreter");
}
END_TEST

START_TEST(test_peos_tcl_exec_cmd)
{

   //va_list arg_list;
   char *cmd;
   char *arg;
   char **argv; 
   int argc, i, max;
   Tcl_CmdInfo info;
   peos_tcl p1;
   Tcl_Interp* interp=NULL;
   Tcl_Obj **objv;	
   Tcl_Obj *resultPtr;
   int objc;
   char* arg1 = "puts stdout TESTING TCL";
   max=20;
   
   peos_tcl_start(&p1);
   interp = p1.interp;
   
//   interp = TCL_VARARGS_START(Tcl_Interp *, arg1, arg_list);
   if(interp == NULL) fail ("interp is NULL");
   
   Tcl_ResetResult(interp);
   
   cmd = "puts";//va_arg(arg_list, char *);
   if(cmd!=NULL){
      if(strlen(cmd)<0)
         fail("invalid string cmd");
   }

   if(cmd!=NULL){
      //fail_unless(Tcl_GetCommandInfo(interp,"puts",&(info)), "Tcl_GetCommandInfo couldn't find \'puts\'. This shouldn't have happened.");
      if(!Tcl_GetCommandInfo(interp, cmd, &(info) )){
       Tcl_AppendResult(interp, "unknown command \"", cmd, "\"", NULL);
     //  va_end(arg_list);
       fail("TCL_ERROR even though a known command was used");
    }
   }
   else fail("cmd is NULL, unexpected error");
 
   objv = (Tcl_Obj **) ckalloc(max * sizeof(Tcl_Obj *));
   objv[0] = Tcl_NewStringObj(cmd, strlen(cmd));
   Tcl_IncrRefCount(objv[0]); 
   objc = 1;
   arg=(char*)malloc(sizeof(char)*100);
   //for (objc = 1; objc < 4; objc++){
   
   while(1){//(sub_str=strtok(NULL," "))!=NULL){
     //arg=va_arg(arg_list, char *);
     //printf("arg: [%s]\n",arg);
     
     switch (objc){
     	case 0:
     		strcpy(arg,"puts");
		break;
	case 1:
		strcpy(arg,"stdout");
		break;
	case 2: 
		strcpy(arg,"TESTING");
		break;
////	case 3: strcpy(arg,"TCL");
//		break;
	default:
		arg=(char *)NULL;
		break;
     }
     if (arg==(char *)NULL){
        objv[objc] = (Tcl_Obj *)NULL;
        break;
     }
    //objc++;
     
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
    
    //va_end(arg_list);
    // system("echo F");
    
  }
  printf("\n");
  for( i = 0;i < objc;++i)
       printf( "-->%s<--\n", Tcl_GetStringFromObj(objv[i],NULL) );
  ///fail_unless(objc==5, "Incorrect number of objects in objc");
  fail_unless(!strcmp(Tcl_GetStringFromObj(objv[0],NULL),"puts"), "Expected \'puts\'");
  fail_unless(!strcmp(Tcl_GetStringFromObj(objv[1],NULL),"stdout"), "Expected \'stdout\'");
  fail_unless(!strcmp(Tcl_GetStringFromObj(objv[2],NULL),"TESTING"), "Expected \'TESTING\'");
 // fail_unless(!strcmp(Tcl_GetStringFromObj(objv[3],NULL),"TCL"), "Expected \'TCL\'");

  if((*(info).objProc)((info).objClientData, interp, objc, objv) != TCL_OK){
    fprintf(stderr,"result is error");
    fflush(0);
    fail("Tcl Error when executing command");
  }
  /*
  #ifdef DEBUG 
  printf("Tcl_GetStringResult(interp)==%s\n",Tcl_GetStringResult(interp));
  #else
  Tcl_GetStringResult(interp);
  #endif
  */
  
}
END_TEST

START_TEST(test_peos_tcl_get_var)
{
    char* result;
    int var1;
    peos_tcl p;
    started=0;
    result = (char*)malloc(sizeof(char)*100);
    peos_tcl_start(&p);
    Tcl_SetVar(p.interp,"var1","dog",0);
    strcpy(result,Tcl_GetVar(p.interp,"var1",TCL_GLOBAL_ONLY));
    printf("\nResult from get var: [%s] %d\n",result, (!strcmp(result,"dog")));
    
    if(strcmp(result,"dog")){
      printf("\nTCL_ERROR Getting Variable");
      fflush(0);
      fail("TCL_ERROR Getting Variable");
    }
    free(result);
}
END_TEST

START_TEST(test_peos_tcl_set_var)
{
    char* result;
    int var1;
    peos_tcl p;
    started=0;
    result = (char*)malloc(sizeof(char)*100);
    peos_tcl_start(&p);
    strcpy(result,Tcl_SetVar(p.interp,"var1","cat",0));
    printf("\nResult from set var: [%s] %d\n",result, (!strcmp(result,"cat")));
    if(strcmp(result,"cat")){
      printf("TCL_ERROR Setting Variable");
      fflush(0);
      fail("TCL_ERROR Setting Variable");
    }
    
}
END_TEST

START_TEST(test_peos_tcl_link_var)
{
    int result;
    int var1;
    peos_tcl p;
    started=0;
    peos_tcl_start(&p);
    result=Tcl_LinkVar(p.interp,"var1",&var1,TCL_LINK_INT);
    if(result!=TCL_OK){
      printf("TCL_ERROR Linking Variable");
      fflush(0);
      fail("TCL_ERROR Linking Variable");
    }
}
END_TEST

int
main(int argc, char *argv[])
{
    int nf;
    SRunner *sr;
    Suite *s = suite_create("tclinterp");
    TCase *tc;

    parse_args(argc, argv);

    tc = tcase_create("execute_tcl_command");
    suite_add_tcase(s,tc);
    tcase_add_test(tc, test_peos_tcl_start);
    tcase_add_test(tc, test_peos_tcl_exec_cmd);

    tc = tcase_create("set_variable_in_tcl");
    suite_add_tcase(s, tc);
    tcase_add_test(tc, test_peos_tcl_set_var);

    tc = tcase_create("get_variable_in_tcl");
    suite_add_tcase(s, tc);
    tcase_add_test(tc, test_peos_tcl_get_var);

    tc = tcase_create("link_variable_in_tcl");
    suite_add_tcase(s, tc);
    tcase_add_test(tc, test_peos_tcl_link_var);
    
/*   to do 
    tc = tcase_create("execute_script_in_tcl");
    suite_add_tcase(s, tc);
    tcase_add_test(tc, test_peos_tcl_exec_script);  
*/      
    sr = srunner_create(s);

    srunner_set_fork_status(sr, fork_status);

    srunner_run_all(sr, verbosity);
    nf = srunner_ntests_failed(sr);
    srunner_free(sr);
    suite_free(s);
    return (nf == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}