

/* 
 * File:   main.c
 * Author: Rich
 *
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>	// this file defines the structure hostent.
#include <string.h> 	// for string comp,  exit
#include <stdbool.h> 	// for boolean flag
void error(const char *msg)
{
	perror(msg);
	exit(0);
}

int main(int argc, char* argv[]) {
    if(argc < 4)
    {
        fprintf(stderr, "usage %s [hostname][ page</index.html>] port [80] Read the README file Bro\n", argv[0]);
	exit(0);
    }
   
    int byte_read_or_written;
    int sockfd, port_number;
    struct hostent *host_server;
    struct sockaddr_in server_addr;     //hold info form hostent *server
    char buffer[256];
    char *asprint_str;                  // string ptr for asprint message
    sockfd = socket(AF_INET, SOCK_STREAM, 0); //socket for client to communicate with the remote host/server
    host_server = gethostbyname(argv[1]);
    if(host_server == NULL)
    {
        fprintf(stderr, "Phew oh Boy I can't find that server. Bye I'm going to take a break\n\n");
        exit(0);
    }
    bzero((char*)&server_addr, sizeof(server_addr));
    port_number = atoi(argv[3]);
    server_addr.sin_port = htons(port_number);
    server_addr.sin_family = AF_INET;
    bcopy((char*) host_server->h_addr, (char *) &server_addr.sin_addr.s_addr, sizeof(server_addr));
    
    if(sockfd < 0)
        error("Error opening the socket");
    if(connect(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
        error("error connecting");
    bzero(buffer, 256);
    asprintf(&asprint_str, "GET %s HTTP/1.1\r\nHOST: %s\r\n\r\n", argv[2], argv[1]);
    printf("%s", asprint_str);
    
    byte_read_or_written = write(sockfd, asprint_str,strlen(asprint_str));
    if(byte_read_or_written < 0)
        error("write the the socket was unsuccessful");
    else
    {
        printf("HTTP request has been sent to the host server\n\n");
        
        while(byte_read_or_written > 0)
        {
            bzero(buffer, 256);
            byte_read_or_written = read(sockfd, buffer, 255);
            if(byte_read_or_written > 0)
                printf("%s", buffer);
        }
    }
    
    
    
    close(sockfd);
    return (EXIT_SUCCESS);
}

 