#ifndef PART4_UART_H
#define PART4_UART_H

#include <stddef.h>

typedef struct
{
    unsigned char start_byte;
    unsigned long timestamp;
    char* payload;
} Packet;

/* Memory management */
char* m0_allocate_buffer(size_t size);
void m0_release_buffer(char* ptr);

/* UART simulation */
int check_device_ready(int status_fd);
void transmit_packet(int data_fd, Packet* pkg);

#endif
