#ifndef _PRINTTHREAD_H_
#define _PRINTTHREAD_H_

// Start background receive thread
void printthreadInit();

// Stop background receive thread and cleanup
void printShutdown(void);

#endif