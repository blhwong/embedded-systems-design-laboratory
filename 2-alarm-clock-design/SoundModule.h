//Filename: SoundModule.c
//Author: Duc Tran, Brandon Wong
//Initial Creation Date: September 13, 2013
//Description: Switch Module
//Lab Number: MW 2-3:30
//TA : Omar & Mahesh
//Date of last revision :September 27, 2

#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
#define GPIO_PORTH_DIR_R       (*((volatile unsigned long *)0x40027400))
#define GPIO_PORTH_DEN_R      (*((volatile unsigned long *)0x4002751C))
#define PH0      						(*((volatile unsigned long *)0x40027004))
#define PH1      						    (*((volatile unsigned long *)0x40027008))

 /****************Sound_Init***************
 Initializes sound port H
 Input: none
 Output: none
    */
void Sound_Init(void);




