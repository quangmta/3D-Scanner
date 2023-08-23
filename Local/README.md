# Control 3D Scanner via SSH 
The 3D printer is controlled via SSH. This API provides the ability to control the functions of the 3D scanner.
# Prepair
To control the scanner, run the file ***main.py***. Pay attention to the necessary variables before running the program (change if necessary):
1. **host** - IP address of 3D scanner
2. **username, password** - login and Mat father crab mat 3D scan
3. **path** - the path to where the file runs on a 3D scanner
4. **localpath** - the path to where data is stored on the computer

# Features
The program will connect to a 3D scanner, compile and run code files in the scanner with the entered variables . The first will read the scanner's original angle of rotation position. Then a menu panel will appear to select the increments of the scanner:
+ **1** - Goto offset position
+ **2** - Capture images in current position
+ **3** - Capture images of the surrounding scenes 
+ **4** - Manual rotate: input the angle and the direction of rotation
+ **0** - Exit

When scanning the surroundings, we will enter the scan angle step (11.25, 22.5, 45, 90 or 180 degrees), the total scan angle and the direction of rotation (clockwise or counterclockwise).
# Example
Capture images of all surrounding scenes (360 degrees) with minimum step (11.25 degrees) and follow clockwise:
```
Step: 1-180 | 2-90 | 3-45 | 4-22.5 | 5-11.25: 5
Direction: 0-follow clockwise | 1-counter clockwise: 0
Total angle: 360
```
The folder containing the data is named by the current time __%d%M%Y-%H%M__. Inside the photos taken from 2 cameras are stored under the folder named by the value of the rotatation sensor (resolution 1024 ~360 degrees). Examples:
```Shell
├── 20072023-1200
    distance.txt
    ├── 5
        ├── l.jpg
        ├── r.jpg
    ├── 38
        ├── l.jpg
        ├── r.jpg
    ├── 70
        ├── l.jpg
        ├── r.jpg
    ...

```


