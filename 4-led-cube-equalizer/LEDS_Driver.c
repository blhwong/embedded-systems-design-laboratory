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


