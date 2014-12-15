//Filename: Pattern_Generator.h
//Author: Duc Tran, Brandon Wong
//Initial Creation Date: November 4, 2013
//Description: Pattern Generator of LED Cube
//Lab Number: W 2-3:30
//TA : Omar & Mahesh
//Date of last revision : December 6, 2013

/****************HorizontalSquare***************
 Description: Horizontal effect
 Input: layer
 Output: none
   */
void HorizontalSquare(int layer);

/****************HorizontalSquareGeneral***************
 Description: Horizontal effect
 Input: LEDarray, layer, size
 Output: none
   */
void HorizontalSquareGeneral(int LEDarray[],int layer,int size);

/****************FireWork***************
 Description: Firework effect
 Input: none
 Output: none
   */
void FireWork(void);

/****************Arrow***************
 Description: arrow effect
 Input: frequency
 Output: none
   */
void Arrow(int frequency);

/****************Animation0***************
 Description: visual effect
 Input: frequency
 Output: none
   */
void Animation0(int frequency);
/****************Animation1***************
 Description: visual effect
 Input: intensity
 Output: none
   */
void Animation1(int intensity);

/****************Animation2***************
 Description: visual effect
 Input: intensity
 Output: none
   */
void Animation2(int intensity);

/****************Animation3***************
 Description: visual effect
 Input: intensity
 Output: none
   */
void Animation3(int intensity);

/****************Animation4***************
 Description: visual effect
 Input: intensity
 Output: none
   */
void Animation4(int intensity);

/****************Animation5***************
 Description: visual effect
 Input: intensity
 Output: none
   */
void Animation5(int intensity);	

/****************VerticalSquare***************
 Description: vertical effect
 Input: layer
 Output: none
   */
void VerticalSquare(int layer);

/****************AllOn***************
 Description: turns on all leds
 Input: none
 Output: none
   */
void AllOn(void);

/****************RainDrop***************
 Description: raindrop effect
 Input: frequency
 Output: none
   */
void RainDrop(int frequency);

/****************Random***************
 Description: random effect
 Input: none
 Output: none
   */
void Random(void);

/****************Boxes***************
 Description: Box effect
 Input: layer
 Output: none
   */
void Boxes(int layer);

/****************Alphabet***************
 Description: prints character into cube
 Input: letter
 Output: none
   */
void Alphabet(char letter);



