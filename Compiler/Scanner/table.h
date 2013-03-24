/* Filename: table.h
 * Transition Table and function declarations necessary for the scanner  
 * as required for CST8152, Assignment #2, Winter 12.
 * Version: 1.12.01
 * The file is incomplete. You are to complete it.
 * Provided by: Svillen Ranev 
 ***************************************************
 * REPLACE THIS HEADER WITH YOUR HEADER
 ***************************************************
 */

#ifndef  TABLE_H_
#define  TABLE_H_ 

#ifndef BUFFER_H_
#include "buffer.h"
#endif

#ifndef NULL
#include <_null.h> /* NULL pointer constant is defined there */
#endif

/*   Source end-of-file (SEOF) sentinel symbol
 *    '\0' or 255 or 0xFF or EOF
 */

/*  Single-lexeme tokens processed separately one by one
 *  in the token-driven part of the scanner
 *  '=' , ' ' , '(' , ')' , '{' , '}' , == , != , '>' , '<' ,
 *       space
 *  !<comment , ',' , '"' , ';' , '-' , '+' , '*' , '/', <> ,
 *  .AND., .OR. , SEOF, 'wrong symbol',
 */


/*REPLACE *ESN* WITH YOUR ERROR STATE NUMBER*/
#define ES -2 /* Error state */
#define IS -1    /* Inavalid state */

/* State transition table definition */

#define TABLE_COLUMNS 7
/*transition table - type of states defined in separate table */
int  st_table[ ][TABLE_COLUMNS] = {
/* State 0 */  {1,4,3,4,IS,IS,IS,},
/* State 1 */  {1,1,1,2,ES,10,2,},
/* State 2 */  {IS,IS,IS,IS,IS,IS,IS},
/* State 3 */  {ES,3,3,12,6,7,7},
/* State 4 */  {ES,6,6,5,ES,ES,ES},
/* State 5 */  {ES,6,6,11,ES,9,9},
/* State 6 */  {ES,6,6,11,ES,8,8},
/* State 7 */  {IS,IS,IS,IS,IS,IS,IS},
/* State 8 */  {IS,IS,IS,IS,IS,IS,IS},
/* State 9 */  {IS,IS,IS,IS,IS,IS,IS},
/* State 10 */  {ES,ES,ES,ES,ES,ES,2},
/* State 11 */  {IS,IS,IS,IS,IS,IS,IS},
/* State 12 */  {ES,ES,11,11,ES,ES,ES},
/* State N */  {0,0,0,0,0,0,0} //default state 
};
/* Accepting state table definition */
/*REPLACE *N1*, *N2*, and *N3* WITH YOUR NUMBERS*/
#define ASWR     01  /* accepting state with retract */
#define ASNR     02  /* accepting state with no retract */
#define NOAS     03  /* not accepting state */

int as_table[ ] = {ASWR, ASNR, NOAS };

/* Accepting action function declarations */

/*FOR EACH OF YOUR ACCEPTING STATES YOU MUST PROVIDE
ONE FUNCTION PROTOTYPE. THEY ALL RETURN Token AND TAKE
ONE ARGUMENT: A string REPRESENTING A TOKEN LEXEME. */

Token aa_func02(char *lexeme); /*VID*/ 

Token aa_func07(char *lexeme); /*DIL*/

Token aa_func08(char *lexeme); /*FPL*/

Token aa_func09(char *lexeme); /*ES / ESR*/

Token aa_func11(char *lexeme); /*OIL*/
/*Replace XX with the number of the accepting state: 01, 03 and so on.*/

/* defining a new type: pointer to function (of one char * argument) 
   returning Token
*/  

typedef Token (*PTR_AAF)(char *lexeme);


/* Accepting function (action) callback table (array) definition */
/* If you do not want to use the typedef, the equvalent declaration is:
 * Token (*aa_table[])(char lexeme[]) = {
 */

/*HERE YOU MUST PROVIDE AN INITIALIZATION FOR AN ARRAY OF POINTERS
TO ACCEPTING FUNCTIONS. THE ARRAY HAS THE SAME SIZE AS as_table[ ].
YOU MUST INITIALIZE THE ARRAY ELEMENTS WITH THE CORRESPONDING
ACCEPTING FUNCTIONS (FOR THE STATES MARKED AS ACCEPTING IN as_table[]).
THE REST OF THE ELEMENTS MUST BE SET TO NULL.*/
PTR_AAF aa_table[ ] =
	{NULL,NULL,*aa_func02,NULL,NULL,NULL,NULL,*aa_func07,*aa_func08,*aa_func09,NULL,*aa_func09,*aa_func11};

/* Keyword lookup table (.AND. and .OR. are not keywords) */

#define KWT_SIZE  8

char * kw_table []= {
                      "ELSE",
                      "IF",
                      "INPUT",
                      "OUTPUT",
                      "PLATYPUS",
                      "REPEAT",
                      "THEN",
                      "USING"   
                     };

#endif
                     