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

extern List *recvPrintList;
static pthread_t threadPID;
extern pthread_cond_t sendPrintCv;
extern pthread_mutex_t sendPrintMtx;

void* printCreate()
{
    // printf("entering printCreate()\n");
    char *printBuf;
    while (1)
    {
        pthread_mutex_lock(&sendPrintMtx) ;
        {
            pthread_cond_wait(&sendPrintCv, &sendPrintMtx);     //Wait until item is added ot the list in receive thread
            if(List_count(recvPrintList) > 0)
            {
                printBuf = (char *) List_remove(recvPrintList);
                printf("Received:");
                puts(printBuf);
            }
        }
        pthread_mutex_unlock(&sendPrintMtx);
    }
    return NULL;
}

void printShutdown()
{
    // printf("starting printshutdown\n");
    // Cancel thread
    pthread_cancel(threadPID);
    // Waits for thread to finish
    pthread_join(threadPID, NULL);
    // printf("exiting printshutdown()\n");
}

void printthreadInit()
{
    // printf("Inside printthreadInit()\n");
    pthread_create(
        &threadPID,         // PID (by pointer)
        NULL,               // Attributes
        printCreate,      // Function
        NULL);   
}