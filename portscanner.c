#define _XOPEN_SOURCE 700   // Needed to add this because VS code is not picking up addrinfo struct from netdb.h.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        exitWithUserMessage("Parameters(s)", "<Server Address> <Port range start> <Port range end>");
        return -1;
    }

    char *servIP = argv[1];
    in_port_t portStart = atoi(argv[2]);
    in_port_t portEnd = atoi(argv[3]);

    printf("Starting port - %d", portStart);
    printf("Ending port %d", portEnd);

    struct addrinfo addrCriteria;
    memset(&addrCriteria, 0, sizeof(addrCriteria));
    addrCriteria.ai_family = AF_UNSPEC;
    addrCriteria.ai_socktype = SOCK_STREAM;
    addrCriteria.ai_protocol = IPPROTO_TCP;

    // Creating a addrinfo struct to get addresses of given host/service.
    struct addrinfo *addrList;

    int ret = getaddrinfo(servIP, argv[2], &addrCriteria, &addrList);
    if (ret != 0)
    {
        exitWithUserMessage("getaddrinfo() failed", gai_strerror(ret));
    }
    // Creating a reliable stream socket using TCP.
    int sock_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock_fd < 0)
    {
        exitWithSystemMessage("socket(), failed");
    }

    struct sockaddr_in servAddr;
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    socklen_t servAddrLen = sizeof(servAddr);
    // Converting the IP address which is in string to it's binary network format.
    int retVal = inet_pton(AF_INET, servIP, &servAddr.sin_addr.s_addr);
    if (retVal == 0)
    {
        exitWithUserMessage("inet_pton() failed", "invalid address string");
    }
    else if (retVal < 0)
    {
        exitWithSystemMessage("inet_pton(), failed");
    }

    // TODO: Connecting to given server.
    for (int i = portStart; i <= portEnd; i++)
    {
        servAddr.sin_port = htons(i);
        if (!connect(sock_fd, (struct sockaddr *)&servAddr, servAddrLen))
        {
            printf("Open port: %d\n", i);
        }
    }
    
    freeaddrinfo(addrList);
    return 0;
}