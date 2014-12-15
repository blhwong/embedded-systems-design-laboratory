3-music-player-and-audio-amp
===========================
Lab 5 Music Player and Audio Amp
Duc Tran & Brandon Wong
10/14/2013

1. Overview

1.1. Objectives: Why are we doing this project? What is the purpose?
The objectives of this project are to design, build and test a music player. Educationally, students are
learning how to interface a DAC, how to design a speaker amplifier, how to store digital music in ROM,
and how to perform DAC output in the background. Your goal is to play your favorite song.

1.2. Process: How will the project be developed?
The project will be developed using the LM3S1968 board. There will be three switches that the operator
will use to control the music player. The system will be built on a solderless breadboard and run on the
usual USB power. The system may use the on board switches or offboard
switches. A
hardware/software interface will be designed that allows software to control the player. There will be at
least three hardware/software modules: switch input, DAC output, and the music player. The process will
be to design and test each module independently from the other modules. After each module is tested, the
system will be built and tested.

1.3. Roles and Responsibilities: Who will do what? Who are the clients?
The TAs are our clients, and Duc and Brandon are the engineers. Duc will be adding and modifying the
switch interface. Brandon will be doing the same with the timer interface. Both the switch interface and
timer interface were used in the previous labs. Together, we will split the load on the main logic and the
speaker interface as well as deciding what music to load into the board.

1.4. Interactions with Existing Systems: How will it fit in?
The system will use the LM3S1968 board, a solderless breadboard, and the speaker as shown in Figure
5.1. It will be powered using the USB cable. You may use a +5V power from the lab bench, but please do
not power the speaker with a voltage above +5V.

1.5. Terminology: Define terms used in the document.

SSI – alternately called serial peripheral interface (SPI) is used to interface mediumspeed
I/O devices.

Linearity – a measure of straightness of the static calibration curve.

Frequency response – a standard technique to describe the dynamic behavior of linear systems.

Loudness – characteristic of sound that relates to amplitude.

Pitch – property that allows the ordering of sound on a frequencyrelated
scale.

Instrument – an embedded system that collects information, same as data acquisition system.

Tempo – is the speed or pace of music.

Envelope – a smooth curve outlining its extremes in amplitude.

Melody – main series of notes that stand out enabling you to remember the song.

Harmony – employed to decorate the musical effect of the song.

1.6. Security: How will intellectual property be managed?

The system may include software from StellarisWare and from the book. No software written for this
project may be transmitted, viewed, or communicated with any other EE445L student past, present, or
future (other than the lab partner of course). It is the responsibility of the team to keep its EE445L lab
solutions secure.

2. Function Description

2.1. Functionality: What will the system do precisely?
If the operator presses the play/pause button the music will play or pause. If the operator presses the
play/pause button once the music should pause. Hitting the play/pause again causes music to continue. The
play/pause button does not restart from the beginning, rather it continues from the position it was paused.
If the rewind button is pressed, the music stops and the next play operation will start from the beginning.
There is a mode switch that allows the operator to control some aspect of the player. Possibilities include
instrument, envelope or tempo.
There must be a C data structure to hold the music. There must be a music driver that plays songs. The
length of the song should be at least 30 seconds and comprise of at least 8 different sounds. Although you
will be playing only one song, the song data itself will be stored in a separate place and be easy to change.
The player runs in the background using interrupts. The foreground (main) initializes the player, then
executes for(;;){} do nothing loop. If you wish to include OLED output, this output should occur in the
foreground. The maximum time to execute one instance of the ISR is xxxx. You will need public functions
Rewind, Play and Stop, which perform operations like a cassette tape player. The Play function has an
input parameter that defines the song to play. A background thread implemented with output compare will
fetch data out of your music structure and send them to the DAC.
There must be a C data structure to store the sound waveform, or instrument. You are free to design your
own format, as long as it uses a formal data structure (i.e., struct). The generated music must sound
beautiful utilizing the SNR of the DAC. Although you only have to implement one instrument, it should be
easy to change instruments.

2.2. Scope: List the phases and what will be delivered in each phase.
Phase 1 is the preparation; phase 2 is the demonstration; and phase 3 is the lab report. Details can be
found in the lab manual.

2.3. Prototypes: How will intermediate progress be demonstrated?
A prototype system running on the LM3S1968 board and solderless breadboard will be demonstrated.
Progress will be judged by the preparation, demonstration and lab report.

2.4. Performance: Define the measures and describe how they will be determined.
The system will be judged by three qualitative measures. First, the software modules must be easy to
understand and wellorganized.
Second, the system must employ an abstract data structures to hold the
sound and the music. There should be a clear and obvious translation from sheet music to the data
structure. Backward jumps in the ISR are not allowed. Waiting for SSI output to complete is an
acceptable backwards jump. Third, all software will be judged according to style guidelines. Software must
follow the style described in Section 3.3 of the book. There are three quantitative measures. First, the
SNR of the DAC output of a sine wave should be measured. Second, the maximum time to run one
instance of the ISR will be recorded. Third, you will measure power supply current to run the system.
There is no particular need to optimize any of these quantitative measures in this system.

2.5. Usability: Describe the interfaces. Be quantitative if possible.
There will be three switch inputs. The DAC will be interfaced to a 32ohm
speaker.

2.6. Safety: Explain any safety requirements and how they will be measured.
If you are using headphones, please verify the sound it not too loud before placing the phones next to your
ears.

3. Deliverables

3.1. Reports: How will the system be described?
A lab report described below is due by the due date listed in the syllabus. This report includes the final
requirements document.

3.2. Audits: How will the clients evaluate progress?
The preparation is due at the beginning of the lab period on October 2, 2013.

3.3. Outcomes: What are the deliverables? How do we know when it is done?
There are three deliverables: preparation, demonstration, and report. Preparation will be completed as
mentioned above. The demonstration will be completed October 7, and the report will be completed on
October 14, 2013.


7.0 Analysis and Discussion

1) Briefly describe three errors in a DAC.
Gain error is a shift in the slope of Vout (Out A) versus digital input static response. Offset error is a shift
in Vout versus digital input static response. Linearity the a measure of the straightness of the static
calibration curve. Worst case of nonlinearity is nonmonotonic, which mean the analog voltage output
decreases at any particular digital input.

2) Calculate the data available and data required intervals in the SSI/DAC interface. Use these
calculations to justify your choice of SSI frequency.
Data available should overlap data required, hence setup
time and hold time must satisfy. Datasheet
show SCLK pulse width low of 25ns, setup time of 10ns and hold time of 20ns. fssi = fbus/(CPSDVSR
*(1+SCR)) => so 3MHz

3) How is the frequency range of a spectrum analyzer determined?
The lower frequency range of a spectrum analyzer is determine by the SB(sideband) noise of oscillator.
The span of frequency range also depend on the sweeptime.

4) Why did we not simply drive the speaker directly from the DAC? I.e., what purpose is the
MC34119?
The signal from the DAC is too small for the speaker to output. Therefore, we pass it through the
MC34119 amplifier so that we get a larger signal that can be heard.
