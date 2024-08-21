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
#include "practical.h"

void PrintSocketAddress(const struct sockaddr *address, FILE *stream);

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        exitWithUserMessage("Parameters(s)", "<Server Address> <Port range start> <Port range end>");
    }

    char *servIP = argv[1];
    in_port_t portStart = atoi(argv[2]);
    in_port_t portEnd = atoi(argv[3]);

    printf("Starting port - %d", portStart);
    printf("Ending port %d", portEnd);

    struct addrinfo addrCriteria;                       // Criteria for address match.
    memset(&addrCriteria, 0, sizeof(addrCriteria));     // Zero out structure.
    addrCriteria.ai_family = AF_UNSPEC;                 // v4 or v6 is ok.
    addrCriteria.ai_socktype = SOCK_STREAM;             // Only streaming socket.
    addrCriteria.ai_protocol = IPPROTO_TCP;             // Only TCP protocol.
    //addrCriteria.ai_flags = AI_NUMERICHOST;

    // Creating a addrinfo struct to get addresses of given host/service.
    struct addrinfo *addrList;

    int ret = getaddrinfo(servIP, argv[2], &addrCriteria, &addrList);
    if (ret != 0)
    {
        exitWithUserMessage("getaddrinfo() failed", gai_strerror(ret));
    }

        // Display returned addresses.
    for (struct addrinfo *addr = addrList; addr != NULL; addr = addr->ai_next)
    {
        PrintSocketAddress(addr->ai_addr, stdout);
        fputc('\n', stdout);
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
    int sock_fd = -1;/*
    for (struct addrinfo *addr = addrList; addr != NULL; addr = addr->ai_next)
    {
        sock_fd = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
        if (sock_fd < 0)
        {
            continue;
        }
        if (connect(sock_fd, addr->ai_addr, addr->ai_addrlen) == 0)
        {

        }
    }
   int sock_fd = -1;
   struct addrinfo *addr = addrList;
   struct sockaddr_in *sockAddr = addr->ai_addr;
   sockAddr->sin_port = htons(5000);*/
    // Connecting to given server.
    for (in_port_t i = portStart; i <= portEnd; i++)
    {
        // Creating a reliable stream socket using TCP.
        sock_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (sock_fd < 0)
        {
            exitWithSystemMessage("socket(), failed");
        }
        
        servAddr.sin_port = htons(i);

        if (!connect(sock_fd, (struct sockaddr *)&servAddr, servAddrLen))
        {
            printf("Open port: %d\n", i);
        }
        close(sock_fd);
    }
    
    freeaddrinfo(addrList);
    return 0;
}

void PrintSocketAddress(const struct sockaddr *address, FILE *stream)
{
    if (address == NULL || stream == NULL)
    {
        return;
    }

    void *numericAddress;                   // pointer to binary address.
    in_port_t port;                         // Port to print.
    
    // Set pointer to address based on address-family
    switch (address->sa_family)
    {
        case AF_INET:
        {
            numericAddress = &((struct sockaddr_in *) address)->sin_addr;
            port = ntohs(((struct sockaddr_in *) address)->sin_port);
            break;
        }
        case AF_INET6:
        {
            numericAddress = &((struct sockaddr_in6 *) address)->sin6_addr;
            port = ntohs(((struct sockaddr_in6 *) address)->sin6_port);
            break;
        }
        default:
        {
            fputs("[unknown type]", stream);
            return;
        }
    }
    // Convert binary to printable address.
    char addrBuffer[INET6_ADDRSTRLEN];      // Buffer to contain result.
    if (inet_ntop(address->sa_family, numericAddress, addrBuffer, sizeof(addrBuffer)) == NULL)
    {
        fputs("[invalid address]", stream);
    }
    else
    {
        fprintf(stream, "%s", addrBuffer);
        if (port != 0)
        {
            fprintf(stream, "-%u", port);
        }
    }
}