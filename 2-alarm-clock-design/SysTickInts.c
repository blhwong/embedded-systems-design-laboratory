//Filename: SysTickInts.c
//Author: Duc Tran, Brandon Wong
//Initial Creation Date: September 18, 2013
//Description: SysTick interrupt handler for Time Manager Module and Alarm
//Lab Number: MW 2-3:30
//TA : Omar & Mahesh
//Date of last revision :September 27, 2013
//Hardware Configuration : NONE

// SysTickInts.c
// Runs on LM3S1968
// Use the SysTick timer to request interrupts at a particular period.
// Daniel Valvano
// June 27, 2011

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to the Arm Cortex M3",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2011

   Program 5.12, section 5.7

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

// oscilloscope or LED connected to PD0 for period measurement

#include "SoundModule.h"

#define NVIC_SYS_PRI3_R         (*((volatile unsigned long *)0xE000ED20))  // Sys. Handlers 12 to 15 Priority
#define NVIC_ST_CTRL_R          (*((volatile unsigned long *)0xE000E010))
#define NVIC_ST_RELOAD_R        (*((volatile unsigned long *)0xE000E014))
#define NVIC_ST_CURRENT_R       (*((volatile unsigned long *)0xE000E018))
#define NVIC_ST_CTRL_CLK_SRC    0x00000004  // Clock Source
#define NVIC_ST_CTRL_INTEN      0x00000002  // Interrupt enable
#define NVIC_ST_CTRL_ENABLE     0x00000001  // Counter mode
#define GPIO_PORTG_DIR_R        (*((volatile unsigned long *)0x40026400))
#define GPIO_PORTG_DEN_R        (*((volatile unsigned long *)0x4002651C))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
#define SYSCTL_RCGC2_GPIOG     0x00000040  // port D Clock Gating Control

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode
volatile unsigned long Counts = 0;
#define PG2             (*((volatile unsigned long *)0x40026010))

int msec=0;
unsigned int countflag = 0;
unsigned int deltasec = 0;
unsigned int alarmsec = 0;


// **************SysTick_Init*********************
// Initialize Systick periodic interrupts
// Input: interrupt period
//        Units of period are 20ns
//        Maximum is 2^24-1
//        Minimum is determined by length of ISR
// Output: none
void SysTick_Init(unsigned long period){
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOG; // activate port D
  Counts = 0;
  GPIO_PORTG_DIR_R |= 0x04;   // make PD0 out
  GPIO_PORTG_DEN_R |= 0x04;   // enable digital I/O on PD0
  NVIC_ST_CTRL_R = 0;         // disable SysTick during setup
  NVIC_ST_RELOAD_R = period-1;// reload value
  NVIC_ST_CURRENT_R = 0;      // any write to current clears it
  NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0x40000000; // priority 2
                              // enable SysTick with core clock and interrupts
  NVIC_ST_CTRL_R = NVIC_ST_CTRL_ENABLE+NVIC_ST_CTRL_CLK_SRC+NVIC_ST_CTRL_INTEN;
}


 /****************SysTick_Handler***************
 Interrupt Handler. Counts milliseconds.
 Toggles heartbeat PG2.
 Input: none
 Output: none
    */
// Interrupt service routine
// Executed every 20ns*(period)
//alarmsec
void SysTick_Handler(void){
	//DisableInterrupts();
  PG2 ^= 0x04;        // toggle PD0
	//status LED
	msec = (msec + 1) % 86400000;
	if(alarmsec == msec){
		deltasec = 1;
	}
//	if(countflag == 0)
//	{
//		Counts = Counts + 1;
//	}
}


/****************SetTime***************
 Takes msec and updates time
 Input: msec
 Output: none
    */
void SetTime(int ms)
{
	NVIC_ST_CTRL_R ^= NVIC_ST_CTRL_INTEN;
	msec = ms;
	NVIC_ST_CTRL_R ^= NVIC_ST_CTRL_INTEN;
}

/****************SetAlarm***************
 Takes msec and Sets Alarm
 Input: msec
 Output: none
    */
void SetAlarm(unsigned int ms)
{
	deltasec =0;
	alarmsec = ms%86400000;
}

/****************AlarmOn***************
 Toggles PH0 and PH1 for alarm ringing
 Input: None
 Output: none
    */
void AlarmOn(void)
{ 
	NVIC_ST_CTRL_R ^= NVIC_ST_CTRL_INTEN; 
	PH0 ^=0x01; 
	PH1 ^=0x02; //Delay1(400); 
	NVIC_ST_CTRL_R ^= NVIC_ST_CTRL_INTEN; // PH0 =0x00; //PH1 =0x01; 
}
