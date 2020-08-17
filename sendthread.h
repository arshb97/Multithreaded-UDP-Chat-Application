#ifndef _SENDTHREAD_H_
#define _SENDTHREAD_H_

// Start background receive thread
// void Receiver_init(char* rxMessage, pthread_mutex_t sharedMutexWithOtherThread);
void sendthreadInit(int sd, char **argv, struct sockaddr_in *temp);

// Stop background receive thread and cleanup
void sendShutdown(void);

#endif