//Filename: MusicModule.c
//Author: Duc Tran, Brandon Wong
//Initial Creation Date: October 2, 2013
//Description: Handle music player
//Lab Number: W 2-3:30
//TA : Omar & Mahesh
//Date of last revision :October 14, 2013
//Hardware Configuration : Lab5_artist.sch

#define WHOLE_NOTE 64000
#define HALF_NOTE 32000
#define QUARTER_NOTE 16000
#define EIGHTH_NOTE 8000
#define QUARTER_NOTE_PAUSE 100
#define SIXTEENTH_NOTE 4000

#define EIGHTH_TRIPLE_NOTE 5333
#define EIGHTH_TRIPLE_NOTE2 5533
#define EIGHTH_TRIPLE_NOTE3 5733
#define EIGHTH_TRIPLE_NOTE4 5933
#define EIGHTH_TRIPLE_NOTE5 6133
#define EIGHTH_TRIPLE_NOTE6 6333
  
  
  
#define DOTTED_SIXTEENTH_NOTE 12000
  
  
#define FINAL_NOTE 65000 


#define REST 0
#define PAUSE 0
#define QUIET 1
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
	//const struct Player *Next[8];
	
	//const struct Player *Song;
	//unsigned short SinTable[256];
};

typedef const struct Player SongType;
typedef SongType *SongPtr;

typedef const struct Player MusicStateType;
const struct Player *Ptc;




//MusicStateType MarioTheme[58]={

//	{E1, SIXTEENTH_NOTE},
//	{E1, EIGHTH_NOTE},
//	{E1, SIXTEENTH_NOTE},
//	{REST, SIXTEENTH_NOTE},
//	{C, SIXTEENTH_NOTE},
//	{E1, EIGHTH_NOTE},
//	{G1, QUARTER_NOTE},
//	{G, QUARTER_NOTE},

//	{C, EIGHTH_NOTE},
//	{REST, SIXTEENTH_NOTE},
//	{G, SIXTEENTH_NOTE},
//	{REST, EIGHTH_NOTE},
//	{E, EIGHTH_NOTE},
//	{REST, SIXTEENTH_NOTE},
//	{A, SIXTEENTH_NOTE},
//	{REST, SIXTEENTH_NOTE},
//	{B, SIXTEENTH_NOTE},
//	{REST, SIXTEENTH_NOTE},
//	{BF, SIXTEENTH_NOTE},
//	{A, EIGHTH_NOTE},

//	{G, EIGHTH_TRIPLE_NOTE},
//	{E1, EIGHTH_TRIPLE_NOTE},
//	{G1, EIGHTH_TRIPLE_NOTE},
//	{A1, EIGHTH_NOTE},
//	{F1, SIXTEENTH_NOTE},
//	{G1, SIXTEENTH_NOTE},
//	{REST, SIXTEENTH_NOTE},
//	{E1, SIXTEENTH_NOTE},
//	{REST, SIXTEENTH_NOTE},
//	{C, SIXTEENTH_NOTE},
//	{D1, SIXTEENTH_NOTE},
//	{B, SIXTEENTH_NOTE},
//	{REST, EIGHTH_NOTE},

//	{C, EIGHTH_NOTE},
//	{REST, SIXTEENTH_NOTE},
//	{G, SIXTEENTH_NOTE},
//	{REST, EIGHTH_NOTE},
//	{E, EIGHTH_NOTE},
//	{REST, SIXTEENTH_NOTE},
//	{A, SIXTEENTH_NOTE},
//	{REST, SIXTEENTH_NOTE},
//	{B, SIXTEENTH_NOTE},
//	{REST, SIXTEENTH_NOTE},
//	{BF, SIXTEENTH_NOTE},
//	{A, EIGHTH_NOTE},


//	{G, EIGHTH_TRIPLE_NOTE},
//	{E1, EIGHTH_TRIPLE_NOTE},
//	{G1, EIGHTH_TRIPLE_NOTE},
//	{A1, EIGHTH_NOTE},
//	{F1, SIXTEENTH_NOTE},
//	{G1, SIXTEENTH_NOTE},
//	{REST, SIXTEENTH_NOTE},
//	{E1, SIXTEENTH_NOTE},
//	{REST, SIXTEENTH_NOTE},
//	{C, SIXTEENTH_NOTE},
//	{D1, SIXTEENTH_NOTE},
//	{B, SIXTEENTH_NOTE},
//	{REST, EIGHTH_NOTE},

//};


MusicStateType MarioTheme[352]={
  
    {E1, SIXTEENTH_NOTE},   //1
    {E1, EIGHTH_NOTE},
    {E1, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {C, SIXTEENTH_NOTE},
    {E1, EIGHTH_NOTE},
    {G1, QUARTER_NOTE},
    {G, QUARTER_NOTE},
  
    {C, EIGHTH_NOTE},    //2
    {REST, SIXTEENTH_NOTE},
    {G, SIXTEENTH_NOTE},
    {REST, EIGHTH_NOTE},
    {E, EIGHTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {A, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {B, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {BF, SIXTEENTH_NOTE},
    {A, EIGHTH_NOTE},
  
    {G, EIGHTH_TRIPLE_NOTE},    //3
    {E1, EIGHTH_TRIPLE_NOTE},
    {G1, EIGHTH_TRIPLE_NOTE},
    {A1, EIGHTH_NOTE},
    {F1, SIXTEENTH_NOTE},
    {G1, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {E1, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {C, SIXTEENTH_NOTE},
    {D1, SIXTEENTH_NOTE},
    {B, SIXTEENTH_NOTE},
    {REST, EIGHTH_NOTE},
  
    {C, EIGHTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {G, SIXTEENTH_NOTE},
    {REST, EIGHTH_NOTE},
    {E, EIGHTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {A, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {B, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {BF, SIXTEENTH_NOTE},
    {A, EIGHTH_NOTE},
  
  
    {G, EIGHTH_TRIPLE_NOTE},
    {E1, EIGHTH_TRIPLE_NOTE},
    {G1, EIGHTH_TRIPLE_NOTE},
    {A1, EIGHTH_NOTE},
    {F1, SIXTEENTH_NOTE},
    {G1, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {E1, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {C, SIXTEENTH_NOTE},
    {D1, SIXTEENTH_NOTE},
    {B, SIXTEENTH_NOTE},
    {REST, EIGHTH_NOTE},
  
    {REST, EIGHTH_NOTE},
    {G1, SIXTEENTH_NOTE},
    {GF1, SIXTEENTH_NOTE},
    {F1, SIXTEENTH_NOTE},
    {EF1, EIGHTH_NOTE},
    {E1, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {AF, SIXTEENTH_NOTE},
    {A, SIXTEENTH_NOTE},
    {C, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {A, SIXTEENTH_NOTE},
    {C, SIXTEENTH_NOTE},
    {D1, SIXTEENTH_NOTE},
      
    {REST, EIGHTH_NOTE},
    {G1, SIXTEENTH_NOTE},
    {GF1, SIXTEENTH_NOTE},
    {F1, SIXTEENTH_NOTE},
    {EF1, EIGHTH_NOTE},
    {E1, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {C1, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {C1, SIXTEENTH_NOTE},
    {C1, QUARTER_NOTE},
      
    {REST, EIGHTH_NOTE},
    {G1, SIXTEENTH_NOTE},
    {GF1, SIXTEENTH_NOTE},
    {F1, SIXTEENTH_NOTE},
    {EF1, EIGHTH_NOTE},
    {E1, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {AF, SIXTEENTH_NOTE},
    {A, SIXTEENTH_NOTE},
    {C, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {A, SIXTEENTH_NOTE},
    {C, SIXTEENTH_NOTE},
    {D1, SIXTEENTH_NOTE},
      
    {REST, EIGHTH_NOTE},
    {EF1, EIGHTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {D1, SIXTEENTH_NOTE},
    {REST, EIGHTH_NOTE},
    {C, QUARTER_NOTE},
    {REST, QUARTER_NOTE},
      
    {C, SIXTEENTH_NOTE},
    {C, EIGHTH_NOTE},
    {C, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {C, SIXTEENTH_NOTE},
    {D1, EIGHTH_NOTE},
    {E1, SIXTEENTH_NOTE},
    {C, EIGHTH_NOTE},
    {A, SIXTEENTH_NOTE},
    {G, QUARTER_NOTE},
      
    {C, SIXTEENTH_NOTE},
    {C, EIGHTH_NOTE},
    {C, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {C, SIXTEENTH_NOTE},
    {D1, EIGHTH_NOTE},
    {E1, SIXTEENTH_NOTE},
    {REST, HALF_NOTE},
      
    {C, SIXTEENTH_NOTE},
    {C, EIGHTH_NOTE},
    {C, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {C, SIXTEENTH_NOTE},
    {D1, EIGHTH_NOTE},
    {E1, SIXTEENTH_NOTE},
    {C, EIGHTH_NOTE},
    {A, SIXTEENTH_NOTE},
    {G, QUARTER_NOTE},
      
    {E1, SIXTEENTH_NOTE},   //17
    {E1, EIGHTH_NOTE},
    {E1, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {C, SIXTEENTH_NOTE},
    {E1, EIGHTH_NOTE},
    {G1, QUARTER_NOTE},
    {G, QUARTER_NOTE},
      
    {C, EIGHTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {G, SIXTEENTH_NOTE},
    {REST, EIGHTH_NOTE},
    {E, EIGHTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {A, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {B, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {BF, SIXTEENTH_NOTE},
    {A, EIGHTH_NOTE},
  
    {G, EIGHTH_TRIPLE_NOTE},
    {E1, EIGHTH_TRIPLE_NOTE},
    {G1, EIGHTH_TRIPLE_NOTE},
    {A1, EIGHTH_NOTE},
    {F1, SIXTEENTH_NOTE},
    {G1, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {E1, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {C, SIXTEENTH_NOTE},
    {D1, SIXTEENTH_NOTE},
    {B, SIXTEENTH_NOTE},
    {REST, EIGHTH_NOTE},
  
    {C, EIGHTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {G, SIXTEENTH_NOTE},
    {REST, EIGHTH_NOTE},
    {E, EIGHTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {A, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {B, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {BF, SIXTEENTH_NOTE},
    {A, EIGHTH_NOTE},
  
  
    {G, EIGHTH_TRIPLE_NOTE},
    {E1, EIGHTH_TRIPLE_NOTE},
    {G1, EIGHTH_TRIPLE_NOTE},
    {A1, EIGHTH_NOTE},
    {F1, SIXTEENTH_NOTE},
    {G1, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {E1, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {C, SIXTEENTH_NOTE},
    {D1, SIXTEENTH_NOTE},
    {B, SIXTEENTH_NOTE},
    {REST, EIGHTH_NOTE},
      
    {E1, SIXTEENTH_NOTE},   //22
    {C, EIGHTH_NOTE},
    {G, SIXTEENTH_NOTE},
    {REST, EIGHTH_NOTE},
    {AF, EIGHTH_NOTE},
    {A, SIXTEENTH_NOTE},
    {F1, EIGHTH_NOTE},
    {F1, SIXTEENTH_NOTE},
    {A, QUARTER_NOTE},
      
    {B, EIGHTH_TRIPLE_NOTE},
    {A1, EIGHTH_TRIPLE_NOTE},
    {A1, EIGHTH_TRIPLE_NOTE},
    {A1, EIGHTH_TRIPLE_NOTE},
    {G1, EIGHTH_TRIPLE_NOTE},
    {F1, EIGHTH_TRIPLE_NOTE},
    {E1, SIXTEENTH_NOTE},
    {C, EIGHTH_NOTE},
    {A, SIXTEENTH_NOTE},
    {G, QUARTER_NOTE},
  
    {E1, SIXTEENTH_NOTE},   
    {C, EIGHTH_NOTE},
    {G, SIXTEENTH_NOTE},
    {REST, EIGHTH_NOTE},
    {AF, EIGHTH_NOTE},
    {A, SIXTEENTH_NOTE},
    {F1, EIGHTH_NOTE},
    {F1, SIXTEENTH_NOTE},
    {A, QUARTER_NOTE},
      
    {B, SIXTEENTH_NOTE},    
    {F1, EIGHTH_NOTE},
    {F1, SIXTEENTH_NOTE},
    {F1, EIGHTH_TRIPLE_NOTE},
    {E1, EIGHTH_TRIPLE_NOTE},
    {D1, EIGHTH_TRIPLE_NOTE},
    {C, SIXTEENTH_NOTE},
    {G, EIGHTH_NOTE},
    {G, SIXTEENTH_NOTE},
    {C0, QUARTER_NOTE},
      
    {E1, SIXTEENTH_NOTE},   
    {C, EIGHTH_NOTE},
    {G, SIXTEENTH_NOTE},
    {REST, EIGHTH_NOTE},
    {AF, EIGHTH_NOTE},
    {A, SIXTEENTH_NOTE},
    {F1, EIGHTH_NOTE},
    {F1, SIXTEENTH_NOTE},
    {A, QUARTER_NOTE},  
      
    {B, EIGHTH_TRIPLE_NOTE},
    {A1, EIGHTH_TRIPLE_NOTE},
    {A1, EIGHTH_TRIPLE_NOTE},
    {A1, EIGHTH_TRIPLE_NOTE},
    {G1, EIGHTH_TRIPLE_NOTE},
    {F1, EIGHTH_TRIPLE_NOTE},
    {E1, SIXTEENTH_NOTE},
    {C, EIGHTH_NOTE},
    {A, SIXTEENTH_NOTE},
    {G, QUARTER_NOTE},
  
    {E1, SIXTEENTH_NOTE},   
    {C, EIGHTH_NOTE},
    {G, SIXTEENTH_NOTE},
    {REST, EIGHTH_NOTE},
    {AF, EIGHTH_NOTE},
    {A, SIXTEENTH_NOTE},
    {F1, EIGHTH_NOTE},
    {F1, SIXTEENTH_NOTE},
    {A, QUARTER_NOTE},  
  
    {B, SIXTEENTH_NOTE},    
    {F1, EIGHTH_NOTE},
    {F1, SIXTEENTH_NOTE},
    {F1, EIGHTH_TRIPLE_NOTE},
    {E1, EIGHTH_TRIPLE_NOTE},
    {D1, EIGHTH_TRIPLE_NOTE},
    {C, SIXTEENTH_NOTE},
    {G, EIGHTH_NOTE},
    {G, SIXTEENTH_NOTE},
    {C0, QUARTER_NOTE},
      
    {C, SIXTEENTH_NOTE},    //30
    {C, EIGHTH_NOTE},
    {C, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {C, SIXTEENTH_NOTE},
    {D1, EIGHTH_NOTE},
    {E1, SIXTEENTH_NOTE},
    {C, EIGHTH_NOTE},
    {A, SIXTEENTH_NOTE},
    {G, QUARTER_NOTE},
      
    {C, SIXTEENTH_NOTE},
    {C, EIGHTH_NOTE},
    {C, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {C, SIXTEENTH_NOTE},
    {D1, EIGHTH_NOTE},
    {E1, SIXTEENTH_NOTE},
    {REST, HALF_NOTE},
      
    {C, SIXTEENTH_NOTE},
    {C, EIGHTH_NOTE},
    {C, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {C, SIXTEENTH_NOTE},
    {D1, EIGHTH_NOTE},
    {E1, SIXTEENTH_NOTE},
    {C, EIGHTH_NOTE},
    {A, SIXTEENTH_NOTE},
    {G, QUARTER_NOTE},
      
    {E1, SIXTEENTH_NOTE},   //33
    {E1, EIGHTH_NOTE},
    {E1, SIXTEENTH_NOTE},
    {REST, SIXTEENTH_NOTE},
    {C, SIXTEENTH_NOTE},
    {E1, EIGHTH_NOTE},
    {G1, QUARTER_NOTE},
    {G, QUARTER_NOTE},  
      
    {E1, SIXTEENTH_NOTE},   
    {C, EIGHTH_NOTE},
    {G, SIXTEENTH_NOTE},
    {REST, EIGHTH_NOTE},
    {AF, EIGHTH_NOTE},
    {A, SIXTEENTH_NOTE},
    {F1, EIGHTH_NOTE},
    {F1, SIXTEENTH_NOTE},
    {A, QUARTER_NOTE},  
      
    {B, EIGHTH_TRIPLE_NOTE},
    {A1, EIGHTH_TRIPLE_NOTE},
    {A1, EIGHTH_TRIPLE_NOTE},
    {A1, EIGHTH_TRIPLE_NOTE},
    {G1, EIGHTH_TRIPLE_NOTE},
    {F1, EIGHTH_TRIPLE_NOTE},
    {E1, SIXTEENTH_NOTE},
    {C, EIGHTH_NOTE},
    {A, SIXTEENTH_NOTE},
    {G, QUARTER_NOTE},
      
    {E1, SIXTEENTH_NOTE},   
    {C, EIGHTH_NOTE},
    {G, SIXTEENTH_NOTE},
    {REST, EIGHTH_NOTE},
    {AF, EIGHTH_NOTE},
    {A, SIXTEENTH_NOTE},
    {F1, EIGHTH_NOTE},
    {F1, SIXTEENTH_NOTE},
    {A, QUARTER_NOTE},  
  
    {B, SIXTEENTH_NOTE},    
    {F1, EIGHTH_NOTE},
    {F1, SIXTEENTH_NOTE},
    {F1, EIGHTH_TRIPLE_NOTE},
    {E1, EIGHTH_TRIPLE_NOTE},
    {D1, EIGHTH_TRIPLE_NOTE},
    {C, SIXTEENTH_NOTE},
    {G, EIGHTH_NOTE},
    {G, SIXTEENTH_NOTE},
    {C0, QUARTER_NOTE}, 
      
    {C, DOTTED_SIXTEENTH_NOTE},
    {G, DOTTED_SIXTEENTH_NOTE},
    {E, EIGHTH_NOTE},
    {A, EIGHTH_TRIPLE_NOTE},
    {B, EIGHTH_TRIPLE_NOTE2},
    {A, EIGHTH_TRIPLE_NOTE3},
    {AF, EIGHTH_TRIPLE_NOTE4},
    {BF, EIGHTH_TRIPLE_NOTE5},
    {AF, EIGHTH_TRIPLE_NOTE6},
      
    {G, FINAL_NOTE},
    {REST, WHOLE_NOTE}
}; 


MusicStateType Carol[]={  
// carol of the bells
//1
  {C,    QUARTER_NOTE}, 
  {B,    EIGHTH_NOTE}, 
  {C,    EIGHTH_NOTE},
  {A,    QUARTER_NOTE_PAUSE}, 
  {QUIET,PAUSE},
//2   
  {C,    QUARTER_NOTE}, 
  {B,    EIGHTH_NOTE}, 
  {C,    EIGHTH_NOTE},
  {A,    QUARTER_NOTE_PAUSE}, 
  {1,PAUSE}
};

MusicStateType BirthdaySong[25]={
	
	{C0, EIGHTH_NOTE},
	{C0, EIGHTH_NOTE},
	{D, QUARTER_NOTE},
	{C0, QUARTER_NOTE},
	{F, QUARTER_NOTE},

	{E, HALF_NOTE},
	{C0, EIGHTH_NOTE},
	{C0, EIGHTH_NOTE},
	{D, QUARTER_NOTE},
	{C0, QUARTER_NOTE},

	{G, QUARTER_NOTE},
	{F, HALF_NOTE},
	{C0, EIGHTH_NOTE},
	{C0, EIGHTH_NOTE},
	{C, QUARTER_NOTE},

	{A, QUARTER_NOTE},
	{F, QUARTER_NOTE},
	{E, QUARTER_NOTE},
	{D, QUARTER_NOTE},
	{BF, EIGHTH_NOTE},
	
	{BF, EIGHTH_NOTE},
	{A, QUARTER_NOTE},
	{F, QUARTER_NOTE},
	{G, QUARTER_NOTE},
	{F, QUARTER_NOTE},
};

unsigned short PauseIndex;
unsigned long Duration;
unsigned short Index;
/****************Music_Init***************
 Description: Initializes stepper motor
 Input: none
 Output: none  */ 
void Music_Init(void){
	PauseIndex = 0;
	Index = 0;
	Ptc = &MarioTheme[0];
	Duration = Ptc->Duration;
}
/****************Music_Play***************
 Description:Play music
 Input: none
 Output: none  */ 
#define TIMER0_TAILR_R          (*((volatile unsigned long *)0x40030028))
void Music_Play(void){
	if(Index == 352){
		Index = 0;
	}
	Ptc = &MarioTheme[Index];
	TIMER0_TAILR_R = Ptc->Pitch;
	Duration = Ptc->Duration;
	Index = Index+PauseIndex;
}
/****************Music_Rewind***************
 Description: Rewind music to beginning
 Input: none
 Output: none  */ 
void Music_Rewind(void){
	Ptc = &MarioTheme[0];
	TIMER0_TAILR_R = 0;
	Duration = Ptc->Duration;
	Index = 0;
}
/****************Music_Stop***************
 Description: Stop music, next play will be from the beginning
 Input: none
 Output: none  */ 
void Music_Stop(void){
	PauseIndex =0;
	Ptc = &MarioTheme[0];
	TIMER0_TAILR_R = 0;
	Duration = Ptc->Duration;
	Index = 0;
}
/****************Music_Pause***************
 Description: Pause music, next play will be where it left off
 Input: none
 Output: none  */ 
void Music_Pause(void){
	PauseIndex ^=1;
	TIMER0_TAILR_R  = 0;
}