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


#include "SwitchModule.h"


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
#define NVIC_PRI0_R             (*((volatile unsigned long *)0xE000E400))
#define NVIC_EN0_R              (*((volatile unsigned long *)0xE000E100))
static unsigned long UP_PG3 =  0x08;//0x08;
static unsigned long DOWN_PG4 = 0x10;//0x10;
static unsigned long LEFT_PG5 = 0x20;//0x20;
static unsigned long RIGHT_PG6 = 0x40;
static unsigned long SELECT_PG7 = 0x80;
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode
unsigned long GPIO;
unsigned char UP,DOWN,LEFT,RIGHT,SELECT;
volatile unsigned long FallingEdges = 0;


/****************Switch_Init***************
 Description: initilizes switches
 Input: none
 Output: none  */ 
void Switch_Init(void){
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOG;
	UP =0;
	DOWN =0;
	LEFT = 0;
	RIGHT =0;
	SELECT = 0;
	FallingEdges =  0;
	GPIO_PORTG_DIR_R &= ~0xF8;
	GPIO_PORTG_DEN_R |= 0xF8;
	GPIO_PORTG_PUR_R  |= 0xF8;
	GPIO_PORTG_IS_R &= ~0xF8;
	GPIO_PORTG_IBE_R &= ~0xF8;
	GPIO_PORTG_IEV_R |= 0xF8;
	GPIO_PORTG_ICR_R = 0xF8;
	GPIO_PORTG_IM_R	|= 0xF8; // enable interrupt
	NVIC_PRI7_R = (NVIC_PRI7_R&0x00FFFFFF)|0x40000000; // (g) priority 5
 // NVIC_EN0_R |= 4;              // (h) enable interrupt 2 in NVIC
	//NVIC_EN0_R |= 0x00000040;
	//NVIC_PRI7_R = (NVIC_PRI0_R&0x00FFFFFF)|0x40000000; // (g) priority 2
	NVIC_EN0_R |= 0x80000000;
	//EnableInterrupts();
}


void GPIOPortG_Handler(void){
	FallingEdges+=1;
	if(GPIO_PORTG_RIS_R& UP_PG3){
		GPIO_PORTG_ICR_R = UP_PG3;
		UP = 1;
	}
	if(GPIO_PORTG_RIS_R& DOWN_PG4){
		GPIO_PORTG_ICR_R = DOWN_PG4;
		DOWN = 1;
	}
	if(GPIO_PORTG_RIS_R& LEFT_PG5){
		GPIO_PORTG_ICR_R = LEFT_PG5;
		LEFT = 1;
	}

	if(GPIO_PORTG_RIS_R& SELECT_PG7){
		GPIO_PORTG_ICR_R = SELECT_PG7;
		SELECT = 1;
	}	
	if(GPIO_PORTG_RIS_R& RIGHT_PG6){
		GPIO_PORTG_ICR_R = RIGHT_PG6;
		RIGHT = 1;
	}	
}









