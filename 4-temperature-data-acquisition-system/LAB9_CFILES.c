//Filename: Calib.c
//Author: Duc Tran, Brandon Wong
//Initial Creation Date: November 10, 2013
//Description: 
//Lab Number: W 2-3:30
//TA : Omar & Mahesh
//Date of last revision : November 18, 2013
//Hardware Configuration : Lab 9.SCH

unsigned short const ADCdata[53]={0,8,6,21,37,53,69,85,102,119,136,				
     153,171,188,206,225,243,262,281,300,319,				
     339,359,379,400,421,442,463,484,506,528,				
     550,573,596,619,642,666,689,713,738,762,				
     787,812,837,863,889,915,941,967,994,1021,1023,1024};				


unsigned short const Tdata[53]={4000,4000,3960,3920,3880,3840,3800,3760,3720,3680,3640,		
     3600,3560,3520,3480,3440,3400,3360,3320,3280,3240,		
     3200,3160,3120,3080,3040,3000,2960,2920,2880,2840,		
     2800,2760,2720,2680,2640,2600,2560,2520,2480,2440,		
     2400,2360,2320,2280,2240,2200,2160,2120,2080,2040,2000,2000};		


unsigned short const Rdata[53]={442,442,451,459,468,477,486,495,505,515,525,						
     535,545,556,567,578,589,601,613,625,638,						
     650,663,677,690,704,718,733,748,763,779,						
     795,811,828,845,862,880,898,917,936,956,						
     976,997,1018,1039,1061,1084,1107,1131,1155,1180,1206,1206};
/****************Find_ADCindex***************
 Get Index of ADC value in ADC conversion array.
 Input: unsigned short raw ADC value
 Output: unsigned short index of ADC conversion array
   
 */ 
unsigned short Find_ADCindex(unsigned short ADC) 
{ 
    unsigned short index =0; 
    unsigned short min = 1024; 
    unsigned short rindex = 0; 
    for(index = 0; index < 53; index++) 
    { 
        if((ADCdata[index]<ADC)) 
        { 
            rindex = index; 
        } 
    } 
    return rindex; 
} 
/****************Interpolate***************
 Convert raw ADC value to a temperature representation.
 Input: Index of ADC value in conversion array ,unsigned short raw ADC value
 Output: unsigned short temperature value 
 */ 
unsigned short Interpolate(unsigned short index, unsigned short ADC) 
{ 
   // return(Tdata[index]); 
    if(ADCdata[index] == ADC) 
    { 
        return ((Tdata[index]+200)); 
    } 
    else if((ADCdata[index + 1] - ADC) < (ADCdata[index - 1] - ADC)) 
    { 
        //conversion 
        //linear interpolation 
        float T = 0.0; 
        float slope = 0.0; 
        //float b = 0.0; 
        slope = (Tdata[index + 1] - Tdata[index])/(ADCdata[index+1] - ADCdata[index]); 
        T = slope*(ADCdata[index] - ADC) + Tdata[index]; 
        return ((unsigned short)T+200); 
    } 
    else
    { 
        float T = 0.0; 
        float slope = 0.0; 
        //float b = 0.0; 
        slope = (Tdata[index - 1] - Tdata[index])/(ADCdata[index-1] - ADCdata[index]); 
        T = slope*(ADCdata[index] - ADC) + Tdata[index]; 
        return ((unsigned short)T+200); 
    } 

} 


//Filename: Fixed.c
//Author: Duc Tran, Brandon Wong
//Initial Creation Date: September 4, 2013
//Description: 
//Lab Number: W 2-3:30
//TA : Omar & Mahesh
//Date of last revision :September 8, 2013
//Hardware Configuration : NONE
#include <stdio.h>
#include "fixed.h"


/****************Fixed_uDecOut2s***************
 Description: converts fixed point number to ASCII string
 format unsigned 32-bit with resolution 0.01
 range 0 to 999.99
 Input: unsigned 32-bit integer part of fixed point number
         greater than 99999 means invalid fixed-point number
 Output: null-terminated string exactly 6 characters plus null
 Examples
 12345 to "123.45"  
 22100 to "221.00"
   102 to "  1.02" 
    31 to "  0.31" 
100000 to "***.**"    */ 
void Fixed_uDecOut2s(unsigned long n, char *string){
	int integer = 0;
	int decimal = 0;
	
	if (n >  99999){
    sprintf(string,"***.**");
	  return;		
  }
		
	integer = n/100;
	decimal = n%100;
	sprintf(string,"%3i.%.2i",integer,decimal);
  return;
}

 /****************Fixed_uDecOut2***************
 outputs the fixed-point value on the OLED
 format unsigned 32-bit with resolution 0.01
 range 0 to 999.99
 Input: unsigned 32-bit integer part of fixed point number
         greater than 99999 means invalid fixed-point number
 Output: none but prints null-terminated string to board
 Examples
 12345 to "123.45"  
 22100 to "221.00"
   102 to "  1.02" 
    31 to "  0.31" 
100000 to "***.**"    */ 
void Fixed_uDecOut2(unsigned long n){
	int integer = 0;
	int decimal = 0;
	
	if (n >  99999){
    printf("***.**");
	  return;		
  }
		
	integer = n/100;
	decimal = n%100;
	printf("%3i.%.2i",integer,decimal);
  return;
}

/****************Fixed_sDecOut3s***************
 converts fixed point number to ASCII string
 format signed 32-bit with resolution 0.001
 range -9.999 to +9.999
 Input: signed 32-bit integer part of fixed point number
 Output: null-terminated string exactly 6 characters plus null
 Examples
  2345 to " 2.345"  
 -8100 to "-8.100"
  -102 to "-0.102" 
    31 to " 0.031" 
   
 */ 
void Fixed_sDecOut3s(long n, char *string){
	int integer = 0;
	int decimal = 0;
	 
	//sprinf(answer,%d,n)
	if(n > 9999 || n <-9999){
		sprintf(string,"**.***");
		return;
	}
	integer = n/1000;
	decimal = n%1000;
	if(n<0){
		decimal = decimal * (-1);
		integer = integer * (-1);
		sprintf(string,"-%i.%.3i",integer,decimal);
	}
	else{
		sprintf(string,"%2i.%.3i",integer,decimal);
	}
	return;
}
/****************Fixed_sDecOut3***************
 converts fixed point number to OLED
 format signed 32-bit with resolution 0.001
 range -9.999 to +9.999
 Input: signed 32-bit integer part of fixed point number
 Output: none but prints null-terminated string to board
 OLED has exactly 6 characters
 Examples
  2345 to " 2.345"  
 -8100 to "-8.100"
  -102 to "-0.102" 
    31 to " 0.031" 
 */ 
void Fixed_sDecOut3(long n){
  int integer = 0;
	int decimal = 0;
	 
	//sprinf(answer,%d,n)
	if(n > 9999 || n < -9999){
		printf("**.***");
		return;
	}
	integer = n/1000;
	decimal = n%1000;
	if(n<0){
		decimal = decimal * (-1);
		integer = integer * (-1);
		printf("-%i.%.3i",integer,decimal);
	}
	else{
		printf("%2i.%.3i",integer,decimal);
	}
	return;
}
/**************Fixed_uBinOut8s***************
 Description:  Convert unsigned 32-bit integer to a fix point value, and output to char string
 Input: unsigned 32-bit integer part of fixed point number
 Output: null-terminated string
Parameter output string
     0     "  0.00"
     2     "  0.01"
    64     "  0.25"
   100     "  0.39"
   50      "  1.95"
   512     "  2.00"
  5000     " 19.53"
 30000     "117.19"
255997     "999.99"
256000     "***.**"
*/
void Fixed_uBinOut8s(unsigned long n,  char *string){
  //fixed point number = I * res
	unsigned long decimal = 0;
	unsigned long integer = 0;
	if(n >= 256000){
		sprintf(string,"***.**");
		return;
	}
	decimal = (((n%256)*100/256));
	integer = n/256;
	sprintf(string,"%3lu.%.2lu",integer,decimal);
	return;
}

/**************Fixed_uBinOut8***************
 unsigned 32-bit binary fixed-point with a resolution of 1/256. 
 The full-scale range is from 0 to 999.99. 
 If the integer part is larger than 256000, it signifies an error. 
 The Fixed_uBinOut8 function takes an unsigned 32-bit integer part 
 of the binary fixed-point number and outputs the fixed-point value on the OLED. 
 Input: unsigned 32-bit integer part of fixed point number
 Output: none but prints null-terminated string to board
Parameter OLED display
     0	  0.00
     2	  0.01
    64	  0.25
   100	  0.39
   500	  1.95
   512	  2.00
  5000	 19.53
 30000	117.19
255997	999.99
256000	***.**
*/
void Fixed_uBinOut8(unsigned long n){
	//fixed point number = I * res
	unsigned long decimal = 0;
	unsigned long integer = 0;
	if(n >= 256000){
		printf("***.**");
		return;
	}
	decimal = ((n%256)*100/256);
	integer = n/256;
	printf("%3lu.%.2lu",integer,decimal);
	return;
}
// ADCPrintResults.c
// Runs on LM3S811
// Use a setup similar to ADCT0ATrigger.c to gather ADC samples
// into a buffer.  When the buffer is full, print them to the
// UART separated by TABs.  The purpose of this is to verify
// that the measured data from the ADC matches what is expected
// for a known input.  If the input signal is a sine wave, then
// the measured data should look like a sine wave.
// Daniel Valvano
// October 12, 2011
// Last Edited By: Duc Tran, Brandon Wong 
// November 18,2013
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

// Sine wave from signal generator connected to ADC1
//
// This program periodically samples ADC channel 1 and stores the
// result to a buffer.  After the buffer is full, it stops
// triggering ADC conversions and outputs the results to the UART
// separated by TABs.

#include "hw_types.h"
#include "sysctl.h"
#include "UART.h"
#include "FIFO.h"
#include "Calib.h"
#include "Fixed.h"
#include "Output.h"
#include "rit128x96x4.h"

#define NVIC_EN0_INT17          0x00020000  // Interrupt 17 enable
#define NVIC_EN0_R              (*((volatile unsigned long *)0xE000E100))  // IRQ 0 to 31 Set Enable Register
#define NVIC_PRI4_R             (*((volatile unsigned long *)0xE000E410))  // IRQ 16 to 19 Priority Register
#define TIMER0_CFG_R            (*((volatile unsigned long *)0x40030000))
#define TIMER0_TAMR_R           (*((volatile unsigned long *)0x40030004))
#define TIMER0_CTL_R            (*((volatile unsigned long *)0x4003000C))
#define TIMER0_IMR_R            (*((volatile unsigned long *)0x40030018))
#define TIMER0_TAILR_R          (*((volatile unsigned long *)0x40030028))
#define TIMER0_TAPR_R           (*((volatile unsigned long *)0x40030038))
#define TIMER_CFG_16_BIT        0x00000004  // 16-bit timer configuration,
                                            // function is controlled by bits
                                            // 1:0 of GPTMTAMR and GPTMTBMR
#define TIMER_TAMR_TAMR_PERIOD  0x00000002  // Periodic Timer mode
#define TIMER_CTL_TAOTE         0x00000020  // GPTM TimerA Output Trigger
                                            // Enable
#define TIMER_CTL_TAEN          0x00000001  // GPTM TimerA Enable
#define TIMER_IMR_TATOIM        0x00000001  // GPTM TimerA Time-Out Interrupt
                                            // Mask
#define TIMER_TAILR_TAILRL_M    0x0000FFFF  // GPTM TimerA Interval Load
                                            // Register Low
#define ADC_ACTSS_R             (*((volatile unsigned long *)0x40038000))
#define ADC0_RIS_R              (*((volatile unsigned long *)0x40038004))
#define ADC0_IM_R               (*((volatile unsigned long *)0x40038008))
#define ADC0_ISC_R              (*((volatile unsigned long *)0x4003800C))
#define ADC0_EMUX_R             (*((volatile unsigned long *)0x40038014))
#define ADC0_SSPRI_R            (*((volatile unsigned long *)0x40038020))
#define ADC0_PSSI_R             (*((volatile unsigned long *)0x40038028))
#define ADC0_SSMUX3_R           (*((volatile unsigned long *)0x400380A0))
#define ADC0_SSCTL3_R           (*((volatile unsigned long *)0x400380A4))
#define ADC0_SSFIFO3_R          (*((volatile unsigned long *)0x400380A8))
#define ADC_ACTSS_ASEN3         0x00000008  // ADC SS3 Enable
#define ADC_RIS_INR3            0x00000008  // SS3 Raw Interrupt Status
#define ADC_IM_MASK3            0x00000008  // SS3 Interrupt Mask
#define ADC_ISC_IN3             0x00000008  // SS3 Interrupt Status and Clear
#define ADC_EMUX_EM3_M          0x0000F000  // SS3 Trigger Select mask
#define ADC_EMUX_EM3_TIMER      0x00005000  // Timer
#define ADC_SSPRI_SS3_4TH       0x00003000  // fourth priority
#define ADC_SSPRI_SS2_3RD       0x00000200  // third priority
#define ADC_SSPRI_SS1_2ND       0x00000010  // second priority
#define ADC_SSPRI_SS0_1ST       0x00000000  // first priority
#define ADC_PSSI_SS3            0x00000008  // SS3 Initiate
#define ADC_SSMUX3_MUX0_M       0x00000003  // 1st Sample Input Select mask
#define ADC_SSMUX3_MUX0_S       0           // 1st Sample Input Select lshift
#define ADC_SSCTL3_TS0          0x00000008  // 1st Sample Temp Sensor Select
#define ADC_SSCTL3_IE0          0x00000004  // 1st Sample Interrupt Enable
#define ADC_SSCTL3_END0         0x00000002  // 1st Sample is End of Sequence
#define ADC_SSCTL3_D0           0x00000001  // 1st Sample Diff Input Select
#define ADC_SSFIFO3_DATA_M      0x000003FF  // Conversion Result Data mask
#define GPIO_PORTG_DATA_R       (*((volatile unsigned long *)0x400263FC))
#define GPIO_PORTG_DIR_R        (*((volatile unsigned long *)0x40026400))
#define GPIO_PORTG_DEN_R        (*((volatile unsigned long *)0x4002651C))
#define SYSCTL_RCGC0_R          (*((volatile unsigned long *)0x400FE100))
#define SYSCTL_RCGC1_R          (*((volatile unsigned long *)0x400FE104))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
#define SYSCTL_RCGC0_ADC        0x00010000  // ADC0 Clock Gating Control
#define SYSCTL_RCGC0_ADCSPD_M   0x00000300  // ADC Sample Speed mask
#define SYSCTL_RCGC0_ADCSPD500K 0x00000200  // 500K samples/second
#define SYSCTL_RCGC1_TIMER0     0x00010000  // timer 0 Clock Gating Control
#define SYSCTL_RCGC2_GPIOG      0x00000040  // port G Clock Gating Control
#define MAXBUFFERSIZE           100          // maximum number of samples
#define SAMPLEFREQ              1000        // sampling frequency (min. 763 Hz)
#define CLOCKFREQ               50000000    // PLL clock frequency


#define ADC2CHSUCCESS 1 // represents success reading from FIFO
#define ADC2CHFAILED  0 // represents failed reading from FIFO

// FIFO macros
AddIndexFifo(Ch1,MAXBUFFERSIZE,unsigned short,ADC2CHSUCCESS,ADC2CHFAILED)
// creates Ch1Fifo_Init() Ch1Fifo_Get() and Ch1Fifo_Put()

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCrit1ical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

// There are many choices to make when using the ADC, and many
// different combinations of settings will all do basically the
// same thing.  For simplicity, this function makes some choices
// for you.  When calling this function, be sure that it does
// not conflict with any other software that may be running on
// the microcontroller.  Particularly, ADC sample sequencer 3
// is used here because it only takes one sample, and only one
// sample is absolutely needed.  Sample sequencer 3 generates a
// raw interrupt when the conversion is complete, and it is then
// promoted to an ADC controller interrupt.  Hardware Timer0A
// triggers the ADC conversion at the programmed interval, and
// software handles the interrupt to process the measurement
// when it is complete.
//
// A simpler approach would be to use software to trigger the
// ADC conversion, wait for it to complete, and then process the
// measurement.
//
// This initialization function sets up the ADC according to the
// following parameters.  Any parameters not explicitly listed
// below are not modified:
// Timer0A: enabled
// Mode: 16-bit, down counting
// One-shot or periodic: periodic
// Prescale value: programmable using variable 'prescale' [0:255]
// Interval value: programmable using variable 'period' [0:65535]
// Sample time is busPeriod*(prescale+1)*(period+1)
// Max sample rate: <=500,000 samples/second
// Sequencer 0 priority: 1st (highest)
// Sequencer 1 priority: 2nd
// Sequencer 2 priority: 3rd
// Sequencer 3 priority: 4th (lowest)
// SS3 triggering event: Timer0A
// SS3 1st sample source: programmable using variable 'channelNum' [0:3]
// SS3 interrupts: enabled and promoted to controller
void ADC_InitTimer0ATriggerSeq3(unsigned char channelNum, unsigned char prescale, unsigned short period){
  volatile unsigned long delay;
  // channelNum must be 0-3 (inclusive) corresponding to ADC0 through ADC3
  if(channelNum > 3){
    return;                                 // invalid input, do nothing
  }
  DisableInterrupts();
  // **** general initialization ****
  SYSCTL_RCGC0_R |= SYSCTL_RCGC0_ADC;       // activate ADC
  SYSCTL_RCGC0_R &= ~SYSCTL_RCGC0_ADCSPD_M; // clear ADC sample speed field
  SYSCTL_RCGC0_R += SYSCTL_RCGC0_ADCSPD500K;// configure for 500K ADC max sample rate
  SYSCTL_RCGC1_R |= SYSCTL_RCGC1_TIMER0;    // activate timer0
  delay = SYSCTL_RCGC1_R;                   // allow time to finish activating
  TIMER0_CTL_R &= ~TIMER_CTL_TAEN;          // disable timer0A during setup
  TIMER0_CTL_R |= TIMER_CTL_TAOTE;          // enable timer0A trigger to ADC
  TIMER0_CFG_R = TIMER_CFG_16_BIT;          // configure for 16-bit timer mode
  // **** timer0A initialization ****
  TIMER0_TAMR_R = TIMER_TAMR_TAMR_PERIOD;   // configure for periodic mode
  TIMER0_TAPR_R = prescale;                 // prescale value for trigger
  TIMER0_TAILR_R = period;                  // start value for trigger
  TIMER0_IMR_R &= ~TIMER_IMR_TATOIM;        // disable timeout (rollover) interrupt
  TIMER0_CTL_R |= TIMER_CTL_TAEN;           // enable timer0A 16-b, periodic, no interrupts
  // **** ADC initialization ****
                                            // sequencer 0 is highest priority (default setting)
                                            // sequencer 1 is second-highest priority (default setting)
                                            // sequencer 2 is third-highest priority (default setting)
                                            // sequencer 3 is lowest priority (default setting)
  ADC0_SSPRI_R = (ADC_SSPRI_SS0_1ST|ADC_SSPRI_SS1_2ND|ADC_SSPRI_SS2_3RD|ADC_SSPRI_SS3_4TH);
  ADC_ACTSS_R &= ~ADC_ACTSS_ASEN3;          // disable sample sequencer 3
  ADC0_EMUX_R &= ~ADC_EMUX_EM3_M;           // clear SS3 trigger select field
  ADC0_EMUX_R += ADC_EMUX_EM3_TIMER;        // configure for timer trigger event
  ADC0_SSMUX3_R &= ~ADC_SSMUX3_MUX0_M;      // clear SS3 1st sample input select field
                                            // configure for 'channelNum' as first sample input
  ADC0_SSMUX3_R += (channelNum<<ADC_SSMUX3_MUX0_S);
  ADC0_SSCTL3_R = (0                        // settings for 1st sample:
                   & ~ADC_SSCTL3_TS0        // read pin specified by ADC0_SSMUX3_R (default setting)
                   | ADC_SSCTL3_IE0         // raw interrupt asserted here
                   | ADC_SSCTL3_END0        // sample is end of sequence (default setting, hardwired)
                   & ~ADC_SSCTL3_D0);       // differential mode not used (default setting)
  ADC0_IM_R |= ADC_IM_MASK3;                // enable SS3 interrupts
  ADC_ACTSS_R |= ADC_ACTSS_ASEN3;           // enable sample sequencer 3
  // **** interrupt initialization ****
                                            // ADC3=priority 2
  NVIC_PRI4_R = (NVIC_PRI4_R&0xFFFF00FF)|0x00004000; // bits 13-15
  NVIC_EN0_R |= NVIC_EN0_INT17;             // enable interrupt 17 in NVIC
  EnableInterrupts();
}
volatile unsigned short indexBuff = 0;
volatile unsigned long ADCbuffer[MAXBUFFERSIZE];
void ADC3_Handler(void){
  ADC0_ISC_R = ADC_ISC_IN3;             // acknowledge ADC sequence 3 completion
  GPIO_PORTG_DATA_R ^= 0x04;            // toggle LED
  Ch1Fifo_Put(ADC0_SSFIFO3_R&ADC_SSFIFO3_DATA_M);
//  ADCbuffer[index] = ADC0_SSFIFO3_R&ADC_SSFIFO3_DATA_M;
  indexBuff = indexBuff + 1;
  if(indexBuff == MAXBUFFERSIZE){
    ADC_ACTSS_R &= ~ADC_ACTSS_ASEN3;    // disable sample sequencer 3
    TIMER0_CTL_R &= ~TIMER_CTL_TAEN;    // disable timer0A
  }
}

int main(void){
  unsigned short i,j;
	int test = 1;
	unsigned short ADCValue,average;
	unsigned short TempValue;
	char TempString;
	char Welcome ;
	sprintf(&Welcome,"Current Temperature");
  //
  // Set the system clock to run at 50 MHz from the main oscillator.
  //
  SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL |
                 SYSCTL_XTAL_8MHZ | SYSCTL_OSC_MAIN);
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOG; // activate port G
	Output_Init();
	Output_Color(15);
  UART_Init();   // initialize the UART as a console for text I/O
  GPIO_PORTG_DIR_R |= 0x04;             // make PG2 out (PG2 built-in LED)
  GPIO_PORTG_DEN_R |= 0x04;             // enable digital I/O on PG2
  GPIO_PORTG_DATA_R &= ~0x04;           // turn off LED
  UART_OutString("\r\n\nADC Test Program");
	
  while(1){
    UART_OutString("\r\n\nSample frequency (Hz): ");
    UART_OutUDec(SAMPLEFREQ);
    UART_OutString("\r\nNumber of samples: ");
    UART_OutUDec(MAXBUFFERSIZE);
    UART_OutString("\r\nPress ENTER to begin sampling.\r\n");
    while(UART_InChar() != 13){};       // wait for ENTER key
    indexBuff = 0;                          // reset counter
                                        // ADC channel 1, SAMPLEFREQ Hz sampling
    ADC_InitTimer0ATriggerSeq3(0, 0, (CLOCKFREQ/SAMPLEFREQ)-1);
			for(i=1;i<20;i=i+1){
				UART_OutChar(13);
				UART_OutChar(10); 
				average = 0;
				Ch1Fifo_Get(&ADCValue);
				UART_OutUDec(ADCValue);
				TempValue = Interpolate(Find_ADCindex(ADCValue),ADCValue);
				Fixed_uDecOut2s(TempValue,&TempString);
				RIT128x96x4StringDraw(&Welcome,10,20,12);
				RIT128x96x4StringDraw(&TempString,20,60,12);
				UART_OutString(&TempString);			
			}
			
  }
}




// UART.c
// Runs on LM3S811, LM3S1968, LM3S8962
// Simple device driver for the UART.
// Daniel Valvano
// July 26, 2011
// Modified by EE345L students Charlie Gough && Matt Hawk
// Modified by EE345M students Agustinus Darmawan && Mingjie Qiu

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to the Arm Cortex M3",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2011
   Program 4.12, Section 4.9.4, Figures 4.26 and 4.40

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

// U0Rx (VCP receive) connected to PA0
// U0Tx (VCP transmit) connected to PA1

#include "UART.h"

#define GPIO_PORTA_AFSEL_R      (*((volatile unsigned long *)0x40004420))
#define GPIO_PORTA_DEN_R        (*((volatile unsigned long *)0x4000451C))
#define UART0_DR_R              (*((volatile unsigned long *)0x4000C000))
#define UART0_FR_R              (*((volatile unsigned long *)0x4000C018))
#define UART0_IBRD_R            (*((volatile unsigned long *)0x4000C024))
#define UART0_FBRD_R            (*((volatile unsigned long *)0x4000C028))
#define UART0_LCRH_R            (*((volatile unsigned long *)0x4000C02C))
#define UART0_CTL_R             (*((volatile unsigned long *)0x4000C030))
#define UART_FR_TXFF            0x00000020  // UART Transmit FIFO Full
#define UART_FR_RXFE            0x00000010  // UART Receive FIFO Empty
#define UART_LCRH_WLEN_8        0x00000060  // 8 bit word length
#define UART_LCRH_FEN           0x00000010  // UART Enable FIFOs
#define UART_CTL_UARTEN         0x00000001  // UART Enable
#define SYSCTL_RCGC1_R          (*((volatile unsigned long *)0x400FE104))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
#define SYSCTL_RCGC1_UART0      0x00000001  // UART0 Clock Gating Control
#define SYSCTL_RCGC2_GPIOA      0x00000001  // port A Clock Gating Control

//------------UART_Init------------
// Wait for new serial port input
// Initialize the UART for 115,200 baud rate (assuming 50 MHz UART clock),
// 8 bit word length, no parity bits, one stop bit, FIFOs enabled
// Input: none
// Output: none
void UART_Init(void){
  SYSCTL_RCGC1_R |= SYSCTL_RCGC1_UART0; // activate UART0
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOA; // activate port A
  UART0_CTL_R &= ~UART_CTL_UARTEN;      // disable UART
  UART0_IBRD_R = 27;                    // IBRD = int(50,000,000 / (16 * 115,200)) = int(27.1267)
  UART0_FBRD_R = 8;                     // FBRD = int(0.1267 * 64 + 0.5) = 8
                                        // 8 bit word length (no parity bits, one stop bit, FIFOs)
  UART0_LCRH_R = (UART_LCRH_WLEN_8|UART_LCRH_FEN);
  UART0_CTL_R |= UART_CTL_UARTEN;       // enable UART
  GPIO_PORTA_AFSEL_R |= 0x03;           // enable alt funct on PA1-0
  GPIO_PORTA_DEN_R |= 0x03;             // enable digital I/O on PA1-0
}

//------------UART_InChar------------
// Wait for new serial port input
// Input: none
// Output: ASCII code for key typed
unsigned char UART_InChar(void){
  while((UART0_FR_R&UART_FR_RXFE) != 0);
  return((unsigned char)(UART0_DR_R&0xFF));
}
//------------UART_OutChar------------
// Output 8-bit to serial port
// Input: letter is an 8-bit ASCII character to be transferred
// Output: none
void UART_OutChar(unsigned char data){
  while((UART0_FR_R&UART_FR_TXFF) != 0);
  UART0_DR_R = data;
}


//------------UART_OutString------------
// Output String (NULL termination)
// Input: pointer to a NULL-terminated string to be transferred
// Output: none
void UART_OutString(char *pt){
  while(*pt){
    UART_OutChar(*pt);
    pt++;
  }
}

//------------UART_InUDec------------
// InUDec accepts ASCII input in unsigned decimal format
//     and converts to a 32-bit unsigned number
//     valid range is 0 to 4294967295 (2^32-1)
// Input: none
// Output: 32-bit unsigned number
// If you enter a number above 4294967295, it will return an incorrect value
// Backspace will remove last digit typed
unsigned long UART_InUDec(void){
unsigned long number=0, length=0;
char character;
  character = UART_InChar();
  while(character != CR){ // accepts until <enter> is typed
// The next line checks that the input is a digit, 0-9.
// If the character is not 0-9, it is ignored and not echoed
    if((character>='0') && (character<='9')) {
      number = 10*number+(character-'0');   // this line overflows if above 4294967295
      length++;
      UART_OutChar(character);
    }
// If the input is a backspace, then the return number is
// changed and a backspace is outputted to the screen
    else if((character==BS) && length){
      number /= 10;
      length--;
      UART_OutChar(character);
    }
    character = UART_InChar();
  }
  return number;
}

//-----------------------UART_OutUDec-----------------------
// Output a 32-bit number in unsigned decimal format
// Input: 32-bit number to be transferred
// Output: none
// Variable format 1-10 digits with no space before or after
void UART_OutUDec(unsigned long n){
// This function uses recursion to convert decimal number
//   of unspecified length as an ASCII string
  if(n >= 10){
    UART_OutUDec(n/10);
    n = n%10;
  }
  UART_OutChar(n+'0'); /* n is between 0 and 9 */
}

//---------------------UART_InUHex----------------------------------------
// Accepts ASCII input in unsigned hexadecimal (base 16) format
// Input: none
// Output: 32-bit unsigned number
// No '$' or '0x' need be entered, just the 1 to 8 hex digits
// It will convert lower case a-f to uppercase A-F
//     and converts to a 16 bit unsigned number
//     value range is 0 to FFFFFFFF
// If you enter a number above FFFFFFFF, it will return an incorrect value
// Backspace will remove last digit typed
unsigned long UART_InUHex(void){
unsigned long number=0, digit, length=0;
char character;
  character = UART_InChar();
  while(character != CR){
    digit = 0x10; // assume bad
    if((character>='0') && (character<='9')){
      digit = character-'0';
    }
    else if((character>='A') && (character<='F')){
      digit = (character-'A')+0xA;
    }
    else if((character>='a') && (character<='f')){
      digit = (character-'a')+0xA;
    }
// If the character is not 0-9 or A-F, it is ignored and not echoed
    if(digit <= 0xF){
      number = number*0x10+digit;
      length++;
      UART_OutChar(character);
    }
// Backspace outputted and return value changed if a backspace is inputted
    else if((character==BS) && length){
      number /= 0x10;
      length--;
      UART_OutChar(character);
    }
    character = UART_InChar();
  }
  return number;
}

//--------------------------UART_OutUHex----------------------------
// Output a 32-bit number in unsigned hexadecimal format
// Input: 32-bit number to be transferred
// Output: none
// Variable format 1 to 8 digits with no space before or after
void UART_OutUHex(unsigned long number){
// This function uses recursion to convert the number of
//   unspecified length as an ASCII string
  if(number >= 0x10){
    UART_OutUHex(number/0x10);
    UART_OutUHex(number%0x10);
  }
  else{
    if(number < 0xA){
      UART_OutChar(number+'0');
     }
    else{
      UART_OutChar((number-0x0A)+'A');
    }
  }
}

//------------UART_InString------------
// Accepts ASCII characters from the serial port
//    and adds them to a string until <enter> is typed
//    or until max length of the string is reached.
// It echoes each character as it is inputted.
// If a backspace is inputted, the string is modified
//    and the backspace is echoed
// terminates the string with a null character
// uses busy-waiting synchronization on RDRF
// Input: pointer to empty buffer, size of buffer
// Output: Null terminated string
// -- Modified by Agustinus Darmawan + Mingjie Qiu --
void UART_InString(char *bufPt, unsigned short max) {
int length=0;
char character;
  character = UART_InChar();
  while(character != CR){
    if(character == BS){
      if(length){
        bufPt--;
        length--;
        UART_OutChar(BS);
      }
    }
    else if(length < max){
      *bufPt = character;
      bufPt++;
      length++;
      UART_OutChar(character);
    }
    character = UART_InChar();
  }
  *bufPt = 0;
}
