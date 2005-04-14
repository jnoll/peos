/************************************************************************
 * File:	features.h						*
 *									*
 * Description:	This file contains useful type and macro definitions.	*
 ************************************************************************/

# ifndef FEATURES_H
# define FEATURES_H

typedef char *String;

# if defined (__STDC__) || defined (__GNUC__) || defined (__cplusplus)
# define ANSI_PROTOTYPES
# endif

#ifdef PALM
#define INIT_SECTION __attribute__ ((section ("init")))
#else
#define INIT_SECTION
#endif

#ifdef PALM
#define KRNL_SECTION __attribute__ ((section ("krnl")))
#else
#define KRNL_SECTION
#endif

# if defined (__cplusplus)
# define BEGIN_PROTOTYPES extern "C" {
# define END_PROTOTYPES }
# else
# define BEGIN_PROTOTYPES
# define END_PROTOTYPES
# endif

# endif /* FEATURES_H */
