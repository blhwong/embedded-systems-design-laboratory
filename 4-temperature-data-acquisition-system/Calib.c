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