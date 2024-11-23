# Vexbox Controller

Vexbox Controller was designed to solve a major problem: I forgot my team's controller on the field at school. Without being able to drive the robot, I'm rather limited on what I can test and do easily.

## Usage

Vexbox should be a drop-in replacement for your real controller. Just copy and paste the controller entry, and replace `pros::Controller` with `VexboxController`.


> [!CAUTION]
> I HIGHLY RECOMMEND COMMENTING OUT THE VEXBOX CONTROLLER WHEN IT'S NOT IN USE TO PREVENT ANY ISSUES THAT MAY ARISE FROM WEIRD CONFLICTIONS BETWEEN THE PROS CONTROLLER AND THIS VEXBOX CONTROLLER. 

> [!IMPORTANT]  
> Note that Vexbox uses a preflix of `xbox` before every line relating to the controller (since I used an xbox controller to build this). Therefore, if you do for some ofhandedly weird reason have a line of code that prints xbox, you should disable or remove it before using Vexbox, or edit both the brain-side implementation and the python script to use a different line preflix.

> [!WARNING]  
> While all member functions are implemented, get new presses may not work as expected or at all. Please if you do run into issues, please open an issue so I can fix it. 

> [!NOTE]  
> The Python script does not send any battery information about the controller to the brain, and the charge is instead hardcoded into the implementation. The Python script also uses multithreading to print any output from the brain into the console.

## Usage over Smartport
I don't have the hardware to create RS-282 connections, but if you wish to use smartport, it will require editing the source code of both the python file and Vexbox's source code.