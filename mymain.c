#define _POSIX_C_SOURCE 200809L  

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <netdb.h>
#include <unistd.h>			// for close()
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <errno.h>
#include <poll.h>
#include "list.h"
#include "sendthread.h"
#include "keythread.h"
#include "recvthread.h"
#include "printthread.h"

// To run ./s-talk 6000 localhost 6060
int main(int argc, char** argv)
{
    printf("starting my main()\n");
    int socketDescriptor;
    struct addrinfo hints, *result = NULL;
    struct sockaddr_in serverSide;
    struct sockaddr_in peerSide;
    struct sockaddr_in *temp;
    int s;
    /* Obtain address(es) matching host/port */
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_DGRAM; /* Datagram socket */
    hints.ai_flags = 0;
    hints.ai_protocol = 0;          /* Any protocol */

    s = getaddrinfo(argv[2], argv[3], &hints, &result);
    temp = (struct sockaddr_in*)result->ai_addr;        // Obtaining the IP 
   
    peerSide.sin_port = htons(strtoul(argv[3], NULL, 0));
    peerSide.sin_family = AF_INET;
    argv[2] = inet_ntoa((struct in_addr)temp->sin_addr);

    if (inet_aton(argv[2], &peerSide.sin_addr) == 0)        // Assigning IP
    {
        printf("Error - invalid remote address '%s'\n", argv[2]);
        return 1;
    }
    // Creating a socket descriptor
    socketDescriptor = socket(AF_INET, SOCK_DGRAM, 0);
    if(socketDescriptor < 0){
        printf("error opening socket\n");
        return 1;
    }
    serverSide.sin_family = AF_INET;
    serverSide.sin_port = htons(strtoul(argv[1], NULL, 0));     //Specifying the Port
    serverSide.sin_addr.s_addr = htonl(INADDR_ANY);

    // Binding the socket Descriptor
    if (bind(socketDescriptor, (struct sockaddr *)(&serverSide), sizeof(serverSide)) < 0) 
    {
        printf("Error - failed to bind socket: %s\n", strerror(errno));
        return 1;
    }
    // printf("calling keyInit Thread\n");
    keyThreadInit();
    recvthreadInit(socketDescriptor);
    sendthreadInit(socketDescriptor, argv, temp);
    printthreadInit();
    keythreadShutdown();
    close(socketDescriptor);
    freeaddrinfo(result);
    free(result);
    // free(temp);
    return 0;
}

   