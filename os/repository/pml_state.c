/*pml_state.c*/

/*-------------------------------------------------------------
A Summary of changes made to this code by Summer 2000.

Function names had to be changed slightly.  When the generic 
exported interface functions took over the pml_state function
names (because the calls in the VM were not changed in Summer 2000),
the names in this file had to be made unique.


-------------------------------------------------------------*/

/* MODULE DESCRIPTION *****************************************
Description:  PML State Repository Implementation

Date Created: 07/14/99 

Dependencies: pml_state.h 
**************************************************************/

/* INCLUDES **************************************************/

#include <sys/types.h>
#include "pml_state.h" 
#include "repository.h"

#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/stat.h>

#include <unistd.h>

/*************************************************************/

/* INTERNAL FUNCTION PROTOTYPES ******************************/

/*************************************************************/

/* INTERNAL MACROS *******************************************/

/* semaphore read/write lock macros */
#define PML_RLOCK semop(semid, &r_lock[0], 2)
#define PML_RULOCK semop(semid, &r_ulock[0], 1)
#define PML_WLOCK semop(semid, &w_lock[0], 2)
#define PML_WULOCK semop(semid, &w_ulock[0], 1)

/*************************************************************/

/* INTERNAL DATA *********************************************/

static int idxfd = -1;                /* index file descriptor */
static int datfd = -1;                /* data file descriptor */
static int shmid = -1;                /* shared memory id */
static int semid = -1;                /* access semaphore id */
static pml_shm_t* shmp = NULL;        /* management structure */ 
static caddr_t idxmap  = MAP_FAILED;  /* index memory map */
static key_t shmkey    = PML_SHM_KEY; /* shared memory key */
static key_t semkey    = PML_SEM_KEY; /* access semaphore key */

/* index file and path */
char idxpth[PML_MAX_PTH] = PML_IDX_FILE; 

/* data file path */
char datpth[PML_MAX_PTH] = PML_DAT_FILE;

/* shared access read/write lock control structures */
struct sembuf r_lock[2]  = { 1, 0, 0, 0, 1, SEM_UNDO };
struct sembuf r_ulock[1] = { 0, -1, IPC_NOWAIT|SEM_UNDO };
struct sembuf w_lock[2]  = { 0, 0, 0, 1, 1, SEM_UNDO };
struct sembuf w_ulock[1] = { 1, -1, IPC_NOWAIT|SEM_UNDO };

/*************************************************************/

/* FUNCTION:  PML OPEN REPOSITORY *****************************
Description:  Opens and Initializes PML State Repository 

Inputs:  
          const char *path - database path

          pml_mode_t mode  - database open mode
            mode = APPEND    -  open and append 
            mode = TRUNCATE  -  truncate and open 

Outputs:  
          int result

          result = 0       -  ok 
          result < 0       -  error 

Notes:    Opens index and database files, creates read/write
          access semaphores, shared memory control structure,
          and maps index into virtual memory.  
        
WARNING:  
          Path argument added for convenience only.  Only one 
          pml repository may be opened at a time.  Opening 
          multiple pml repositories will result in corruption 
          of internal managment structures.
NOTE: (summer 2000) toward the end of this function, "floor"
      is commented out.  It was causing an undefined
      error, so it was removed.  It is not needed due to truncation
      regardless.
**************************************************************/
int pmlstate_open_repository(const char *path, pml_mode_t mode)
{
  /* local variable declarations */
  union semun semv;        /* semaphore initializer */
  off_t iofs = 0;          /* index file offset */

  /* build path */
  if (path != NULL)
  {
    strncpy(idxpth, path, PML_MAX_PTH - 1);
    strncat(idxpth, PML_IDX_FILE, PML_MAX_PTH - 1);
    strncpy(datpth, path, PML_MAX_PTH - 1);
    strncat(datpth, PML_DAT_FILE, PML_MAX_PTH - 1); 
  }

  /* append mode */
  if (mode == APPEND)
  {
    /* open index file */
    while ((idxfd = 
            open(idxpth, O_RDWR|O_CREAT, PML_PRM_MSK)) < 0)
    {
      if (errno == EINTR)
      {
        continue;
      }
      return -1;
    }

    /* open data file */
    while ((datfd = 
            open(datpth, O_RDWR|O_CREAT, PML_PRM_MSK)) < 0)
    {
      if (errno == EINTR)
      {
        continue;
      }

      pmlstate_close_repository();

      return -1;
    }
  }
  else if (mode == TRUNCATE)
  {
    /* open index file */
    while ((idxfd = 
            open(idxpth, O_RDWR|O_TRUNC, PML_PRM_MSK)) < 0)
    {
      if (errno == EINTR)
      {
        continue;
      }
      else if (errno == ENOENT)
      {
        while ((idxfd = 
                open(idxpth, O_RDWR|O_CREAT, PML_PRM_MSK)) < 0)
        {
          if (errno == EINTR)
          {
            continue;
          }

          return -1;
        }
      }
      else
      {
        return -1;
      }
    }

    /* open data file */
    while ((datfd = 
            open(datpth, O_RDWR|O_TRUNC, PML_PRM_MSK)) < 0)
    {
      if (errno == EINTR)
      {
        continue;
      }
      else if (errno == ENOENT)
      {
        while ((datfd = 
                open(datpth, O_RDWR|O_CREAT, PML_PRM_MSK)) < 0)
        {
          if (errno == EINTR)
          {
            continue;
          }

          pmlstate_close_repository();

          return -1;
        }
      }
      else
      {
        pmlstate_close_repository();

        return -1;
      }
    }
  }
  else
  {
    return -1;
  }

  /* map index file into virtual memory */
  if ((idxmap = mmap((caddr_t)0, PML_MAP_SIZE, 
       PROT_READ|PROT_WRITE, MAP_SHARED, idxfd, 0)) 
       == MAP_FAILED)
  {
    pmlstate_close_repository();
    return -1;
  }

  /* create shared memory segment */
  if ((shmid = 
       shmget(shmkey, sizeof(pml_shm_t), IPC_CREAT|PML_PRM_MSK)) < 0)
  {
    pmlstate_close_repository();
    return -1;  
  }

  /* attach to shared memory structure */
  if ((shmp = 
      (pml_shm_t *)shmat(shmid, (char *)0, PML_PRM_MSK)) 
       == (pml_shm_t *)-1)
  {
    pmlstate_close_repository();
    return -1;  
  }

  /* create access semaphore set */
  if ((semid = semget(semkey, 2, 0)) < 0)
  {
    if ((semid = semget(semkey, 2, IPC_CREAT|PML_PRM_MSK)) < 0)
    {
      pmlstate_close_repository();
      return -1;
    }

    semv.val = 0;
    if (semctl(semid, 0, SETVAL, semv) < 0 ||
        semctl(semid, 1, SETVAL, semv) < 0)
    {
      pmlstate_close_repository();
      return -1;
    }
  }

  /* lock table for reading */
  if (PML_WLOCK == 0)
  {
    /* set index offset */
    if ((iofs = lseek(idxfd, 0, SEEK_END)) < 0)
    {
      PML_WULOCK;
      pmlstate_close_repository();
      return -1;
    }

    /* get current record count */
    shmp->nrec = (unsigned long) /* floor*/ (iofs / sizeof(pml_rec_t));
     
    /* unlock table */
    PML_WULOCK;
  }

  return 0; 
}
/*************************************************************/

/* FUNCTION:  PML CLOSE REPOSITORY ****************************
Description:  Closes PML State Repository

Inputs:  None

Outputs:  
          int result

          result = 0       -  ok 
          result < 0       -  error 

Notes:  Closes index and data files, resets file descriptors
        and unmaps index from virtual memory.
*************************************************************/
int pmlstate_close_repository(void)
{
  /* write lock for i/o flushing */
  if (PML_WLOCK == 0) 
  {
    /* close index file */
    if (idxfd >= 0)
    {
      while(close(idxfd) < 0)
      {
        if (errno == EINTR)
        {
          continue;
        }
        break;
      }
    }

    /* close data file */
    if (datfd >= 0)
    {
      while(close(datfd) < 0)
      {
        if (errno == EINTR)
        {
          continue;
        }
        break;
      }
    }

    /* reset descriptors */
    idxfd = -1;
    datfd = -1;
 
    /* unmap index file */
    if (idxmap != MAP_FAILED)
    {
      munmap(idxmap, PML_MAP_SIZE);
    }

    /* reset memory map */
    idxmap = MAP_FAILED;

    /* unlock table */
    PML_WULOCK;

    return 0;
  }

  return -1;
}
/*************************************************************/

/* FUNCTION:  PML CREATE OBJECT *******************************
Description:  Creates a PML State Object

Inputs:   none 

Outputs:  
          pml_obj_t
            repository state object on success
            NULL on error

Notes:  Appends a blank index record to the pmlstate
        index file, and returns a pointer to index record
        location in virtual memory.  A maximum of PML_MAX_OBJ
        objects may be written to the pml state database.
*************************************************************/
pml_obj_t pmlstate_create_object(void)
{
  /* local variable declarations */  
  int err = 0;
  off_t iofs = 0;
  off_t dofs = 0;
  pml_rec_t rec; 
  pml_obj_t objp = NULL;

  /* initialize record structure */
  memset(&rec, 0, sizeof(pml_rec_t));

  /* lock table for writing */
  if (PML_WLOCK == 0)
  {
    /* seek to end of index file */
    iofs = lseek(idxfd, 0, SEEK_END);

    /* write index record */
    while (write(idxfd, &rec, 
           sizeof(pml_rec_t)) != sizeof(pml_rec_t))
    {
      if (errno == EINTR)
      {
        lseek(idxfd, 0, SEEK_END);
        continue;
      } 
      else
      {
        err = 1;
        break;
      }
    }

    if (!err)
    {
      /* increment record count */
      shmp->nrec++; 

      /* return record pointer */
      objp = 
      (pml_obj_t)(idxmap + ((shmp->nrec-1)*sizeof(pml_rec_t)));
    }

    /* unlock table */
    PML_WULOCK;
  }

  /* error */
  return objp;
}
/*************************************************************/

/* FUNCTION:  PML DELETE OBJECT *******************************
Description:  Marks a PML State Object for Deletion

Inputs:  
          pml_obj_t
            repository state object 

Outputs:  
          int result

          result = 0       -  ok 
          result < 0       -  error 

Notes:  Objects marked for deletion are not actually deleted
        until a call to pml_pack_objects is executed.
*************************************************************/
int pmlstate_delete_object(pml_obj_t objp)
{
  /* test for valid pointer */
  if (objp != NULL)
  {
    /* lock table for writing */
    if (PML_WLOCK == 0)
    {
      /* flag object for deletion */
      objp->deleted = DELETED;
    }
    
    /* unlock table */
    PML_WULOCK;

    /* no errors */
    return 0;
  }

  /* error */
  return -1;
}
/*************************************************************/

/* FUNCTION:  PML PACK OBJECTS ********************************
Description:  Removes PML State Objects Marked for Deletion

Inputs:  None 

Outputs:  
          int result

          result = 0       -  ok 
          result < 0       -  error 

Notes:  Unimplemented.
*************************************************************/
int pmlstate_pack_objects(void)
{
  /* lock table for reading */
  if (PML_RLOCK == 0)
  {
 
    /* unlock table */
    PML_RULOCK;

    /* no errors */
    return 0;
  }

  /* error */
  return -1;
}
/*************************************************************/

/* FUNCTION:  PML READ ATTRIBUTE ******************************
Description:  Reads a PML State Object Attribute

Inputs:  
          pml_obj_t objp
            repository state object

          void *atrp
            attribute to read

          size_t alen
            size of attribute to read 

          void *valp
            value read buffer 

          size_t vlen
            size of value read buffer

Outputs:  
          int result

          result = 0       -  ok 
          result < 0       -  error 

Notes:  Returns -1 if object attribute does not exist. 
*************************************************************/
int pmlstate_read_attribute(void* objptr, 
                       void *atrp, size_t alen,
                       void *valp, size_t vlen)
{
  /* cast the void* correctly */
  pml_obj_t objp = (pml_obj_t)objptr;
  /* local variable declarations */
  int i = 0;
  int err = 0;
  int nread = 0;
  int rlen = PML_BLK_SIZE;
  char *rbuf = NULL;

  /* test for valid input */
  if (objp == NULL || atrp == NULL || alen <= 0)
  {
    return -1;
  }

  /* allocate read buffer */
  rbuf = malloc(rlen);

  /* lock table for reading */
  if (PML_RLOCK == 0)
  {
    /* test for deleted object */
    if (!objp->deleted)
    {
      /* traverse attributes looking for match */
      for (i = 0; i < objp->natr; i++)
      {
        if (alen == objp->alen[i])
        {
          if (objp->alen[i] > rlen)
          {
            rbuf = realloc(rbuf, objp->alen[i]);
            rlen = objp->alen[i];
          }
          lseek(datfd, objp->aofs[i], SEEK_SET);
          while (read(datfd, rbuf, objp->alen[i]) 
                 != objp->alen[i])
          {
            if (errno == EINTR)
            {
              lseek(datfd, objp->aofs[i], SEEK_SET);
              continue;
            }
            else
            {
              err = 1;
              break;
            }
          }

          if (err)
          {
            break;
          }

          if (memcmp(rbuf, atrp, objp->alen[i]) == 0) 
          {
            if (vlen >= objp->vlen[i])
            {
              if (objp->vlen[i] > rlen)
              {
                rbuf = realloc(rbuf, objp->vlen[i]);
                rlen = objp->vlen[i];
              }
              lseek(datfd, objp->vofs[i], SEEK_SET);
              while (read(datfd, rbuf, objp->vlen[i])
                     != objp->vlen[i]) 
              {
                if (errno == EINTR)
                {
                  lseek(datfd, objp->vofs[i], SEEK_SET);
                  continue;
                }
                else
                {
                  err = 1;
                  break;
                }
              }
   
              if (err)
              {
                break;
              }
              else
              {
                memcpy(valp, rbuf, objp->vlen[i]); 
                nread = objp->vlen[i];
              }
            }
            else
            {
              break;
            }
          }
        }
      }
    }

    /* unlock table */
    PML_RULOCK;
  }

  /* free read buffer */
  free(rbuf);

  if (err)
  {
    nread = -1;
  }

  return nread;
} /* pmlstate_read_attribute */

/*************************************************************/
/* FUNCTION:  PML LIST ATTRIBUTES ******************************
Description:  For the input object, finds all attributes and
their values, fills array of char pointers with alternating
attribute/value pairs; attribs[i]==NAME, attribs[i+1]==VALUE. 

Inputs:  
          pml_obj_t objp
            repository state object
	    
	  char * attrib[]
	    array of all attribute names/values for the object
Outputs:  
          int result

          result >= 0       -  ok - result == # of attributes. 
          result < 0       -  error 

Notes:    Could easily define a struct of name/value pairs,
fill an array of these structs, instead of alternating name
and values.
*************************************************************/
int pml_list_attributes(void* objptr, char** attrib[], char** values[])
{
  /* cast the void* correctly */
  pml_obj_t objp = (pml_obj_t) objptr;

  /* local variable declarations */
  int i, j, k;
  int err = 0;
  int natr = 0;
  int rlen = PML_BLK_SIZE;
  char *rbuf = NULL;
  
  /* test for valid input */
  if (objp == NULL || objp->deleted) {
    return -1;
  }
  
  /* Allocate an array of natr char * */ 
  *attrib = calloc(objp->natr, sizeof(char*)); 
  *values = calloc(objp->natr, sizeof(char*)); 

  /* lock table for reading */
  if (PML_RLOCK == 0) {
    /* traverse attributes of the object */
    for (i = 0; i < objp->natr; i++) {
      (*attrib)[i] = malloc(objp->alen[i] + 1);
      rbuf = (*attrib)[i];
      lseek(datfd, objp->aofs[i], SEEK_SET); 
      while (read(datfd, rbuf, objp->alen[i]) != objp->alen[i]) {
	if (errno == EINTR) {
	  lseek(datfd, objp->aofs[i], SEEK_SET);
	  continue;
	} else {
	  err = 1;
	  break;
	}
      }
      
      if (err) {
	break;
      }
 
      /* Ugh.  Because the repository doesn't delete old attributes,
         we have to search through previous attributes to see if this
         one has already been seen. */
      for (k = 0; k < natr; k++) {
	  if (strcmp(rbuf, (*attrib)[k]) == 0) {
	      break;
	  }
      }

      if (k < natr) {
	  (*values)[k] = realloc((*values)[k], objp->vlen[i] + 1);
	  rbuf = (*values)[k];
      } else {
	  natr++;
	  (*values)[i] = malloc(objp->vlen[i] + 1);
	  rbuf = (*values)[i];
      }

      lseek(datfd, objp->vofs[i], SEEK_SET); 
      while (read(datfd, rbuf, objp->vlen[i]) != objp->vlen[i]) {
	if (errno == EINTR) {
	  lseek(datfd, objp->vofs[i], SEEK_SET);
	  continue;
	} else {
	  err = 1;
	  break;
	}
      } /* while */
      
      if (err) {
	break;
      }
    } 
    
    /* unlock table */
    PML_RULOCK;
  }
  
  if (err) {
    return -1;
  }
  
  return natr;
}

/* FUNCTION:  PML WRITE ATTRIBUTE *****************************
Description:  Writes an Attribute to a PML State Object

Inputs:  
          pml_obj_t objp
            repository state object

          void *atrp
            attribute to write 

          size_t alen
            size of attribute to write 

          void *valp
            value to write 

          size_t vlen
            size of value to write 

Outputs:  
          int result

          result = 0       -  ok 
          result < 0       -  error 

Notes:  A maximum of PML_MAX_ATR attributes may be written
        to an object.  
*************************************************************/
int pmlstate_write_attribute(pml_obj_t objp, 
                        void *atrp, size_t alen,
                        void *valp, size_t vlen)
{
  /* local variable declarations */  
  int err = 0;
  int rcode = -1;
  off_t dofs = 0;

  /* test for valid input */
  if (objp == NULL || 
      atrp == NULL || alen <= 0 || 
      valp == NULL || vlen <= 0)
  {
    return -1;
  }

  /* lock table for writing */
  if (PML_WLOCK == 0)
  {
    if (!objp->deleted || 
        objp->natr == PML_MAX_ATR)
    {
      dofs = lseek(datfd, 0, SEEK_END);
      objp->aofs[objp->natr] = dofs;
      objp->alen[objp->natr] = alen;
      while (write(datfd, atrp, alen) != alen)
      {
        if (errno == EINTR)
        {
          lseek(datfd, 0, SEEK_END);
          continue;
        }
        else
        {
          err = 1;
          break;
        }
      }

      if (!err)
      {
        dofs = lseek(datfd, 0, SEEK_END);
        objp->vofs[objp->natr] = dofs;
        objp->vlen[objp->natr] = vlen;
        while (write(datfd, valp, vlen) != vlen)
        {
          if (errno == EINTR)
          {
            lseek(datfd, 0, SEEK_END);
            continue;
          }
          else
          {
            err = 1;
            break;
          }
        }

        if (!err)
        {
          objp->natr++;
          rcode = 0;
        }
      }
    }

    /* unlock table */
    PML_WULOCK;
  }

  /* error */
  return rcode;
}
/*************************************************************/

/* FUNCTION:  PML QUERY OPEN **********************************
Description:  Queries the PML State Repository for all Objects
              Meeting Query Constraint

Inputs:  
          pml_obj_t *objp
            pointer to repository state object

          void *atrp
            attribute to compare 

          size_t alen
            size of attribute to compare 

          int op
            comparison operand (EQ, NE, GT, LT, GE, LE)

          void *valp
            value to compare 

          size_t vlen
            size of value to compare 

Outputs:  
          int nrecs 

          nrecs >= 0    -    number of records found 
          nrecs <  0    -    error 

Notes:  For queries which return nrecs > 0 pml_query_close
        must be called to free query result structures.
*************************************************************/
int pmlstate_query_open(void* **objptr, 
                   void *atrp, size_t alen, 
                   int op, 
                   void *valp, size_t vlen)
{
  /* cast the void* correctly */
  pml_obj_t** objp = (pml_obj_t**)objptr;

  /* local variable declarations */
  int i = 0;
  int j = 0;
  int err = 0;
  int nrec = 0;
  int opcmp = 0;
  int arlen = PML_BLK_SIZE;
  int vrlen = PML_BLK_SIZE;
  char *abuf = NULL;
  char *vbuf = NULL;
  pml_rec_t *recp = (pml_rec_t *)idxmap;

  /* validate argument inputs */
  if (objp == NULL || atrp == NULL || valp == NULL)
  {
    return -1;
  }

  /* allocate read buffer */
  abuf = malloc(arlen);
  vbuf = malloc(vrlen);

  /* lock table for reading */
  if (PML_RLOCK == 0)
  {
    /* traverse records */
    for (i = 0; i < shmp->nrec; i++, recp++)
    {
      /* skip deleted records */
      if (recp->deleted)
      {
        continue;
      }

      /* traverse attributes */ 
      for (j = 0; j < recp->natr; j++)
      {
        if (alen == recp->alen[j])
        {
          /* read attribute */
          if (recp->alen[j] > arlen)
          {
            abuf = realloc(abuf, recp->alen[i]);
            arlen = recp->alen[j];
          }
          lseek(datfd, recp->aofs[j], SEEK_SET);
          while (read(datfd, abuf, recp->alen[j])
                 != recp->alen[j])
          {
            if (errno == EINTR)
            {
              lseek(datfd, recp->aofs[j], SEEK_SET);
              continue;
            }
            else
            {
              err = 1;
              break;
            }
          }

          if (err)
          {
            break;
          }

          if (memcmp(abuf, atrp, alen) == 0)
          {
            /* read value */
            if (recp->vlen[j] > vrlen)
            {
              vbuf = realloc(vbuf, recp->vlen[j]);
              vrlen = recp->vlen[j];
            }
            lseek(datfd, recp->vofs[j], SEEK_SET);
            while (read(datfd, vbuf, recp->vlen[j])
                   != recp->vlen[j])
            {
              if (errno == EINTR)
              {
                lseek(datfd, recp->vofs[j], SEEK_SET);
                continue;
              }
              else
              {
                err = 1;
                break;
              }
            }

            if (err)
            {
              break;
            }

            switch (op)
            {
              case EQ:
                if (memcmp(valp, vbuf, vlen) == 0)
                {
                  opcmp = 1;
                }
                break;
  
              case NE:
                if (memcmp(valp, vbuf, vlen) != 0)
                {
                  opcmp = 1;
                }
                break;
  
              case GT:
                if (memcmp(valp, vbuf, vlen) < 0)
                {
                  opcmp = 1;
                }
                break;
 
              case LT:
                if (memcmp(valp, vbuf, vlen) > 0)
                {
                  opcmp = 1;
                }
                break;

              case GE:
                if (memcmp(valp, vbuf, vlen) <= 0)
                {
                  opcmp = 1;
                }
                break;

              case LE:
                if (memcmp(valp, vbuf, vlen) >= 0)
                {
                  opcmp = 1;
                }
                break;

              default:
                err = 1; 
                break;
            }

            if (err)
            {
              break;
            }

            if (opcmp)
            {
              if (!nrec)
              {
                *objp = malloc(sizeof(pml_rec_t *));
                **objp = recp;
              }
              else
              {
                *objp = 
                realloc(*objp, (nrec+1)*sizeof(pml_rec_t *));
                *(*objp+nrec) = recp;
              }
              opcmp = 0;
              nrec++;
            }
          }
        }
      }

      /* test for errors */
      if (err && nrec)
      {
        nrec = -1;
        free(*objp);
        break;
      }
      else if (err)
      {
        nrec = -1;
      }
    }

    /* unlock table */
    PML_RULOCK;
  }

  /* free buffers */
  free(abuf);
  free(vbuf);

  /* return number of records in result */
  return nrec;
} /* pmlstate_query_open */

/*************************************************************/

/* FUNCTION:  PML LIST OPEN **********************************
Description:  Queries the PML State Repository for ALL objects.
              
Inputs:  
          pml_obj_t *objp
            pointer to repository state object

Outputs:  
          int nobjs

          nobjs >= 0    -    number of objects found 
          nobjs <  0    -    error 

notes:  For queries which return nrecs > 0 pml_query_close
        must be called to free query result structures.
*************************************************************/
int pml_list_open(void* **objptr)
{
  /* cast the void* correctly */
  pml_obj_t* result;
  
  /* local variable declarations */
  int i = 0; /* loop counter */
  int nrec = 0; /* total # of objects in the repository */
  int num_obj = 0; /* the # of un-deleted objects in the rep. */
  pml_rec_t *recp = (pml_rec_t *)idxmap;
  
  /* validate argument inputs */
  if (objptr == NULL) {
    return -1;
  }   
  
  nrec = shmp->nrec;
  result = (pml_obj_t *) calloc(nrec, sizeof(pml_obj_t));

  /* lock table for reading */
  if (PML_RLOCK == 0) {
    /* traverse all records */
    for (i = 0; i < nrec; i++, recp++) {
      /* skip deleted records */
      if (!recp->deleted) 
	result[num_obj++] = recp; /* add object pointer from repository */

    }/* traversed all objects in the repository */
    
    /* unlock table */
    PML_RULOCK;
  }
  
  /* return number of *non-deleted* records/objects in repository */
  *objptr = (void **)result;
  return num_obj; 
}

/*************************************************************/

/* FUNCTION:  PML QUERY CLOSE *********************************
   Description:  Closes a Query Object
   
Inputs:  
          pml_obj_t *objp
            pointer to repository state object

Outputs:  
          int result

          result = 0       -  ok 
          result < 0       -  error 

Notes:  Frees query result.  Must be called for all queries
        which return nrecs > 0.
*************************************************************/
int pmlstate_query_close(void* **objptr)
{
  /* cast the void* correctly */
  pml_obj_t** objp = (pml_obj_t**)objptr;

  /* free query result */
  if (objp != NULL || *objp != NULL)
  {
    free(*objp);
    return 0;
  }

  return -1;
}

/*************************************************************

FUNCTION: PML FREE ALIST (free attribute list)
Purpose : free the memory allocated by a successful call to
          pml_list_attributes()-an attribute list & a values list. 
*************************************************************/
int pml_free_alist(char ** objptr, int num_elements)
{
  int i = 0;

  if (objptr == NULL || *objptr == NULL)
    return -1;
  
  /* free each element of the array independently */
  for (i = 0; i < num_elements; i++) {
    free(objptr[i]);
  }
  /* free the array itself */
  free(objptr);
  
  return 0; 
}

/*************************************************************/

/*-------------------------------------------------------------
  Function: pmlstate_init
  Purpose : To export an interface to the pml_state_repository
  -------------------------------------------------------------*/
int       pmlstate_init()
{
  repository_initexternal("PML-STATE",
			  pmlstate_open_repository,
			  NULL,    /* *** see comment below */
			  pmlstate_close_repository,
			  NULL,    /* *** see comment below */
			  pmlstate_create_object,
			  pmlstate_delete_object, 
			  pmlstate_pack_objects,
			  pmlstate_read_attribute,
			  pmlstate_write_attribute, 
			  pmlstate_query_open,
			  pmlstate_query_close,
			  NULL, NULL);
  return (int)0;
}

/* The NULL above is for the other close function for a unixfs close.  
   It requires a repository as an argument, and the pmlstate version 
   has a void argument list.  When the VM is changed, the generic version 
   of close could be made to accept a dummy argume of type repository* 
   to allow for either call to be made from within repository_close, 
   which is now pml_close_repository(void)  */

