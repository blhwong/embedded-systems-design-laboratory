//Filename: SoundModule.c
//Author: Duc Tran, Brandon Wong
//Initial Creation Date: September 13, 2013
//Description: Switch Module
//Lab Number: MW 2-3:30
//TA : Omar & Mahesh
//Date of last revision :September 27, 2013
//Hardware Configuration :  NONE

#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
#define GPIO_PORTH_DIR_R        (*((volatile unsigned long *)0x40027400))
#define GPIO_PORTH_DEN_R        (*((volatile unsigned long *)0x4002751C))
#define PH0      						    (*((volatile unsigned long *)0x40027004))
#define PH1      						    (*((volatile unsigned long *)0x40027008))
#define GPIO_PORTH_PUR_R        (*((volatile unsigned long *)0x40027510))


 /****************Sound_Init***************
 Initializes sound port H
 Input: none
 Output: none
    */
void Sound_Init(void){volatile unsigned long delay;
	SYSCTL_RCGC2_R |=0x00000080;
	delay =SYSCTL_RCGC2_R;
	GPIO_PORTH_DIR_R |= 0x03;
  GPIO_PORTH_DEN_R |= 0x03;
	GPIO_PORTH_PUR_R |= 0x03;
	
	PH1 &= 0x02;
	PH0 |= 0x01;
}
