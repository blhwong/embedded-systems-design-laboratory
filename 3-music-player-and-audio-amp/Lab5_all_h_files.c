//Filename: MusicModule.h
//Author: Duc Tran, Brandon Wong
//Initial Creation Date: October 2, 2013
//Description: 
//Lab Number: W 2-3:30
//TA : Omar & Mahesh
//Date of last revision :October 14, 2013
//Hardware Configuration : Lab5_artist.sch

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
	//const struct Player *Song;
	//unsigned short SinTable[256];
};

/****************Music_Init***************
 Description: Initializes stepper motor
 Input: none
 Output: none  */ 
void Music_Init(void);

/****************Music_Play***************
 Description:Play music
 Input: none
 Output: none  */ 
void Music_Play(void);
/****************Music_Rewind***************
 Description: Rewind music to beginning
 Input: none
 Output: none  */ 
void Music_Rewind(void);
/****************Music_Stop***************
 Description: Stop music, next play will be from the beginning
 Input: none
 Output: none  */ 
void Music_Stop(void);
/****************Music_Pause***************
 Description: Pause music, next play will be where it left off
 Input: none
 Output: none  */ 
void Music_Pause(void);



//Filename: Timer0A.h
//Author: Duc Tran, Brandon Wong
//Initial Creation Date: October 2, 2013
//Description: 
//Lab Number: W 2-3:30
//TA : Omar & Mahesh
//Date of last revision :October 14, 2013
//Hardware Configuration : Lab5_artist.sch

// Timer0A.h
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

// ***************** Timer0A_Init ****************
// Activate Timer0A interrupts to run user task periodically
// Inputs:  task is a pointer to a user function
//          period in usec
// Outputs: none
void Timer0A_Init(void(*task)(void), unsigned short period);
// ***************** Timer1A_Init ****************
// Activate Timer1A interrupts to run user task periodically
// Inputs:  task is a pointer to a user function
//          period in usec
// Outputs: none
void Timer1A_Init(void(*task)(void), unsigned short period);

// TLV5618.h
// Runs on LM3S1968
// Daniel Valvano
// November 5, 2012
/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2012

 Copyright 2013 by Jonathan W. Valvano, valvano@mail.utexas.edu
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

// SSI0Clk (SCLK, pin 4) connected to PA2
// SSI0Fss (!CS, pin 2) connected to PA3
// SSI0Tx (DIN, pin 3) connected to PA5
// see Volume 2 Figure 7.19 for complete schematic

//********DAC_Init*****************
// Initialize MAX5353 12-bit DAC
// inputs:  initial voltage output (0 to 4095)
// outputs: none
// assumes: system clock of 80 MHz
void DAC_Init(unsigned short data);

//********DAC_Out*****************
// Send data to MAX5353 12-bit DAC
// inputs:  voltage output (0 to 4095)
// outputs: none
void DAC_Out(unsigned short code);

// SysTick.h
// Runs on LM3S1968 at 50 MHz
// Provide functions that initialize the SysTick module, wait at least a
// designated number of clock cycles, and wait approximately a multiple
// of 10 milliseconds using busy wait.
// Daniel Valvano
// June 15, 2011

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to the Arm Cortex M3",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2011

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

// Initialize SysTick with busy wait running at bus clock.
void SysTick_Init(void);

// Time delay using busy wait.
// The delay parameter is in units of the 50 MHz core clock. (20 nsec)
void SysTick_Wait(unsigned long delay);

// Time delay using busy wait.
// 10000us equals 10ms
void SysTick_Wait10ms(unsigned long delay);


// PLL.h
// Runs on LM3S1968
// A software function to change the bus speed using the PLL.
// Commented lines in the function PLL_Init() initialize the PWM
// to either 25 MHz or 50 MHz.  When using an oscilloscope to
// look at LED0, it should be clear to see that the LED flashes
// about 2 (50/25) times faster with a 50 MHz clock than with a
// 25 MHz clock.
// Daniel Valvano
// February 21, 2012

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to the Arm Cortex M3",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2011
   Program 2.10, Figure 2.31

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

// configure the system to get its clock from the PLL
void PLL_Init(void);
