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



