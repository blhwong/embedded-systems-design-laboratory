//Filename: Calib.h
//Author: Duc Tran, Brandon Wong
//Initial Creation Date: November 10, 2013
//Description: 
//Lab Number: W 2-3:30
//TA : Omar & Mahesh
//Date of last revision : November 18, 2013
//Hardware Configuration : Lab 9.SCH


/****************Find_ADCindex***************
 Get Index of ADC value in ADC conversion array.
 Input: unsigned short raw ADC value
 Output: unsigned short index of ADC conversion array
   
 */ 
unsigned short Find_ADCindex(unsigned short ADC); 


/****************Interpolate***************
 Convert raw ADC value to a temperature representation.
 Input: Index of ADC value in conversion array ,unsigned short raw ADC value
 Output: unsigned short temperature value 
 */ 
unsigned short Interpolate(unsigned short index, unsigned short ADC);