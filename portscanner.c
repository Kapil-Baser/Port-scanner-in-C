#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        //printf("Parameters(s)", "<Server Address> <Port range start> <Port range end>");
        return -1;
    }

    char *servIP = argv[1];
    in_port_t portStart = atoi(argv[2]);
    in_port_t portEnd = atoi(argv[3]);

    printf("Starting port - %d", portStart);
    printf("Ending port %d", portEnd);
    return 0;
}