//Filename: lab5.c
//Author: Duc Tran, Brandon Wong
//Initial Creation Date: October 2, 2013
//Description: 
//Lab Number: W 2-3:30
//TA : Omar & Mahesh
//Date of last revision :October 14, 2013
//Hardware Configuration : Lab5_artist.sch



// lab5.c
// Runs on LM3S1968
// Use a pointer implementation of a Moore finite state machine to operate
// a traffic light.
// Daniel Valvano
// June 15, 2011

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to the Arm Cortex M3",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2011
   Program 3.1, Example 3.1

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

// north facing car detector connected to PE1 (1=car present)
// east facing car detector connected to PE0 (1=car present)
// east facing red light connected to PF5
// east facing yellow light connected to PF4
// east facing green light connected to PF3
// north facing red light connected to PF2
// north facing yellow light connected to PF1
// north facing green light connected to PF0
//#include "hw_types.h"
#include "PLL.h"
//#include "sysctl.h"
#include "SysTick.h"
#include "Timer0A.h"
#include "Switch.h"
#include "TLV5618.h"
#include "MusicModule.h"
#define GPIO_PORTE_IN           (*((volatile unsigned long *)0x4002400C)) // bits 1-0
#define GPIO_PORTE_DIR_R        (*((volatile unsigned long *)0x40024400))
#define GPIO_PORTE_AFSEL_R      (*((volatile unsigned long *)0x40024420))
#define GPIO_PORTE_DEN_R        (*((volatile unsigned long *)0x4002451C))
#define GPIO_PORTF_OUT          (*((volatile unsigned long *)0x400250FC)) // bits 5-0
#define GPIO_PORTF_DIR_R        (*((volatile unsigned long *)0x40025400))
#define GPIO_PORTF_AFSEL_R      (*((volatile unsigned long *)0x40025420))
#define GPIO_PORTF_DEN_R        (*((volatile unsigned long *)0x4002551C))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
#define SYSCTL_RCGC2_GPIOF      0x00000020  // port F Clock Gating Control
#define SYSCTL_RCGC2_GPIOE      0x00000010  // port E Clock Gating Control
#define PG2             (*((volatile unsigned long *)0x40026010))
#define GPIO_PORTF0             (*((volatile unsigned long *)0x40025004))
#define GPIO_PORTF1             (*((volatile unsigned long *)0x40025008))

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode


extern unsigned char UP,DOWN,LEFT,RIGHT,SELECT;

unsigned short SinTable[256] = {  // must be in RAM, can't DMA out of ROM
  2048,2097,2146,2195,2244,2293,2341,2390,2438,2486,2534,2581,2629,2675,2722,2768,
  2813,2858,2903,2947,2991,3034,3076,3118,3159,3200,3239,3278,3317,3354,3391,3427,
  3462,3496,3530,3562,3594,3625,3654,3683,3711,3738,3763,3788,3812,3834,3856,3876,
  3896,3914,3931,3947,3962,3976,3988,3999,4010,4019,4026,4033,4038,4043,4046,4047,
  4048,4047,4046,4043,4038,4033,4026,4019,4010,3999,3988,3976,3962,3947,3931,3914,
  3896,3876,3856,3834,3812,3788,3763,3738,3711,3683,3654,3625,3594,3562,3530,3496,
  3462,3427,3391,3354,3317,3278,3239,3200,3159,3118,3076,3034,2991,2947,2903,2858,
  2813,2768,2722,2675,2629,2581,2534,2486,2438,2390,2341,2293,2244,2195,2146,2097,
  2048,1999,1950,1901,1852,1803,1755,1706,1658,1610,1562,1515,1467,1421,1374,1328,
  1283,1238,1193,1149,1105,1062,1020,978,937,896,857,818,779,742,705,669,634,600,
  566,534,502,471,442,413,385,358,333,308,284,262,240,220,200,182,165,149,134,120,
  108,97,86,77,70,63,58,53,50,49,48,49,50,53,58,63,70,77,86,97,108,120,134,149,165,
  182,200,220,240,262,284,308,333,358,385,413,442,471,502,534,566,600,634,669,705,
  742,779,818,857,896,937,978,1020,1062,1105,1149,1193,1238,1283,1328,1374,1421,
  1467,1515,1562,1610,1658,1706,1755,1803,1852,1901,1950,1999};

unsigned long I; 
// 12-bit 32-element sine wave 
const unsigned short Wave[32]= { 
 2048,2438,2813,3159,3462,3711,3896,4010,4048,4010,3896, 
 3711,3462,3159,2813,2438,2048,1658,1283,937,634,385, 
 200,86,48,86,200,385,634,937,1283,1658}; 

const unsigned short Flute[32] = {  	
  1007,1252,1374,1548,1698,1797,1825,1797,1675,1562,1383,	
  1219,1092,1007,913,890,833,847,810,777,744,674,	
  598,551,509,476,495,533,589,659,758,876	
};  	

const unsigned short SineUpdateDelay[64] = {
11945,11274,10641,10044,9480,8948,8446,7972,7525,7102,6704
,6327,5972,5637,5321,5022,4740,4474,4223,3986,3762,3551,3352
,3164,2986,2819,2660,2511,2370,2237,2112,1993,1881,1776,1676,1582
,1493,1409,1330,1256,1185,1119,1056,997,941,888,838,791,747,705,665
,628,593,559,528,498,470,444,444,419,419,395,395,373};

const unsigned short Guitar[64] = {1024,
1024,
1026,
1001,
935,
833,
730,
647,
619,
666,
782,
964,
1172,
1337,
1461,
1536,
1558,
1503,
1396,
1235,
1040,
864,
755,
758,
877,
1087,
1358,
1613,
1789,
1846,
1772,
1625,
1454,
1285,
1173,
1096,
1052,
1034,
1027,
1036,
1079,
1137,
1184,
1247,
1264,
1250,
1182,
1081,
974,
865,
801,
779,
810,
844,
859,
859,
851,
849,
875,
922,
977,1024,1024,1024};

unsigned char PlayFlag=0;
void UserTask(void){
	unsigned long Output,Delay;
	unsigned char Input = 0;
	GPIO_PORTF0 =1;
//	Input = RIGHT+LEFT*2+SELECT*4;
	PG2 ^= 0x04;        // toggle PD0
 	if(PlayFlag==1){
			I = (I+1)&0x3F;
			//Delay(5000);
			DAC_Out(SineUpdateDelay[I]);
			
	 }
	// DAC_Out(Wave[I]);
	 GPIO_PORTF0 =0;
}
void UserTask2(void){
	GPIO_PORTF1 = 1;
	if(UP){
		UP =0;
		Music_Pause();
		if(PlayFlag){
			PlayFlag = 0;
		}else{PlayFlag = 1;}
	}
	if(LEFT){
		LEFT =0;
	  PlayFlag = 0;
		 Music_Rewind();
	  PlayFlag = 1;
	}
	if(RIGHT){
		RIGHT = 0;
		Music_Stop();
		PlayFlag = 0;
	}
	GPIO_PORTF1= 0;
}

int main(void){volatile unsigned long delay;
	unsigned char Input;
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOG; // activate port G
	
  delay = SYSCTL_RCGC2_R ;
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF;
	delay = SYSCTL_RCGC2_R ;
	
	DisableInterrupts();
	Switch_Init();
	GPIO_PORTF_DIR_R |= 0x03; // make PF0/1 out (connect to scope/logic analyzer) 
  GPIO_PORTF_DEN_R |= 0x03; // enable digital I/O on PF0
  GPIO_PORTG_DIR_R |= 0x04;   // make PG2 out
  GPIO_PORTG_DEN_R |= 0x04;   // enable digital I/O on PG2
	//GPIO_PORTF0 =0;
	//GPIO_PORTF1 =0;
  PLL_Init(); 
	Music_Init();		// configure for 50 MHz clock
	Timer0A_Init(&UserTask,100);
	Timer1A_Init(&UserTask2,1500);
	DAC_Init(0x0100);// initialize with command: Vout = Vref
	
	EnableInterrupts();	
	while(1){
	}
		
}
