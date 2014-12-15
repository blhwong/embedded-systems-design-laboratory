//Filename: LEDS_Driver.h
//Author: Duc Tran, Brandon Wong
//Initial Creation Date: November 4, 2013
//Description: Pattern Generator of LED Cube
//Lab Number: W 2-3:30
//TA : Omar & Mahesh
//Date of last revision : December 6, 2013


/****************LED_Init***************
 Description: Initializes LED Port
 Input: none
 Output: none
   */
void LED_Init(void);

/****************LED_Tester***************
 Description: Turns on all LEDs
 Input: none
 Output: none
   */
void LED_Tester(void);

/****************Turn_On***************
 Description: Turns on LED
 Input: int LED number
 Output: none
   */
void Turn_On(int led);

/****************Turn_Off***************
 Description: Turns off all LEDs
 Input: none
 Output: none
   */
void Turn_Off(void);
