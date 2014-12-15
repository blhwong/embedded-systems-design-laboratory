// Timer0A.c
// Runs on LM3S1968
// Use Timer0A in periodic mode to request interrupts at a particular
// period.
// Daniel Valvano
// September 14, 2011

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to the Arm Cortex M3",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2011
  Program 7.5, example 7.6

 Copyright 2011 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */

#include "MusicModule.h"
#define NVIC_EN0_INT19          0x00080000  // Interrupt 19 enable
#define NVIC_EN0_INT21          0x00200000  // Interrupt 19 enable
#define NVIC_EN0_R              (*((volatile unsigned long *)0xE000E100))  // IRQ 0 to 31 Set Enable Register
#define NVIC_PRI4_R             (*((volatile unsigned long *)0xE000E410))  // IRQ 16 to 19 Priority Register
#define NVIC_PRI5_R             (*((volatile unsigned long *)0xE000E414))
#define TIMER0_CFG_R            (*((volatile unsigned long *)0x40030000))
#define TIMER0_TAMR_R           (*((volatile unsigned long *)0x40030004))
#define TIMER0_CTL_R            (*((volatile unsigned long *)0x4003000C))
#define TIMER0_IMR_R            (*((volatile unsigned long *)0x40030018))
#define TIMER0_MIS_R            (*((volatile unsigned long *)0x40030020))
#define TIMER0_ICR_R            (*((volatile unsigned long *)0x40030024))
#define TIMER0_TAILR_R          (*((volatile unsigned long *)0x40030028))
#define TIMER0_TAPR_R           (*((volatile unsigned long *)0x40030038))
#define TIMER0_TAR_R            (*((volatile unsigned long *)0x40030048))
#define TIMER_CFG_16_BIT        0x00000004  // 16-bit timer configuration,
                                            // function is controlled by bits
                                            // 1:0 of GPTMTAMR and GPTMTBMR
#define TIMER_TAMR_TAMR_PERIOD  0x00000002  // Periodic Timer mode
#define TIMER_CTL_TAEN          0x00000001  // GPTM TimerA Enable
#define TIMER_IMR_TATOIM        0x00000001  // GPTM TimerA Time-Out Interrupt
                                            // Mask
#define TIMER_ICR_TATOCINT      0x00000001  // GPTM TimerA Time-Out Raw
                                            // Interrupt
#define TIMER_TAILR_TAILRL_M    0x0000FFFF  // GPTM TimerA Interval Load
                                            // Register Low
																						
#define TIMER1_CFG_R               (*((volatile unsigned long *)0x40031000))
#define TIMER1_TAMR_R              (*((volatile unsigned long *)0x40031004))
#define TIMER1_TBMR_R              (*((volatile unsigned long *)0x40031008))
#define TIMER1_CTL_R               (*((volatile unsigned long *)0x4003100C))
#define TIMER1_IMR_R               (*((volatile unsigned long *)0x40031018))
#define TIMER1_RIS_R               (*((volatile unsigned long *)0x4003101C))
#define TIMER1_MIS_R               (*((volatile unsigned long *)0x40031020))
#define TIMER1_ICR_R               (*((volatile unsigned long *)0x40031024))
#define TIMER1_TAILR_R             (*((volatile unsigned long *)0x40031028))
#define TIMER1_TBILR_R             (*((volatile unsigned long *)0x4003102C))
#define TIMER1_TAMATCHR_R          (*((volatile unsigned long *)0x40031030))
#define TIMER1_TBMATCHR_R          (*((volatile unsigned long *)0x40031034))
#define TIMER1_TAPR_R              (*((volatile unsigned long *)0x40031038))
#define TIMER1_TBPR_R              (*((volatile unsigned long *)0x4003103C))
#define TIMER1_TAPMR_R             (*((volatile unsigned long *)0x40031040))
#define TIMER1_TBPMR_R             (*((volatile unsigned long *)0x40031044))
#define TIMER1_TAR_R               (*((volatile unsigned long *)0x40031048))
#define TIMER1_TBR_R               (*((volatile unsigned long *)0x4003104C))

#define SYSCTL_RCGC1_R          (*((volatile unsigned long *)0x400FE104))
#define SYSCTL_RCGC1_TIMER0     0x00010000  // timer 0 Clock Gating Control
#define SYSCTL_RCGC1_TIMER1     0x00020000  // Timer 1 Clock Gating Control
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode
void (*PeriodicTask)(void);  // user function
void (*PeriodicTask2)(void);



#ifdef __TI_COMPILER_VERSION__
	//Code Composer Studio Code
	void Delay(unsigned long ulCount){
	__asm (	"    subs    r0, #1\n"
			"    bne     Delay\n"
			"    bx      lr\n");
}

#else
	//Keil uVision Code
	__asm void
	Delay(unsigned long ulCount)
	{
    subs    r0, #1
    bne     Delay
    bx      lr
	}

#endif

// ***************** Timer0A_Init ****************
// Activate Timer0A interrupts to run user task periodically
// Inputs:  task is a pointer to a user function
//          period in usec
// Outputs: none
void Timer0A_Init(void(*task)(void), unsigned short period){ 
  SYSCTL_RCGC1_R |= SYSCTL_RCGC1_TIMER0; // 0) activate timer0
  PeriodicTask = task;             // user function 
  TIMER0_CTL_R &= ~0x00000001;     // 1) disable timer0A during setup
  TIMER0_CFG_R = 0x00000004;       // 2) configure for 16-bit timer mode
  TIMER0_TAMR_R = 0x00000002;      // 3) configure for periodic mode
  TIMER0_TAILR_R = period-1;       // 4) reload value
  //TIMER0_TAPR_R = 49;              // 5) 1us timer0A
  TIMER0_ICR_R = 0x00000001;       // 6) clear timer0A timeout flag
  TIMER0_IMR_R |= 0x00000001;      // 7) arm timeout interrupt
  NVIC_PRI4_R = (NVIC_PRI4_R&0x00FFFFFF)|0x80000000; // 8) priority 2
  NVIC_EN0_R |= NVIC_EN0_INT19;    // 9) enable interrupt 19 in NVIC
  TIMER0_CTL_R |= 0x00000001;      // 10) enable timer0A
	
}
// ***************** Timer1A_Init ****************
// Activate Timer1A interrupts to run user task periodically
// Inputs:  task is a pointer to a user function
//          period in usec
// Outputs: none
void Timer1A_Init(void(*task)(void),unsigned short period){volatile unsigned long delay;
	SYSCTL_RCGC1_R |= SYSCTL_RCGC1_TIMER1; // 0) activate timer1
  delay = SYSCTL_RCGC1_R;
	PeriodicTask2 = task;    
  TIMER1_CTL_R &= ~0x00000001;     // 1) disable timer1A during setup
  TIMER1_CFG_R = 0x00000004;       // 2) configure for 16-bit timer mode
  TIMER1_TAMR_R = 0x00000002;      // 3) configure for periodic mode
  TIMER1_TAILR_R = period-1;       // 4) reload value
  //TIMER0_TAPR_R = 49;              // 5) 1us timer0A
  TIMER1_ICR_R = 0x00000001;       // 6) clear timer0A timeout flag
  TIMER1_IMR_R |= 0x00000001;      // 7) arm timeout interrupt
	
  NVIC_PRI5_R = (NVIC_PRI4_R&0xFFF00FFF)|0x00004000; // 8) priority 2
  NVIC_EN0_R |= NVIC_EN0_INT21;    // 9) enable interrupt 19 in NVIC
  TIMER1_CTL_R |= 0x00000001;      // 10) enable timer0A
	
}
//const unsigned short Note[25] ={G, G ,A, G, C, B, G, G, A, G ,D1, C, G, G ,G1, E1, C, C, B, A, F, F, E1, C, D1 };
unsigned short I2,Timems =0;
extern unsigned long Duration;
void Timer0A_Handler(void){
//	if(Timems == Duration){ //Duration of each note
//			if(I2 == 25){
//				I2 = 0 ;	
//			}
//			Delay(990000);
//		  Music_Play(I2);
//			I2 = I2+ 1;
//		Timems = 0;
//	}else{Timems = Timems +1;}
	//
  TIMER0_ICR_R = TIMER_ICR_TATOCINT;// acknowledge timer0A timeout
  (*PeriodicTask)();                // execute user task
	//Delay(500);
}

void Timer1A_Handler(void){
		if(Timems == Duration){ //Duration of each note
//			if(I2 == 25){
//				I2 = 0 ;	
//			}
		  Music_Play();
			Delay(10000);
//			I2 = I2+ 1;
		Timems = 0;
	}else{Timems = Timems +1;}
	
	TIMER1_ICR_R = TIMER_ICR_TATOCINT;
	
	 (*PeriodicTask2)();  
}