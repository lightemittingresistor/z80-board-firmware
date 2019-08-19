#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

#include <fcntl.h>
#include <termios.h>

#define PROTOCOL_START_CODE 0x5A

uint8_t protocol_checksum(unsigned char* bytes, unsigned int length)
{
    uint8_t sum = 0;
    for(unsigned int i = 0; i < length; ++i)
    {
        sum += bytes[i];
    }
    return (~sum)-1;
}

void sendMsg(int serial, uint8_t* buffer, size_t length)
{
    write(serial, buffer, length);
    char result;
    // wait for ack/nak
    if(read(serial, &result, 1) > 0)
    {
        if(result == 0x06)
        {
            printf("Got ACK\n");
        }
        else if(result == 0x15)
        {
            printf("Got NAK\n");
        }
        else
        {
            printf("Got unknown char\n");
        }
    }
    else
    {
        printf("No data\n");
    }
}

int main(int argc, char** argv)
{
    if(argc < 2)
    {
        fprintf(stderr, "Usage: %s <serial port>\n", argv[0]);
        return 1;
    }

    int serial = open(argv[1], O_RDWR | O_NOCTTY | O_NDELAY);
    if(serial < 0)
    {
        fprintf(stderr, "Unable to open %s\n", argv[1]);
    }

    fcntl(serial, F_SETFL, 0);

    struct termios options;
    tcgetattr(serial, &options);
    cfsetospeed(&options, B57600);
    cfsetispeed(&options, B57600);
    options.c_cflag |= (CLOCAL | CREAD);
    tcsetattr(serial, TCSANOW, &options);

    //send ping
    uint8_t buffer[] = {
        PROTOCOL_START_CODE,
        3, // length
        1, // ping
        0, // serial
        0 // checksum
    };
    buffer[4] = protocol_checksum(buffer, 3);

    sendMsg(serial, buffer, 5);
    
    read(serial, buffer, 5);

    if(buffer[2] == 2)
    {
        printf("Got pong\n");
        char ack = 0x06;
        write(serial, &ack, 1);
    }

    close(serial);
    return 0;
}