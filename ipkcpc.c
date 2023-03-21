/**
 *IPK Project 1 
 *@author Daniel Blaško <xblask05>
 *@file ipkcpc.c
 *@brief Client for the IPK Calculator Protocol
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/time.h>

///UNIX-specific header files
#ifdef __unix__  
  #include <unistd.h>
  #include <sys/socket.h>
  #include <netdb.h>
  #include <netinet/in.h>
  #include <arpa/inet.h>
#endif
///Windows-specific header files
#ifdef _WIN32 
  #include <winsock2.h>
  #include <windows.h>
  #include <io.h>
#endif

#define TCP_BUFSIZE 1025 ///1024+1 for the '\n' fgets appends
#define UDP_BUFSIZE 256  ///255+1 for the '\n' fgets appends 
#define h_addr h_addr_list[0]

///Global var to make connection closing after catching SIGINT POSSIBLE
int client_socket;

/**
* @function sig_handler_tcp
* @brief Function used to close TCP connection with server after catching SIGINT
*/
void sig_handler_tcp(int signum) {
  int bytestx, bytesrx;
  if (signum == SIGINT) {
    char end_buf[TCP_BUFSIZE];
    memset(end_buf, 0, TCP_BUFSIZE);
    strcpy(end_buf, "BYE\n");
    ///send BYE to server, wait for answer and close connection after catching SIGINT
    bytestx = send(client_socket, end_buf, strlen(end_buf), 0);
    if (bytestx < 0) 
      perror("ERROR in sendto");
    
    ///Receive answer from server
    memset(end_buf, 0, TCP_BUFSIZE);
    bytesrx = recv(client_socket, end_buf, TCP_BUFSIZE, 0);
    if (bytesrx < 0) 
      perror("ERROR in recvfrom");

    printf("%s", end_buf);
    
    close(client_socket);
    exit(EXIT_SUCCESS);
  }
}

/**
* @function sig_handler_udp
* @brief Function used to end program catching SIGINT during UDP communication
*/
void sig_handler_udp(int signum) {
  if (signum == SIGINT){
    exit(EXIT_SUCCESS);
  }
}


int main (int argc, const char * argv[]) {
	  int port_number, bytestx, bytesrx;
    const char *server_hostname, *mode;
    struct hostent *server;
    socklen_t len;
    struct sockaddr_in server_address;
    char tcp_buf[TCP_BUFSIZE];
    char udp_buf[UDP_BUFSIZE];
    char udp_message[UDP_BUFSIZE+2]; //+2 for opcode and payload length
    bool hostpres = false;
    bool portpres = false;
    bool modepres = false;
    bool arg_err = false;

    mode = "";
    port_number = 0;
    server_hostname = "";
    ///checking for input arguments
    if (argc != 7) { 
      arg_err = true;
    }
    
    if (!arg_err) {
      for (int i = 0; i < 7; i++) {
        ///host 
        if (!strcmp(argv[i], "-h")) {
          if (hostpres == false) {
            server_hostname = argv[i+1];
            hostpres = true;
          }
          else { 
            arg_err = true; ///host already present
          }
        }
        ///port
        else if (!strcmp(argv[i], "-p")) {
          if (!portpres) {
            port_number = atoi(argv[i+1]);
            portpres = true;
          }
          else { 
            arg_err = true; ///port already present
          }
        }
        ///mode
        else if (!strcmp(argv[i], "-m")) {
          if (!modepres) {
            mode = argv[i+1];
            modepres = true;
          }
          else {
            arg_err = true; ///mode already present
          }
        }
      }
    }
    
    if ((arg_err) || (!hostpres) || (!portpres) || (!modepres)) {
        fprintf(stderr,"Usage: %s -h <host> -p <port> -m <mode (tcp/udp)>\n", argv[0]);
        fprintf(stderr,"Input parameter order is interchangeable\n");
        exit(EXIT_FAILURE);
    }
    
    ///TCP protocol
    /**
    *@Reference: IPK 2022/2023 Code Stubs
    */
    if (!strcmp(mode, "tcp")) {
        ///Get server address via DNS
        if ((server = gethostbyname(server_hostname)) == NULL) {
            fprintf(stderr,"ERROR: no such host as %s\n", server_hostname);
            exit(EXIT_FAILURE);
        }
        
        ///Find IP address of server and initialize server_address structure
        memset((char *) &server_address, 0, sizeof(server_address));
        server_address.sin_family = AF_INET;
        memcpy((char *)&server_address.sin_addr.s_addr, (char *)server->h_addr, server->h_length);
        server_address.sin_port = htons(port_number);

        ///Create socket
        if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) <= 0)
        {
          perror("ERROR: socket");
          exit(EXIT_FAILURE);
        }

        if (connect(client_socket, (const struct sockaddr *) &server_address, sizeof(server_address)) != 0)
        {
          perror("ERROR: connect");
          exit(EXIT_FAILURE);        
        }
            
        ///Setup SIGINT watch
        signal(SIGINT, sig_handler_tcp);

        memset(tcp_buf, 0, TCP_BUFSIZE);
        /// input loading and message send/receive cycle 
        while (fgets(tcp_buf, TCP_BUFSIZE, stdin)) {
          if (tcp_buf[strlen(tcp_buf)-1] != '\n') { ///Too long of a string
            fprintf(stderr, "Input string surpasses the allowed length of 1024");
            continue;
          } 
        
          ///Send message to server
          bytestx = send(client_socket, tcp_buf, strlen(tcp_buf), 0);
          if (bytestx < 0) 
            perror("ERROR in sendto");
          
          ///Receive answer from server
          memset(tcp_buf, 0, TCP_BUFSIZE);
          bytesrx = recv(client_socket, tcp_buf, TCP_BUFSIZE, 0);
          if (bytesrx < 0) 
            perror("ERROR in recvfrom");
          
          ///close connection after "BYE" from server
          printf("%s", tcp_buf);
          if ((!strcmp(tcp_buf, "BYE\n")) || (!strcmp(tcp_buf, "bye\n"))) {
            break;
          }
          ///reset buffer
          memset(tcp_buf, 0, TCP_BUFSIZE);
        }
        close(client_socket);
    }

    ///UDP Protocol
    /**
    *@Reference: IPK 2022/2023 Code Stubs
    */ 
    else if (!strcmp(mode, "udp")) {
          ///Setting timeout value to 10 seconds
          struct timeval timeout;
          timeout.tv_sec = 10;
          timeout.tv_usec = 0;

          ///Get server address via DNS
          if ((server = gethostbyname(server_hostname)) == NULL) {
              fprintf(stderr,"ERROR: no such host as %s\n", server_hostname);
              exit(EXIT_FAILURE);
          }

          ///Find IP address and initialize server_address structure
          memset((char *) &server_address, 0, sizeof(server_address));
          server_address.sin_family = AF_INET;
          memcpy((char *)&server_address.sin_addr.s_addr, (char *)server->h_addr, server->h_length);
          server_address.sin_port = htons(port_number);

          ///Create socket
          if ((client_socket = socket(AF_INET, SOCK_DGRAM, 0)) <= 0)
          {
            perror("ERROR: socket");
            exit(EXIT_FAILURE);
          }

          ///Setup SIGINT watch
          signal(SIGINT, sig_handler_udp);

          memset(udp_buf, 0, UDP_BUFSIZE);

          ///Message load and send/receive cycle
          while(fgets(udp_buf, UDP_BUFSIZE, stdin)) {
            if (udp_buf[strlen(udp_buf)-1] != '\n') { ///Too long of a string
              fprintf(stderr, "Input string surpasses the allowed length of 255");
              exit(EXIT_FAILURE);
            } 

            ///UDP request message crafting
            memset(udp_message, 0, UDP_BUFSIZE+2);
            int payload_len_int = strlen(udp_buf);
            udp_message[0] = '\0';
            udp_message[1] = payload_len_int; 
            for (int i = 0; udp_buf[i] != '\n'; i++) {
              udp_message[2+i] = udp_buf[i];
            }
            len = sizeof(server_address);

            ///Send message to server
            bytestx = sendto(client_socket, udp_message, strlen(udp_buf)+2, 0, (struct sockaddr *) &server_address, len);
            if (bytestx < 0) {
              perror("ERROR in SEND");
              exit(EXIT_FAILURE);
            }
            
            ///Set timeout for answer
            setsockopt(client_socket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

            ///Receive answer
            memset(udp_buf, 0, UDP_BUFSIZE);
            bytesrx = recvfrom(client_socket, udp_buf, UDP_BUFSIZE, 0, (struct sockaddr *) &server_address, &len);
            if (bytesrx < 0) 
              perror("ERROR in recvfrom");
              
            ///Status code checking
            if (udp_buf[1] == '\0') {
              strcpy(udp_buf, udp_buf+3);
              printf("OK: %s\n", udp_buf);
            }
            else if (udp_buf[1] == '\1'){
              strcpy(udp_buf, udp_buf+3);
              printf("ERR: %s\n", udp_buf);
            }
            else {
              perror("ERROR in received message status code");
            }
            memset(udp_buf, 0, UDP_BUFSIZE);
          }
    }
    else {
        fprintf(stderr, "Only TCP/UDP modes are supported");
        exit(EXIT_FAILURE);
    }

  return 0;
}
