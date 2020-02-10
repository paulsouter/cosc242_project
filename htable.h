/********************************************************\           
 * htable.h -- declaration file for htable.c            *
 *                                                      *   
 * Authors:    James Douglas, Paul Souter,              *
 *             Ryan Swanepoel                           *   
 *                                                      *   
 * Purpose:    to declare each function in htable.c     *
 *                                                      *  
\********************************************************/

#ifndef HTABLE_H_
#define HTABLE_H_

#include <stdio.h>

typedef struct htablerec *htable;
typedef enum hashing_e{LINEAR_P, DOUBLE_H} hashing_t;

extern void htable_free(htable h);
extern int htable_insert(htable h, char *str);
extern htable htable_new(int capacity, hashing_t m);
extern void htable_print(htable h, void f(int x,char *str));
extern int htable_search(htable h, char *str);
extern void htable_print_entire_table(htable h);
extern void htable_print_stats(htable h, FILE *stream, int num_stats);

#endif
