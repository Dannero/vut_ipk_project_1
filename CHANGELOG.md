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

## Usage
Query formatting:
```
operator = "+" / "-" / "*" / "/"
expr = "(" operator 2*(SP expr) ")" / 1*DIGIT
query = "(" operator 2*(SP expr) ")"
```
Source: [IPK Calculator Protocol](https://git.fit.vutbr.cz/NESFIT/IPK-Projekty/src/branch/master/Project%201/Protocol.md)

#### TCP Mode 
1. Run the application with `-m tcp` argument
2. Begin the initiation state with a "HELLO" greeting
3. Send a query in a `SOLVE query` format
4. Close the connection and terminate the program with the "BYE" message or by calling the SIGINT signal by pressing `ctrl+c`

#### UDP Mode
1. Run the application with `-m udp` argument 
2. Send a query in a `query` format
3. Close the connection and terminate the program by raising the SIGINT signal by pressing `ctrl+c`

## Other
1. The program uses `signal`, `sig_handler_tcp` and `sig_handler_udp` functions to tackle SIGINT raising and exit the program gracefully. \
`sig_handler_tcp` sends a "BYE" message to the server after being called by the `signal` function, waits for the server to answer with "BYE" and closes connection. \
`sig_handler_udp` closes connection after being called by the `signal` function.

2. UDP communication has an answer timeout set at 10 seconds using the `sys/time.h` header file and the `setsockopt` function.

## Issues
1. Program is only compatible with UNIX systems, despite including Windows specific header files I have not been able to make the program run on this system
2. Although it was not recommended in the project discussion forum, I have made the `client_socket` variable into a global one to make connection closing possible in functions handling a SIGINT raise