2-alarm-clock-design
===========================
Lab 3 Alarm Clock
Brandon Wong and Duc Tran
9/27/2013

1.0 OBJECTIVES:

Our objective for the third lab was to design an alarm clock. The design of the alarm clock includes
developing four interfaces that effectively communicate with each other. The four different modules
include the switch interface, SysTick time management interface, OLED interface, and speaker
interface. The graphics driver for the OLED will be in charge of displaying the time as well as drawing
the lines for an analog clock. The switch interface will debounce the inputs using edge triggered
interrupts. The timer and alarm will interrupt by SysTick periodic interrupts. We will be designing these
modules as well as debug and test our software by using the techniques from the previous lab. Finally,
our goal is to obtain a better understanding of interrupts on the LM3S1968.

1. Overview

1.1. Objectives: Why are we doing this project? What is the purpose?
The objectives of this project are to design, build and test an alarm clock. Educationally,
students are learning how to design and test modular software and how to perform switch/keypad
input in the background.

1.2. Process: How will the project be developed?
The project will be developed using the LM3S1968 board. There will be switches or a
keypad. The system will be built on a solderless breadboard and run on the usual USB power. The
system may use the on board switches and/or the on board sound. Alternatively, the system may
include an external keypad and/or speaker. There will be at least four hardware/software
modules: switch/keypad input, time management, OLED graphics, and sound output. The process
will be to design and test each module independently from the other modules. After each module
is tested, the system will be built and tested.

1.3. Roles and Responsibilities: Who will do what? Who are the clients?
Duc will be responsible for the OLED graphics, and switch/keypad input. Brandon will be
responsible for the time management, and sound output.

1.4. Interactions with Existing Systems: How will it fit in?
The system will use the LM3S1968 board, a solderless breadboard, and be powered using the
USB cable.

1.5. Terminology: Define terms used in the document.
Power budget – one can estimate the operation time of a batterypowered
embedded system by
dividing the energy storage by the average current required to run the system.
device driver – a set of software functions that facilitate the use of an I/O port
critical section – a loss of input, corruption of data, or shifting of data after a subroutine due to
shared global variables
latency – time between hardware trigger and software response
time jitter – undesired deviation from true periodicity
modular programming – a style of software development that divides the software problem into
distinct and independent modules
1.6. Security: How will intellectual property be managed?
The system may include software from StellarisWare and from the book. No software
written for this project may be transmitted, viewed, or communicated with any other EE445L
student past, present, or future (other than the lab partner of course). It is the responsibility of the
team to keep its EE445L lab solutions secure.

2. Function Description

2.1. Functionality: What will the system do precisely?
The clock must be able to perform five functions. 1) It will display hours and minutes in both
graphical and numeric forms on the OLED. The graphical output will include the 12 numbers
around a circle, the hour hand, and the minute hand. The numerical output will be easy to read. 2)
It will allow the operator to set the current time using switches or a keypad. 3) It will allow the
operator to set the alarm time including enabling/disabling alarms. 4) It will make a sound at the
alarm time. 5) It will allow the operator to stop the sound. An LED heartbeat will show when the
system is running.6) Allow user to snooze the alarm. 7) The interface will show emoticons to
indicate time range (morning, lunch, afternoon, night). 8) The Alarm has welcome screen.

2.2. Scope: List the phases and what will be delivered in each phase.
Phase 1 is the preparation; phase 2 is the demonstration; and phase 3 is the lab report.
Details can be found in the lab manual.

2.3. Prototypes: How will intermediate progress be demonstrated?
A prototype system running on the LM3S1968 board and solderless breadboard will be
demonstrated. Progress will be judged by the preparation, demonstration and lab report.

2.4. Performance: Define the measures and describe how they will be determined.
The system will be judged by three qualitative measures. First, the software modules must
be easy to understand and wellorganized.
Second, the clock display should be beautiful and
effective in telling time. Third, the operation of setting the time and alarm should be simple and
intuitive. The system should not have critical sections. All shared global variables must be
identified with documentation that a critical section does not exist. Backward jumps in the ISR
should be avoided if possible. The interrupt service routine used to maintain time must complete in
as short a time as possible. This means all OLED I/O occurs in the main program. The average
current on the +5V power will be measured with and without the alarm sounding.

2.5. Usability: Describe the interfaces. Be quantitative if possible.
There will be four switch inputs. In the main menu, the switches can be used to activate 1)
set time; 2) set alarm; 3) turn on/off alarm; and 4) display mode. In set time and alarm modes, two
switches add and subtract hours and the other two add and subtract minutes. After 10 seconds of
inactivity the system reverts to the main menu. The display mode switch toggles between
graphical and numeric displays. The switches will be debounced, so only one action occurs when
the operator touches a switch once.
The OLED display shows the time using graphical display typical of a standard on the wall
clock. The 12 numbers, the minute hand, and the hour hand are large and easy to see. The clock
can also display the time in numeric mode using numbers.
The alarm sound can be a simple square wave. The sound amplitude will be just loud
enough for the TA to hear when within 3 feet.

2.6. Safety: Explain any safety requirements and how they will be measured.
The alarm sound will be VERY quiet in order to respect other people in the room during
testing. Connecting or disconnecting wires on the protoboard while power is applied may damage
the board.

3. Deliverables

3.1. Reports: How will the system be described?
A lab report described below is due by the due date listed in the syllabus. This report includes the
final requirements document.

3.2. Audits: How will the clients evaluate progress?
The preparation is due at the beginning of the lab period on the date listed in the syllabus.

3.3. Outcomes: What are the deliverables? How do we know when it is done?
There are three deliverables: preparation, demonstration, and report.


5.0 ANALYSIS AND DISCUSSION:
1) Give two ways to remove a critical section.
You can remove this by avoiding the use of global variables for ISRs and also by first disabling an
interrupt when entering an ISR and disabling before returning from an ISR.

2) How long does it take to update the OLED with a new time?
The time that it takes for the OLED to update depends on where the PC is at during the while
loop. So in theory, the longest time it would take to display the time onto the OLED is however long it
would take to run the infinite while loop just once.

3) What would be the disadvantage of updating the OLED in the background ISR?
By updating it during the background ISR, it would lead into critical sections if you are using a
lot of clock cycles to display time to OLED. If you try to disabling/enabling interrupts in the
ISR, then you are not properly managing the time.

4) Did you use a OLED clear function? If so, how could you have redesigned the OLED update to run
much faster?
We used an OLED clear function that we designed ourselves by instead of clearing the entire
display, it just cleared the line functions for displaying the analog clock. This saved time because the only
thing you needed to clear on the analog display were the lines.

5) Assuming the system were battery powered, list three ways you could have saved power.
Include a power saving mode for the OLED display (put the display to sleep when not needed).
Write software that can shutdown output pins or reduce current to pins when not needed. Include a
volume/mute button to turn off the speaker, since we observed that in part 4.3 the current increased
with alarm sound.