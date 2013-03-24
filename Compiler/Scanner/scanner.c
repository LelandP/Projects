/* Filename: scanner.c
 * /* PURPOSE:
 *    SCANNER.C: Functions implementing a Lexical Analyzer (Scanner)
 *    as required for CST8152, Assignment #2, Winter 12
 *    scanner_init() must be called before using the scanner.
 *    The file is incomplete;
 *    Provided by: Svillen Ranev
 *    Version: 1.12.01
 *    Date: 1 February 2012
 *******************************************************************
 *    REPLACE THIS HEADER WITH YOUR HEADER
 *******************************************************************
 */
#include <stdio.h>   /* standard input / output */
#include <ctype.h>   /* conversion functions */
#include <stdlib.h>  /* standard library functions */
#include <string.h>  /* string functions */
#include <limits.h>  /* integer type constants */
#include <float.h>   /* floating-point type constants */

/*#define NDEBUG        to suppress assert() call */
#include <assert.h>  /* assert() prototype */

/* project header files */
#include "buffer.h"
#include "token.h"
#include "table.h"

#define DEBUG  /* for conditional processing */
#undef  DEBUG

#define FALSE -1 /*for boolean testing*/
#define TRUE 1   /*for boolean testing*/

/* Global objects - variables */
/* This buffer is used as a repository for string literals.
   It is defined in platy_st.c */
extern Buffer * str_LTBL; /*String literal table  */
int line; /* current line number of the source code */
extern int scerrnum;     /* defined in platy_st.c - run-time error number */

/* No other global variable declarations/definitiond are allowed */

/* Function prototypes */
static int char_class(char c); /* character class function */
static int get_next_state(int, char, int *); /* state machine function */
static int iskeyword(char * kw_lexeme); /*keywords lookup functuion */
static long atool(char * lexeme); /* converts octal string to decimal value */
void reset_flags(int* flag,int* flag2, int* flag3, int* flag4, int* flag5, int* flag6);/*resets flags to 0*/

void scanner_init(Buffer *buf)
{
		ca_addc(buf, '\0'); /* in case EOF is not in the buffer */
		b_reset(str_LTBL);  /* reset the string literal table */
		line = 1;           /*set the source code line number to 1*/
}

Token malpar_next_token(Buffer * sc_buf)
{
		Token t;         /*token to be returned */
		unsigned char c; /* input symbol */
		Buffer *lex_buf; /* temporary buffer for holding lexemes */       
		int accept = NOAS; /* Not Accepting State */
		int state = 0;     /* Start state in the Transition Table */
		int lexstart;      /* current lexeme start offset */ 
 static int forward;  /* current input char offset */                                       
/* 
forward is the offset from the beginning of the char buffer of the
input buffer (sc_buf) to the current character, which is to be processed
by the scanner.
lexstart is the offset from the beginning of the char buffer of the
input buffer (sc_buf) to the first character of the current lexeme,
which is being processed by the scanner.
*/ 
		
		
	   /* DECLARE YOUR VARIABLES HERE IF NEEDED */
 int VID_FLAG = 0; /*flag if start of VID reconised*/
 int FPL_FLAG = 0; /*flag for if FPL*/
 int DIL_FLAG = 0; /*flag for if DIL is reconised*/
 int OIL_FLAG = 0; /*flag for if octal literal is reconised*/
 int ZERO_FLAG = 0; /*flag for if first digit is a 0*/
 int NUM_FLAG = 0; /*flag if number of unknown type is reconised*/
 int i; /*generic counter for a for loop*/
 
 lexstart = sc_buf->addc_offset;
 forward = lexstart;
				
 while (1)/*constant used for infinite loop*/
 { 
	 c = sc_buf->ca_head[forward++];  /*may need to be ++forward if getting wierd errors*/

	 if (isalnum(c)==0 || c=='"')
	 {
		 switch (c)
		 {
		 case '"':
			 {
				 ca_setmark(sc_buf,forward);
				 c = sc_buf->ca_head[forward++];
				 if (c != '"') /*checks to see if it is an empty string*/
				 {
					 while (c = sc_buf->ca_head[forward++])
					 {
						 if (c=='\n' || c=='\r') /*potential weird string error*/
						 {
							 t.code = ERR_T;
							 if ((forward - ca_getmark(sc_buf)) >= 20)
							 {
								 for (i=0;i<17;i++)
								 {
									 t.attribute.err_lex[i] = sc_buf->ca_head[ca_getmark(sc_buf)+1];
								 }
								 t.attribute.err_lex[17]='.';
								 t.attribute.err_lex[18]='.';
								 t.attribute.err_lex[19]='.';
								 t.attribute.err_lex[20]='\0';
							 }
							 else
							 {
								 for (i=0;i<20;i++)
								 {
									 t.attribute.err_lex[i]= sc_buf->ca_head[ca_getmark(sc_buf)+1];
								 }
								 t.attribute.err_lex[20]='\0';
							 }
						 }
						 if (c == '"') 
						 {
							 t.attribute.str_offset = str_LTBL->addc_offset;
							 for (i=(ca_getmark(sc_buf)+1);i<forward;i++) /*forward may point to 1 past the quotation mark*/
							 {
								 ca_addc(str_LTBL,sc_buf->ca_head[i]);
							 }
							 ca_addc(str_LTBL,'\0'); /*end of string marker*/
							 t.code = STR_T;
						 }
					 }
				 }
				 else
				 {
					 t.code = ERR_T;
					 t.attribute.err_lex[0]='R';
					 t.attribute.err_lex[1]='U';
					 t.attribute.err_lex[2]='N';
					 t.attribute.err_lex[3]=' ';
					 t.attribute.err_lex[4]='T';
					 t.attribute.err_lex[5]='I';
					 t.attribute.err_lex[6]='M';
					 t.attribute.err_lex[7]='E';
					 t.attribute.err_lex[8]=' ';
					 t.attribute.err_lex[9]='E';
					 t.attribute.err_lex[10]='R';
					 t.attribute.err_lex[11]='R';
					 t.attribute.err_lex[12]='O';
					 t.attribute.err_lex[13]='R';
					 t.attribute.err_lex[14]=':';
					 t.attribute.err_lex[15]='\0';
				 }
				 break;
			 }
		 case '+':
			 {
				 t.code = ART_OP_T;
				 t.attribute.arr_op = PLUS;
				 break;
			 }

		 case '-':
			 {
				 t.code = ART_OP_T;
				 t.attribute.arr_op = MINUS;
				 break;
			 }

		 case '*':
			 {
				 t.code = ART_OP_T;
				 t.attribute.arr_op = MULT;
				 break;
			 }

		 case '/':
			 {
				 t.code = ART_OP_T;
				 t.attribute.arr_op = DIV;
				 break;
			 }

		 case '{':
			 {
				 t.code = LPR_T;
				 break;
			 }

		 case '}':
			 {
				 t.code = RPR_T;
				 break;
			 }

		 case '>':
			 {
				 t.code = REL_OP_T;
				 t.attribute.rel_op = GT;
				 break;
			 }

		 case '<':
			 {
				 t.code = REL_OP_T;
				 t.attribute.rel_op = LT;
				 break;
			 }

		 case '=':
			 {
				 if (sc_buf->ca_head[forward++] == '=')
				 {
					 t.code = REL_OP_T;
					 t.attribute.rel_op = EQ;
				 }
				 else
				 {
					 forward--;
					t.code = ASS_OP_T;
				 }
				 break;
			 }

		 case ' ':
			 {
				 //c = sc_buf->ca_head[forward++];
				 break;
			 }
		 case '.':
			 {	
				 c= sc_buf->ca_head[forward++];
				 if (c == 'A')
				 {	
					 c= sc_buf->ca_head[forward++];
					 if (c == 'N')
					 {
						 c = sc_buf->ca_head[forward++];
						 if (c =='D')
						 {
							 t.code = LOG_OP_T;
							 t.attribute.log_op = AND;
						 }
						 else
						 {
							 t.code = ERR_T;
						 }
					 }
					 else
					 {
						 t.code = ERR_T;
					 }
				 }
				 if (c == 'O')
				 {
					 c = sc_buf->ca_head[forward++];
					 if (c == 'R')
					 {
						 t.code = LOG_OP_T;
						 t.attribute.log_op = OR;
					 }
					 else 
					 {
						 t.code = ERR_T;
					 }
				 }
				 else
				 {
					 t.code = ERR_T;
				 }
				 break;
			 }
		 case '!':
			 {	
				 c = sc_buf->ca_head[forward++];
				 if (c != '<')
				 {
					 t.code = ERR_T;
					 forward--;
				 }
				 if (c == '=')
				 {
					 t.code = REL_OP_T;
					 t.attribute.rel_op = NE;
				 }
				 else
				 {
					 while (sc_buf->ca_head[forward++] != '\n'){} //move through buffer untill comment is done
				 }
				 break;
			 }
		 case ';':
			 {
				 t.code = EOS_T;
				 break;
			 }
		 case '(':
			 {
				 t.code = LBR_T;
				 break;
			 }
		 case ')':
			 {
				 t.code = RBR_T;
				 break;
			 }
		 case EOF:
			 {
				 t.code=SEOF_T;
				 break;
			 }
		 default:/* may pick up others like string, so wierd errors could come from here*/
			 {
				 t.code = ERR_T;
				 t.attribute.err_lex[0] = c;
				 t.attribute.err_lex[1] = '/0'; /*store end of string inidicator to the string*/
				 break;
			 }
		 }
	 }
	 else /*character is a digit or letter*/
		 /*final state machine*/
	 {
		 ca_setmark(sc_buf,forward);
		 
		 state = 0;
		 lexstart = ca_getmark(sc_buf);
		 state = get_next_state (state,c,&accept);

		 c = sc_buf->ca_head[forward++];

		 while (accept == NOAS)
			 {
				 state = get_next_state(state,c,&accept);
				 if (accept == NOAS) /*if statemet to check if sate has changed*/
				 {
					 c = sc_buf->ca_head[forward++];
				 }
			 }

		 lex_buf = b_create(sc_buf->capacity,sc_buf->inc_factor,sc_buf->mode);

		 if (state == (2||7||8||11))
		 {
			 forward = lexstart;
		 }
		 
		 c = sc_buf->ca_head[lexstart];

		 while (1)
		 {
			 switch (c)/*character most recently added in*/
			 {
			 case 0: /*[a-zA-Z]*/
				 {
					 /*VID or SVID or Keyword may need to take this out check w/ leland*/
					 if (iskeyword(lex_buf->ca_head) != FALSE) /*keyword token found*/
					 {
						 if (lex_buf->addc_offset>0)
						 {
							 forward--; /*character not added to lex_buf move pointer back 1*/
						 }
						 t.code = KW_T;
						 t.attribute.kwt_idx = iskeyword(lex_buf->ca_head); /*set attribute*/
					 }
					 else
					 {
						 /*check to see if any of the identifier flags have been checked*/
						 if (FPL_FLAG!=0||DIL_FLAG!=0||OIL_FLAG!=0||NUM_FLAG!=0)
						 {
							 if (FPL_FLAG == 1) 
							 {
								 if (lex_buf->addc_offset>0)
								 {
									 forward--; /*character not added to lex_buf move pointer back 1*/
								 }
								 reset_flags(&FPL_FLAG,&DIL_FLAG,&OIL_FLAG,&ZERO_FLAG,&VID_FLAG,&NUM_FLAG);
								 t=aa_table[8](lex_buf->ca_head);
							 }
							 if (DIL_FLAG == 1) 
							 {
								 if (lex_buf->addc_offset>0)
								 {
									 forward--; /*character not added to lex_buf move pointer back 1*/
								 }
								 reset_flags(&FPL_FLAG,&DIL_FLAG,&OIL_FLAG,&ZERO_FLAG,&VID_FLAG,&NUM_FLAG);
								 t=aa_table[7](lex_buf->ca_head);

							 }
							 if (OIL_FLAG == 1) 
							 {
								 if (lex_buf->addc_offset>0)
								 {
									 forward--; /*character not added to lex_buf move pointer back 1*/
								 }
								 reset_flags(&FPL_FLAG,&DIL_FLAG,&OIL_FLAG,&ZERO_FLAG,&VID_FLAG,&NUM_FLAG);
								 t=aa_table[12](lex_buf->ca_head);
							 }
							 if (ZERO_FLAG == 1) 
							 {
								 if (lex_buf->addc_offset>0)
								 {
									 forward--; /*character not added to lex_buf move pointer back 1*/
								 }
								 reset_flags(&FPL_FLAG,&DIL_FLAG,&OIL_FLAG,&ZERO_FLAG,&VID_FLAG,&NUM_FLAG);
								 t=aa_table[7](lex_buf->ca_head);
							 }
							 if (NUM_FLAG==1&&ZERO_FLAG==0) 
							 {
								 if (lex_buf->addc_offset>0)
								 {
									 forward--; /*character not added to lex_buf move pointer back 1*/
								 }
								 reset_flags(&FPL_FLAG,&DIL_FLAG,&OIL_FLAG,&ZERO_FLAG,&VID_FLAG,&NUM_FLAG);
								 t=aa_table[7](lex_buf->ca_head);
							 }

						 }
						 VID_FLAG = 1; /*set variable identifier flag*/
					 }
					 ca_addc(lex_buf,c);
					 break;
				 }
			 case 1: /*0*/
				 {
					 /*octal or DIL 0 or FPL 0*/
					 if (ZERO_FLAG==1)/*leading 0 and another digit*/
					 {
						 OIL_FLAG=1;
					 }
					 if (FPL_FLAG==0&&NUM_FLAG==0&&DIL_FLAG==0&&ZERO_FLAG==0)
					 {
						 ZERO_FLAG=1; /*identify as a numeric token starting with 0*/
					 }
					 if (NUM_FLAG==1&&ZERO_FLAG==0) /*number with no leading 0*/
					 {
						 /*could be FPL or DIL*/
					 }
					 if (VID_FLAG==1)
					 {
						 reset_flags(&FPL_FLAG,&DIL_FLAG,&OIL_FLAG,&ZERO_FLAG,&VID_FLAG,&NUM_FLAG);
						 VID_FLAG=1;
					 }
					 /*if FPL_FLAG is set it is known to be an FPL*/
					 ca_addc(lex_buf,c);
					 break;
				 }
			 case 2: /*[8-9]*/ 
				 {
					 if (ZERO_FLAG==1&&FPL_FLAG==0&&OIL_FLAG==0&&DIL_FLAG==0)/*leading 0 and no decimal place*/
					 {
						 reset_flags(&FPL_FLAG,&DIL_FLAG,&OIL_FLAG,&ZERO_FLAG,&VID_FLAG,&NUM_FLAG);
						 /*number is single 0*/
						 if (lex_buf->addc_offset > 0)
						 {
							 forward--;
						 }
						 t=aa_table[7](lex_buf->ca_head);
					 }
					 if (OIL_FLAG==1)
					 {
						 reset_flags(&FPL_FLAG,&DIL_FLAG,&OIL_FLAG,&ZERO_FLAG,&VID_FLAG,&NUM_FLAG); /*reset the flags*/
						 if (lex_buf->addc_offset > 0)
						 {
							 forward--; 
						 }
						 t=aa_table[12](lex_buf->ca_head);

					 }
					 if (NUM_FLAG==0&&DIL_FLAG==0&&FPL_FLAG==0) /*no preceeding digits*/
					 {
						 /*could be a DIL or FPL*/
						 NUM_FLAG=1;
					 }
					 if (VID_FLAG==1)/*is already known VID*/
					 {
						 reset_flags(&FPL_FLAG,&DIL_FLAG,&OIL_FLAG,&ZERO_FLAG,&VID_FLAG,&NUM_FLAG);
						 VID_FLAG=1;
					 }
					 ca_addc(lex_buf,c);
					 break;
				 }
			 case 3: /*[1-7]*/
				 {
					 if (ZERO_FLAG==1&&OIL_FLAG==1) /*already known oil*/
					 {
						 /*still oil*/
					 }
					 if (ZERO_FLAG==1&&OIL_FLAG==0)/*leading 0 with no other digits*/
					 {
						 OIL_FLAG=1;
					 }
					 if (NUM_FLAG==0&&FPL_FLAG==0&&DIL_FLAG==0)/*no numeric flags set*/
					 {
						 NUM_FLAG=1;
					 }
					 if (VID_FLAG==1)
					 {
						 reset_flags(&FPL_FLAG,&DIL_FLAG,&OIL_FLAG,&ZERO_FLAG,&VID_FLAG,&NUM_FLAG);
						 VID_FLAG=1;
					 }
					 ca_addc(lex_buf,c);
					 break;
				 }
			 case 4: /*.*/
				 {
					 if (NUM_FLAG==0&&ZERO_FLAG==0&&DIL_FLAG==0&&FPL_FLAG==0)/*if no numeric flags are set*/
					 {
						 reset_flags(&FPL_FLAG,&DIL_FLAG,&OIL_FLAG,&ZERO_FLAG,&VID_FLAG,&NUM_FLAG);
						 ca_addc(lex_buf,c);
						 t=aa_table[9](lex_buf->ca_head);
					 }
					 if (OIL_FLAG==1) /*if known OIL*/
					 {
						 reset_flags(&FPL_FLAG,&DIL_FLAG,&OIL_FLAG,&ZERO_FLAG,&VID_FLAG,&NUM_FLAG);
						 ca_addc(lex_buf,c);
						 t=aa_table[9](lex_buf->ca_head);
					 }
					  if (FPL_FLAG==1) /*if known FPL*/
					 {
						 reset_flags(&FPL_FLAG,&DIL_FLAG,&OIL_FLAG,&ZERO_FLAG,&VID_FLAG,&NUM_FLAG);
						 ca_addc(lex_buf,c);
						 t=aa_table[9](lex_buf->ca_head);
					 }
					 if ((DIL_FLAG==1||NUM_FLAG==1)&&FPL_FLAG==0)/*if identified as a DIL or NUM already*/
					 {
						 FPL_FLAG=1;
					 }
					 if (VID_FLAG==1) /*if VID*/
					 {
						 if (lex_buf->addc_offset>0)
						 {
							 forward--; /*character not added to lex_buf move pointer back 1*/
						 }
						 reset_flags(&FPL_FLAG,&DIL_FLAG,&OIL_FLAG,&ZERO_FLAG,&VID_FLAG,&NUM_FLAG);
						 t=aa_table[3](lex_buf->ca_head);
					 }
					 ca_addc(lex_buf,c);
					 break;
				 }
			 case 5: /*#*/
				 {
					 if (VID_FLAG==0&&FPL_FLAG==0&&DIL_FLAG==0&&OIL_FLAG==0&&ZERO_FLAG==0)
					 {
						 ca_addc(lex_buf,c);
						 t=aa_table[9](lex_buf->ca_head);
					 }
					 if (FPL_FLAG==1)
					 {
						 if(lex_buf->addc_offset>0)
						 {
							 forward--;
						 }
						 t=aa_table[8](lex_buf->ca_head);
					 }
					 if (VID_FLAG==1)
					 {
						 reset_flags(&FPL_FLAG,&DIL_FLAG,&OIL_FLAG,&ZERO_FLAG,&VID_FLAG,&NUM_FLAG);
						 ca_addc(lex_buf,c);
						 t=aa_table[2](lex_buf->ca_head);
					 }
					 break;
				 }
			 case 6: /*other*/
				 {
					 if (lex_buf->addc_offset!=0)
					 {
						 sc_buf->addc_offset--; /*character never written to lex_buf set pointer back 1*/
					 }
					 if ((c==';'||c ==' ')&&VID_FLAG==1)
					 {
						 reset_flags(&FPL_FLAG,&DIL_FLAG,&OIL_FLAG,&ZERO_FLAG,&VID_FLAG,&NUM_FLAG);
						 t = aa_table[2](lex_buf->ca_head);
					 }
					 if ((c==';'||c ==' ')&&FPL_FLAG==1)
					 {
						 reset_flags(&FPL_FLAG,&DIL_FLAG,&OIL_FLAG,&ZERO_FLAG,&VID_FLAG,&NUM_FLAG);
						 t=aa_table[8](lex_buf->ca_head);
					 }
					 if ((c==';'||c ==' ')&&DIL_FLAG==1)
					 {
						 reset_flags(&FPL_FLAG,&DIL_FLAG,&OIL_FLAG,&ZERO_FLAG,&VID_FLAG,&NUM_FLAG);
						 t=aa_table[7](lex_buf->ca_head);
					 }
					 if ((c==';'||c ==' ')&&OIL_FLAG==1)
					 {
						 reset_flags(&FPL_FLAG,&DIL_FLAG,&OIL_FLAG,&ZERO_FLAG,&VID_FLAG,&NUM_FLAG);
						 t=aa_table[11](lex_buf->ca_head);
					 }
					 if ((c==';'||c ==' ')&&NUM_FLAG==1)
					 {
						 reset_flags(&FPL_FLAG,&DIL_FLAG,&OIL_FLAG,&ZERO_FLAG,&VID_FLAG,&NUM_FLAG);
						 t=aa_table[11](lex_buf->ca_head);
					 }
					 if ((c==';'||c ==' ')&&ZERO_FLAG==1)
					 {
						 reset_flags(&FPL_FLAG,&DIL_FLAG,&OIL_FLAG,&ZERO_FLAG,&VID_FLAG,&NUM_FLAG);
						 t=aa_table[7](lex_buf->ca_head);
					 }
					 else
					 {
						 reset_flags(&FPL_FLAG,&DIL_FLAG,&OIL_FLAG,&ZERO_FLAG,&VID_FLAG,&NUM_FLAG);
						 ca_addc(lex_buf,c);
						 t=aa_table[9](lex_buf->ca_head);
					 }
					 break;
				 }
			 }
			 c=(lex_buf,sc_buf->ca_head[forward++]);/*add next character to the buffer*/
		 }
	 }
	 b_destroy(lex_buf);
	 return t;               
   }//end while(1)
}


//DO NOT MODIFY THE CODE OF THIS FUNCTION
//YOU CAN REMOVE THE COMMENTS

int get_next_state(int state, char c, int *accept)
{
	int col;
	int next;
	col = char_class(c);
	next = st_table[state][col];
#ifdef DEBUG
printf("Input symbol: %c Row: %d Column: %d Next: %d \n",c,state,col,next);
#endif
/*
The assert(int test) macro can be used to add run-time diagnostic to programs
and to "defend" from producing unexpected results.
assert() is a macro that expands to an if statement;
if test evaluates to false (zero) , assert aborts the program
(by calling abort()) and sends the following message on stderr:

Assertion failed: test, file filename, line linenum

The filename and linenum listed in the message are the source file name
and line number where the assert macro appears.
If you place the #define NDEBUG directive ("no debugging")
in the source code before the #include <assert.h> directive,
the effect is to comment out the assert statement.
*/
	   assert(next != IS);

/*
The other way to include diagnostics in a program is to use
conditional preprocessing as shown bellow. It allows the programmer
to send more details describing the run-time problem. 
Once the program is tested thoroughly #define DEBUG is commented out
or #undef DEBUF is used - see the top of the file.
*/ 
#ifdef DEBUG
	if(next == IS){
	  printf("Scanner Error: Illegal state:\n");
	  printf("Input symbol: %c Row: %d Column: %d\n",c,state,col);
	  exit(1);
	}
#endif
	*accept = as_table[next];
	return next;
}

int char_class (char c)
{
    int val;
    /*a-zA-Z*/
    if( isalpha(c)){
        val = 0;
    }
    /*0*/
    if(c == 0){
        val = 1;
    }
    /*8-9*/
    if(c == 8 || c == 9){
        val = 2;
    }
    /*1-7*/
    if(c == (1||2||3||4||5||6||7)){
        val = 3;
    }
    /*.*/
    if(c == '.'){
        val = 4;
    }
    /*#*/
    if(c == '#'){
        val = 5;
    }else{/*other*/
        val = 6;
    }
        
    return val;
}



/*HERE YOU WRITE THE DEFINITIONS FOR YOUR ACCEPTING FUNCTIONS. 

ACCEPTING FUNCTION FOR THE variable identifier (VID)
REPLACE XX WITH THE CORRESPONDING ACCEPTING STATE NUMBER

Token aa_funcXX(char lexeme[]){

WHEN CALLED THE FUNCTION MUST
1. CHECK IF THE LEXEME IS A KEYWORD.
   IF YES, IT MUST RETURN A TOKEN WITH THE CORRESPONDING ATTRIBUTE
   FOR THE KEYWORD. THE ATTRIBUTE CODE FOR THE KEYWORD
   IS ITS INDEX IN THE KEYWORD LOOKUP TABLE (kw_table in table.h).
   IF THE LEXEME IS NOT A KEYWORD, GO TO STEP 2.

2. CHECK THE LAST CHARACTER OF THE lexeme.
   IF IT IS # IT SETS a SVID TOKEN, OTHERWISE IT SETS a AVID TOKEN.
   IF THE lexeme ISd LONGER than VID_LEN (see token.h) CHARACTERS,
   ONLY FIRST VID_LEN CHARACTERS ARE STORED
   INTO THE VARIABLE ATTRIBUTE ARRAY vid_lex[].
   
   IN THE CASE OF THE SVID (STRING VARIABLE)
   IF THE lexime IS LONGER than VID_LEN characters,
   ONLY FIRST VID_LEN-1 CHARACTERS ARE STORED
   INTO THE VARIABLE ATTRIBUTE ARRAY vid_lex[],
   AND THEN THE # CHARACTER IS APPENDED TO THE NAME.

  return t;
}


*/
Token aa_func08(char lexeme[]){
    int i = 0;
    int f = 0;
    int intpart;
    int floatpart;
    float flt;
    Token t;
    char * tempstr0;
    char * tempstr1;
    flt = atof(lexeme);
    while (lexeme[i] != '.'){
        tempstr0[i] = lexeme[i];
        i++;
    }
    intpart = atoi(tempstr0);
    i++;
    while (lexeme[i] != '\0'){
        tempstr1[f] = lexeme[i];
        i++;
        f++;
    }
    floatpart = atoi(tempstr1);
    if (intpart > 32767||intpart < -32767){
        t.code = ERR_T;
        t.attribute.err_lex;
        return t;
    }
    if (floatpart<0||floatpart>65535){
        t.code = ERR_T;
        t.attribute.err_lex;
        return t;
    }
    t.code = FPL_T;
    t.attribute.flt_value = flt;
    return t;
}

Token aa_func07(char lexeme[]){

    Token t;
    int dec;
    dec = atoi(lexeme);
    t.code = INL_T;
    t.attribute.int_value = dec;
    return t;
}
/*
ACCEPTING FUNCTION FOR THE integer literal(IL) - octal constant (OIL)

Token aa_funcXX(char lexeme[]){

THE FUNCTION MUST CONVERT THE LEXEME REPRESENTING AN OCTAL CONSTANT
TO A DECIMAL INTEGER VALUE WHICH IS THE ATTRIBUTE FOR THE TOKEN.
THE VALUE MUST BE IN THE SAME RANGE AS the value of 2-byte int in C.
THIS FUNCTION IS SIMILAR TO THE FUNCTION ABOVE AND THEY CAN BE
COMBINED INTO ONE FUNCTION
THE MAIN DIFFERENCE IE THAT THIS FUNCTION CALLS
THE FUNCTION atool(char * lexeme) WHICH CONVERTS AN ASCII STRING
REPRESENTING AN OCTAL NUMBER TO INTEGER VALUE
IN CASE OF ERROR (OUT OF RANGE) THE FUNCTION MUST RETURN ERROR TOKEN
THE ERROR TOKEN ATTRIBUTE IS  lexeme

  return t;
}

ACCEPTING FUNCTION FOR THE ERROR TOKEN 

Token aa_funcXX(char lexeme[]){

THE FUNCTION SETS THE ERROR TOKEN. lexeme[] CONTAINS THE ERROR
THE ATTRIBUTE OF THE ERROR TOKEN IS THE lexeme ITSELF
AND IT MUST BE STORED in err_lex.  IF THE ERROR LEXEME IS LONGER
than ERR_LEN caharacters, only the first ERR_LEN character are
stored in err_lex.

  return t;
}
*/

long atool(char * lexeme){
    long num;
    num = atoi(lexeme);
    return num;
}


int iskeyword(char * kw_lexeme)
{
    int resp = FALSE;
    int i = 0;
    for(i = 0;i<KWT_SIZE;i++){
        if(kw_lexeme == kw_table[i])resp = i;
		if (resp != FALSE) return resp; 
    }
    return resp;
}

void reset_flags(int* flag,int* flag2, int* flag3, int* flag4, int* flag5, int* flag6)
{
	flag=0;
	flag2=0;
	flag3=0;
	flag4=0;
	flag5=0;
	flag6=0;
}