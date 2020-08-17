# Multithreaded UDP Chat Application
Created a simple "chat"-like facility that enables someone at one terminal (or Xterm) to communicate with someone at another terminal.

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
