5-led-cube-equalizer
===========================
Lab 11 Final Embedded System
Duc Tran & Brandon Wong
12/6/2013

1.0 Overview

1.1 Objectives: Why are we doing this project? What is the purpose?
The objective is to combine our knowledge and tools from our previous labs and to create a final
embedded system project. We chose to do a 5x5x5 LED cube equalizer. Educationally students are
learning the issue of power management, clock , reset, programming embedded system, cost efficiency,
and layout of PCB

1.2 Roles and Responsibilities: Who will do what Who are the clients?
The TA are our clients, and Duc and Brandon are the engineers. Duc will be modifying
the code he wrote previously for the music FSM and switch interface, and Systick interface.
Additionally, Duc will be adding code used for the GPIO for the outputs of the LM3S811. Duc and
Brandon will both come up with visual effects and ideas for the LED cube. Brandon will help revise
code as well as assembling the 5x5x5 and soldering components to the PCB. Together, we will modify
the design of LED PCB and the LM3S811 schematic.

2.0 Function Description

2.1 Functionality: What will the system do precisely?
A music player will be set up with the LED cube via audio jack. When music is played, the LEDs on the
cube will make a visual effect as the music is playing. Each LED will turn on depending on the
frequencies the music is playing. The audio will be connected via audio splitter so that the LED cube can
take in analog signals, and also users can hear the music and see the visual effects. Instead of the LED
cube being powered by a battery, we will use a wall outlet to power the LED cube due to the amount of
current for 125 LEDs was impossible for finding a cheap and long lasting battery.

2.2 Performance: Define the measures and describe how they will be determined.
The system will be judged by three qualitative measures. First, the software modules must be easy to
understand and well organized. Second, the system must employ a finite state machine running in the
background. There should be a clear and obvious abstraction, separating what the machine does (the
FSM state diagram) from how the machine works (the software ISR). Third, all software will be judged
according to style guidelines. There are three quantitative measures. First, we must measure the
frequency at what toggles the LED. Second, the maximum time to run one instance of the ISR will be
recorded. Third, you will measure power supply current to run the system.

2.3 Usability: Describe the interfaces. Be quantitative if possible.
There will be 125 LEDs, five 38
decoders that will multiplex each individual LED, and an audio jack
that will read the music. An LED will be mapped onto an output of an decoder, and the 5th decoder will
be the selector of which layer you want to activate. When the ADC samples a frequency, the ADC
converts it into a voltage. The voltage can tell us what frequency and which LEDs we want to activate
onto the cube. The effects created are up to the us, and we can generate infinite amounts of effects. The
idea is that we can create which LEDs we want to activate, create an array for the selected LEDs, and
use a random function to iterate random LEDs to achieve an effect.

3.0 Deliverables

3.1 Reports
A lab report described below is due by December 6, 2013. This report includes the final requirements
document.

3.2 Outcomes
There are three deliverables: demonstration, exhibit, and report. If selected during demonstration, then
we get to present our project in front of a group of judges on December 6, 2013.


7.0 Analysis and Discussion
The construction and design of our 5x5x5 LED cube equalizer turned out successful. Initially some of
the LEDs on the cube were not working, but we realized through measurements that some of the LEDs
were unconnected and therefore leaving a column of LEDs unlit. The software and the effects were
designed by us, and of course there is an infinite amount of effects that can be achieved. However,
everything worked as in the software can read the ADC value and you can see the effects change within
the beat of the music. We did not end up using a battery for the LED because the amount of current
from the LEDs require a long lasting battery that was expensive even though battery is not within our
budget. Instead, we powered it by the LM3S1968, and the LM3S1968 was powered by a USB
connected to a wall outlet. In the future, we hope to create more effects for the LED driver so that users
are more entertained by the visualizations.
