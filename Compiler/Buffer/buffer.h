/* REPLACE the header below with your file header (see CST8152_ASSAMG.doc for details).
 * File Name: buffer.h
 * Version: 1.12.01
 * Author: S^R
 * Date: January 9, 2012
 * Declarations and prototypes necessary for buffer implementation 
 * 
 */
#ifndef BUFFER_H_
#define BUFFER_H_

/* standard header files */
#include <stdio.h>  /* standard input/output */
#include <malloc.h> /* for dynamic memory allocation*/
#include <limits.h> /* implementation-defined data type ranges and limits */

/* constant definitions */
/* You can add your own constant definitions here */
#define R_FAIL_1 -1         /* fail return value */
#define R_FAIL_2 -2         /* fail return value */
#define LOAD_FAIL -2       /* load fail error */
#define SET_R_FLAG 1       /* realloc flag set value */


/* user data type declarations */
typedef struct BufferDescriptor {
       char * ca_head ;    /* pointer to the beginning of character array (character buffer) */
       int capacity ;      /* current dynamic memory size (in bytes) allocated to character buffer */
       char inc_factor;    /* character array increment factor */
       int addc_offset ;    /* the offset (in char elements) to the app-character location */
       int mark_offset ;   /* the offset (in chars elements) to the mark location */
       char r_flag;        /* reallocation flag */
       char mode;          /* operational mode indicator*/
} Buffer ;



/* function declarations */
Buffer * b_create ( int init_capacity, char inc_factor,char o_mode );
Buffer * ca_addc ( Buffer *pB, char symbol );
int b_reset ( Buffer *pB );
void b_destroy (Buffer *pB);
char ca_isfull (Buffer *pB);
int ca_getsize (Buffer *pB);
int ca_getcapacity (Buffer *pB);
int ca_setmark (Buffer *pB, int mark);
int ca_getmark (Buffer *pB);
int ca_getmode (Buffer *pB);
Buffer *ca_pack( Buffer *pB );
int ca_print (Buffer *pB);
int ca_load (FILE *fi,Buffer *pB);

#endif
