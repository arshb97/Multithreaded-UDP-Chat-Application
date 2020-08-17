buildTest:
	gcc -std=c11 -pthread -o s-talk instructorList.o mymain.c printthread.c keythread.c sendthread.c recvthread.c -Wall 
