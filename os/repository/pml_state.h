/* MODULE DESCRIPTION ****************************************
Description:  PML State Repository Interface Definition 

Date Created:  07/14/99 

Dependencies:  none
*************************************************************/

/* MODIFICATION RECORD ***************************************

*************************************************************/

/************************************************************/
#ifndef _PML_STATE_H_
#define _PML_STATE_H_
/************************************************************/

/* INCLUDES *************************************************/

#include <sys/types.h>

/************************************************************/

/* DEFINES **************************************************/

/* query operands */
#define EQ 1                  /*   ==   */
#define NE 2                  /*   !=   */
#define GT 3                  /*   >    */
#define LT 4                  /*   <    */
#define GE 5                  /*   >=   */
#define LE 6                  /*   <=   */

enum { ACTIVE, DELETED };     /* record active-deleted flags */
#define PML_MAX_PTH 256       /* maximum database path length */
#define PML_MAX_OBJ 10000    /* maximum database objects */ 
#define PML_MAX_ATR 1024        /* maximum attributes per object */
#define PML_BLK_SIZE 1024     /* default read buffer size */
#define PML_PRM_MSK 0666      /* permissions mask */
#define PML_SHM_KEY 0x5050    /* shared memory key */
#define PML_SEM_KEY 0x5050    /* access semaphore key */
#define PML_IDX_FILE "pmlstate.idx"  /* index file */
#define PML_DAT_FILE "pmlstate.dat"  /* data file */

/* index memory map size */
#define PML_MAP_SIZE PML_MAX_OBJ * sizeof(pml_rec_t)

/************************************************************/

/* STRUCTURE DEFINITIONS ************************************/

/* The following from the semctl man page, which however incorrectly 
   states that the GNU library defines semun in sem.h. -jn */
/* according to X/OPEN we have to define it ourselves */
union semun {
        int val;                    /* value for SETVAL */
        struct semid_ds *buf;       /* buffer for IPC_STAT, IPC_SET */
        unsigned short int *array;  /* array for GETALL, SETALL */
        struct seminfo *__buf;      /* buffer for IPC_INFO */
};


/* database open modes */
typedef enum { APPEND, TRUNCATE }pml_mode_t;

/* shared memory management structure */
typedef struct
{
  unsigned long nrec;
}pml_shm_t;

/* index record structure */
typedef struct 
{
  int deleted;                     /* marked for deletion */
  unsigned natr;                   /* number of attributes */
  off_t aofs[PML_MAX_ATR];         /* attribute offsets */
  unsigned long alen[PML_MAX_ATR]; /* attribute lengths */
  unsigned nval;                   /* number of attribute values */
  off_t vofs[PML_MAX_ATR];         /* attribute value offsets */
  unsigned long vlen[PML_MAX_ATR]; /* attribute value lengths */
}pml_rec_t;

/* pml object definition */
typedef pml_rec_t * pml_obj_t;

/************************************************************/

/* EXPORTED FUNCTION PROTOTYPES *****************************/

#ifdef __cplusplus
extern "C"{
#endif

int       pmlstate_open_repository(const char *, pml_mode_t);
int       pmlstate_close_repository(void);
pml_obj_t pmlstate_create_object(void);
int       pmlstate_delete_object(pml_obj_t);
int       pmlstate_pack_objects(void);
int       pmlstate_read_attribute(void *, void *, size_t, void *, size_t);
int       pmlstate_write_attribute(pml_obj_t, void *, size_t, void *, size_t);               
int       pmlstate_query_open(void* **, void *, size_t, int, void *, size_t);
int       pmlstate_query_close(void* **);

int       pmlstate_init();

#ifdef __cplusplus
}
#endif

/************************************************************/

/************************************************************/
#endif 
/************************************************************/
