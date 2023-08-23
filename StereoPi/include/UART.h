#ifndef UART_h
#define UART_h

#define SERIAL_PORT_PATH "/dev/ttyAMA0"

extern int g_fd;

int file_open_and_get_descriptor(const char *fname);
int file_write_data(int fd, uint8_t *buff, uint32_t len_buff);
static int file_read_data(int fd, uint8_t *buff, uint32_t len_buff);
int file_close(int fd);
void open_serial_port(void);
void configure_serial_port(void);
void close_serial_port(void);

#endif