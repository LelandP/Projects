/******************************************************************************
File name: buffer.c
Author: [Leland Peterson, 040657337]
Course: CST8152 - Compilers
Assignment: [ass1]
Date: 01/31/1012
Professor: Sv. Ranev
Purpose: Controles a buffer of chars un three modes
Function list:  b_create () ca_addc () b_reset () b_destroy () ca_isfull ()
	ca_getsize () ca_getcapacity () ca_setmark () ca_getmark () ca_getmode ()
	ca_pack() ca_print () ca_load ()
*******************************************************************************/

#include "buffer.h"

/********************************************************************************
Purpose: creates the buffer and initialised the char array and the variables
Author: Leland Peterson
History/Versions: 1.0
Called functions: malloc()
Parameters: int init_capacity, char inc_factor,char o_mode
Return value: Buffer *
Algorithm: switch that initiates the right values for the right mode
********************************************************************************/
Buffer * b_create ( int init_capacity, char inc_factor,char o_mode ){
	/*creates temprary variables to be returned*/
	Buffer* temp;
	char* tempar;
	/*checks for invalid peramiters*/
	if(init_capacity < 1)return NULL;

	temp = (Buffer*) malloc(sizeof(Buffer));
	/*checks for invalid peramiters*/
	if(temp==NULL)return NULL;
	tempar = (char*) malloc(init_capacity);
	/*checks for invalid peramiters and frees old buffer*/
	if(tempar==NULL){
		free(temp);
		return NULL;
	}
	temp->ca_head = tempar;
	/*finds the buffer mode and sets the correct vaiables i used IF statments to help
	error check*/
	if(o_mode == 'f'||inc_factor == 0){
		temp->mode = 0;
		temp->inc_factor = 0;
	}else if(o_mode == 'a'){
		temp->mode = 1;
		temp->inc_factor =inc_factor;
	}else if(o_mode == 'm'&& inc_factor>0 && inc_factor<=100){
		temp->mode = -1;
		temp->inc_factor =  inc_factor;
	}else return NULL;
	temp->capacity = init_capacity;
	temp->addc_offset = 0;
	temp->mark_offset = 0;
	temp->r_flag = 0;
	return temp;
}

/********************************************************************************
Purpose: adds one char in the buffer array and inflates if needed
Author: Leland Peterson
History/Versions: 1.0
Called functions: N/A
Parameters: Buffer *pB, char symbol
Return value: Buffer *
Algorithm: calcuales if buffer if full and finds the apropriate run mode, calculates
the new capacity and reallocs the new buffer
********************************************************************************/
Buffer * ca_addc ( Buffer *pB, char symbol ){
	/*creates temprary variables so i dont tamper with the buffer*/
	char* temp;
	int newcapacity = 0;
	temp = pB->ca_head;
	/*checks for invalid peramiters*/
	if(pB == NULL)return NULL;
	pB->r_flag = 0;
	/*checks if buffer if full and need to increase in size*/
	if(pB->addc_offset == pB->capacity){
		if((int)pB->mode == 0)return 0;
		if((int)pB->mode == 1)
		{
			/*calcualtes new capacity*/
			newcapacity = pB->capacity +(unsigned char) pB ->inc_factor;
			/*checks for invalid peramiters*/
			if (newcapacity < 0)return NULL;
			temp = (char*) realloc(temp,newcapacity);
			/*checks for invalid peramiters*/
			if(temp == NULL)return NULL;
			if(pB->ca_head != temp)pB->r_flag = 1;
			pB->capacity = newcapacity;
			pB->ca_head = temp;
		}

		if((int)pB->mode == -1){
			int available;
			int increment;
			if (pB->capacity==INT_MAX)return NULL;
			/*creates temprary variables for new capacity*/
			
			/*calcualtes new capacity*/
			available = INT_MAX - pB->capacity;
			increment = available * ((float)pB->inc_factor / (float)100);
			newcapacity = pB->capacity +(int) increment;
			/*checks for invalid peramiters*/
			if (newcapacity == 0 ){
				newcapacity = INT_MAX;
			}
			temp = (char*) realloc(temp,newcapacity);
			if(temp == NULL)return NULL;
			if(pB->ca_head != temp)pB->r_flag = SET_R_FLAG;
			pB->capacity = newcapacity;
			pB->ca_head = temp;
		}

	}
	/*add char to buffer*/
	pB->ca_head[pB->addc_offset]= symbol;
	pB->addc_offset++;
	return pB;
}

/********************************************************************************
Purpose: resets the buffer to default
Author: Leland Peterson
History/Versions: 1.0
Called functions: N/A
Parameters: Buffer *pB
Return value: int
Algorithm:
********************************************************************************/
int b_reset ( Buffer *pB ){
	/*checks for invalid peramiters*/
	if(pB == NULL)return R_FAIL_1;
	pB->addc_offset = 0;
	pB->mark_offset = 0;
	pB->r_flag = 0;
	return 0;
}

/********************************************************************************
Purpose: destroys the buffer
Author: Leland Peterson
History/Versions: 1.0
Called functions: free
Parameters: Buffer *pB
Return value: void
Algorithm:
********************************************************************************/
void b_destroy (Buffer *pB){
	/*checks for invalid peramiters*/
	if(!pB){
	free(pB->ca_head);
	pB->ca_head = NULL;
	free(pB);
	pB = NULL;
	}
}

/********************************************************************************
Purpose: checks if full
Author: Leland Peterson
History/Versions: 1.0 
Called functions: N/A
Parameters: Buffer *pB
Return value: char, 1,0,-1
Algorithm:
********************************************************************************/
char ca_isfull (Buffer *pB){
	/*checks for invalid peramiters*/
	if(pB == NULL)return R_FAIL_1;
	if(pB->addc_offset == pB->capacity)return 1;
	else return 0;
}

/********************************************************************************
Purpose: get the buffer size
Author: Leland Peterson
History/Versions: 1.0
Called functions: N/A
Parameters: Buffer *pB
Return value: int, size of buffer
Algorithm:
********************************************************************************/
int ca_getsize (Buffer *pB){
	/*checks for invalid peramiters*/
	if(pB == NULL)return R_FAIL_1;
	return pB->addc_offset;
}

/********************************************************************************
Purpose: gets the capacity
Author: Leland Peterson
History/Versions: 1.0 
Called functions: N/A
Parameters: Buffer *pB
Return value: int, capacity
Algorithm:
********************************************************************************/
int ca_getcapacity (Buffer *pB){
	/*checks for invalid peramiters*/
	if(pB == NULL)return R_FAIL_1;
	return pB->capacity;
}

/********************************************************************************
Purpose: sets the mark
Author: Leland Peterson
History/Versions: 1.0
Called functions: N/A
Parameters: Buffer *pB, int mark)
Return value: int, mark
Algorithm:
********************************************************************************/
int ca_setmark (Buffer *pB, int mark){
	/*checks for invalid peramiters*/
	if(pB == NULL)return R_FAIL_1;
	if(mark<0 || mark>pB->capacity)return R_FAIL_1;
	pB->mark_offset = mark;
	return mark;
}

/********************************************************************************
Purpose: get the mark in the buffer
Author: Leland Peterson
History/Versions: 1.0
Called functions: N/A
Parameters: Buffer *pB
Return value: int, mark
Algorithm:
********************************************************************************/
int ca_getmark (Buffer *pB){
	/*checks for invalid peramiters*/
	if(pB == NULL)return R_FAIL_1;
	return pB->mark_offset;
}

/********************************************************************************
Purpose: get the buffer mode 
Author: Leland Peterson
History/Versions: 1.0
Called functions: N/A
Parameters: Buffer *pB
Return value: int, 1,0,-1
Algorithm:
********************************************************************************/
int ca_getmode (Buffer *pB){
	/*checks for invalid peramiters*/
	if(pB == NULL)return R_FAIL_2;
	return pB->mode;
}

/********************************************************************************
Purpose: pack the buffer and adds 1 spot
Author: Leland Peterson
History/Versions: 1.0
Called functions: N/A
Parameters: Buffer *pB
Return value: Buffer *
Algorithm: calculates the smallest size of the buffer + 1 the reallocs the array
********************************************************************************/
Buffer *ca_pack( Buffer *pB ){
	/*creates temprary variables for packing the buffer*/
	int newcapacity;
	char* temp;
	/*checks for invalid peramiters*/
	if(pB == NULL)return NULL;
	/*calculates new size*/
	newcapacity = pB->addc_offset + 1;
	if (newcapacity < 0)return NULL;
	temp =(char*) realloc(pB->ca_head,newcapacity);
	if(pB->ca_head != temp)pB->r_flag = 1;
	/*checks for invalid peramiters*/
	if(temp == NULL)return NULL;
	pB->ca_head = temp;
	pB->capacity = newcapacity;
	return pB;
}

/********************************************************************************
Purpose: prints the contesnts of the buffer
Author: Leland Peterson
History/Versions: 1.0
Called functions: N/A
Parameters: Buffer *pB
Return value: int number of lines printed
Algorithm: loops through all the chars in the buffer
********************************************************************************/
int ca_print (Buffer *pB){
	/*creates temprary variables for the count of printed items*/
	int i = 0;
	/*checks for invalid peramiters*/
	if(pB == NULL)return R_FAIL_1;
	/*loops trough the whole buffer and prints the chars*/
	for(i=0;i<pB->addc_offset;i++){
		printf("%c",pB->ca_head[i]);
	}
	printf("\n");
	return i;

}

/********************************************************************************
Purpose: loads the file content into the buffer
Author: Leland Peterson
History/Versions: 1.0 
Called functions: ca_addc()
Parameters: FILE *fi,Buffer *pB)
Return value: int, number of chars loded in the file
Algorithm:loops through all the chars in the file and prints the current char
********************************************************************************/
int ca_load (FILE *fi,Buffer *pB){
	/*creates temprary variables to loops threw the files*/
	int count=0;
	char temp;
	/*checks for invalid peramiters*/
	if(pB == NULL)return R_FAIL_1;
	if(fi == NULL)return R_FAIL_1;
	/*loops trough the files getting a char*/
	while(!feof(fi)){
		temp = fgetc(fi);
		if(temp == -1)break;
		/*calles add to add to the buffer*/
		if(ca_addc(pB,(char)temp)==NULL)return LOAD_FAIL;
		count++;
	}
	return count;
}
