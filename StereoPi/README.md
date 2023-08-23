# 3D scanner
The program is used to control 3D scanners on StereoPi. The main task of the program is to synchronize the shooting, read the rotation angle sensor through the UART, read the LiDAR distance sensor and transmit the stepper motor control command.  The parameters of the rotation step, the total angle of rotation, the direction of rotation are transmitted when executing the program.
To compile the program, execute the following command line:
```
cd media/Program
g++ -o main stereoPi_new.cpp source/lidarlite_v3.cpp source/GPIO.cpp source/UART.cpp -I .
```
To run the program, for example to read current angle, execute the following command line:
```
media/Program/main 0 0 0 0
```
