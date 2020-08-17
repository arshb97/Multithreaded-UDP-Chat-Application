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
#include "recvthread.h"
#include "sendthread.h"
#include "printthread.h"

static pthread_t threadPID;
#define MSG_MAX_LEN 512     //Maximum message length that could be received
List* keySendList;          // Shared List between input thread and send thread
pthread_cond_t keySendCv = PTHREAD_COND_INITIALIZER;
pthread_mutex_t keySendMtx = PTHREAD_MUTEX_INITIALIZER;
pthread_t keyThreadId;

void* keyCreate()
{
    char userInput[MSG_MAX_LEN];        // To hold User Input
    keyThreadId = threadPID;
    while (fgets(userInput, MSG_MAX_LEN, stdin))
    {
        if (strncmp(userInput, "!", 1) == 0)
        {
            pthread_mutex_lock(&keySendMtx);
            {
                List_prepend(keySendList, &userInput);       
                pthread_cond_signal(&keySendCv);
            }
            pthread_mutex_unlock(&keySendMtx);
            printf("Ending the conversation.....BYE\n");       // Ending the conversation because we received !
            keythreadShutdown();
            break;
        }
        else
        {
            pthread_mutex_lock(&keySendMtx);
            {
                List_prepend(keySendList, &userInput);         // Add it to the list and notify send
                pthread_cond_signal(&keySendCv);
            }
            pthread_mutex_unlock(&keySendMtx);
            printf("\n");
        }   
    }    return NULL;
}
void keythreadShutdown()
{
    pthread_join(threadPID, NULL);
    // printf("starting keythreadshutdown\n");
    sleep(2);
    sendShutdown();
    sleep(2);
    recvthreadShutdown();
    sleep(2);
    printShutdown();
    // Cancel thread
    pthread_cancel(threadPID);
    // Waits for thread to finish
    pthread_join(threadPID, NULL);
    // printf("exiting keythreadshutdown()\n");
}

void keyThreadInit(int sd, struct sockaddr_in *myPeer)
{
    // printf("Inside keythreadINIT\n");
    keySendList = List_create();            // Creating the list shared between input and send threads.
    pthread_create(
        &threadPID,         // PID (by pointer)
        NULL,               // Attributes
        keyCreate,      // Function
        NULL);   
}
