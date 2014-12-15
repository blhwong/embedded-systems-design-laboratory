//Filename: Switch.c
//Author: Duc Tran, Brandon Wong
//Initial Creation Date: September 25, 2013
//Description: 
//Lab Number: W 2-3:30
//TA : Omar & Mahesh
//Date of last revision : October 14, 2013
//Hardware Configuration : Lab5_artist.sch


#include "Switch.h"
#include "SysTick.h"

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
#define PG3							(*((volatile unsigned long *)0x40026020))
#define PG4							(*((volatile unsigned long *)0x40026040))
#define PG5             (*((volatile unsigned long *)0x40026080))
#define PG6             (*((volatile unsigned long *)0x40026100))
#define PG7             (*((volatile unsigned long *)0x40026200))

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


/****************Switch_Init***************
 Description: initilizes switches
 Input: none
 Output: none  */ 
void Switch_Init(void){volatile unsigned long delay;
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOG;
	delay =SYSCTL_RCGC2_R; 
	SysTick_Init();
	UP &= 0x00;
	DOWN &= 0x00;
	LEFT &= 0x00;
	RIGHT &=0x00;
	SELECT &=0x00;
	GPIO_PORTG_DIR_R &= ~0xF8; //Make G IN
	GPIO_PORTG_DEN_R |= 0xF8; //enabe digital I/O
	GPIO_PORTG_PUR_R  |= 0xF8;
	GPIO_PORTG_IS_R &= ~0xF8;
//	GPIO_PORTG_IBE_R |= 0xF8;//oRI
		GPIO_PORTG_IBE_R &= ~0xF8;//
	GPIO_PORTG_IEV_R &= ~0xF8; //Falling edge event
	GPIO_PORTG_ICR_R = 0xF8;
	GPIO_PORTG_IM_R	|= 0xF8; // enable interrupt
	NVIC_PRI7_R = (NVIC_PRI7_R&0x00FFFFFF)|0x20000000; // (g) priority 
	NVIC_EN0_R |= 0x80000000;

	//EnableInterrupts();
}


void GPIOPortG_Handler(void){
	//SysTick_Wait10ms(1); 
	if(GPIO_PORTG_RIS_R& UP_PG3){
		GPIO_PORTG_ICR_R = UP_PG3;

		if(PG3){
			UP = 0;
		} else{
			UP = 1;
		}

	}
	if(GPIO_PORTG_RIS_R& DOWN_PG4){
		GPIO_PORTG_ICR_R = DOWN_PG4;

		if(PG4){
			DOWN = 0;
		} else{
			DOWN = 1;
		}

	}
	
	if(GPIO_PORTG_RIS_R& LEFT_PG5){
		GPIO_PORTG_ICR_R = LEFT_PG5;

		if(PG5){
			LEFT =0;
		}else{ LEFT =1;}
	}

	if(GPIO_PORTG_RIS_R& SELECT_PG7){
		GPIO_PORTG_ICR_R = SELECT_PG7;
		
		if(PG7){
			SELECT =0;
		}else{ SELECT =1;}

	}	
	if(GPIO_PORTG_RIS_R& RIGHT_PG6){
		GPIO_PORTG_ICR_R = RIGHT_PG6;
		if(PG6){
			RIGHT =0;
		}else{ RIGHT =1;}
	}
}



