# IPK 2023 Project 1
### Author: Daniel Blaško (xblask05)
<hr>

## Results: 16/20
* Issues with mangled input and missing theory in documentation

## Description
Client implementation of the IPK Calculator Protocol using TCP and UDP connection protocols, written in the C programming language.

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

## Usage
Query formatting:
```
operator = "+" / "-" / "*" / "/"
expr = "(" operator 2*(SP expr) ")" / 1*DIGIT
query = "(" operator 2*(SP expr) ")"
```

#### TCP Mode 
1. Run the application with `-m tcp` argument
2. Begin the initiation state with a "HELLO" greeting
3. Send a query in a `SOLVE query` format
4. Close the connection and terminate the program with the "BYE" message or by calling the SIGINT signal by pressing `ctrl+c`

#### UDP Mode
1. Run the application with `-m udp` argument 
2. Send a query in a `query` format
3. Close the connection and terminate the program by raising the SIGINT signal by pressing `ctrl+c`

## Example
```
./ipkcpc -h localhost -p 2023 -m tcp

C: HELLO
S: HELLO
C: SOLVE (+ 2 3)
S: RESULT 5
C: BYE
S: BYE
```

```
./ipkcpc -h localhost -p 2023 -m udp

C: (+ 2 3)
S: OK: 5
C: (/ 4 0)
S: ERR:<Error message>
```

## Used libraries
```
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/time.h>
```

Unix specific:
```
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
```

Windows specific:
```
#include <winsock2.h>
#include <windows.h>
#include <io.h>
```

## Sources
Vladimír Veselý, DemoTcp [online]. Publisher: Brno University of Technology, Faculty of Information Technology, January 30th 2023. [cit. 2023-03-21]. Available at: https://git.fit.vutbr.cz/NESFIT/IPK-Projekty/src/branch/master/Stubs/cpp/DemoTcp. 

Vladimír Veselý, DemoTcp [online]. Publisher: Brno University of Technology, Faculty of Information Technology, January 30th 2023. [cit. 2023-03-21]. Available at: https://git.fit.vutbr.cz/NESFIT/IPK-Projekty/src/branch/master/Stubs/cpp/DemoUdp/client.c 

Michal Koutenský, IPK Client Protocol [online]. Publisher: Brno University of Technology, Faculty of Information Technology, January 30th 2023. [cit. 2023-03-21]. Available at: https://git.fit.vutbr.cz/NESFIT/IPK-Projekty/src/branch/master/Project%201/Protocol.md 

Dirk Eddelbuettel, Catch Ctrl-C in C [online]. Publisher: Stack Overflow, November 18th 2010. [cit. 2023-03-21]. Available at: https://stackoverflow.com/a/4217052 

Neal, UDP Socket Set Timeout [online.] Publisher: Stack Overflow, November 25th 2012. [cit. 2023-03-21]. Available at: https://stackoverflow.com/a/13547864

## Testing examples
OS: NixOS 22.11 (Reference testing machine) \
Platform: AMD64
### TCP

1. OK \
Input:
```
HELLO
SOLVE (+ 1 2)
SOLVE (/ 1 4)
SOLVE (- 1 8)
SOLVE (* 4 5)
BYE
```
Output:
```
HELLO
RESULT 3
RESULT 1/4
RESULT -7
RESULT 20
BYE
```

2. Incorrect syntax \
Input:
```
HELLO
SOLVE (_ 1 2)
BYE
```
Output:
```
HELLO
BYE
```

3. SIGINT raise \
Input:
```
HELLO 
SOLVE (+ 1 2)
C-c
```
Output:
```
HELLO
RESULT 3
BYE
```

### UDP

1. OK \
Input:
```
(+ 1 2)
(/ 0 4)
(* 3 4)
(- 4 5)
```
Output:
```
OK: 3
OK: 0
OK: 12
OK: -1
```

2. Mixed correct and incorrect syntax \
Input:
```
(+ 1 2)
(/ 4 0)
(* 0 0)
(_ 1 3)
(a b)
Lorem Ipsum
()
(a)
(1)
(- 1 4)
```
Output:
```
OK: 3
ERR: Could not parse the message
OK: 0
ERR: Could not parse the message
ERR: Could not parse the message
ERR: Could not parse the message
ERR: Could not parse the message
ERR: Could not parse the message
ERR: Could not parse the message
OK: -3
```

3. String exceeding character limit of 255 \
Input:
```
qtkrtoqyxhbjhmpnbidnzfqkyvkduiha ...
```
Output:
```
stderr: Input string surpasses the allowed length of 255
```

4. SIGINT raise \
Input:
```
(+ 1 2)
C-c
```
Output:
```
OK: 3
```

### Other

1. Missing argument \
Input:
```
./ipkcpc -h localhost -p 2023 
```
Output:
```
stderr: usage: ./ipkcpc -h <host> -p <port> -m <mode (tcp/udp)>
        Input parameter order is interchangeable
```

2. Switched argument order \
Input:
```
./ipkcpc -m udp -h localhost -p 2023
```
Output:
```
No error
```

3. Incorrect connection mode \
Input: 
```
./ipkcpc -h localhost -p 2023 -m smtp
```
Output:
```
stderr: Only TCP/UDP modes are supported
```
