//Filename: logo.h
//Author: Duc Tran, Brandon Wong
//Initial Creation Date: September 10, 2013
//Description: 	Functions for drawing lines on analog clock
//Lab Number: MW 2-3:30
//TA : Omar & Mahesh
//Date of last revision :September 27, 2013
//Hardware Configuration : NONE

// Logo.h
// Runs on LM3S1968 or LM3S8962
// Jonathan Valvano
// November 12, 2012

/* This example accompanies the books
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2012
   
   "Embedded Systems: Introduction to Arm Cortex M Microcontrollers",
   ISBN: 978-1469998749, Jonathan Valvano, copyright (c) 2012
 Copyright 2012 by Jonathan W. Valvano, valvano@mail.utexas.edu
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


// //*************RIT128x96x4_Logo********************************************
// //  Displays the 4-bit color BMP image stored in LogoBuffer
// //  Inputs: ypos (94 is bottom, 80 is near the bottom)
// //  Outputs: none
// //  Must be less than or equal to 128 pixels wide by 80 rows high
// //  The width must be an even number
// void RIT128x96x4_Logo(unsigned long ypos);

//*************RIT128x96x4_BMP********************************************
//  Displays the 4-bit color BMP image 
//  Inputs: xpos horizontal position to display this image, columns from the left edge 
//          ypos (94 is bottom, 80 is near the bottom)
//          pointer to a BMP image
//  Outputs: none
//  Must be less than or equal to 128 pixels wide by 80 rows high
//  The width must be an even number
void RIT128x96x4_BMP(unsigned long xpos, unsigned long ypos, const unsigned char *Buffer);

//*************RIT128x96x4_Line********************************************
// Draws one line in the RAM version of the image
// Inputs: (x1,y1) is the start point
// (x2,y2) is the end point
// color is 0 (off) to 15 (white)
// coordinates range from 0 to MAX, 0 is bottom or left, MAX is top or right
// Outputs: none
void RIT128x96x4_Line(int x1, int y1, int x2, int y2, unsigned char color);

	//*************RIT128x96x4_ClearImage**************************************
// Clears the RAM version of the image
// Inputs: none
// Outputs: none
void RIT128x96x4_ClearImage(void);

//*************RIT128x96x4_ShowImage**************************************
// Copies the RAM version of the image to the OLED
// Inputs: none
// Outputs: none
void RIT128x96x4_ShowImage(void);





//Filename: SoundModule.c
//Author: Duc Tran, Brandon Wong
//Initial Creation Date: September 13, 2013
//Description: Switch Module
//Lab Number: MW 2-3:30
//TA : Omar & Mahesh
//Date of last revision :September 27, 2

#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
#define GPIO_PORTH_DIR_R       (*((volatile unsigned long *)0x40027400))
#define GPIO_PORTH_DEN_R      (*((volatile unsigned long *)0x4002751C))
#define PH0      						(*((volatile unsigned long *)0x40027004))
#define PH1      						    (*((volatile unsigned long *)0x40027008))

 /****************Sound_Init***************
 Initializes sound port H
 Input: none
 Output: none
    */
void Sound_Init(void);




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





//Filename: SysTickInts.h
//Author: Duc Tran, Brandon Wong
//Initial Creation Date: September 18, 2013
//Description: SysTick interrupt handler for Time Manager Module and Alarm
//Lab Number: MW 2-3:30
//TA : Omar & Mahesh
//Date of last revision :September 27, 2013
//Hardware Configuration : NONE

// SysTickInts.h
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



// **************SysTick_Init*********************
// Initialize Systick periodic interrupts
// Input: interrupt period
//        Units of period are 20ns
//        Maximum is 2^24-1
//        Minimum is determined by length of ISR
// Output: none
void SysTick_Init(unsigned long period);

/****************SetTime***************
 Takes msec and updates time
 Input: msec
 Output: none
    */
void SetTime(unsigned int ms);

/****************SetAlarm***************
 Takes msec and Sets Alarm
 Input: msec
 Output: none
    */
void SetAlarm(unsigned int ms);

/****************AlarmOn***************
 Toggles PH0 and PH1 for alarm ringing
 Input: None
 Output: none
    */
void AlarmOn(void);
