#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>

#include "part4_uart.h"

int main(void)
{
    int status_fd;
    int data_fd;
    int random_fd;
    unsigned char random_byte;
    Packet pkg;

    /* Seed random number generator */
    srand((unsigned int)time(NULL));
    random_byte = (unsigned char)(rand() % 256);

    /* Create and populate status.reg */
    random_fd = open("status.reg", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (random_fd < 0)
    {
        printf("Failed to create status.reg");
        return 1;
    }


    /* Write 1-byte random entry */
    write(random_fd, &random_byte, 1);
    close(random_fd);

    /* Open simulated status register */
    status_fd = open("status.reg", O_RDONLY);
    if (status_fd < 0)
    {
        printf("Failed to open status.reg");
        return 1;
    }

    /* Open simulated UART output */
    data_fd = open("data.out", O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (data_fd < 0)
    {
        printf("Failed to open data.out");
        close(status_fd);
        return 1;
    }

    /* Initialize packet */
    pkg.start_byte = 0xAA;
    pkg.timestamp = (unsigned long)time(NULL);

    /* Allocate payload memory */
    pkg.payload = m0_allocate_buffer(64);
    if (pkg.payload == NULL)
    {
        close(status_fd);
        close(data_fd);
        return 1;
    }

    /* Fill payload */
    strcpy(pkg.payload, "Code_fz95");

    /* Check UART status */
    if (check_device_ready(status_fd))
    {
        printf("UART device ready\n");
        transmit_packet(data_fd, &pkg);
    }
    else
    {
        printf("UART device NOT ready\n");
    }

    /* Cleanup */
    m0_release_buffer(pkg.payload);
    close(status_fd);
    close(data_fd);
    return 0;
}
