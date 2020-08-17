#ifndef _KEYTHREAD_H_
#define _KEYTHREAD_H_

List* keySendList;

void keyThreadInit();

// Stop background receive thread and cleanup
void keythreadShutdown();

#endif