# IPK 2023 Project 1
### Author: Daniel Blaško (xblask05)
<hr>

## How to build and run
```
$ make
$ ./ipkcpc -h <host> -p <port> -m <mode>
```

`$ make clean` to remove build files

**host** - IPv4 address of the server the program connects to \
**port** - server port \
**mode** - connection mode (only "tcp"/"udp") 

The order of the arguments is interchangeable

## Implementation details
The ipkcpc script has been written in the C language. After launching it, the input arguments are checked for their validity. Based on the mode that was chosen by the user, the program connects to the desired server in said mode. The program enters a while cycle processing user's input, sending the input to the server and receiving and printing the output from the server. 
In TCP mode, the input is sent directly to the server, while in UDP mode the input is reformatted to the IPK Calculator protocol specification (a null byte at the front of the string, followed by a byte containing the length of the string and then the string itself). After receiving the server's answer in UDP mode, the opcode and status code are checked and OK/ERR messaged are displayed.

The program uses `signal`, `sig_handler_tcp` and `sig_handler_udp` functions to tackle SIGINT raising and exit the program gracefully. \
`sig_handler_tcp` sends a "BYE" message to the server after being called by the `signal` function, waits for the server to answer with "BYE" and closes connection. \
`sig_handler_udp` closes connection after being called by the `signal` function.

UDP communication has an answer timeout set at 10 seconds using the `sys/time.h` header file and the `setsockopt` function.

## Issues
1. Program is only compatible with UNIX systems, I have included necessary Windows specific header files, but I was not able to make the program run on this system
2. Although it was not recommended in the project discussion forum, I have made the `client_socket` variable into a global one to make connection closing possible in functions handling a SIGINT raise

## Repository changelog
commit 7d844f017d06aea6eda3baed23d280d3a7488343
Author: Daniel Blasko <xblask05@stud.fit.vutbr.cz>
Date:   Tue Mar 21 19:30:16 2023 +0100

    fix: Small changes to README and CHANGELOG

commit 78d2e382fc7f314f48321253300fde44512a89e0
Author: Daniel <xblask05@stud.fit.vutbr.cz>
Date:   Tue Mar 21 18:07:55 2023 +0000

    feat: added test files, updates to README and CHANGELOG
    fix: fgets input loading, argument parsing, error messages

commit 8dad1ee242a636e2e7318da864f3e2ec414a3793
Author: Daniel <xblask05@stud.fit.vutbr.cz>
Date:   Mon Mar 20 11:20:25 2023 +0000

    feat: UDP Timeout and input length control
    fix: Removed client "BYE" print from SIGINT handler

commit d02887096bed4d268f0b1f5390ed782c50083546
Author: Daniel Blasko <xblask05@stud.fit.vutbr.cz>
Date:   Mon Mar 20 00:19:32 2023 +0100

    System-specific header files added, updated README, added CHANGELOG file

commit 8b88320d2f737ca37e190f7f55a8bcef7845a0ba
Author: Daniel Blasko <xblask05@stud.fit.vutbr.cz>
Date:   Sun Mar 19 23:44:31 2023 +0100

    Renamed program file, added license, Readme update

commit bb30b9c13274bbc94b018dd42051ce0f4c42d646
Author: Daniel Blasko <xblask05@stud.fit.vutbr.cz>
Date:   Sun Mar 19 23:25:48 2023 +0100

    Readme update

commit abdeda358c8105aedee6417d14060ffb55906f0d
Author: Daniel Blasko <xblask05@stud.fit.vutbr.cz>
Date:   Sun Mar 19 23:18:32 2023 +0100

    Readme updated, small code fixes

commit a7b8c691a3e031b16696ed0fdad894ac2058c7ed
Author: Daniel <xblask05@stud.fit.vutbr.cz>
Date:   Sun Mar 19 21:29:06 2023 +0000

    bzero and bcopy functions replaced due to their legacy status

commit 1689d1a778429b653ad313924ca9776119a1af92
Author: Daniel <xblask05@stud.fit.vutbr.cz>
Date:   Sat Mar 18 20:32:02 2023 +0000

    SIGINT handling added

commit e33f485325778abc532d137ebbfdbf5ef92f3c35
Author: Daniel Blasko <xblask05@stud.fit.vutbr.cz>
Date:   Fri Mar 17 23:59:43 2023 +0100

    UDP functionality added, updated comments

commit 76de04048c9f63f1094896ca33eb71fe3fe937b6
Author: Daniel Blasko <xblask05@stud.fit.vutbr.cz>
Date:   Thu Mar 16 21:40:22 2023 +0100

    Corrected program name, testing interface compatibility fix

commit 2cbcfb0f61d5c624e1ae306b7872995d1289b42d
Author: Daniel Blasko <xblask05@stud.fit.vutbr.cz>
Date:   Tue Mar 14 17:17:09 2023 +0100

    Initial TCP functionality, UDP Work in Progress

commit 4b4024f9d5b50e7ac43c6ebc0677fdd0c71866b4
Author: Daniel Blasko <xblask05@stud.fit.vutbr.cz>
Date:   Tue Mar 14 17:09:56 2023 +0100

    first commit