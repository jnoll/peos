/******************************************************************************
**  File Information: commonly used values and symbols 
******************************************************************************/


#ifndef _DEFS_H_
#define _DEFS_H_

typedef int    Boolean;

#ifndef FALSE
#define FALSE  0
#endif

#ifndef TRUE
#define TRUE  1
#endif

#ifndef STOP
#define STOP 2
#endif

Boolean old_format;
/* To generate text CPML file. */
#ifndef TEXT_MODE
#define TEXT_MODE "text"
#endif

#ifndef CPML_MODE
#define CPML_MODE "cpml"
#endif

/* To generate gdbm CPML file. */
#ifndef GDBM_MODE
#define GDBM_MODE "gdbm"
#endif

#endif	/* End of _DEFS_H_ */
