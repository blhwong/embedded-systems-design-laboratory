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

//Filename: MusicModule.c
//Author: Duc Tran, Brandon Wong
//Initial Creation Date: October 2, 2013
//Description: Handle music player
//Lab Number: W 2-3:30
//TA : Omar & Mahesh
//Date of last revision :October 14, 2013
//Hardware Configuration : Lab5_artist.sch

#define WHOLE_NOTE 64000
#define HALF_NOTE 32000
#define QUARTER_NOTE 16000
#define EIGHTH_NOTE 8000
#define QUARTER_NOTE_PAUSE 100
#define SIXTEENTH_NOTE 4000

#define EIGHTH_TRIPLE_NOTE 5333
#define EIGHTH_TRIPLE_NOTE2 5533
#define EIGHTH_TRIPLE_NOTE3 5733
#define EIGHTH_TRIPLE_NOTE4 5933
#define EIGHTH_TRIPLE_NOTE5 6133
#define EIGHTH_TRIPLE_NOTE6 6333
  
  
  
#define DOTTED_SIXTEENTH_NOTE 12000
  
  
#define FINAL_NOTE 65000 


#define REST 0
#define PAUSE 0
#define QUIET 1
#define C_2 11945   // 65.406 Hz	
#define DF_1 11274   // 69.296 Hz	
#define D_1 10641   // 73.416 Hz	
#define EF_1 10044   // 77.782 Hz	
#define E_1 9480   // 82.407 Hz	
#define F_1 8948   // 87.307 Hz	
#define GF_1 8446   // 92.499 Hz	
#define G_1 7972   // 97.999 Hz	
#define AF_1 7525   // 103.826 Hz	
#define A_1 7102   // 110.000 Hz	
#define BF_1 6704   // 116.541 Hz	
#define B_1 6327   // 123.471 Hz	
#define C_1 5972   // 130.813 Hz	
#define DF0 5637   // 138.591 Hz	
#define D0 5321   // 146.832 Hz	
#define EF0 5022   // 155.563 Hz	
#define E0 4740   // 164.814 Hz	
#define F0 4474   // 174.614 Hz	
#define GF0 4223   // 184.997 Hz	
#define G0 3986   // 195.998 Hz	
#define AF0 3762   // 207.652 Hz	
#define A0 3551   // 220.000 Hz	
#define BF0 3352   // 233.082 Hz	
#define B0 3164   // 246.942 Hz	
#define C0 2986   // 261.626 Hz	
#define DF 2819   // 277.183 Hz	
#define D 2660   // 293.665 Hz	
#define EF 2511   // 311.127 Hz	
#define E 2370   // 329.628 Hz	
#define F 2237   // 349.228 Hz	
#define GF 2112   // 369.994 Hz	
#define G 1993   // 391.995 Hz	
#define AF 1881   // 415.305 Hz	
#define A 1776   // 440.000 Hz	
#define BF 1676   // 466.164 Hz	
#define B 1582   // 493.883 Hz	
#define C 1493   // 523.251 Hz	
#define DF1 1409   // 554.365 Hz	
#define D1 1330   // 587.330 Hz	
#define EF1 1256   // 622.254 Hz	
#define E1 1185   // 659.255 Hz	
#define F1 1119   // 698.456 Hz	
#define GF1 1056   // 739.989 Hz	
#define G1 997   // 783.991 Hz	
#define AF1 941   // 830.609 Hz	
#define A1 888   // 880.000 Hz	
#define BF1 838   // 932.328 Hz	
#define B1 791   // 987.767 Hz	
#define C1 747   // 1046.502 Hz	
#define DF2 705   // 1108.731 Hz	
#define D2 665   // 1174.659 Hz	
#define EF2 628   // 1244.508 Hz	
#define E2 593   // 1318.510 Hz	
#define F2 559   // 1396.913 Hz	
#define GF2 528   // 1479.978 Hz	
#define G2 498   // 1567.982 Hz	
#define AF2 470   // 1661.219 Hz	
#define A2 444   // 1760.000 Hz	
#define BF2 419   // 1864.655 Hz	
#define B2 395   // 1975.533 Hz	
#define C2 373   // 2093.005 Hz	






struct Player{
	unsigned short Pitch;
	unsigned short Duration;
	//const struct Player *Next[8];
	
	//const struct Player *Song;
	//unsigned short SinTable[256];
};

typedef const struct Player SongType;
typedef SongType *SongPtr;

typedef const struct Player MusicStateType;
const struct Player *Ptc;




//MusicStateType MarioTheme[58]={

//	{E1, SIXTEENTH_NOTE},
//	{E1, EIGHTH_NOTE},
//	{E1, SIXTEENTH_NOTE},
//	{REST, SIXTEENTH_NOTE},
//	{C, SIXTEENTH_NOTE},
//	{E1, EIGHTH_NOTE},
//	{G1, QUARTER_NOTE},
//	{G, QUARTER_NOTE},

//	{C, EIGHTH_NOTE},
//	{REST, SIXTEENTH_NOTE},
//	{G, SIXTEENTH_NOTE},
//	{REST, EIGHTH_NOTE},
//	{E, EIGHTH_NOTE},
//	{REST, SIXTEENTH_NOTE},
//	{A, SIXTEENTH_NOTE},
//	{REST, SIXTEENTH_NOTE},
//	{B, SIXTEENTH_NOTE},
//	{REST, SIXTEENTH_NOTE},
//	{BF, SIXTEENTH_NOTE},
//	{A, EIGHTH_NOTE},

//	{G, EIGHTH_TRIPLE_NOTE},
//	{E1, EIGHTH_TRIPLE_NOTE},
//	{G1, EIGHTH_TRIPLE_NOTE},
//	{A1, EIGHTH_NOTE},
//	{F1, SIXTEENTH_NOTE},
//	{G1, SIXTEENTH_NOTE},
//	{REST, SIXTEENTH_NOTE},
//	{E1, SIXTEENTH_NOTE},
//	{REST, SIXTEENTH_NOTE},
//	{C, SIXTEENTH_NOTE},
//	{D1, SIXTEENTH_NOTE},
//	{B, SIXTEENTH_NOTE},
//	{REST, EIGHTH_NOTE},

//	{C, EIGHTH_NOTE},
//	{REST, SIXTEENTH_NOTE},
//	{G, SIXTEENTH_NOTE},
//	{REST, EIGHTH_NOTE},
//	{E, EIGHTH_NOTE},
//	{REST, SIXTEENTH_NOTE},
//	{A, SIXTEENTH_NOTE},
//	{REST, SIXTEENTH_NOTE},
//	{B, SIXTEENTH_NOTE},
//	{REST, SIXTEENTH_NOTE},
//	{BF, SIXTEENTH_NOTE},
//	{A, EIGHTH_NOTE},


//	{G, EIGHTH_TRIPLE_NOTE},
//	{E1, EIGHTH_TRIPLE_NOTE},
//	{G1, EIGHTH_TRIPLE_NOTE},
//	{A1, EIGHTH_NOTE},
//	{F1, SIXTEENTH_NOTE},
//	{G1, SIXTEENTH_NOTE},
//	{REST, SIXTEENTH_NOTE},
//	{E1, SIXTEENTH_NOTE},
//	{REST, SIXTEENTH_NOTE},
//	{C, SIXTEENTH_NOTE},
//	{D1, SIXTEENTH_NOTE},
//	{B, SIXTEENTH_NOTE},
//	{REST, EIGHTH_NOTE},

//};


MusicStateType MarioTheme[352]={
  
    {E1, SIXTEENTH_NOTE},   //1
    {E1, EIGHTH_NOTE},
    {E1, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {C, SIXTEENTH_NOTE},
    {E1, EIGHTH_NOTE},
    {G1, QUARTER_NOTE},
    {G, QUARTER_NOTE},
  
    {C, EIGHTH_NOTE},    //2
    {REST, SIXTEENTH_NOTE},
    {G, SIXTEENTH_NOTE},
    {REST, EIGHTH_NOTE},
    {E, EIGHTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {A, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {B, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {BF, SIXTEENTH_NOTE},
    {A, EIGHTH_NOTE},
  
    {G, EIGHTH_TRIPLE_NOTE},    //3
    {E1, EIGHTH_TRIPLE_NOTE},
    {G1, EIGHTH_TRIPLE_NOTE},
    {A1, EIGHTH_NOTE},
    {F1, SIXTEENTH_NOTE},
    {G1, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {E1, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {C, SIXTEENTH_NOTE},
    {D1, SIXTEENTH_NOTE},
    {B, SIXTEENTH_NOTE},
    {REST, EIGHTH_NOTE},
  
    {C, EIGHTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {G, SIXTEENTH_NOTE},
    {REST, EIGHTH_NOTE},
    {E, EIGHTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {A, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {B, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {BF, SIXTEENTH_NOTE},
    {A, EIGHTH_NOTE},
  
  
    {G, EIGHTH_TRIPLE_NOTE},
    {E1, EIGHTH_TRIPLE_NOTE},
    {G1, EIGHTH_TRIPLE_NOTE},
    {A1, EIGHTH_NOTE},
    {F1, SIXTEENTH_NOTE},
    {G1, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {E1, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {C, SIXTEENTH_NOTE},
    {D1, SIXTEENTH_NOTE},
    {B, SIXTEENTH_NOTE},
    {REST, EIGHTH_NOTE},
  
    {REST, EIGHTH_NOTE},
    {G1, SIXTEENTH_NOTE},
    {GF1, SIXTEENTH_NOTE},
    {F1, SIXTEENTH_NOTE},
    {EF1, EIGHTH_NOTE},
    {E1, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {AF, SIXTEENTH_NOTE},
    {A, SIXTEENTH_NOTE},
    {C, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {A, SIXTEENTH_NOTE},
    {C, SIXTEENTH_NOTE},
    {D1, SIXTEENTH_NOTE},
      
    {REST, EIGHTH_NOTE},
    {G1, SIXTEENTH_NOTE},
    {GF1, SIXTEENTH_NOTE},
    {F1, SIXTEENTH_NOTE},
    {EF1, EIGHTH_NOTE},
    {E1, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {C1, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {C1, SIXTEENTH_NOTE},
    {C1, QUARTER_NOTE},
      
    {REST, EIGHTH_NOTE},
    {G1, SIXTEENTH_NOTE},
    {GF1, SIXTEENTH_NOTE},
    {F1, SIXTEENTH_NOTE},
    {EF1, EIGHTH_NOTE},
    {E1, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {AF, SIXTEENTH_NOTE},
    {A, SIXTEENTH_NOTE},
    {C, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {A, SIXTEENTH_NOTE},
    {C, SIXTEENTH_NOTE},
    {D1, SIXTEENTH_NOTE},
      
    {REST, EIGHTH_NOTE},
    {EF1, EIGHTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {D1, SIXTEENTH_NOTE},
    {REST, EIGHTH_NOTE},
    {C, QUARTER_NOTE},
    {REST, QUARTER_NOTE},
      
    {C, SIXTEENTH_NOTE},
    {C, EIGHTH_NOTE},
    {C, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {C, SIXTEENTH_NOTE},
    {D1, EIGHTH_NOTE},
    {E1, SIXTEENTH_NOTE},
    {C, EIGHTH_NOTE},
    {A, SIXTEENTH_NOTE},
    {G, QUARTER_NOTE},
      
    {C, SIXTEENTH_NOTE},
    {C, EIGHTH_NOTE},
    {C, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {C, SIXTEENTH_NOTE},
    {D1, EIGHTH_NOTE},
    {E1, SIXTEENTH_NOTE},
    {REST, HALF_NOTE},
      
    {C, SIXTEENTH_NOTE},
    {C, EIGHTH_NOTE},
    {C, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {C, SIXTEENTH_NOTE},
    {D1, EIGHTH_NOTE},
    {E1, SIXTEENTH_NOTE},
    {C, EIGHTH_NOTE},
    {A, SIXTEENTH_NOTE},
    {G, QUARTER_NOTE},
      
    {E1, SIXTEENTH_NOTE},   //17
    {E1, EIGHTH_NOTE},
    {E1, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {C, SIXTEENTH_NOTE},
    {E1, EIGHTH_NOTE},
    {G1, QUARTER_NOTE},
    {G, QUARTER_NOTE},
      
    {C, EIGHTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {G, SIXTEENTH_NOTE},
    {REST, EIGHTH_NOTE},
    {E, EIGHTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {A, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {B, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {BF, SIXTEENTH_NOTE},
    {A, EIGHTH_NOTE},
  
    {G, EIGHTH_TRIPLE_NOTE},
    {E1, EIGHTH_TRIPLE_NOTE},
    {G1, EIGHTH_TRIPLE_NOTE},
    {A1, EIGHTH_NOTE},
    {F1, SIXTEENTH_NOTE},
    {G1, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {E1, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {C, SIXTEENTH_NOTE},
    {D1, SIXTEENTH_NOTE},
    {B, SIXTEENTH_NOTE},
    {REST, EIGHTH_NOTE},
  
    {C, EIGHTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {G, SIXTEENTH_NOTE},
    {REST, EIGHTH_NOTE},
    {E, EIGHTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {A, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {B, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {BF, SIXTEENTH_NOTE},
    {A, EIGHTH_NOTE},
  
  
    {G, EIGHTH_TRIPLE_NOTE},
    {E1, EIGHTH_TRIPLE_NOTE},
    {G1, EIGHTH_TRIPLE_NOTE},
    {A1, EIGHTH_NOTE},
    {F1, SIXTEENTH_NOTE},
    {G1, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {E1, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {C, SIXTEENTH_NOTE},
    {D1, SIXTEENTH_NOTE},
    {B, SIXTEENTH_NOTE},
    {REST, EIGHTH_NOTE},
      
    {E1, SIXTEENTH_NOTE},   //22
    {C, EIGHTH_NOTE},
    {G, SIXTEENTH_NOTE},
    {REST, EIGHTH_NOTE},
    {AF, EIGHTH_NOTE},
    {A, SIXTEENTH_NOTE},
    {F1, EIGHTH_NOTE},
    {F1, SIXTEENTH_NOTE},
    {A, QUARTER_NOTE},
      
    {B, EIGHTH_TRIPLE_NOTE},
    {A1, EIGHTH_TRIPLE_NOTE},
    {A1, EIGHTH_TRIPLE_NOTE},
    {A1, EIGHTH_TRIPLE_NOTE},
    {G1, EIGHTH_TRIPLE_NOTE},
    {F1, EIGHTH_TRIPLE_NOTE},
    {E1, SIXTEENTH_NOTE},
    {C, EIGHTH_NOTE},
    {A, SIXTEENTH_NOTE},
    {G, QUARTER_NOTE},
  
    {E1, SIXTEENTH_NOTE},   
    {C, EIGHTH_NOTE},
    {G, SIXTEENTH_NOTE},
    {REST, EIGHTH_NOTE},
    {AF, EIGHTH_NOTE},
    {A, SIXTEENTH_NOTE},
    {F1, EIGHTH_NOTE},
    {F1, SIXTEENTH_NOTE},
    {A, QUARTER_NOTE},
      
    {B, SIXTEENTH_NOTE},    
    {F1, EIGHTH_NOTE},
    {F1, SIXTEENTH_NOTE},
    {F1, EIGHTH_TRIPLE_NOTE},
    {E1, EIGHTH_TRIPLE_NOTE},
    {D1, EIGHTH_TRIPLE_NOTE},
    {C, SIXTEENTH_NOTE},
    {G, EIGHTH_NOTE},
    {G, SIXTEENTH_NOTE},
    {C0, QUARTER_NOTE},
      
    {E1, SIXTEENTH_NOTE},   
    {C, EIGHTH_NOTE},
    {G, SIXTEENTH_NOTE},
    {REST, EIGHTH_NOTE},
    {AF, EIGHTH_NOTE},
    {A, SIXTEENTH_NOTE},
    {F1, EIGHTH_NOTE},
    {F1, SIXTEENTH_NOTE},
    {A, QUARTER_NOTE},  
      
    {B, EIGHTH_TRIPLE_NOTE},
    {A1, EIGHTH_TRIPLE_NOTE},
    {A1, EIGHTH_TRIPLE_NOTE},
    {A1, EIGHTH_TRIPLE_NOTE},
    {G1, EIGHTH_TRIPLE_NOTE},
    {F1, EIGHTH_TRIPLE_NOTE},
    {E1, SIXTEENTH_NOTE},
    {C, EIGHTH_NOTE},
    {A, SIXTEENTH_NOTE},
    {G, QUARTER_NOTE},
  
    {E1, SIXTEENTH_NOTE},   
    {C, EIGHTH_NOTE},
    {G, SIXTEENTH_NOTE},
    {REST, EIGHTH_NOTE},
    {AF, EIGHTH_NOTE},
    {A, SIXTEENTH_NOTE},
    {F1, EIGHTH_NOTE},
    {F1, SIXTEENTH_NOTE},
    {A, QUARTER_NOTE},  
  
    {B, SIXTEENTH_NOTE},    
    {F1, EIGHTH_NOTE},
    {F1, SIXTEENTH_NOTE},
    {F1, EIGHTH_TRIPLE_NOTE},
    {E1, EIGHTH_TRIPLE_NOTE},
    {D1, EIGHTH_TRIPLE_NOTE},
    {C, SIXTEENTH_NOTE},
    {G, EIGHTH_NOTE},
    {G, SIXTEENTH_NOTE},
    {C0, QUARTER_NOTE},
      
    {C, SIXTEENTH_NOTE},    //30
    {C, EIGHTH_NOTE},
    {C, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {C, SIXTEENTH_NOTE},
    {D1, EIGHTH_NOTE},
    {E1, SIXTEENTH_NOTE},
    {C, EIGHTH_NOTE},
    {A, SIXTEENTH_NOTE},
    {G, QUARTER_NOTE},
      
    {C, SIXTEENTH_NOTE},
    {C, EIGHTH_NOTE},
    {C, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {C, SIXTEENTH_NOTE},
    {D1, EIGHTH_NOTE},
    {E1, SIXTEENTH_NOTE},
    {REST, HALF_NOTE},
      
    {C, SIXTEENTH_NOTE},
    {C, EIGHTH_NOTE},
    {C, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {C, SIXTEENTH_NOTE},
    {D1, EIGHTH_NOTE},
    {E1, SIXTEENTH_NOTE},
    {C, EIGHTH_NOTE},
    {A, SIXTEENTH_NOTE},
    {G, QUARTER_NOTE},
      
    {E1, SIXTEENTH_NOTE},   //33
    {E1, EIGHTH_NOTE},
    {E1, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {C, SIXTEENTH_NOTE},
    {E1, EIGHTH_NOTE},
    {G1, QUARTER_NOTE},
    {G, QUARTER_NOTE},  
      
    {E1, SIXTEENTH_NOTE},   
    {C, EIGHTH_NOTE},
    {G, SIXTEENTH_NOTE},
    {REST, EIGHTH_NOTE},
    {AF, EIGHTH_NOTE},
    {A, SIXTEENTH_NOTE},
    {F1, EIGHTH_NOTE},
    {F1, SIXTEENTH_NOTE},
    {A, QUARTER_NOTE},  
      
    {B, EIGHTH_TRIPLE_NOTE},
    {A1, EIGHTH_TRIPLE_NOTE},
    {A1, EIGHTH_TRIPLE_NOTE},
    {A1, EIGHTH_TRIPLE_NOTE},
    {G1, EIGHTH_TRIPLE_NOTE},
    {F1, EIGHTH_TRIPLE_NOTE},
    {E1, SIXTEENTH_NOTE},
    {C, EIGHTH_NOTE},
    {A, SIXTEENTH_NOTE},
    {G, QUARTER_NOTE},
      
    {E1, SIXTEENTH_NOTE},   
    {C, EIGHTH_NOTE},
    {G, SIXTEENTH_NOTE},
    {REST, EIGHTH_NOTE},
    {AF, EIGHTH_NOTE},
    {A, SIXTEENTH_NOTE},
    {F1, EIGHTH_NOTE},
    {F1, SIXTEENTH_NOTE},
    {A, QUARTER_NOTE},  
  
    {B, SIXTEENTH_NOTE},    
    {F1, EIGHTH_NOTE},
    {F1, SIXTEENTH_NOTE},
    {F1, EIGHTH_TRIPLE_NOTE},
    {E1, EIGHTH_TRIPLE_NOTE},
    {D1, EIGHTH_TRIPLE_NOTE},
    {C, SIXTEENTH_NOTE},
    {G, EIGHTH_NOTE},
    {G, SIXTEENTH_NOTE},
    {C0, QUARTER_NOTE}, 
      
    {C, DOTTED_SIXTEENTH_NOTE},
    {G, DOTTED_SIXTEENTH_NOTE},
    {E, EIGHTH_NOTE},
    {A, EIGHTH_TRIPLE_NOTE},
    {B, EIGHTH_TRIPLE_NOTE2},
    {A, EIGHTH_TRIPLE_NOTE3},
    {AF, EIGHTH_TRIPLE_NOTE4},
    {BF, EIGHTH_TRIPLE_NOTE5},
    {AF, EIGHTH_TRIPLE_NOTE6},
      
    {G, FINAL_NOTE},
    {REST, WHOLE_NOTE}
}; 


MusicStateType Carol[]={  
// carol of the bells
//1
  {C,    QUARTER_NOTE}, 
  {B,    EIGHTH_NOTE}, 
  {C,    EIGHTH_NOTE},
  {A,    QUARTER_NOTE_PAUSE}, 
  {QUIET,PAUSE},
//2   
  {C,    QUARTER_NOTE}, 
  {B,    EIGHTH_NOTE}, 
  {C,    EIGHTH_NOTE},
  {A,    QUARTER_NOTE_PAUSE}, 
  {1,PAUSE}
};

MusicStateType BirthdaySong[25]={
	
	{C0, EIGHTH_NOTE},
	{C0, EIGHTH_NOTE},
	{D, QUARTER_NOTE},
	{C0, QUARTER_NOTE},
	{F, QUARTER_NOTE},

	{E, HALF_NOTE},
	{C0, EIGHTH_NOTE},
	{C0, EIGHTH_NOTE},
	{D, QUARTER_NOTE},
	{C0, QUARTER_NOTE},

	{G, QUARTER_NOTE},
	{F, HALF_NOTE},
	{C0, EIGHTH_NOTE},
	{C0, EIGHTH_NOTE},
	{C, QUARTER_NOTE},

	{A, QUARTER_NOTE},
	{F, QUARTER_NOTE},
	{E, QUARTER_NOTE},
	{D, QUARTER_NOTE},
	{BF, EIGHTH_NOTE},
	
	{BF, EIGHTH_NOTE},
	{A, QUARTER_NOTE},
	{F, QUARTER_NOTE},
	{G, QUARTER_NOTE},
	{F, QUARTER_NOTE},
};

unsigned short PauseIndex;
unsigned long Duration;
unsigned short Index;
/****************Music_Init***************
 Description: Initializes stepper motor
 Input: none
 Output: none  */ 
void Music_Init(void){
	PauseIndex = 0;
	Index = 0;
	Ptc = &MarioTheme[0];
	Duration = Ptc->Duration;
}
/****************Music_Play***************
 Description:Play music
 Input: none
 Output: none  */ 
#define TIMER0_TAILR_R          (*((volatile unsigned long *)0x40030028))
void Music_Play(void){
	if(Index == 352){
		Index = 0;
	}
	Ptc = &MarioTheme[Index];
	TIMER0_TAILR_R = Ptc->Pitch;
	Duration = Ptc->Duration;
	Index = Index+PauseIndex;
}
/****************Music_Rewind***************
 Description: Rewind music to beginning
 Input: none
 Output: none  */ 
void Music_Rewind(void){
	Ptc = &MarioTheme[0];
	TIMER0_TAILR_R = 0;
	Duration = Ptc->Duration;
	Index = 0;
}
/****************Music_Stop***************
 Description: Stop music, next play will be from the beginning
 Input: none
 Output: none  */ 
void Music_Stop(void){
	PauseIndex =0;
	Ptc = &MarioTheme[0];
	TIMER0_TAILR_R = 0;
	Duration = Ptc->Duration;
	Index = 0;
}
/****************Music_Pause***************
 Description: Pause music, next play will be where it left off
 Input: none
 Output: none  */ 
void Music_Pause(void){
	PauseIndex ^=1;
	TIMER0_TAILR_R  = 0;
}

//Filename: MusicModule.c
//Author: Duc Tran, Brandon Wong
//Initial Creation Date: October 2, 2013
//Description: Handle music player
//Lab Number: W 2-3:30
//TA : Omar & Mahesh
//Date of last revision :October 14, 2013
//Hardware Configuration : Lab5_artist.sch

#define WHOLE_NOTE 64000
#define HALF_NOTE 32000
#define QUARTER_NOTE 16000
#define EIGHTH_NOTE 8000
#define QUARTER_NOTE_PAUSE 100
#define SIXTEENTH_NOTE 4000

#define EIGHTH_TRIPLE_NOTE 5333
#define EIGHTH_TRIPLE_NOTE2 5533
#define EIGHTH_TRIPLE_NOTE3 5733
#define EIGHTH_TRIPLE_NOTE4 5933
#define EIGHTH_TRIPLE_NOTE5 6133
#define EIGHTH_TRIPLE_NOTE6 6333
  
  
  
#define DOTTED_SIXTEENTH_NOTE 12000
  
  
#define FINAL_NOTE 65000 


#define REST 0
#define PAUSE 0
#define QUIET 1
#define C_2 11945   // 65.406 Hz	
#define DF_1 11274   // 69.296 Hz	
#define D_1 10641   // 73.416 Hz	
#define EF_1 10044   // 77.782 Hz	
#define E_1 9480   // 82.407 Hz	
#define F_1 8948   // 87.307 Hz	
#define GF_1 8446   // 92.499 Hz	
#define G_1 7972   // 97.999 Hz	
#define AF_1 7525   // 103.826 Hz	
#define A_1 7102   // 110.000 Hz	
#define BF_1 6704   // 116.541 Hz	
#define B_1 6327   // 123.471 Hz	
#define C_1 5972   // 130.813 Hz	
#define DF0 5637   // 138.591 Hz	
#define D0 5321   // 146.832 Hz	
#define EF0 5022   // 155.563 Hz	
#define E0 4740   // 164.814 Hz	
#define F0 4474   // 174.614 Hz	
#define GF0 4223   // 184.997 Hz	
#define G0 3986   // 195.998 Hz	
#define AF0 3762   // 207.652 Hz	
#define A0 3551   // 220.000 Hz	
#define BF0 3352   // 233.082 Hz	
#define B0 3164   // 246.942 Hz	
#define C0 2986   // 261.626 Hz	
#define DF 2819   // 277.183 Hz	
#define D 2660   // 293.665 Hz	
#define EF 2511   // 311.127 Hz	
#define E 2370   // 329.628 Hz	
#define F 2237   // 349.228 Hz	
#define GF 2112   // 369.994 Hz	
#define G 1993   // 391.995 Hz	
#define AF 1881   // 415.305 Hz	
#define A 1776   // 440.000 Hz	
#define BF 1676   // 466.164 Hz	
#define B 1582   // 493.883 Hz	
#define C 1493   // 523.251 Hz	
#define DF1 1409   // 554.365 Hz	
#define D1 1330   // 587.330 Hz	
#define EF1 1256   // 622.254 Hz	
#define E1 1185   // 659.255 Hz	
#define F1 1119   // 698.456 Hz	
#define GF1 1056   // 739.989 Hz	
#define G1 997   // 783.991 Hz	
#define AF1 941   // 830.609 Hz	
#define A1 888   // 880.000 Hz	
#define BF1 838   // 932.328 Hz	
#define B1 791   // 987.767 Hz	
#define C1 747   // 1046.502 Hz	
#define DF2 705   // 1108.731 Hz	
#define D2 665   // 1174.659 Hz	
#define EF2 628   // 1244.508 Hz	
#define E2 593   // 1318.510 Hz	
#define F2 559   // 1396.913 Hz	
#define GF2 528   // 1479.978 Hz	
#define G2 498   // 1567.982 Hz	
#define AF2 470   // 1661.219 Hz	
#define A2 444   // 1760.000 Hz	
#define BF2 419   // 1864.655 Hz	
#define B2 395   // 1975.533 Hz	
#define C2 373   // 2093.005 Hz	






struct Player{
	unsigned short Pitch;
	unsigned short Duration;
	//const struct Player *Next[8];
	
	//const struct Player *Song;
	//unsigned short SinTable[256];
};

typedef const struct Player SongType;
typedef SongType *SongPtr;

typedef const struct Player MusicStateType;
const struct Player *Ptc;




//MusicStateType MarioTheme[58]={

//	{E1, SIXTEENTH_NOTE},
//	{E1, EIGHTH_NOTE},
//	{E1, SIXTEENTH_NOTE},
//	{REST, SIXTEENTH_NOTE},
//	{C, SIXTEENTH_NOTE},
//	{E1, EIGHTH_NOTE},
//	{G1, QUARTER_NOTE},
//	{G, QUARTER_NOTE},

//	{C, EIGHTH_NOTE},
//	{REST, SIXTEENTH_NOTE},
//	{G, SIXTEENTH_NOTE},
//	{REST, EIGHTH_NOTE},
//	{E, EIGHTH_NOTE},
//	{REST, SIXTEENTH_NOTE},
//	{A, SIXTEENTH_NOTE},
//	{REST, SIXTEENTH_NOTE},
//	{B, SIXTEENTH_NOTE},
//	{REST, SIXTEENTH_NOTE},
//	{BF, SIXTEENTH_NOTE},
//	{A, EIGHTH_NOTE},

//	{G, EIGHTH_TRIPLE_NOTE},
//	{E1, EIGHTH_TRIPLE_NOTE},
//	{G1, EIGHTH_TRIPLE_NOTE},
//	{A1, EIGHTH_NOTE},
//	{F1, SIXTEENTH_NOTE},
//	{G1, SIXTEENTH_NOTE},
//	{REST, SIXTEENTH_NOTE},
//	{E1, SIXTEENTH_NOTE},
//	{REST, SIXTEENTH_NOTE},
//	{C, SIXTEENTH_NOTE},
//	{D1, SIXTEENTH_NOTE},
//	{B, SIXTEENTH_NOTE},
//	{REST, EIGHTH_NOTE},

//	{C, EIGHTH_NOTE},
//	{REST, SIXTEENTH_NOTE},
//	{G, SIXTEENTH_NOTE},
//	{REST, EIGHTH_NOTE},
//	{E, EIGHTH_NOTE},
//	{REST, SIXTEENTH_NOTE},
//	{A, SIXTEENTH_NOTE},
//	{REST, SIXTEENTH_NOTE},
//	{B, SIXTEENTH_NOTE},
//	{REST, SIXTEENTH_NOTE},
//	{BF, SIXTEENTH_NOTE},
//	{A, EIGHTH_NOTE},


//	{G, EIGHTH_TRIPLE_NOTE},
//	{E1, EIGHTH_TRIPLE_NOTE},
//	{G1, EIGHTH_TRIPLE_NOTE},
//	{A1, EIGHTH_NOTE},
//	{F1, SIXTEENTH_NOTE},
//	{G1, SIXTEENTH_NOTE},
//	{REST, SIXTEENTH_NOTE},
//	{E1, SIXTEENTH_NOTE},
//	{REST, SIXTEENTH_NOTE},
//	{C, SIXTEENTH_NOTE},
//	{D1, SIXTEENTH_NOTE},
//	{B, SIXTEENTH_NOTE},
//	{REST, EIGHTH_NOTE},

//};


MusicStateType MarioTheme[352]={
  
    {E1, SIXTEENTH_NOTE},   //1
    {E1, EIGHTH_NOTE},
    {E1, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {C, SIXTEENTH_NOTE},
    {E1, EIGHTH_NOTE},
    {G1, QUARTER_NOTE},
    {G, QUARTER_NOTE},
  
    {C, EIGHTH_NOTE},    //2
    {REST, SIXTEENTH_NOTE},
    {G, SIXTEENTH_NOTE},
    {REST, EIGHTH_NOTE},
    {E, EIGHTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {A, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {B, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {BF, SIXTEENTH_NOTE},
    {A, EIGHTH_NOTE},
  
    {G, EIGHTH_TRIPLE_NOTE},    //3
    {E1, EIGHTH_TRIPLE_NOTE},
    {G1, EIGHTH_TRIPLE_NOTE},
    {A1, EIGHTH_NOTE},
    {F1, SIXTEENTH_NOTE},
    {G1, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {E1, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {C, SIXTEENTH_NOTE},
    {D1, SIXTEENTH_NOTE},
    {B, SIXTEENTH_NOTE},
    {REST, EIGHTH_NOTE},
  
    {C, EIGHTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {G, SIXTEENTH_NOTE},
    {REST, EIGHTH_NOTE},
    {E, EIGHTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {A, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {B, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {BF, SIXTEENTH_NOTE},
    {A, EIGHTH_NOTE},
  
  
    {G, EIGHTH_TRIPLE_NOTE},
    {E1, EIGHTH_TRIPLE_NOTE},
    {G1, EIGHTH_TRIPLE_NOTE},
    {A1, EIGHTH_NOTE},
    {F1, SIXTEENTH_NOTE},
    {G1, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {E1, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {C, SIXTEENTH_NOTE},
    {D1, SIXTEENTH_NOTE},
    {B, SIXTEENTH_NOTE},
    {REST, EIGHTH_NOTE},
  
    {REST, EIGHTH_NOTE},
    {G1, SIXTEENTH_NOTE},
    {GF1, SIXTEENTH_NOTE},
    {F1, SIXTEENTH_NOTE},
    {EF1, EIGHTH_NOTE},
    {E1, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {AF, SIXTEENTH_NOTE},
    {A, SIXTEENTH_NOTE},
    {C, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {A, SIXTEENTH_NOTE},
    {C, SIXTEENTH_NOTE},
    {D1, SIXTEENTH_NOTE},
      
    {REST, EIGHTH_NOTE},
    {G1, SIXTEENTH_NOTE},
    {GF1, SIXTEENTH_NOTE},
    {F1, SIXTEENTH_NOTE},
    {EF1, EIGHTH_NOTE},
    {E1, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {C1, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {C1, SIXTEENTH_NOTE},
    {C1, QUARTER_NOTE},
      
    {REST, EIGHTH_NOTE},
    {G1, SIXTEENTH_NOTE},
    {GF1, SIXTEENTH_NOTE},
    {F1, SIXTEENTH_NOTE},
    {EF1, EIGHTH_NOTE},
    {E1, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {AF, SIXTEENTH_NOTE},
    {A, SIXTEENTH_NOTE},
    {C, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {A, SIXTEENTH_NOTE},
    {C, SIXTEENTH_NOTE},
    {D1, SIXTEENTH_NOTE},
      
    {REST, EIGHTH_NOTE},
    {EF1, EIGHTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {D1, SIXTEENTH_NOTE},
    {REST, EIGHTH_NOTE},
    {C, QUARTER_NOTE},
    {REST, QUARTER_NOTE},
      
    {C, SIXTEENTH_NOTE},
    {C, EIGHTH_NOTE},
    {C, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {C, SIXTEENTH_NOTE},
    {D1, EIGHTH_NOTE},
    {E1, SIXTEENTH_NOTE},
    {C, EIGHTH_NOTE},
    {A, SIXTEENTH_NOTE},
    {G, QUARTER_NOTE},
      
    {C, SIXTEENTH_NOTE},
    {C, EIGHTH_NOTE},
    {C, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {C, SIXTEENTH_NOTE},
    {D1, EIGHTH_NOTE},
    {E1, SIXTEENTH_NOTE},
    {REST, HALF_NOTE},
      
    {C, SIXTEENTH_NOTE},
    {C, EIGHTH_NOTE},
    {C, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {C, SIXTEENTH_NOTE},
    {D1, EIGHTH_NOTE},
    {E1, SIXTEENTH_NOTE},
    {C, EIGHTH_NOTE},
    {A, SIXTEENTH_NOTE},
    {G, QUARTER_NOTE},
      
    {E1, SIXTEENTH_NOTE},   //17
    {E1, EIGHTH_NOTE},
    {E1, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {C, SIXTEENTH_NOTE},
    {E1, EIGHTH_NOTE},
    {G1, QUARTER_NOTE},
    {G, QUARTER_NOTE},
      
    {C, EIGHTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {G, SIXTEENTH_NOTE},
    {REST, EIGHTH_NOTE},
    {E, EIGHTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {A, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {B, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {BF, SIXTEENTH_NOTE},
    {A, EIGHTH_NOTE},
  
    {G, EIGHTH_TRIPLE_NOTE},
    {E1, EIGHTH_TRIPLE_NOTE},
    {G1, EIGHTH_TRIPLE_NOTE},
    {A1, EIGHTH_NOTE},
    {F1, SIXTEENTH_NOTE},
    {G1, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {E1, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {C, SIXTEENTH_NOTE},
    {D1, SIXTEENTH_NOTE},
    {B, SIXTEENTH_NOTE},
    {REST, EIGHTH_NOTE},
  
    {C, EIGHTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {G, SIXTEENTH_NOTE},
    {REST, EIGHTH_NOTE},
    {E, EIGHTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {A, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {B, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {BF, SIXTEENTH_NOTE},
    {A, EIGHTH_NOTE},
  
  
    {G, EIGHTH_TRIPLE_NOTE},
    {E1, EIGHTH_TRIPLE_NOTE},
    {G1, EIGHTH_TRIPLE_NOTE},
    {A1, EIGHTH_NOTE},
    {F1, SIXTEENTH_NOTE},
    {G1, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {E1, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {C, SIXTEENTH_NOTE},
    {D1, SIXTEENTH_NOTE},
    {B, SIXTEENTH_NOTE},
    {REST, EIGHTH_NOTE},
      
    {E1, SIXTEENTH_NOTE},   //22
    {C, EIGHTH_NOTE},
    {G, SIXTEENTH_NOTE},
    {REST, EIGHTH_NOTE},
    {AF, EIGHTH_NOTE},
    {A, SIXTEENTH_NOTE},
    {F1, EIGHTH_NOTE},
    {F1, SIXTEENTH_NOTE},
    {A, QUARTER_NOTE},
      
    {B, EIGHTH_TRIPLE_NOTE},
    {A1, EIGHTH_TRIPLE_NOTE},
    {A1, EIGHTH_TRIPLE_NOTE},
    {A1, EIGHTH_TRIPLE_NOTE},
    {G1, EIGHTH_TRIPLE_NOTE},
    {F1, EIGHTH_TRIPLE_NOTE},
    {E1, SIXTEENTH_NOTE},
    {C, EIGHTH_NOTE},
    {A, SIXTEENTH_NOTE},
    {G, QUARTER_NOTE},
  
    {E1, SIXTEENTH_NOTE},   
    {C, EIGHTH_NOTE},
    {G, SIXTEENTH_NOTE},
    {REST, EIGHTH_NOTE},
    {AF, EIGHTH_NOTE},
    {A, SIXTEENTH_NOTE},
    {F1, EIGHTH_NOTE},
    {F1, SIXTEENTH_NOTE},
    {A, QUARTER_NOTE},
      
    {B, SIXTEENTH_NOTE},    
    {F1, EIGHTH_NOTE},
    {F1, SIXTEENTH_NOTE},
    {F1, EIGHTH_TRIPLE_NOTE},
    {E1, EIGHTH_TRIPLE_NOTE},
    {D1, EIGHTH_TRIPLE_NOTE},
    {C, SIXTEENTH_NOTE},
    {G, EIGHTH_NOTE},
    {G, SIXTEENTH_NOTE},
    {C0, QUARTER_NOTE},
      
    {E1, SIXTEENTH_NOTE},   
    {C, EIGHTH_NOTE},
    {G, SIXTEENTH_NOTE},
    {REST, EIGHTH_NOTE},
    {AF, EIGHTH_NOTE},
    {A, SIXTEENTH_NOTE},
    {F1, EIGHTH_NOTE},
    {F1, SIXTEENTH_NOTE},
    {A, QUARTER_NOTE},  
      
    {B, EIGHTH_TRIPLE_NOTE},
    {A1, EIGHTH_TRIPLE_NOTE},
    {A1, EIGHTH_TRIPLE_NOTE},
    {A1, EIGHTH_TRIPLE_NOTE},
    {G1, EIGHTH_TRIPLE_NOTE},
    {F1, EIGHTH_TRIPLE_NOTE},
    {E1, SIXTEENTH_NOTE},
    {C, EIGHTH_NOTE},
    {A, SIXTEENTH_NOTE},
    {G, QUARTER_NOTE},
  
    {E1, SIXTEENTH_NOTE},   
    {C, EIGHTH_NOTE},
    {G, SIXTEENTH_NOTE},
    {REST, EIGHTH_NOTE},
    {AF, EIGHTH_NOTE},
    {A, SIXTEENTH_NOTE},
    {F1, EIGHTH_NOTE},
    {F1, SIXTEENTH_NOTE},
    {A, QUARTER_NOTE},  
  
    {B, SIXTEENTH_NOTE},    
    {F1, EIGHTH_NOTE},
    {F1, SIXTEENTH_NOTE},
    {F1, EIGHTH_TRIPLE_NOTE},
    {E1, EIGHTH_TRIPLE_NOTE},
    {D1, EIGHTH_TRIPLE_NOTE},
    {C, SIXTEENTH_NOTE},
    {G, EIGHTH_NOTE},
    {G, SIXTEENTH_NOTE},
    {C0, QUARTER_NOTE},
      
    {C, SIXTEENTH_NOTE},    //30
    {C, EIGHTH_NOTE},
    {C, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {C, SIXTEENTH_NOTE},
    {D1, EIGHTH_NOTE},
    {E1, SIXTEENTH_NOTE},
    {C, EIGHTH_NOTE},
    {A, SIXTEENTH_NOTE},
    {G, QUARTER_NOTE},
      
    {C, SIXTEENTH_NOTE},
    {C, EIGHTH_NOTE},
    {C, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {C, SIXTEENTH_NOTE},
    {D1, EIGHTH_NOTE},
    {E1, SIXTEENTH_NOTE},
    {REST, HALF_NOTE},
      
    {C, SIXTEENTH_NOTE},
    {C, EIGHTH_NOTE},
    {C, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {C, SIXTEENTH_NOTE},
    {D1, EIGHTH_NOTE},
    {E1, SIXTEENTH_NOTE},
    {C, EIGHTH_NOTE},
    {A, SIXTEENTH_NOTE},
    {G, QUARTER_NOTE},
      
    {E1, SIXTEENTH_NOTE},   //33
    {E1, EIGHTH_NOTE},
    {E1, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {C, SIXTEENTH_NOTE},
    {E1, EIGHTH_NOTE},
    {G1, QUARTER_NOTE},
    {G, QUARTER_NOTE},  
      
    {E1, SIXTEENTH_NOTE},   
    {C, EIGHTH_NOTE},
    {G, SIXTEENTH_NOTE},
    {REST, EIGHTH_NOTE},
    {AF, EIGHTH_NOTE},
    {A, SIXTEENTH_NOTE},
    {F1, EIGHTH_NOTE},
    {F1, SIXTEENTH_NOTE},
    {A, QUARTER_NOTE},  
      
    {B, EIGHTH_TRIPLE_NOTE},
    {A1, EIGHTH_TRIPLE_NOTE},
    {A1, EIGHTH_TRIPLE_NOTE},
    {A1, EIGHTH_TRIPLE_NOTE},
    {G1, EIGHTH_TRIPLE_NOTE},
    {F1, EIGHTH_TRIPLE_NOTE},
    {E1, SIXTEENTH_NOTE},
    {C, EIGHTH_NOTE},
    {A, SIXTEENTH_NOTE},
    {G, QUARTER_NOTE},
      
    {E1, SIXTEENTH_NOTE},   
    {C, EIGHTH_NOTE},
    {G, SIXTEENTH_NOTE},
    {REST, EIGHTH_NOTE},
    {AF, EIGHTH_NOTE},
    {A, SIXTEENTH_NOTE},
    {F1, EIGHTH_NOTE},
    {F1, SIXTEENTH_NOTE},
    {A, QUARTER_NOTE},  
  
    {B, SIXTEENTH_NOTE},    
    {F1, EIGHTH_NOTE},
    {F1, SIXTEENTH_NOTE},
    {F1, EIGHTH_TRIPLE_NOTE},
    {E1, EIGHTH_TRIPLE_NOTE},
    {D1, EIGHTH_TRIPLE_NOTE},
    {C, SIXTEENTH_NOTE},
    {G, EIGHTH_NOTE},
    {G, SIXTEENTH_NOTE},
    {C0, QUARTER_NOTE}, 
      
    {C, DOTTED_SIXTEENTH_NOTE},
    {G, DOTTED_SIXTEENTH_NOTE},
    {E, EIGHTH_NOTE},
    {A, EIGHTH_TRIPLE_NOTE},
    {B, EIGHTH_TRIPLE_NOTE2},
    {A, EIGHTH_TRIPLE_NOTE3},
    {AF, EIGHTH_TRIPLE_NOTE4},
    {BF, EIGHTH_TRIPLE_NOTE5},
    {AF, EIGHTH_TRIPLE_NOTE6},
      
    {G, FINAL_NOTE},
    {REST, WHOLE_NOTE}
}; 


MusicStateType Carol[]={  
// carol of the bells
//1
  {C,    QUARTER_NOTE}, 
  {B,    EIGHTH_NOTE}, 
  {C,    EIGHTH_NOTE},
  {A,    QUARTER_NOTE_PAUSE}, 
  {QUIET,PAUSE},
//2   
  {C,    QUARTER_NOTE}, 
  {B,    EIGHTH_NOTE}, 
  {C,    EIGHTH_NOTE},
  {A,    QUARTER_NOTE_PAUSE}, 
  {1,PAUSE}
};

MusicStateType BirthdaySong[25]={
	
	{C0, EIGHTH_NOTE},
	{C0, EIGHTH_NOTE},
	{D, QUARTER_NOTE},
	{C0, QUARTER_NOTE},
	{F, QUARTER_NOTE},

	{E, HALF_NOTE},
	{C0, EIGHTH_NOTE},
	{C0, EIGHTH_NOTE},
	{D, QUARTER_NOTE},
	{C0, QUARTER_NOTE},

	{G, QUARTER_NOTE},
	{F, HALF_NOTE},
	{C0, EIGHTH_NOTE},
	{C0, EIGHTH_NOTE},
	{C, QUARTER_NOTE},

	{A, QUARTER_NOTE},
	{F, QUARTER_NOTE},
	{E, QUARTER_NOTE},
	{D, QUARTER_NOTE},
	{BF, EIGHTH_NOTE},
	
	{BF, EIGHTH_NOTE},
	{A, QUARTER_NOTE},
	{F, QUARTER_NOTE},
	{G, QUARTER_NOTE},
	{F, QUARTER_NOTE},
};

unsigned short PauseIndex;
unsigned long Duration;
unsigned short Index;
/****************Music_Init***************
 Description: Initializes stepper motor
 Input: none
 Output: none  */ 
void Music_Init(void){
	PauseIndex = 0;
	Index = 0;
	Ptc = &MarioTheme[0];
	Duration = Ptc->Duration;
}
/****************Music_Play***************
 Description:Play music
 Input: none
 Output: none  */ 
#define TIMER0_TAILR_R          (*((volatile unsigned long *)0x40030028))
void Music_Play(void){
	if(Index == 352){
		Index = 0;
	}
	Ptc = &MarioTheme[Index];
	TIMER0_TAILR_R = Ptc->Pitch;
	Duration = Ptc->Duration;
	Index = Index+PauseIndex;
}
/****************Music_Rewind***************
 Description: Rewind music to beginning
 Input: none
 Output: none  */ 
void Music_Rewind(void){
	Ptc = &MarioTheme[0];
	TIMER0_TAILR_R = 0;
	Duration = Ptc->Duration;
	Index = 0;
}
/****************Music_Stop***************
 Description: Stop music, next play will be from the beginning
 Input: none
 Output: none  */ 
void Music_Stop(void){
	PauseIndex =0;
	Ptc = &MarioTheme[0];
	TIMER0_TAILR_R = 0;
	Duration = Ptc->Duration;
	Index = 0;
}
/****************Music_Pause***************
 Description: Pause music, next play will be where it left off
 Input: none
 Output: none  */ 
void Music_Pause(void){
	PauseIndex ^=1;
	TIMER0_TAILR_R  = 0;
}

//Filename: Timer0A.c
//Author: Duc Tran, Brandon Wong
//Initial Creation Date: October 2, 2013
//Description: 
//Lab Number: W 2-3:30
//TA : Omar & Mahesh
//Date of last revision :October 14, 2013
//Hardware Configuration : Lab5_artist.sch

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






