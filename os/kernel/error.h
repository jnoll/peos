/* @(#)error.h
 */

#ifndef _ERROR_H
#define _ERROR_H (1)

#define peos_error(...) peos_set_error_msg (__FILE__, __LINE__, __VA_ARGS__)

extern char peos_error_msg[];
extern char *peos_set_error_msg(char *file, int line, const char *fmt, ...);
extern void peos_perror(const char *s);

#endif /* _ERROR_H */

