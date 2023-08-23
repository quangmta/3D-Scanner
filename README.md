# 3D-Scanner
The 3D scanning system consists of two parts:
- The upper part is a rotary platform with cameras and a StereoPi module. The cameras are installed in such a way that their lenses are directed parallel, and the distance between them is 65 mm.
- The lower part consists of a stepper motor, an angle sensor, a power supply system and a rotation control board based on the STM32 microcontroller.
The system rotates and takes images from two cameras at different angles to get images of all points of the environment. Commands to control the rotating platform are sent from Stereo Pi to STM32 via the UART protocol. The rotating angle data is transmitted from the STM32 to the Stereo Pi. The captured images are sent to a computer to create a 3D model.
This repository consits of 3 folder:
 - Local: Program in computer to controll 3D scanner via SSH
 - StereoPi: Program in StereoPi
 - USB_and_Motor_and_Simple_qt_Terminal-master: Program in STM32