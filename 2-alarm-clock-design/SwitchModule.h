//Filename: SwitchModule.c
//Author: Duc Tran, Brandon Wong
//Initial Creation Date: September 13, 2013
//Description: Switch Module
//Lab Number: MW 2-3:30
//TA : Omar & Mahesh
//Date of last revision :September 27, 2013
//Hardware Configuration :  Switches on board. 
//							Can be found in Lab3_artist.sch
//							UP_PG3 =  0x08;//0x08;
//							DOWN_PG4 = 0x10;//0x10;
//							LEFT_PG5 = 0x20;//0x20;
//							RIGHT_PG6 = 0x40;
//							SELECT_PG7 = 0x80;

#define GPIO_PORTG_DATA_R       (*((volatile unsigned long *)0x400263FC))
#define GPIO_PORTG_DIR_R        (*((volatile unsigned long *)0x40026400))
#define GPIO_PORTG_AFSEL_R      (*((volatile unsigned long *)0x40026420))
#define GPIO_PORTG_DEN_R        (*((volatile unsigned long *)0x4002651C))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
#define SYSCTL_RCGC2_GPIOG      0x00000040  // port G Clock Gating Control
#define GPIO_PORTG_RIS_R        (*((volatile unsigned long *)0x40026414))
#define GPIO_PORTG_PUR_R 				(*((volatile unsigned long *)0x40026510)) 
#define GPIO_PORTG_IS_R         (*((volatile unsigned long *)0x40026404))
#define GPIO_PORTG_IBE_R        (*((volatile unsigned long *)0x40026408))
#define GPIO_PORTG_IEV_R        (*((volatile unsigned long *)0x4002640C))
#define GPIO_PORTG_ICR_R        (*((volatile unsigned long *)0x4002641C))
#define GPIO_PORTG_IM_R         (*((volatile unsigned long *)0x40026410))
#define NVIC_PRI7_R             (*((volatile unsigned long *)0xE000E41C))
#define NVIC_EN0_R              (*((volatile unsigned long *)0xE000E100))


/****************Switch_Init***************
 Description: initilizes switches
 Input: none
 Output: none  */ 
void Switch_Init(void);
//void Switch_WaitPress(void);
//void Switch_WaitRelease(void);
/****************Debounce***************
 Description: debounces switch signal
 Input: switch signal
 Output: none  */ 
//unsigned long Debounce(void);
//unsigned long Switch_Input(void);
