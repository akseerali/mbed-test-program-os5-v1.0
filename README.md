# mbed-test-program-os5-v1.0

- This is an example program to test the working of Mbed LPC1768 microcontroller. In this program, the controller prints the measurements of on-board temperature 
and accelerometer sensors on serial monitor, updates the status of buzzer, and also shows the status of all the leds (LED1, LED2, LED3, LED4, Red led, Green led, & 
Blue led) with a sampling rate of 5 seconds. The graphical orientation data of data obtained from the on-board accelerometer is printed on on-board LCD as a bubble and the numerial readings of X/Y axis are printed on serial monitor. While the four leds (LED1 to Led4) are programmed to ON & OFF in the loop, the Red, Green, & Blue leds are turned ON/OFF based on the rising edge interrupt of joystick. This program also prints the status of joystick buttons on serial monitor whenever any joystick button is pressed.

- Libraries used in this program
  - mbed.h --> main library of microcontroller
  - MMA7660.h --> library for on-board accelerometer
  - LM75B.h --> library for on-board temperature sensor
  - C12832.h --> library for on-board LCD
