#ifndef _TCLINTERP_H_
#define _TCLINTERP_H_
#include <tcl.h>
#include <string.h>
#include <stdlib.h>
#if defined(__STDC__) || defined(HAS_STDARG)
#   include <stdarg.h>
#else
#   include <varargs.h>
#endif

#undef TCLINTERP_DEBUG
#define  DEFAULT_CAPACITY 30

typedef struct{
  Tcl_Interp *interp;
}peos_tcl;

extern int peos_tcl_script(peos_tcl* ptcl, char* file_name);
// Start the TCL interpreter (initialize)
extern int peos_tcl_start(peos_tcl** ptcl);
// Execute a TCL command contained in a string
extern int peos_tcl_exec_cmd TCL_VARARGS_DEF(Tcl_Interp*, arg1);
//int peos_tcl_exec_cmd_from_string(peos_tcl* ptcl, char* cmd);
//Terminate the TCL interpreter
extern int peos_tcl_delete(peos_tcl* ptcl);
// Release the TCL objects buffer
//extern int peos_tcl_release(peos_tcl* ptcl);
// Get a string value of a TCL variable
extern char* peos_tcl_get_var(peos_tcl* ptcl, char* var_name);
// Set a variable in TCL
extern int peos_tcl_set_var(peos_tcl* ptcl, char* var_name, char* var_value);
// Establish a link between the address of a C variable and a TCL variable
extern int peos_tcl_link_var(peos_tcl* ptcl, char* var_name, char* var_addr, int type);
// Global variable
//peos_tcl ptcl01;
extern int peos_tcl_eval(peos_tcl* ptcl, char* name_str, char* eval_str, char* result_str );
//peos_tcl* p;
#endif

/**eof**/
