#include <check.h>
#include <stdlib.h>
#include <unistd.h>		/* unlink() */
#include <sys/stat.h>		/* mkdir() */
#include <sys/types.h>		/* mkdir() */

#include "test_util.h"



#include "tclinterp.h"
#include <stdio.h>


START_TEST(test_peos_tcl_start)
{
    Tcl_Interp* p=NULL;
    int temp;
    fail_unless((p=Tcl_CreateInterp())!=NULL, "Tcl_CreateInterp did not return a valid token");
    fail_unless((temp = Tcl_Init(p)) == TCL_OK || (temp = Tcl_Init(p)) == TCL_ERROR,
      "Tcl_Init returned neither TCL_OK, nor TCL_ERROR");
    Tcl_DeleteInterp(p);
}
END_TEST

START_TEST(test_peos_tcl_exec_cmd)
{
   char *cmd;
   char *arg;
   int i, max;
   Tcl_CmdInfo info;
   peos_tcl* p;
   Tcl_Interp* interp=NULL;
   Tcl_Obj **objv;	
   int objc;
   max=20;
   
   peos_tcl_start(&p);
   interp = p->interp;
   
   if(interp == NULL) fail ("interp is NULL");
   
   Tcl_ResetResult(interp);
   
   cmd = "puts";
   if(cmd!=NULL){
      if(strlen(cmd)<0)
         fail("invalid string cmd");
   }

   if(cmd!=NULL){
      if(!Tcl_GetCommandInfo(interp, cmd, &(info) )){
       Tcl_AppendResult(interp, "unknown command \"", cmd, "\"", NULL);
       fail("TCL_ERROR even though a known command was used");
      }
   }
   else fail("cmd is NULL, unexpected error");
 
   objv = (Tcl_Obj **) ckalloc(max * sizeof(Tcl_Obj *));
   objv[0] = Tcl_NewStringObj(cmd, strlen(cmd));
   Tcl_IncrRefCount(objv[0]); 
   objc = 1;
   arg=(char*)malloc(sizeof(char)*100);
   
   
   while(1){
     
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
	default:
		arg=(char *)NULL;
		break;
     }
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
  
  }
  printf("\n");
  for( i = 0;i < objc;++i)
       printf( "-->%s<--\n", Tcl_GetStringFromObj(objv[i],NULL) );
  fail_unless(!strcmp(Tcl_GetStringFromObj(objv[0],NULL),"puts"), "Expected \'puts\'");
  fail_unless(!strcmp(Tcl_GetStringFromObj(objv[1],NULL),"stdout"), "Expected \'stdout\'");
  fail_unless(!strcmp(Tcl_GetStringFromObj(objv[2],NULL),"TESTING"), "Expected \'TESTING\'");


  if((*(info).objProc)((info).objClientData, interp, objc, objv) != TCL_OK){
    fprintf(stderr,"result is error");
    fflush(0);
    fail("Tcl Error when executing command");
  }
  
}
END_TEST

START_TEST(test_peos_tcl_get_var)
{
    char* result;
    peos_tcl* ptr;
    result = (char*)malloc(sizeof(char)*100);
    peos_tcl_start(&ptr);
    Tcl_SetVar(ptr->interp,"var1","dog",0);
    strcpy(result,Tcl_GetVar(ptr->interp,"var1",TCL_GLOBAL_ONLY));
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
    peos_tcl* ptr;
    result = (char*)malloc(sizeof(char)*100);
    peos_tcl_start(&ptr);
    strcpy(result,Tcl_SetVar(ptr->interp,"var1","cat",0));
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
    peos_tcl* ptr;
    peos_tcl_start(&ptr);
    result=(int)Tcl_LinkVar(ptr->interp,"var1",(char*)&var1,TCL_LINK_INT);
    if(result!=TCL_OK){
      printf("TCL_ERROR Linking Variable");
      fflush(0);
      fail("TCL_ERROR Linking Variable");
    }
}
END_TEST

START_TEST(test_peos_tcl_script)
{
    int status;
    peos_tcl* ptr;
    peos_tcl_start(&ptr);
    system("touch tcl_file_for_testing");
    system("echo \"puts stdout {testing TCL script running functionality. if you see this, it's working.} \"> tcl_file_for_testing");
    status = Tcl_EvalFile(ptr->interp, "tcl_file_for_testing");
    if (*ptr->interp->result != 0){
        printf("Issue Running Script: %s\n",ptr->interp->result); 
    }
    printf("Script ran, result: %s\n", status == TCL_OK ? "TCL_OK" : "TCL_ERROR");
    fail_unless (status == TCL_OK,"Failed b/c status is not TCL_OK after executing a valid tcl script file");
    system("rm tcl_file_for_testing");
}
END_TEST

START_TEST(test_peos_tcl_eval)
{
    
    peos_tcl* ptcl;
    char* name_str="VARIABLE2";
    char* eval_str="${VARIABLE1}AND${VARIABLE1}";
    char* result_str=NULL;
    
    int result=-1;
    char* str=NULL;
    char* str_dollarsigns=NULL;
    if(peos_tcl_start(&ptcl)==TCL_ERROR){
       printf("Tcl interpreter did not start");
       fail("Tcl interpreter did not start");
    }
    if(Tcl_Eval(ptcl->interp,"set VARIABLE1 VARIABLEVALUE1")!=TCL_OK){
       fprintf(stderr,"Couldn't set a variable in the interpreter\n");
       fprintf(stderr,"ERROR: %s", ptcl->interp->result);
       fail("Couldn't set a variable in the interpreter");
    }
    if(!(result_str=(char*)malloc(sizeof(char)*255))){
       fprintf(stderr,"ERROR: Insufficient memory, malloc failed in peos_tcl_eval\n");
       fail("error allocating memory");
    }
    if(!(str_dollarsigns=(char*)malloc(sizeof(char)*255))){
       fprintf(stderr,"ERROR: Insufficient memory, malloc failed in peos_tcl_eval\n");
       fail("error allocating memory");
    }
    if(!(str=(char*)malloc(sizeof(char)*(strlen(eval_str)+strlen(name_str)+50)))){
       fprintf(stderr,"ERROR: Insufficient memory, malloc failed in peos_tcl_eval\n");
       fail("error allocating memory");
    }else{
       if(!strcmp(eval_str,"$$")){
          fail("unexpected eval_str==\"$$\"");
       }else{
	   sprintf(str,"set %s %s", name_str, eval_str);
	   fail_unless(!strcmp("set VARIABLE2 ${VARIABLE1}AND${VARIABLE1}",str),"str value is incorrect");
	}
    }
    /** dollar signs **/
    sprintf(str_dollarsigns,"set %s \\$\\$", name_str);
    if(strcmp("set VARIABLE2 \\$\\$", str_dollarsigns)){
       printf("unexpected value for str_dollarsigns");
       fflush(0);
       fail("unexpected value for str_dollarsigns");
    }
    result=Tcl_Eval(ptcl->interp, str);
    /** Looking at the result **/
    if( result == TCL_ERROR){
       fprintf(stderr,"ERROR: failed with TCL statement: %s\n", str);
       fprintf(stderr,"ERROR: Tcl returned TCL_ERROR in peos_tcl_eval\n");
       fail ("ERROR: Interpreter returned an unexpected TCL_ERROR");
    }else if(result == TCL_OK){
       strcpy(result_str,ptcl->interp->result);//peos_tcl_get_var(ptcl, name_str));
       printf("MESSAGE: successfully evaluated TCL statement: %s\n", str);
       printf("MESSAGE: variable set: %s %s \n", name_str, peos_tcl_get_var(ptcl, name_str));
    }
    if(!strcmp(peos_tcl_get_var(ptcl, name_str),"VARIABLEVALUE1ANDVARIABLEVALUE1") ||
       !strcmp(result_str,"VARIABLEVALUE1ANDVARIABLEVALUE1")){
       printf("MESSAGE: Test Passed\n");
    }else{
       printf("Unexpected value for VARIABLE2");
       fail("Unexpected value for VARIABLE2");
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

    tc = tcase_create("testing_execute_tcl_command");
    suite_add_tcase(s,tc);
    tcase_add_test(tc, test_peos_tcl_start);
    tcase_add_test(tc, test_peos_tcl_exec_cmd);

    tc = tcase_create("testing_set_variable_in_tcl");
    suite_add_tcase(s, tc);
    tcase_add_test(tc, test_peos_tcl_set_var);

    tc = tcase_create("testing_get_variable_in_tcl");
    suite_add_tcase(s, tc);
    tcase_add_test(tc, test_peos_tcl_get_var);

    tc = tcase_create("testing_link_variable_in_tcl");
    suite_add_tcase(s, tc);
    tcase_add_test(tc, test_peos_tcl_link_var);
    
    tc = tcase_create("testing_run_a_tcl_script");
    suite_add_tcase(s, tc);
    tcase_add_test(tc, test_peos_tcl_script);

    tc = tcase_create("testing_peos_tcl_eval");
    suite_add_tcase(s, tc);
    tcase_add_test(tc, test_peos_tcl_eval);
        
    sr = srunner_create(s);

    srunner_set_fork_status(sr, fork_status);

    srunner_run_all(sr, verbosity);
    nf = srunner_ntests_failed(sr);
    srunner_free(sr);
    suite_free(s);
    return (nf == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}