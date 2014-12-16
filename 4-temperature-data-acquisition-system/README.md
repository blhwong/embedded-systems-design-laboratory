4-temperature-data-acquisition-system
===========================

Lab 9 Temperature Data Acquisition System
Duc Tran & Brandon Wong
11/17/2013

1.0 Objectives
The objectives of this lab is to study Analog to digital conversion. Student applied Nyquist
theorem and valvano postulate to determine appropriate sampling rate for the temperature data
acquisition system.

5.0 Analysis and Discussion (give short answers to these questions)

1) What is the Nyquist theorem and how does it apply to this lab?
Nyquist theorem is a sampling theorem. It stated that the sampling rate must be at least two time
the processed signal frequency. In this lab the temperature signal is between 0 to 10 Hz, hence
we need a sampling rate of 20Hz or higher.

2) Explain the difference between resolution and accuracy?
Accuracy depend on resolution in addition to calibration drift.

3) Derive an equation to relate reproducibility and precision of the thermometer.
where s is standard deviation, E is the expected value, X is a random variable, and µ is the
average. We calculated the standard deviation to be roughly 0.098. Therefore each temperature
deviated within 0.1 °C making the system reproduce numbers that are roughly the same.

4) What is the purpose of the LPF?
The purpose of the low pass filter is to pass low frequencies and attenuate the higher
frequencies to prevent antialiasing.

5) If the R versus T curve of the thermistor is so nonlinear, why does the voltage
versus temperature curve look so linear?
Voltage conversion is being linearized and calibrated manually while the low level nonlinear
output must be condition properly for a linear correlation.

6) There are four methods (a,b,c,d) listed in the 4) Software Conversion section of
methods and constraints. For one of the methods you did not implement, give reasons
why your method is better, and give reasons why this alternative method would have
been better.

We did not implement a linear method because the temperature versus thermistor resistance is
not linear rather it is an exponential function. By linearly interpolating values between the
minimum temperature and the maximum temperature would be incorrect and therefore result in
large errors. However, if we took 1024 samples and linearly interpolated the values in between
each sample, then we get much more accurate results in smaller error.
