# Multithreaded UDP Chat Application
Created a simple "chat"-like facility that enables someone at one terminal (or Xterm) to communicate with someone at another terminal.


# Process Structure
This program is done using pthreads, a kernel-level thread implementation for LINUX. As you may or may not know, pthreads allows you to create any number of threads inside one UNIX process. All threads running in the same UNIX process share memory (which means pointers valid for one thread are valid in another thread) and also have access to semaphores (mutexes) and the ability to use conditional signal/wait to synchronize their actions in relation to each other. UNIX itself also allows you to create multiple processes. Communication between UNIX processes can be done using something called "datagram sockets" which use a protocol called UDP (universal datagram protocol).
In this program, we will be dealing with processes/threads on two levels. First, we will have two UNIX processes. Each one is started by one of the people who want to talk (as a result of executing s-talk). Second, within each s-talk process a pthreads environment will be running four threads:

- One of the threads does nothing other than await input from the keyboard (keythread.c)
- The other thread does nothing other than await a UDP datagram (recvthread.c)
- There will also be a thread which prints characters to the screen (printthread.c)
- Finally a thread which sends data to the remote UNIX process over the network using UDP (sendthread.c)

All four threads will share access to a list ADT. Clearly the lists that are shared between the threads will need to be carefully controlled to prevent concurrent access. This will be done by using mutexes and signalling/waiting on condition variables.

# Running

Say that Fred and Barney want to talk. Fred is on machine "csil-cpu1" and will use port number 6060. Barney is on machine "csil-cpu3" and will use port number 6001. To initiate s-talk, Fred must type:

```shell
$ s-talk 6060 csil-cpu3 6001
```
And Barney must type:
```shell
$ s-talk 6001 csil-cpu1 6060
```

So, (in case you haven't figured it out) the general format is:
```shell
$ s-talk [my port number] [remote machine name] [remote port number]
```

The result will be that every line typed at each terminal will appear on BOTH terminals: it will appear character-by-character on the sender’s terminal as they type it, and it will appear on the receiver’s terminal once the sender presses enter.

