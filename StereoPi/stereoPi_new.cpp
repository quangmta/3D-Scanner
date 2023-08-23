#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <linux/types.h>
// #include <wiringPi.h>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <iomanip>

#include "include/lidarlite_v3.h"
#include "include/UART.h"
#include "include/GPIO.h"

#define SERIAL_PORT_PATH "/dev/ttyAMA0"

LIDARLite_v3 myLidarLite;

int g_fd;
const int pin = 17; // GPIO pin 17

// FILE OPERATION

uint8_t crc8(uint8_t *pcBlock, uint8_t len)
{
    uint8_t crc = 0xFF;
    uint8_t i;

    while (len--)
    {
        crc ^= *pcBlock++;
        for (i = 0; i < 8; i++)
        {
            crc = crc & 0x80 ? (crc << 1) ^ 0x31 : crc << 1;
        }
    }
    return crc;
}

int msleep(unsigned int tms)
{
    return usleep(tms * 1000);
}
int step_cmd(int step)
{
    switch (step)
    {
    case 1:
        return 0x02;
    case 2:
        return 0x12;
    case 3:
        return 0x0A;
    case 4:
        return 0x1A;
    case 5:
        return 0x1E;
    default:
        break;
    }
}
int send_cmd_crc8(int g_fd, uint8_t head,uint8_t id,uint8_t cmd)
{
    uint8_t buff[4] = {head, id, cmd, 0x00};
    buff[3] = crc8(buff, sizeof(buff) - 1);
    int flag = write(g_fd, buff, sizeof(buff));
    return flag;
}
float read_distance(uint16_t loop)
{
    uint16_t distance;
    uint8_t  busyFlag;
    uint16_t sum=0,count=0;
    for (uint16_t i = 0;i<loop;i++)
    {
        // Each time through the loop, check BUSY
        busyFlag = myLidarLite.getBusyFlag();

        if (busyFlag == 0x00)
        {
            // When no longer busy, immediately initialize another measurement
            // and then read the distance data from the last measurement.
            // This method will result in faster I2C rep rates.
            myLidarLite.takeRange();
            distance = myLidarLite.readDistance();
            sum+=distance;
            count+=1;
        }
    }
    if (count==0)
    {
        return 0;
    } 
    else
    {
        return float(sum)/count;
    }
    
}
static void demo(int argc, char *argv[])
{
    uint8_t buff_tx[4] = {0x02, 0x00, 0x00, 0x00};
    uint8_t buff_rx[5];
    int id_tx = -1, id_rx = -1;
    uint8_t flag_rx = 0;
    int loops, step, dir, cap, cmd,result;
    char time_now[30],command [80];
    float distance;

    //Read arguments from PC
    //step - 1-180 | 2-90 | 3-45 | 4-22.5 | 5-11.25
    //dir - direction, 0-follow clockwise | 1-counter clockwise:
    //cap - capture images or not
    //loops - number of loop
    step = atoi(argv[1]);
    dir = atoi(argv[2]);
    cap = atoi(argv[3]);
    loops = atoi(argv[4]);
    
    //if capture images and number of arguments >5, then take the time from arg[5] for name of folder
    if(cap && argc > 5)
    {
        strcpy(time_now,argv[5]);
        
        // Initialize i2c peripheral in the cpu core
        myLidarLite.i2c_init();

        // Optionally configure LIDAR-Lite
        myLidarLite.configure(5);

        //enable lidar
        GPIOWrite(pin,HIGH);
        // digitalWrite(pin, HIGH); // Write value of 1 to pin
    }
    
    //create folder
    sprintf(command,"mkdir /media/DCIM/%s",time_now);
    system(command);

    sprintf(command,"/media/DCIM/%s/distance.txt",time_now);
    //open file to save distance
    std::ofstream output_file(command, std::ios_base::app);
    //change step code to cmd code
    cmd = step_cmd(step);
    cmd += dir;
    
    
    if (loops<0)
    {
        //turn on led
        if (abs(loops)%2==1){
            result = send_cmd_crc8(g_fd,0x02,0,0x89+abs((int)(loops/2)));
        }
        //turn off led
        else{
            result = send_cmd_crc8(g_fd,0x02,0,0x81+abs((int)(loops/2)+1));
        }
        return;
    }

    //x2 loop for roll and capture image
    loops = loops*2;

    //start looping
    while (id_tx < loops)
    {
        buff_tx[0] = 0x02;
        buff_tx[1] = ++id_tx;
        flag_rx = 0;
        
        if (id_tx % 2)
        {
            buff_tx[2] = cmd;
        }
        else
        {
            buff_tx[2] = 0x40;
        }

        buff_tx[3] = crc8(buff_tx, sizeof(buff_tx) - 1);
        int tx_fl = write(g_fd, buff_tx, sizeof(buff_tx));

        //transmitted
        if (tx_fl)
        {
            printf("Transmitted ");
            for (int i = 0; i < sizeof(buff_tx); i++)
                printf("%x ", buff_tx[i]);
            printf("\n");

            //only rolling
            if (buff_tx[2] < 0x40)
            {
                msleep(1000);
            }

            //capture images
            else if (buff_tx[2] == 0x40)
            {
                flag_rx = 1;
                msleep(100);
            }

            //rolling and capture images
            else if (buff_tx[2] > 0x40 && buff_tx[2] < 0x80)
            {
                flag_rx = 1;
                msleep(1000);
            }
        }
        //transmit error
        else
        {
            printf("Can't transmit\n");
        }
        //if need to receive strings from stm32
        if (flag_rx)
        {
            int rx_fl = read(g_fd, buff_rx, sizeof(buff_rx));
            //if received 
            if (rx_fl && buff_rx[1] != id_rx)
            {
                //check crc8 code
                uint8_t crc = crc8(buff_rx, sizeof(buff_rx) - 1);
                printf("Received ");
                for (int i = 0; i < sizeof(buff_rx); i++)
                    printf("%x ", buff_rx[i]);
                printf("\n");
                if (crc != buff_rx[sizeof(buff_rx) - 1])
                    printf("CRC8 is not correct!\n");

                //if crc8 is correct
                else
                {
                    //read angle (resolution 1024 ~ 360 cencius)
                    uint16_t angle_int = ((uint16_t)buff_rx[2] << 8) + (uint16_t)buff_rx[3];
                    //float angle = (float)angle_int / 1024 * 360;
                    printf("Angle: %d\n", angle_int);
                    //if capture image
                    if (cap)
                    {
                        //turn on led 1
                        result = send_cmd_crc8(g_fd,0x02,0,0x89);

                        //create folders to contain photos
                        sprintf(command,"mkdir /media/DCIM/%s/%d",time_now,angle_int);
                        system(command);

                        //capture left image
                        sprintf(command, "raspistill -cs 0 -n -t 500 -vf -hf -o /media/DCIM/%s/%d/l.jpg",time_now, angle_int);
                        system("killall -q raspivid");
                        system(command);

                        //capture right image
                        sprintf(command, "raspistill -cs 1 -n -t 500 -vf -hf -o /media/DCIM/%s/%d/r.jpg",time_now, angle_int);
                        system("killall -q raspivid");
                        system(command);
                        system("sudo sync");
                        printf("Captured 2 images!\n\n");
                        distance = read_distance(100);
                        output_file << angle_int << "-"<< std::fixed << std::setprecision(1)<<distance<<std::endl;

                        //turn off led 1
                        result = send_cmd_crc8(g_fd,0x02,0,0x81);
                        //msleep(500);
                    }
                }
                id_rx = buff_rx[1];
            }
        }
        // if(cap) msleep(1500);
        // else msleep(500);
    }
    //captured images! 
    output_file.close();

    //disable lidar
    GPIOWrite(pin,LOW);

    //blink led 2 3 tiems
    for(int i=0;i<3;i++)
    {
        //turn on led 2
        result = send_cmd_crc8(g_fd,0x02,0,0x8A);
        msleep(100);
        //turn off led 2
        result = send_cmd_crc8(g_fd,0x02,0,0x82);
        msleep(100);
    }    
}

int main(int argc, char *argv[])
{
    int flagGPIO;
    flagGPIO = GPIOExport(pin);
    flagGPIO = GPIODirection(pin, OUT);
    flagGPIO = GPIOWrite(pin,LOW);

    open_serial_port();

    configure_serial_port();

    demo(argc, argv);

    close_serial_port();

    return 0;
}
