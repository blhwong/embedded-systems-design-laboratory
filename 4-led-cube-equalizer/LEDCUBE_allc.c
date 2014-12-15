//Filename: GPIO.c
//Author: Duc Tran, Brandon Wong
//Initial Creation Date: November 4, 2013
//Description: General Port Input/Output that controls LED
//Lab Number: W 2-3:30
//TA : Omar & Mahesh
//Date of last revision : December 6, 2013
//Hardware Configuration : NONE

// GPIO.c
// Runs on LM3S811
// Initialize four GPIO pins as outputs.  Continually generate output to
// drive simulated stepper motor.
// Daniel Valvano
// July 11, 2011

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to the Arm Cortex M3",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2011
   Example 2.2, Program 2.8, Figure 2.27

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

#include "LEDS_Driver.h"
#include "Pattern_Generator.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "Systick.h"
#include <stdlib.h>
#include <stdio.h>
// PD3 is an output to LED3, negative logic
// PD2 is an output to LED2, negative logic
// PD1 is an output to LED1, negative logic
// PD0 is an outpSut to LED0, negative logic

#define GPIO_PORTD_DATA_R       (*((volatile unsigned long *)0x400073FC))
#define GPIO_PORTD_DIR_R        (*((volatile unsigned long *)0x40007400))
#define GPIO_PORTD_AFSEL_R      (*((volatile unsigned long *)0x40007420))
#define GPIO_PORTD_DEN_R        (*((volatile unsigned long *)0x4000751C))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
#define SYSCTL_RCGC2_GPIOD      0x00000008  // port D Clock Gating Control
#define LEDS                    (*((volatile unsigned long *)0x4000703C))

//--------------------------------------------------------------------------------------------------------------//

// Sine wave from signal generator connected to ADC1
//
// This program periodically samples ADC channel 1 and stores the
// result to a buffer.  After the buffer is full, it stops
// triggering ADC conversions and outputs the results to the UART
// separated by TABs.



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
#define GPIO_PORTA_AFSEL_R      (*((volatile unsigned long *)0x40004420))
#define GPIO_PORTC_DATA_R       (*((volatile unsigned long *)0x400063FC))
#define GPIO_PORTC_DIR_R        (*((volatile unsigned long *)0x40006400))
#define GPIO_PORTC_DEN_R        (*((volatile unsigned long *)0x4000651C))
#define SYSCTL_RCGC0_R          (*((volatile unsigned long *)0x400FE100))
#define SYSCTL_RCGC1_R          (*((volatile unsigned long *)0x400FE104))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
#define SYSCTL_RCGC0_ADC        0x00010000  // ADC0 Clock Gating Control
#define SYSCTL_RCGC0_ADCSPD_M   0x00000300  // ADC Sample Speed mask
#define SYSCTL_RCGC0_ADCSPD500K 0x00000200  // 500K samples/second
#define SYSCTL_RCGC1_TIMER0     0x00010000  // timer 0 Clock Gating Control
#define SYSCTL_RCGC2_GPIOC      0x00000004  // port C Clock Gating Control
#define SYSCTL_RCGC2_GPIOA      0x00000001  // port A Clock Gating Control
#define MAXBUFFERSIZE           50          // maximum number of samples
#define SAMPLEFREQ              1000        // sampling frequency (min. 92 Hz)
#define CLOCKFREQ               6000000     // default clock frequency


void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
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

/****************ADC_InitTimer0ATriggerSeq3***************
 Description: Initializes ADC Interrupt
 range 0 to 999.99
 Input: unsigned char channelNum, unsigned char prescale, unsigned short period
 Output: none
   */ 
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
volatile unsigned short index = 0;
volatile unsigned long ADCbuffer[MAXBUFFERSIZE];

/****************ADC3_Handler***************
 Description: ADC Handler
 Input: none
 Output: none
   */
void ADC3_Handler(void){
  ADC0_ISC_R = ADC_ISC_IN3;             // acknowledge ADC sequence 3 completion
  ADCbuffer[index] = ADC0_SSFIFO3_R&ADC_SSFIFO3_DATA_M;
  index = index + 1;
  if(index == MAXBUFFERSIZE){
    ADC_ACTSS_R &= ~ADC_ACTSS_ASEN3;    // disable sample sequencer 3
    TIMER0_CTL_R &= ~TIMER_CTL_TAEN;    // disable timer0A
  }
}






//--------------------------------------------------------------------------------------------------------------//
// access PD3-PD0
// delay function for testing from sysctl.c
// which delays 3*ulCount cycles
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

/****************Find_ADCindex***************
 Description: Finds Index of ADC value
 Input: unsigned short ADC
 Output: index of ADC
   */
int Find_ADCindex(unsigned short ADC) 
{ 
    int index1 =0; 
    int min = 1024; 
    int rindex = 0; 
    for(index1 = 0; index1 < 53; index1++) 
    { 
        if((ADCdata[index1]<ADC)) 
        { 
            rindex = index1; 
        } 
    } 
		
    return rindex; 
} 


int main(void){  volatile unsigned long delay;
	int i,j,state,intensity,counter;
	SysTick_Init();
	LED_Init();
	counter = 0;
	state = 1;
	Alphabet('T');
	Alphabet('E');
    Alphabet('X');
	Alphabet('A');
	Alphabet('S');
	for (i =0;i < 25;i++){
		SpinUT();
	}
	while(1){
		index = 0;
		ADC_InitTimer0ATriggerSeq3(0, 0, CLOCKFREQ/SAMPLEFREQ);
		while(index < MAXBUFFERSIZE){};
		for(i=1; i<MAXBUFFERSIZE; i=i+1){
			Turn_Off();
			if(ADCbuffer[i] > 8){
				intensity = Find_ADCindex(ADCbuffer[i]);
				switch (state){
					case 0://welcome
						Animation0(Tdata[intensity]-1000);
						if(counter > 200){
							state = 1;
							counter = 0;
						}else{counter = counter +1;}
					break;
					case 1:
						Animation1(intensity);
						state = 1;
						if(counter > 200){
							state = 2;
							counter = 0;
						}else{counter = counter +1;}
					break;
					case 2:
						Animation4(intensity);
						if(counter > 200){
							state = 3;
							counter = 0;
						}else{counter = counter +1;}
					break;
					case 3:
						Animation3(intensity);
						state = 3;
						if(counter > 200){
							state = 4;
							counter = 0;
						}else{counter = counter +1;}
					break;
					case 4:
						RainDrop(Tdata[intensity]);
						if(counter > 200){
							state = 5;
							counter = 0;
						}else{counter = counter +1;}
						break;
						case 5:
							Animation2(intensity);
							if(counter > 200){
							state = 6;
							counter = 0;
						}else{counter = counter +1;}
						break;
						case 6:
							Animation5(intensity);
							if(counter > 200){
							state = 0;
							counter = 0;
						}else{counter = counter +1;}
							break;
				 default:
						Random();
						break;
				}
			}
    }
	} 
}

//Filename: LEDS_Driver.c
//Author: Duc Tran, Brandon Wong
//Initial Creation Date: November 4, 2013
//Description: 
//Lab Number: W 2-3:30
//TA : Omar & Mahesh
//Date of last revision : December 6, 2013
//Hardware Configuration : UTX-2013S304.sch



#define GPIO_PORTD_DATA_R       (*((volatile unsigned long *)0x400073FC))
#define GPIO_PORTD_DIR_R        (*((volatile unsigned long *)0x40007400))
#define GPIO_PORTD_AFSEL_R      (*((volatile unsigned long *)0x40007420))
#define GPIO_PORTD_DEN_R        (*((volatile unsigned long *)0x4000751C))
#define SYSCTL_RCGC2_GPIOD      0x00000008  // port D Clock Gating Control


//USED PORT A TO TEST IN 1968

#define GPIO_PORTA_DATA_R       (*((volatile unsigned long *)0x400043FC))
#define GPIO_PORTA_DIR_R        (*((volatile unsigned long *)0x40004400))
#define GPIO_PORTA_AFSEL_R      (*((volatile unsigned long *)0x40004420))
#define GPIO_PORTA_DEN_R        (*((volatile unsigned long *)0x4000451C))
#define SYSCTL_RCGC2_GPIOA      0x00000001  // port D Clock Gating Control

#define GPIO_PORTB_DATA_R       (*((volatile unsigned long *)0x400053FC))
#define GPIO_PORTB_DIR_R        (*((volatile unsigned long *)0x40005400))
#define GPIO_PORTB_AFSEL_R      (*((volatile unsigned long *)0x40005420))
#define GPIO_PORTB_DEN_R        (*((volatile unsigned long *)0x4000551C))
#define SYSCTL_RCGC2_GPIOB      0x00000002  // port B Clock Gating Control
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
#define GPIO_PORTB_DR2R_R       (*((volatile unsigned long *)0x40005500))
#define GPIO_PORTB_DR4R_R       (*((volatile unsigned long *)0x40005504))
#define GPIO_PORTB_DR8R_R       (*((volatile unsigned long *)0x40005508))
struct LED{

	unsigned char decoder;
  unsigned char column;           // Output
	unsigned char output;
};
typedef const struct LED LEDType;

LEDType CUBE[25] = {
	{0,0,0x00},
	{0,1,0x01},
	{0,2,0x02},
	{0,3,0x03},
	{0,4,0x04},
	{0,5,0x05},
	{0,6,0x06},
	{0,7,0x07},

	{1,8,0x08},
	{1,9,0x09},
	{1,10,0x0A},
	{1,11,0x0B},
	{1,12,0x0C},
	{1,13,0x0D},
	{1,14,0x0E},
	{1,15,0x0F},
		
	{2,16,0x10},
	{2,17,0x11},
	{2,18,0x12},
	{2,19,0x13},
	{2,20,0x14},
	{2,21,0x15},
	{2,22,0x16},
	{2,23,0x17},
	
	{3,24,0x18}
};

/****************LED_Init***************
 Description: Initializes LED Port
 Input: none
 Output: none
   */
void LED_Init(void){
	volatile unsigned long delay;
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOD + SYSCTL_RCGC2_GPIOB; // activate port D
  delay = SYSCTL_RCGC2_R;  
  GPIO_PORTD_DIR_R |= 0xfF;    // make PD5-0 out
  GPIO_PORTD_AFSEL_R &= ~0xfF; // regular port function 
  GPIO_PORTD_DEN_R |= 0xfF;    // enable digital I/O on PD5-0
	
	GPIO_PORTB_DIR_R |= 0x1F;    // make PB4-0 out
  GPIO_PORTB_AFSEL_R &= ~0x1F; // regular port function 
  GPIO_PORTB_DEN_R |= 0x1F;    // enable digital I/O on PB4-0
	GPIO_PORTB_DR2R_R = 0x00;
	GPIO_PORTB_DR8R_R = 0xFF;

}


/****************LED_Tester***************
 Description: Turns on all LEDs
 Input: none
 Output: none
   */
void LED_Tester(void){
	int j;
	unsigned char i;
	while(1){
		for (i = 0; i < 8; i ++){
			GPIO_PORTD_DATA_R = 0x00;
			for ( j = 0; j <1000000; j++){}
		}
	}
}

/****************Turn_On***************
 Description: Turns on LED
 Input: int LED number
 Output: none
   */
void Turn_On(int led){
	int index;
	if(led < 25){ 			//layer 1
		index = led;
		GPIO_PORTB_DATA_R = 0x01;
		GPIO_PORTD_DATA_R =0x20 + CUBE[index].output;
	}
	else if(led < 50){	//layer 2
		index =  led -25;
		GPIO_PORTB_DATA_R = 0x02;
		GPIO_PORTD_DATA_R = 0x20 + CUBE[index].output;
	}
	else if(led <75){		//layer 3
		index = led -50;
		GPIO_PORTB_DATA_R = 0x04;
		GPIO_PORTD_DATA_R = 0x20 + CUBE[index].output;
	}
	else if(led < 100){	//layer 4
		index = led - 75;
	  GPIO_PORTB_DATA_R = 0x08;
		GPIO_PORTD_DATA_R = 0x20 + CUBE[index].output;
		
	}
	else if(led <125){	//layer 5
		index = led -100;
	  GPIO_PORTB_DATA_R = 0x10;
		GPIO_PORTD_DATA_R = 0x20 + CUBE[index].output;
	}

}

/****************Turn_Off***************
 Description: Turns off all LEDs
 Input: none
 Output: none
   */
void Turn_Off(void){
	GPIO_PORTB_DATA_R = 0x00;
}


//Filename: Pattern_Generator.c
//Author: Duc Tran, Brandon Wong
//Initial Creation Date: November 4, 2013
//Description: Pattern Generator of LED Cube
//Lab Number: W 2-3:30
//TA : Omar & Mahesh
//Date of last revision : December 6, 2013

#include "LEDS_Driver.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "Systick.h"
/*
layer1					
	4	9	14	19	24
	3	8	13	18	23
	2	7	12	17	22
	1	6	11	16	21
	0	5	10	15	20
layer2					
	29	34	39	44	49
	28	33	38	43	48
	27	32	37	42	47
	26	31	36	41	46
	25	30	35	40	45
layer 3					
	54	59	64	69	74
	53	58	63	68	73
	52	57	62	67	72
	51	56	61	66	71
	50	55	60	65	70
layer 4					
	79	84	89	94	99
	78	83	88	93	98
	77	82	87	92	97
	76	81	86	91	96
	75	80	85	90	95
layer 5					
	104	109	114	119	124
	103	108	113	118	123
	102	107	112	117	122
	101	106	111	116	121
	100	105	110	115	120

*/

//The following are arrays for pattern

int Fire_Work[12] ={53,57,73,73,60,64,83,93,81,86,107,117};
int diagonal_1[6] = {100,80,60,40,20,126};
int diagonal_5[6] = {124,94,64,34,4,126};
int U_1[11] = {5,10,15,30,40,55,65,80,90,105,115};
int T_1[10] = {10,35,60,85,110,100,105,115,120};
int E_1[17] = {0,5,10,15,20,25,50,55,60,65,70,75,100,105,110,115,120};
int X_1[9] = {0,20,30,40,60,80,90,100,120};
int A_1[10] = {5,15,30,40,55,60,65,80,90,110};
int S_1[17] = {0,5,10,15,20,45,50,55,60,65,70,75,100,105,110,115,120};
int B_1[16] = {0,5,10,15,20,25,45,50,65,75,95,100,105,110,115,120};
int C_1[13] = {0,5,10,15,20,25,50,75,100,105,110,115,120};
int UT[14] =  {10,30,35,40,55,60,65,75,80,85,90,95,105,115};
int center_1[10] = {8	,13,	18,	
									7,	12,	17,
									6,	11,	16,126};
int VSquare[25]={0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100,105,110,115,120};
int Box[16] = {0,5,10,15,20,4,9,14,19,24,3,2,1,23,22,21};
int cube_1[8] = {0,1,6,5,25,26,30,31};
int cube_2[8] = {15,16,21,20,41,40,45,46};
int cube_3[8] = {18,19,23,24,43,44,48,49};
int cube_4[8] = {3,4,8,9,28,29,33,34};
int CubeHist[3] = {0,0,0};
int Vlayer1_1[5] = {0,5,10,15,20};
int cornerArray[25]= {24,
23,
22,
21,
20,

49,
48,
47,
46,
45,

74,
73,
72,
71,
70,

99,
98,
97,
96,
95,

124,
123,
122,
121,
120
};
int Animation1Hist1[25];
int Animation1Hist2[25];
int Animation1Hist3[25];
int Animation1Hist0[25];
int Animation1Array[25];
int Animation2Counter;
int Index;
int arrowArray[14]={10,25,30,35,40,60,65,70,75,80,85,90,110,126};
int squareOutline[18] = {
	4,	9,	14,	19,	24,
	3,							23,
	2,							22,
	1,       				21,
	0,	5,	10,	15,	20};
int rainArray[25]={
		104,	109,	114,	119,	124,
	103,	108,	113,	118,	123,
	102,	107,	112,	117,	122,
	101,	106,	111,	116,	121,
	100,	105,	110,	115,	120};
int rainDropBot[25]={126};
int rainIndex = 0;

/****************addToArrayUniversal***************
 Description: destination, index, LED
 Input: none
 Output: none
   */
void addToArrayUniversal(int destination[],int index, int led){
	destination[index] = led;
}

/****************addToArray***************
 Description: adds to array
 Input: LED
 Output: none
   */
void addToArray(int led){
	Animation1Array[Index] = led;
	Index = Index + 1;
	if(Index == 25){
		Index = 0;
	}
}

/****************moveHorizontal***************
 Description: moves LED horizontally
 Input: LED array, size	of array
 Output: none
   */
void moveHorizontal(int LEDarray[],int size){
	int i,j;
	int move;
	int led;
	move = 5;
	for(j = 0;j <size;j++){
		if(LEDarray[j]  < 25){ 			//layer 1
			led = LEDarray[j];
		}
		else if(LEDarray[j]  < 50){	//layer 2
			led = LEDarray[j]-25;
		}
		else if(LEDarray[j]  <75){		//layer 3
			led = LEDarray[j]-50;
		}
		else if(LEDarray[j]  < 100){	//layer 4
			led = LEDarray[j]-75;
		}
		else if(LEDarray[j]  <125){	//layer 5
			led = LEDarray[j]-100;
		}
		
		if (led ==0 || led == 5 || led == 10 || led == 15){
			move = 5;
		}
		else if (led ==20 || led == 21 || led == 22 || led == 23){
			move = 1;
		}
		else if (led ==24 || led == 19 || led == 14 || led == 9){
			move = -5;
		}
		else if (led ==4|| led == 3 || led == 2|| led == 1){
			move =-1;
		}
		
		LEDarray[j] = LEDarray[j] + move;

	}
}

/****************SpinUT***************
 Description: UT symbol animation
 Input: none
 Output: none
   */
void SpinUT(void){
	int i,j,duration,layer;
	i=0;
	layer =0;

	for(duration = 0; duration <10;duration++){
		while(UT[i]!= 126){
			Turn_On(UT[i]);
			for (j = 1 ;j <1000; j++){}
			i++;
		}
			i=0;
	}
		moveHorizontal(UT,14);
}

/****************SpinUT***************
 Description: UT symbol animation
 Input: none
 Output: none
   */
void Cube(int cubeArray[8],int layer){
	int i,j;
		for (i = 0; i < 8;i++){
			Turn_On(cubeArray[i]+(layer*25));
			for (j = 1 ;j <2000; j++){}
			}
}

/****************makeArray***************
 Description: makes array
 Input: destination, data
 Output: none
   */
void makeArray(int destination[],int data){
	
}

/****************makeCopy***************
 Description: makes array
 Input: destination, source
 Output: none
   */
void makeCopy(int destination [25], int source [25]){
	int i;
	for(i= 0; i<25;i++){
		destination[i]= source[i];
	}
}

/****************HorizontalSquare***************
 Description: Horizontal effect
 Input: layer
 Output: none
   */
void HorizontalSquare(int layer){
	int i,j;
	int sum ;
	sum = layer *25;
	for (i = 0; i < 25;i++){
		Turn_On(i+sum);
		for (j = 1 ;j <1125; j++){}
		}
}

/****************HorizontalSquareGeneral***************
 Description: Horizontal effect
 Input: LEDarray, layer, size
 Output: none
   */
void HorizontalSquareGeneral(int LEDarray[],int layer,int size){
	int i,j;
	int sum ;
	sum = layer *25;
	for (i = 0; i < size;i++){
		Turn_On(LEDarray[i]+sum);
		for (j = 1 ;j <1125; j++){}
		}
}


/****************FireWork***************
 Description: Firework effect
 Input: none
 Output: none
   */
void FireWork(void){
	int i,j,k,m;
	int center_led = 12;
	for (i = 0; i <5;i++){
		for (m =0;m<5;m++){
			Turn_On(center_led + i*25);
			for (j = 1 ;j <3000; j++){}
			}
	}
	for (k = 0;k<12;k++){
			Turn_On(Fire_Work[k]);
		for (j = 1 ;j <1000; j++){}
	}
}

/****************Arrow***************
 Description: arrow effect
 Input: frequency
 Output: none
   */
void Arrow(int frequency){
	int i,j,duration,layer;
	i=0;
	layer =0;

	for(duration = 0; duration <10;duration++){
		while(arrowArray[i]!= 126){
			Turn_On(arrowArray[i]);
			for (j = 1 ;j <1000; j++){}
			i++;
		}
			i=0;
	}

		moveHorizontal(arrowArray,13);
}

/****************Animation0***************
 Description: visual effect
 Input: frequency
 Output: none
   */
void Animation0(int frequency){
	int i,j,k,layer,duration;
	if(frequency > 3500){
		layer = 0;
	}
	else if(frequency > 2700){
		layer = 2;
	}
	else if(frequency > 2500){
		layer = 3;
	}
	else if(frequency < 1500){
		layer = 4;
	}
	Arrow(frequency);
	for (k = 0 ;k < 1000; k++){}

	i=0;
	if (layer > 0){
		for (j=0;j <layer;j++){
			while(center_1[i]!= 126){
				Turn_On(center_1[i]+j*25);
				for (k = 0 ;k < 1000; k++){}
				i++;
			}
			i=0;
		}
	}
}

/****************Animation1***************
 Description: visual effect
 Input: intensity
 Output: none
   */
void Animation1(int intensity){
	

	int i,j;
	int random,duration;
	Index = 0;
	makeCopy(Animation1Hist3,Animation1Hist2);
	makeCopy(Animation1Hist2,Animation1Hist1);
	makeCopy(Animation1Hist1,Animation1Hist0);
	makeCopy(Animation1Hist0,Animation1Array);
	
	random  = rand() %4;
	
		//layer 1
	
		if(intensity > 50){
			for (i = 0; i <5;i++){
				addToArray(Vlayer1_1[i]);
				
			}
			addToArray(Vlayer1_1[random] +25);
			addToArray(Vlayer1_1[random%2+1 ]+50);
			addToArray(Vlayer1_1[random] +50);
			addToArray(Vlayer1_1[random%2+1 ]+50);
			addToArray(Vlayer1_1[random] +75);
			addToArray(Vlayer1_1[random%2+1] +75);
			addToArray(Vlayer1_1[random] +100);
			addToArray(Vlayer1_1[random%2+1] +100);
			addToArray(126);
		}
		else if(intensity > 40){
			for (i = 0; i <5;i++){
				addToArray(Vlayer1_1[i]);
				
			}
			addToArray(Vlayer1_1[random] +25);
			addToArray(Vlayer1_1[random] +50);
			addToArray(126);
		}
		else if(intensity > 30){
			for (i = 0; i <5;i++){
				addToArray(Vlayer1_1[i]);
			}
			addToArray(Vlayer1_1[random] +25);
			addToArray(Vlayer1_1[random%2+1] +25);
			addToArray(126);
		}
		else{
			for (i = 0; i <5;i++){
				addToArray(Vlayer1_1[i]);
			}
			addToArray(Vlayer1_1[random] +25);
			addToArray(126);
		}
		
	
	
	for(duration  = 0;duration < 10;duration ++){
		i = 0;
		//layer1
		while(Animation1Array[i]!= 126){
				Turn_On(Animation1Array[i]);
				for(j =0;j<100;j++){}
				i++;
			}
		i = 0;	
		//layer 2
			for(j =0;j<200;j++){}
	  while(Animation1Hist0[i]!= 126){
				Turn_On(Animation1Hist0[i]+1);
				for(j =0;j<100;j++){}
				i++;
			}
		i = 0;	

	  //layer3
			for(j =0;j<200;j++){}
		while(Animation1Hist1[i]!= 126){
				Turn_On(Animation1Hist1[i]+2);
				for(j =0;j<100;j++){}
				i++;
			}
					i = 0;	

		//layer4
		for(j =0;j<200;j++){}
		while(Animation1Hist2[i]!= 126){
				Turn_On(Animation1Hist2[i]+3);
				for(j =0;j<100;j++){}
				i++;
			}
		//layer5
		i = 0;	
		for(j =0;j<200;j++){}
		while(Animation1Hist3[i]!= 126){
				Turn_On(Animation1Hist3[i]+4);
				for(j =0;j<100;j++){}
				i++;
			}
	}
	
}

/****************Animation2***************
 Description: visual effect
 Input: intensity
 Output: none
   */
void Animation2(int intensity){
	int layer,duration,i,random;
	layer = intensity/10;
	CubeHist[2] = CubeHist[1];
	CubeHist[1] = CubeHist[0];
	CubeHist[0] = layer;
	
	
	for(duration = 0;duration <5;duration ++){
		for (i = 0; i < layer;i++){
			Cube(cube_1,i);
		}
		for (i = 0; i < CubeHist[0];i++){
			Cube(cube_2,i);
		}
		for (i = 0; i < CubeHist[1];i++){
			Cube(cube_3,i);
		}
		for (i = 0; i < CubeHist[2];i++){
			Cube(cube_4,i);
		}
	}
}

/****************Animation3***************
 Description: visual effect
 Input: intensity
 Output: none
   */
void Animation3(int intensity){
						if (intensity > 45){
							HorizontalSquare(0);
							HorizontalSquare(1);
							HorizontalSquare(2);
							HorizontalSquare(3);
							HorizontalSquare(4);
							HorizontalSquare(4);
						}
						else if (intensity > 40){
							HorizontalSquare(0);
							HorizontalSquare(1);
							HorizontalSquare(2);
							HorizontalSquare(3);
							HorizontalSquare(3);
						}
						else if(intensity > 30){
							HorizontalSquare(0);
							HorizontalSquare(1);
							HorizontalSquare(2);
							HorizontalSquare(2);
						}
						else if(intensity > 20){
							HorizontalSquare(0);
							HorizontalSquare(1);
							HorizontalSquare(1);
							
						}
						else if(intensity > 3){
							HorizontalSquare(0);
							HorizontalSquare(0);
						}
}

/****************Animation4***************
 Description: visual effect
 Input: intensity
 Output: none
   */
void Animation4(int intensity){
	
							if (intensity > 45){
							HorizontalSquareGeneral(squareOutline,0,18);
							HorizontalSquareGeneral(squareOutline,1,18);
							HorizontalSquareGeneral(squareOutline,2,18);
							HorizontalSquareGeneral(squareOutline,3,18);
							HorizontalSquareGeneral(squareOutline,4,18);
							HorizontalSquareGeneral(squareOutline,4,18);
						}
						else if (intensity > 40){
							HorizontalSquareGeneral(squareOutline,0,18);
							HorizontalSquareGeneral(squareOutline,1,18);
							HorizontalSquareGeneral(squareOutline,2,18);
							HorizontalSquareGeneral(squareOutline,3,18);
							HorizontalSquareGeneral(squareOutline,3,18);
						}
						else if(intensity > 30){
							HorizontalSquareGeneral(squareOutline,0,18);
							HorizontalSquareGeneral(squareOutline,1,18);
							HorizontalSquareGeneral(squareOutline,2,18);
							HorizontalSquareGeneral(squareOutline,2,18);
						}
						else if(intensity > 20){
							HorizontalSquareGeneral(squareOutline,0,18);
							HorizontalSquareGeneral(squareOutline,1,18);
							HorizontalSquareGeneral(squareOutline,1,18);
							
						}
						else if(intensity > 3){
							HorizontalSquareGeneral(squareOutline,0,18);
							HorizontalSquareGeneral(squareOutline,0,18);
						}
	
}

/****************Animation5***************
 Description: visual effect
 Input: intensity
 Output: none
   */
void Animation5(int intensity){
  int i,k,j,duration,layer;
	i=0;
	layer =0;
	if (intensity > 45){
		duration = 5;
	}
//	else if (intensity > 40){
//		FireWork();

//	}
	else{	
		duration = 10;
	}

	for(k = 0; k <duration;k++){
		while(diagonal_1[i]!= 126){
			Turn_On(diagonal_1[i]);
			for (j = 1 ;j <1000; j++){}
			Turn_On(diagonal_5[i]);
		  for (j = 1 ;j <1000; j++){}
			i++;
		}
			i=0;
	}
		moveHorizontal(diagonal_5,5);
		moveHorizontal(diagonal_1,5);
	//}
}	

/****************VerticalSquare***************
 Description: vertical effect
 Input: layer
 Output: none
   */
void VerticalSquare(int layer){
	int i,j;
	for (i = 0; i < 25;i++){
		Turn_On(VSquare[i]+layer);
		for (j = 1 ;j <1125; j++){}
		}
}

/****************AllOn***************
 Description: turns on all leds
 Input: none
 Output: none
   */
void AllOn(void){
	int i,j;
	for (i = 0; i < 125;i++){
		Turn_On(i);
		for (j = 1 ;j <1000; j++){}
		}
}

/****************RainDrop***************
 Description: raindrop effect
 Input: frequency
 Output: none
   */
void RainDrop(int frequency){
	int i,j,k,l,m;
	int layer;
	int bottomUp =1;
	layer = 100;
	i = rand() %25;
	if(frequency < 3000){
		bottomUp = 4;
	}

	while(layer > -1){
		Turn_On(i+layer);
		layer = layer -25;
		//i = rand() %25;
		for(k = 0;k <frequency;k++){}
		for(j=0;j<25;j++){
			Turn_On(rainArray[j]);
			for(k = 0;k <500;k++){}
		}
		for(m=1;m<=bottomUp;m++){
			for(j=0;j<25;j++){
				Turn_On(rainDropBot[j]+(m-1)*25);
				for(k = 0;k <500;k++){}
			}
		}
		if((i+layer) < 25){
			rainDropBot[rainIndex] = i+layer;
			rainIndex =rainIndex +1;
			if(rainIndex == 25){
				rainIndex =0;
			}
		}
	}
}

/****************Random***************
 Description: random effect
 Input: none
 Output: none
   */
void Random(void){
	int i,j;
	i = rand()%125;
	Turn_On(i);
	for (j = 1 ;j <10125; j++){}
}

/****************Boxes***************
 Description: Box effect
 Input: layer
 Output: none
   */
void Boxes(int layer){
  int i,j;
	int sum ;
	sum = layer *25;
	for (i = 0; i < 16;i++){
		Turn_On(Box[i]+sum);
		for (j = 1 ;j <1125; j++){}
		}
}

/****************Alphabet***************
 Description: prints character into cube
 Input: letter
 Output: none
   */
void Alphabet(char letter){
	int i,j,duration;
	int layer = 0;
	switch(letter){
		case 'A':
			for(layer = 4;layer > -1;layer--){
				for(duration = 0;duration < 20;duration++){
					for(i = 0; i < 10;i++){
						Turn_On(A_1[i]+layer);
						for(j =0;j <1000;j++){}
					}
				}
			}
		break;
	  case 'B':
			for(layer = 4;layer > -1;layer--){
				for(duration = 0;duration < 20;duration++){			
					for(i = 0; i < 16;i++){
						Turn_On(B_1[i]+layer);
						for(j =0;j <1000;j++){}
					}
				}
			}
		break;
		case 'C':
			for(layer = 4;layer > -1;layer--){
				for(duration = 0;duration < 20;duration++){
					for(i = 0; i < 13;i++){
						Turn_On(C_1[i]+layer);
						for(j =0;j <1000;j++){}
					}
				}
			}
		break;
		case 'D':
		break;
		case 'S':
		 for(layer = 4;layer > -1;layer--){
				for(duration = 0;duration < 20;duration++){
					for(i = 0; i < 17;i++){
						Turn_On(S_1[i]+layer);
						for(j =0;j <1000;j++){}
					}
				}
			}
		break;
		case 'E':
			for(layer = 4;layer > -1;layer--){
				for(duration = 0;duration < 20;duration++){
					for(i = 0; i < 17;i++){
						Turn_On(E_1[i]+layer);
						for(j =0;j <1000;j++){}
					}
				}
			}
		break;
		case 'T':
			for(layer = 4;layer > -1;layer--){
				for(duration = 0;duration < 20;duration++){
					for(i = 0; i < 10;i++){
						Turn_On(T_1[i]+layer);
						for(j =0;j <1000;j++){}
					}
				}
			}
		break;
		case 'U':
			for(layer = 4;layer > -1;layer--){
				for(duration = 0;duration < 20;duration++){
					for(i = 0; i < 11;i++){
						Turn_On(U_1[i]+layer);
						for(j =0;j <1000;j++){}
					}
				}
			}
		break;
		case 'X':
			for(layer = 4;layer > -1;layer--){
				for(duration = 0;duration < 20;duration++){
					for(i = 0; i < 11;i++){
						Turn_On(X_1[i]+layer);
						for(j =0;j <1000;j++){}
					}
				}
			}
		break;
	}
}



