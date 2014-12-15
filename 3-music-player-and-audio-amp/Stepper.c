//Filename: Stepper.c
//Author: Duc Tran, Brandon Wong
//Initial Creation Date: September 25, 2013
//Description: 
//Lab Number: W 2-3:30
//TA : Omar & Mahesh
//Date of last revision :September 30, 2013
//Hardware Configuration : Lab4_artist.sch



// Runs on LM3S811
// Provide functions that step the motor once clockwise, step
// once counterclockwise, and initialize the stepper motor
// interface.
// Daniel Valvano
// June 29, 2011

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to the Arm Cortex M3",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2011
   Example 4.1, Programs 4.4, 4.5, and 4.6
   Hardware circuit diagram Figure 4.27

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

// PD3 connected to driver for stepper motor coil A
// PD2 connected to driver for stepper motor coil A'
// PD1 connected to driver for stepper motor coil B
// PD0 connected to driver for stepper motor coil B'

#include "lm3s1968.h"
#include "systick.h"
struct State{
  unsigned char Out;           // Output
  const struct State *Next[8]; // CW/CCW
};




typedef const struct State StateType;
typedef StateType *StatePtr;
#define clockwise 0        // Next index
#define counterclockwise 1 // Next index

#define 	S1	&FSM[0]
#define 	S2	&FSM[1]
#define 	S3	&FSM[2]
#define 	S4	&FSM[3]
#define 	S5	&FSM[4]
#define 	S6	&FSM[5]
#define 	S7	&FSM[6]
#define 	S8	&FSM[7]
#define 	S9	&FSM[8]
#define 	S10	&FSM[9]
#define 	S11	&FSM[10]
#define 	S12	&FSM[11]
#define 	S13	&FSM[12]
#define 	S14	&FSM[13]
#define 	S15	&FSM[14]
#define 	S16	&FSM[15]
#define 	S17	&FSM[16]
#define 	S18	&FSM[17]
#define 	S19	&FSM[18]
#define 	S20	&FSM[19]
#define 	S21	&FSM[20]
#define 	S22	&FSM[21]
#define 	S23	&FSM[22]
#define 	S24	&FSM[23]
#define 	S25	&FSM[24] //STEP
#define 	S26	&FSM[25]
#define 	S27	&FSM[26]
#define 	S28	&FSM[27]
#define 	S29	&FSM[28]

StateType FSM[27]={
  { 10,{S1,S2,S4,S5,S21,S1,S1,S1}}, //0
	{ 9, {S2,S3,S1,S25,S22,S2,S2,S2}},//1
	{ 5, {S3,S4,S2,S26,S23,S3,S3,S3}},//2
  { 6, {S4,S1,S3,S27,S24,S4,S4,S4}},//3

  { 9, {S5,S2,S4,S6,S22,S5,S5,S5}},	//4
	{ 5, {S6,S3,S1,S7,S23,S6,S6,S6}},	//5
	{ 6, {S7,S4,S2,S8,S24,S7,S7,S7}},	//6
  { 10,{S8,S1,S3,S9,S21,S8,S8,S8}},	//7
  { 9, {S9,S2,S4,S10,S22,S9,S9,S9}},	//8
	{ 5, {S10,S3,S1,S11,S23,S10,S10,S10}},//9
	{ 6, {S11,S4,S2,S12,S24,S11,S11,S11}},//10
  { 10,{S12,S1,S3,S13,S21,S12,S12,S12}},//11
  

  { 6, {S13,S2,S4,S14,S24,S13,S13,S13}},//12
	{ 5, {S14,S3,S1,S15,S23,S14,S14,S14}},//13
	{ 9, {S15,S4,S2,S16,S22,S15,S15,S15}},//14
  { 10,{S16,S1,S3,S17,S21,S16,S16,S16}},//15
  { 6, {S17,S2,S4,S18,S24,S17,S17,S17}},//16
	{ 5, {S18,S3,S1,S19,S23,S18,S18,S18}},//17
	{ 9, {S19,S4,S2,S20,S22,S19,S19,S19}},//18
  { 10,{S20,S1,S3,S5,S21,S20,S20,S20}},//19
	
	
	{ 10,{S2,S2,S4,S5,S21,S1,S1,S1}},		//20 //step
	{ 9, {S3,S3,S1,S6,S22,S2,S2,S2}},
	{ 5, {S4,S4,S2,S7,S23,S3,S3,S3}},
  { 6, {S1,S1,S3,S8,S24,S4,S4,S4}},
	
	{ 5,{S25,S4,S2,S26,S23,S3,S3,S3}},
	{ 6, {S26,S1,S3,S27,S24,S4,S4,S4}},
	{ 10, {S27,S2,S4,S5,S21,S1,S1,S1}}
	
//	{ 10,{S1,S2,S4,S5,S21,S1,S1,S1}}, //0
//	{ 9, {S2,S3,S1,S27,S22,S2,S2,S2}},//1
//	{ 5, {S3,S4,S2,S26,S23,S3,S3,S3}},//2
//  { 6, {S4,S1,S3,S25,S24,S4,S4,S4}},//3

//  { 6, {S5,S2,S4,S6,S22,S5,S5,S5}},	//4
//	{ 5, {S6,S3,S1,S7,S23,S6,S6,S6}},	//5
//	{ 9, {S7,S4,S2,S8,S24,S7,S7,S7}},	//6
//  { 10,{S8,S1,S3,S9,S21,S8,S8,S8}},	//7
//  { 6, {S9,S2,S4,S10,S22,S9,S9,S9}},	//8
//	{ 5, {S10,S3,S1,S11,S23,S10,S10,S10}},//9
//	{ 9, {S11,S4,S2,S12,S24,S11,S11,S11}},//10
//  { 10,{S12,S1,S3,S13,S21,S12,S12,S12}},//11
//  

//  { 9, {S13,S2,S4,S14,S24,S13,S13,S13}},//12
//	{ 5, {S14,S3,S1,S15,S23,S14,S14,S14}},//13
//	{ 6, {S15,S4,S2,S16,S22,S15,S15,S15}},//14
//  { 10,{S16,S1,S3,S17,S21,S16,S16,S16}},//15
//  { 9, {S17,S2,S4,S18,S24,S17,S17,S17}},//16
//	{ 5, {S18,S3,S1,S19,S23,S18,S18,S18}},//17
//	{ 6, {S19,S4,S2,S20,S22,S19,S19,S19}},//18
//  { 10,{S20,S1,S3,S5,S21,S20,S20,S20}},//19
//	
//	
//	{ 10,{S2,S2,S4,S5,S21,S1,S1,S1}},		//20 //step
//	{ 9, {S3,S3,S1,S27,S22,S2,S2,S2}},	//21
//	{ 5, {S4,S4,S2,S26,S23,S3,S3,S3}},		//22
//  { 6, {S1,S1,S3,S25,S24,S4,S4,S4}},		//23
//	
//	{ 5,{S25,S4,S2,S26,S23,S3,S3,S3}},	//24	
//	{ 9, {S26,S1,S3,S27,S24,S4,S4,S4}},	//25
//	{ 10, {S27,S2,S4,S5,S21,S1,S1,S1}}		//26

};
unsigned char Pos;     // between 0 and 199
const struct State *Ptc;// Current State

#define STEPPER  (*((volatile unsigned long *)0x4000703C))


void StepperFSM(unsigned char Input){
	Ptc = Ptc->Next[Input];
	STEPPER = Ptc->Out;
}


void Stepper_Init(void){volatile unsigned long delay;
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOD; //activate port D
	delay =  SYSCTL_RCGC2_R;
  Pos = 0;      
  Ptc = S1;
  GPIO_PORTD_DIR_R |= 0x0F;   // make PD3-0 out
  GPIO_PORTD_DEN_R |= 0x0F;   // enable digital I/O on PD3-0 
  GPIO_PORTD_DR8R_R |= 0x0F;  // enable 8 mA drive

} 



