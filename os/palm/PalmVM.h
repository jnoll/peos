/*
 * File: PalmVM.h
 * Author: Mark Mastroieni
 * Date: 11/15/02
 */

typedef enum {
  COMPLETE,
  SYSCALL,
  PROC_ERROR,
  INTERN_ERROR
} VM_state;
