#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

#include "include/UART.h"

struct termios g_tty;

int file_open_and_get_descriptor(const char *fname)
{
    int fd;

    fd = open(fname, O_RDWR | O_NONBLOCK);
    if (fd < 0)
    {
        printf("Could not open file %s...%d\r\n", fname, fd);
    }
    return fd;
}

// static int file_write_data(int fd, uint8_t *buff, uint32_t len_buff)
// {
//     return write(fd, buff, len_buff);
// }

// static int file_read_data(int fd, uint8_t *buff, uint32_t len_buff)
// {
//     return read(fd, buff, len_buff);
// }

// static int file_close(int fd)
// {
//     return close(fd);
// }

void open_serial_port(void)
{
    g_fd = file_open_and_get_descriptor(SERIAL_PORT_PATH);
    if (g_fd < 0)
    {
        printf("Something went wrong while opening the port...\r\n");
        exit(EXIT_FAILURE);
    }
}

void configure_serial_port(void)
{
    if (tcgetattr(g_fd, &g_tty))
    {
        printf("Something went wrong while getting port attributes...\r\n");
        exit(EXIT_FAILURE);
    }

    cfsetispeed(&g_tty, B115200);
    cfsetospeed(&g_tty, B115200);

    cfmakeraw(&g_tty);

    if (tcsetattr(g_fd, TCSANOW, &g_tty))
    {
        printf("Something went wrong while setting port attributes...\r\n");
        exit(EXIT_FAILURE);
    }
}

void close_serial_port(void)
{
    close(g_fd);
}