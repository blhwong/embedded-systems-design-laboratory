//Filename: Fixed.c
//Author: Duc Tran, Brandon Wong
//Initial Creation Date: September 4, 2013
//Description: 
//Lab Number: W 2-3:30
//TA : Omar & Mahesh
//Date of last revision :September 8, 2013
//Hardware Configuration : NONE
#include <stdio.h>
#include "fixed.h"


/****************Fixed_uDecOut2s***************
 Description: converts fixed point number to ASCII string
 format unsigned 32-bit with resolution 0.01
 range 0 to 999.99
 Input: unsigned 32-bit integer part of fixed point number
         greater than 99999 means invalid fixed-point number
 Output: null-terminated string exactly 6 characters plus null
 Examples
 12345 to "123.45"  
 22100 to "221.00"
   102 to "  1.02" 
    31 to "  0.31" 
100000 to "***.**"    */ 
void Fixed_uDecOut2s(unsigned long n, char *string){
	int integer = 0;
	int decimal = 0;
	
	if (n >  99999){
    sprintf(string,"***.**");
	  return;		
  }
		
	integer = n/100;
	decimal = n%100;
	sprintf(string,"%3i.%.2i",integer,decimal);
  return;
}

 /****************Fixed_uDecOut2***************
 outputs the fixed-point value on the OLED
 format unsigned 32-bit with resolution 0.01
 range 0 to 999.99
 Input: unsigned 32-bit integer part of fixed point number
         greater than 99999 means invalid fixed-point number
 Output: none but prints null-terminated string to board
 Examples
 12345 to "123.45"  
 22100 to "221.00"
   102 to "  1.02" 
    31 to "  0.31" 
100000 to "***.**"    */ 
void Fixed_uDecOut2(unsigned long n){
	int integer = 0;
	int decimal = 0;
	
	if (n >  99999){
    printf("***.**");
	  return;		
  }
		
	integer = n/100;
	decimal = n%100;
	printf("%3i.%.2i",integer,decimal);
  return;
}

/****************Fixed_sDecOut3s***************
 converts fixed point number to ASCII string
 format signed 32-bit with resolution 0.001
 range -9.999 to +9.999
 Input: signed 32-bit integer part of fixed point number
 Output: null-terminated string exactly 6 characters plus null
 Examples
  2345 to " 2.345"  
 -8100 to "-8.100"
  -102 to "-0.102" 
    31 to " 0.031" 
   
 */ 
void Fixed_sDecOut3s(long n, char *string){
	int integer = 0;
	int decimal = 0;
	 
	//sprinf(answer,%d,n)
	if(n > 9999 || n <-9999){
		sprintf(string,"**.***");
		return;
	}
	integer = n/1000;
	decimal = n%1000;
	if(n<0){
		decimal = decimal * (-1);
		integer = integer * (-1);
		sprintf(string,"-%i.%.3i",integer,decimal);
	}
	else{
		sprintf(string,"%2i.%.3i",integer,decimal);
	}
	return;
}
/****************Fixed_sDecOut3***************
 converts fixed point number to OLED
 format signed 32-bit with resolution 0.001
 range -9.999 to +9.999
 Input: signed 32-bit integer part of fixed point number
 Output: none but prints null-terminated string to board
 OLED has exactly 6 characters
 Examples
  2345 to " 2.345"  
 -8100 to "-8.100"
  -102 to "-0.102" 
    31 to " 0.031" 
 */ 
void Fixed_sDecOut3(long n){
  int integer = 0;
	int decimal = 0;
	 
	//sprinf(answer,%d,n)
	if(n > 9999 || n < -9999){
		printf("**.***");
		return;
	}
	integer = n/1000;
	decimal = n%1000;
	if(n<0){
		decimal = decimal * (-1);
		integer = integer * (-1);
		printf("-%i.%.3i",integer,decimal);
	}
	else{
		printf("%2i.%.3i",integer,decimal);
	}
	return;
}
/**************Fixed_uBinOut8s***************
 Description:  Convert unsigned 32-bit integer to a fix point value, and output to char string
 Input: unsigned 32-bit integer part of fixed point number
 Output: null-terminated string
Parameter output string
     0     "  0.00"
     2     "  0.01"
    64     "  0.25"
   100     "  0.39"
   50      "  1.95"
   512     "  2.00"
  5000     " 19.53"
 30000     "117.19"
255997     "999.99"
256000     "***.**"
*/
void Fixed_uBinOut8s(unsigned long n,  char *string){
  //fixed point number = I * res
	unsigned long decimal = 0;
	unsigned long integer = 0;
	if(n >= 256000){
		sprintf(string,"***.**");
		return;
	}
	decimal = (((n%256)*100/256));
	integer = n/256;
	sprintf(string,"%3lu.%.2lu",integer,decimal);
	return;
}

/**************Fixed_uBinOut8***************
 unsigned 32-bit binary fixed-point with a resolution of 1/256. 
 The full-scale range is from 0 to 999.99. 
 If the integer part is larger than 256000, it signifies an error. 
 The Fixed_uBinOut8 function takes an unsigned 32-bit integer part 
 of the binary fixed-point number and outputs the fixed-point value on the OLED. 
 Input: unsigned 32-bit integer part of fixed point number
 Output: none but prints null-terminated string to board
Parameter OLED display
     0	  0.00
     2	  0.01
    64	  0.25
   100	  0.39
   500	  1.95
   512	  2.00
  5000	 19.53
 30000	117.19
255997	999.99
256000	***.**
*/
void Fixed_uBinOut8(unsigned long n){
	//fixed point number = I * res
	unsigned long decimal = 0;
	unsigned long integer = 0;
	if(n >= 256000){
		printf("***.**");
		return;
	}
	decimal = ((n%256)*100/256);
	integer = n/256;
	printf("%3lu.%.2lu",integer,decimal);
	return;
}
