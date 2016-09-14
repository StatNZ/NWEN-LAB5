// This is just a mock server that I will create on the local network, just to test the actual client server that I must make

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>

#define bufsize 1024

#define RECVDMSG "Received Message"

int main(){

  // variables
  int sock, rval, port, mysock;
  struct sockaddr_in server;
  char buf[bufsize];
  
  port = 5000;

  // create socket

  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0){
    perror("Error creating socket\n");
    return(-1);
  }

  server.sin_family = AF_INET;
  server.sin_port = htons(port);
  inet_aton("127.0.0.1", &server.sin_addr.s_addr);
  
  // bind the socket
  if (bind(sock, (struct sockaddr *)&server, sizeof(server))){
    perror("Error binding the socket\n");
    return(-1);
  }

  // listen for socket(s)
  listen(sock, 5); // upto 5 connections

  // to have multiple sockets, we will need to create an array of sockets
  int sockets[5];
  
  // accept a connection
  mysock = accept(sock, (struct sockaddr *) 0, 0);
  if (mysock < 0){
    perror("ERROR: connecting the sock\n");
    return(-1);
  }

  // read a message from the socket
  bzero(buf, bufsize+1);

  rval = read(mysock, buf, bufsize);
  if (rval < 0){
    perror("ERROR: reading from socket\n");
    return(-1);
  }

  // now we have the message, display on the server
  printf("Received MSG: %s\n",buf);

  // return an acknowledgement to the socket
  rval = write(mysock, RECVDMSG, sizeof(RECVDMSG));
  if (rval < 0){
    perror("ERROR: writing to socket\n");
    return(-1);
  }

  // exit and close sock
  close(sock);
  return(0);  
}
