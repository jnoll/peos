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
   
   cmd = "set";
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
     		strcpy(arg,"set");
		break;
	case 1:
		strcpy(arg,"foo");
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
  fail_unless(!strcmp(Tcl_GetStringFromObj(objv[0],NULL),"set"), "Expected \'set\'");
  fail_unless(!strcmp(Tcl_GetStringFromObj(objv[1],NULL),"foo"), "Expected \'foo\'");
  fail_unless(!strcmp(Tcl_GetStringFromObj(objv[2],NULL),"TESTING"), "Expected \'TESTING\'");


  if((*(info).objProc)((info).objClientData, interp, objc, objv) != TCL_OK){
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
    
    if(strcmp(result,"dog")){
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
    if(strcmp(result,"cat")){
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
    system("echo \"set foo {peos2004} \"> tcl_file_for_testing");
    status = Tcl_EvalFile(ptr->interp, "tcl_file_for_testing");
    fail_unless (status == TCL_OK,"Failed b/c status is not TCL_OK after executing a valid tcl script file");
    system("rm tcl_file_for_testing");
}
END_TEST

START_TEST(test_peos_tcl_eval_validinput)
{
    
    peos_tcl* ptcl;
    char* name_str="VARIABLE2";
    char* eval_str="${VARIABLE1}AND${VARIABLE1}";
    char* result_str=NULL;
    
    int result=-1;
    char* str=NULL;
    char* str_dollarsigns=NULL;
    if(peos_tcl_start(&ptcl)==TCL_ERROR){
       fail("Tcl interpreter did not start");
    }
    if(Tcl_Eval(ptcl->interp,"set VARIABLE1 VARIABLEVALUE1")!=TCL_OK){
       fail("Couldn't set a variable in the interpreter");
    }
    if(!(result_str=(char*)malloc(sizeof(char)*255))){
       fail("error allocating memory");
    }
    if(!(str_dollarsigns=(char*)malloc(sizeof(char)*255))){
       fail("error allocating memory");
    }
    if(!(str=(char*)malloc(sizeof(char)*(strlen(eval_str)+strlen(name_str)+50)))){
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
       fail("unexpected value for str_dollarsigns");
    }
    result=Tcl_Eval(ptcl->interp, str);
    /** Looking at the result **/
    if( result == TCL_ERROR){
       fail ("ERROR: Interpreter returned an unexpected TCL_ERROR");
    }else if(result == TCL_OK){
       strcpy(result_str,ptcl->interp->result);//peos_tcl_get_var(ptcl, name_str));
    }
    if(!strcmp(peos_tcl_get_var(ptcl, name_str),"VARIABLEVALUE1ANDVARIABLEVALUE1") ||
       !strcmp(result_str,"VARIABLEVALUE1ANDVARIABLEVALUE1")){
    }else{
       fail("Unexpected value for VARIABLE2");
    }
}
END_TEST

START_TEST(test_peos_tcl_eval_vardoesntexist)
{
    
    peos_tcl* ptcl;
    char* name_str="IDONTEXIST";
    char* eval_str="hi${IDONTEXIST}hi";
    char* result_str=NULL;
    
    int result=-1;
    char* str=NULL;
    char* str_dollarsigns=NULL;
    if(peos_tcl_start(&ptcl)==TCL_ERROR){
       fail("Tcl interpreter did not start");
    }
    if(!(result_str=(char*)malloc(sizeof(char)*255))){
       fail("error allocating memory");
    }
    if(!(str_dollarsigns=(char*)malloc(sizeof(char)*255))){
       fail("error allocating memory");
    }
    if(!(str=(char*)malloc(sizeof(char)*(strlen(eval_str)+strlen(name_str)+50)))){
       fail("error allocating memory");
    }else{
       if(!strcmp(eval_str,"$$")){
          fail("unexpected eval_str==\"$$\"");
       }else{
	   sprintf(str,"set %s %s", name_str, eval_str);
	}
    }
    result=Tcl_Eval(ptcl->interp, str);
    /** Looking at the result **/
    if( result == TCL_ERROR){
       fail_unless(!strcmp("can't read \"IDONTEXIST\": no such variable",ptcl->interp->result),"The interpreter should have complained about the non-existent variable.");
    }  
}
END_TEST

START_TEST(test_peos_tcl_eval_badtcl)
{
    
    peos_tcl* ptcl;
    char* name_str="IMBAD $${}{{";
    char* eval_str="${IDONTEXIST";
    char* result_str=NULL;
    
    int result=-1;
    char* str=NULL;
    char* str_dollarsigns=NULL;
    if(peos_tcl_start(&ptcl)==TCL_ERROR){
       fail("Tcl interpreter did not start");
    }
    if(!(result_str=(char*)malloc(sizeof(char)*255))){
       fail("error allocating memory");
    }
    if(!(str_dollarsigns=(char*)malloc(sizeof(char)*255))){
       fail("error allocating memory");
    }
    if(!(str=(char*)malloc(sizeof(char)*(strlen(eval_str)+strlen(name_str)+50)))){
       fail("error allocating memory");
    }else{
       if(!strcmp(eval_str,"$$")){
          fail("unexpected eval_str==\"$$\"");
       }else{
	   sprintf(str,"set %s %s", name_str, eval_str);
	}
    }
    result=Tcl_Eval(ptcl->interp, str);
    /** Looking at the result **/
    if( result == TCL_ERROR){
       fail_unless(!strcmp("missing close-brace for variable name",ptcl->interp->result),"The interpreter should have complained about the non-existent variable.");
    }
}
END_TEST


START_TEST(test_peos_tcl_delete)
{
    peos_tcl* ptcl;
    int result=-1;
   
    if(peos_tcl_start(&ptcl)==TCL_ERROR){
       fail("Tcl interpreter did not start");
    }
    if(Tcl_Eval(ptcl->interp,"set VAR VALUE")!=TCL_OK){
       fail("Couldn't set a variable in the interpreter");
    }else{
        fail_unless(!Tcl_InterpDeleted(ptcl->interp), "Tcl should have returned zero value because the interpreter hasn't been deleted.");
        Tcl_DeleteInterp(ptcl->interp);
        fail_unless(Tcl_InterpDeleted(ptcl->interp), "Tcl should have returned a non zero value because the interpreter is scheduled for deletion");
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

    tc = tcase_create("testing_peos_tcl_eval_validinput");
    suite_add_tcase(s, tc);
    tcase_add_test(tc, test_peos_tcl_eval_validinput);
    
    tc = tcase_create("testing_peos_tcl_eval_vardoesntexist");
    suite_add_tcase(s, tc);
    tcase_add_test(tc, test_peos_tcl_eval_vardoesntexist);
    
    tc = tcase_create("testing_peos_tcl_eval_badtcl");
    suite_add_tcase(s, tc);
    tcase_add_test(tc, test_peos_tcl_eval_badtcl);
    
    tc = tcase_create("testing_peos_tcl_delete");
    suite_add_tcase(s, tc);
    tcase_add_test(tc, test_peos_tcl_delete);
        
    sr = srunner_create(s);

    srunner_set_fork_status(sr, fork_status);

    srunner_run_all(sr, verbosity);
    nf = srunner_ntests_failed(sr);
    srunner_free(sr);
    suite_free(s);
    return (nf == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}