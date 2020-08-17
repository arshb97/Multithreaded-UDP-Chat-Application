#ifndef _RECVTHREAD_H_
#define _RECVTHREAD_H_

// Start background receive thread
// void Receiver_init(char* rxMessage, pthread_mutex_t sharedMutexWithOtherThread);
void recvthreadInit(int sd);

// Stop background receive thread and cleanup
void recvthreadShutdown(void);

#endif