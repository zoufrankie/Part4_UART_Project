#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "part4_uart.h"


/* Allocate memory */

char* m0_allocate_buffer(size_t size)
{
    char* buffer;

    buffer = (char*)malloc(size);

    if (buffer == NULL)
    {
        perror("malloc failed");

        return NULL;
    }

    return buffer;
}


/* Release memory */

void m0_release_buffer(char* ptr)
{
    free(ptr);
}


/* Check UART status register */

int check_device_ready(int status_fd)
{
    unsigned char status;

    /* Read from beginning of file */
    lseek(status_fd, 0, SEEK_SET);

    if (read(status_fd, &status, 1) != 1)
    {
        perror("Failed to read status.reg");

        return 0;
    }

    /* Check 3rd least significant bit */
    if (status & 0x04)
    {
        return 1;
    }

    return 0;
}


/* Transmit packet */

void transmit_packet(int data_fd, Packet* pkg)
{
    /* Write start byte */
    write(data_fd, &pkg->start_byte, sizeof(unsigned char));

    /* Write timestamp */
    write(data_fd, (char*)&pkg->timestamp, sizeof(unsigned long));

    /* Write payload */
    write(data_fd, pkg->payload, strlen(pkg->payload));

    printf("Packet transmitted successfully\n");
}
