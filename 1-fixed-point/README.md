1-fixed-point
===========================
EE445L Lab 1: Fixed-point Output
Duc Tran and Brandon Wong
9/13/2013

OBJECTIVES
Our main objective for this lab is to familiarize ourselves with the LM3S1968 programming environment
as well as the Keil uVision4 software. Our task was to complete the fixedpoint
routine from the
provided starter files so that we can use these functions for future labs. After completing the functions,
we learned the use of the debugger and mounting the program onto the board.

ANALYSIS AND DISCUSSION
1. We do not know how the OLED display is written. We are only provided with the drivers of the
display so we can output our values onto the display using the functions that were provided by the
designers. This is a good design because it encapsulates information between our code for fixed points
and the writers for the OLED display.

2. One of the constraint for the lab is that the subroutine must output six characters with a fixed amount
of digit for decimal and the integer. If we move the decimal point, the resolution changes. For example if
the number is “ 1.01”, and if we decide to shift the decimal point to the left then the number become “
1.010”. Although the two have the same value but “ 1.01” have a resolution of .01 while “ 1.010” have
a resolution of .001.

3. Fixed point is practical for numbers that have consistent fraction values and decimal values. If the
smallest fraction you need to represent is a value of a hundredth, then we can fix two points for the
fraction and the rest for the decimal. Fixed point is also much easier to read. However fixed point
integers are limited in the range of numbers that it can display. When you want to display a wide range
of numbers, floating point will be better.

4. To represent fractions, decimal fixed point provides a much wider resolution than binary fixed point.
This means that binary fixed point have difficulties in representing certain fractions. However, binary
fixed point numbers are much quicker in evaluation because it’s ability to logically shift left or right rather
than division or multiplication for decimal fixed point.

5. Currency would be an application where you can apply fixed point because two points would
represent the change, and the rest of the points would represent the dollar amount.

6. We can use floating point on Arm Cortex M3 but because it doesn’t have any hardware for floating
point application, speed of calculation will be slow. The Pentium w/MMX one of the processors known
for adding single precision floating point. So floating point arithmetic operations would be faster.
