/********************************************************\           
 * mylib.h --  Declaration file for mylib.c             *
 *                                                      *   
 * Authors:    James Douglas, Paul Souter,              *
 *             Ryan Swanepoel                           *   
 *                                                      *   
 * Purpose:    Declare each function in mylib.c         *
 *                                                      *
\********************************************************/

#ifndef MYLIB_H_
#define MYLIB_H_

#include <stddef.h>

extern void *emalloc(size_t);
extern void *erealloc(void *, size_t);
extern int getword(char*, int, FILE *);

#endif
