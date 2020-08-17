#define _POSIX_C_SOURCE 200809L         ///tranfer this to makefile

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <netdb.h>
#include <unistd.h>			// for close()
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include "list.h"
#include "keythread.h"

static pthread_t threadPID;     
static int sendSD;      // Socket descriptro
static struct sockaddr_in sendPeer;    // Struct to hold the info of peer computer 
extern List* keySendList;       // Shared List between input thread and Send thread

extern pthread_cond_t keySendCv;
extern pthread_mutex_t keySendMtx;

void* sendCreate()
{
    // printf("inside sendCreate \n");
    char *outBuf;       // To extract items from List
    int numBytes;       // To store the length of bytes transmitted
    char outBufArr[512];   
    struct sockaddr_in *sendPeerPointer = &sendPeer;    //Pointer to the sendPeer struct
    while (1)
    {
        pthread_mutex_lock(&keySendMtx);
        {
            pthread_cond_wait(&keySendCv, &keySendMtx);
            if(List_count(keySendList) > 0)
            {
                outBuf = (char *)List_trim(keySendList);    //Remove the item from list
                strcpy(outBufArr, outBuf);      // Type casting from char * to char[]
                printf("Sending: %s\n", outBufArr);
                numBytes = sendto(sendSD, (const char *) outBufArr, strlen(outBufArr), 0, (struct sockaddr *)sendPeerPointer, sizeof(struct sockaddr_in));      //Sending the data to peer
                if (numBytes < 0) 
                {
                    printf("Error - sendto error: %s\n", strerror(errno));
                    // break;
                }
                // fflush(outBufArr);
            }
        }
        pthread_mutex_unlock(&keySendMtx);
    }
    return NULL;
}

void sendShutdown(void)
{
    // Cancel thread
    pthread_cancel(threadPID);
    // Waits for thread to finish
    pthread_join(threadPID, NULL);
}

void sendthreadInit(int sd, char **argv)
{
    // printf("entering sendthreadInit\n");
    sendSD = sd;

    sendPeer.sin_family = AF_INET;
    sendPeer.sin_port = htons(strtoul(argv[3], NULL, 0));       // Assigning the port
    // sendPeer.sin_addr.s_addr = htonl(inet_ntoa((struct in_addr)temp->sin_addr));        //Maybe wrong
    if (inet_aton(argv[2], &sendPeer.sin_addr) == 0)    // Assigning the IP
    {
        printf("Error - invalid remote address '%s'\n", argv[2]);
        return;
    }
    pthread_create(
        &threadPID,         // PID (by pointer)
        NULL,               // Attributes
        sendCreate,      // Function
        NULL);   
}