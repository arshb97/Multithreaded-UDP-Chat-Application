
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
static int receiveSD;       // Socket descriptor
List *recvPrintList;
pthread_cond_t sendPrintCv = PTHREAD_COND_INITIALIZER;
pthread_mutex_t sendPrintMtx = PTHREAD_MUTEX_INITIALIZER;
extern pthread_t keyThreadId;

void* recvCreate()
{
    int numBytes;       // To store total number of bytes received
    int temp;           // Variable to hold return values from List_insert()
    char inputBuf[512];     // Char Array to store the recevied message
    while(1)
    {   
            numBytes = recvfrom(receiveSD, inputBuf, sizeof(inputBuf),
                            0, NULL, NULL);
                pthread_mutex_lock(&sendPrintMtx);
                {
                    if (numBytes < 0) 
                    {
                        printf("Error - recvfrom error: %s\n", strerror(errno));
                        break;
                    }
                    if (numBytes > 0) 
                    {
                        temp = List_insert(recvPrintList, inputBuf);
                        if (strncmp(inputBuf, "!", 1) == 0)
                        {
                            printf("Received in receive ! Ending the conversation. BYE \n");
                            pthread_cancel(keyThreadId);
                            keythreadShutdown();
                        }
                        pthread_cond_signal(&sendPrintCv);      //Notifying print to go ahead and print
                        if(temp == -1)
                        {
                            printf ("Failed to insert item in recvCreate\n");
                        }
                    }
                }
                pthread_mutex_unlock(&sendPrintMtx);
    }
    return NULL;
}

void recvthreadShutdown(void)
{
    // Cancel thread
    pthread_cancel(threadPID);
    // Waits for thread to finish
    pthread_join(threadPID, NULL);
}

void recvthreadInit(int sd)
{
    // printf("Inside receivethreadInit()\n");
    recvPrintList = List_create();      // Creatring the list shared between receive thread and print thread
    receiveSD = sd;
    pthread_create(
        &threadPID,         // PID (by pointer)
        NULL,               // Attributes
        recvCreate,      // Function
        NULL);   
}