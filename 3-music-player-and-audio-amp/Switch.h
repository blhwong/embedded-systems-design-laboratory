//Filename: SwitchModule.h
//Author: Duc Tran, Brandon Wong
//Initial Creation Date: September 25, 2013
//Description: 
//Lab Number: W 2-3:30
//TA : Omar & Mahesh
//Date of last revision :September 25, 2013
//Hardware Configuration : Lab4_artist.sch

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
#define PG3							(*((volatile unsigned long *)0x40026020))
#define PG4							(*((volatile unsigned long *)0x40026040))
#define PG5             (*((volatile unsigned long *)0x40026080))
#define PG6             (*((volatile unsigned long *)0x40026100))
#define PG7             (*((volatile unsigned long *)0x40026200))

/****************Switch_Init***************
 Description: initilizes switches
 Input: none
 Output: none  */ 
void Switch_Init(void);

