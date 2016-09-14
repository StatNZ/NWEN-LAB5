// NWEN 243, Victoria University of Wellington. Author, Kris Bubendorfer.

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>

#define bufsize 1024

int main(argc, argv) int argc; char *argv[];{
  
  // argv our DNS server, probably by ip address
  
  int sock, rval;
  struct hostent *host;  // the server
  struct sockaddr_in server;  // not a pointer. the server address
  char buf[bufsize];
  
  if(argc != 4){
    printf("usage:\ntcpclient hostname port string\n\n");
    return(-1);
  }
  
  // look up hostname (server) using DNS

  if ((host = gethostbyname(argv[1])) == 0) {
    fprintf(stderr, "%s: unknown host\n", argv[1]); 
    return(-1);  
  }

  // Set up fields for socket to point to host and port

  bcopy(host->h_addr, &server.sin_addr, host->h_length);
  server.sin_family = AF_INET;  
  server.sin_port = htons(atoi(argv[2])); // port no.
 
  // Create socket  
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0){
    printf("Error opening socket\n");
    return(-1);
  }

  // connect (3-way handshake)

  // connect (when we connect, this is essentially doing a three way connection)
  if (connect(sock, (struct sockaddr *) &server, sizeof(server))){
    printf("ERROR, connecting\n");
    return(-1);
  }

  // Copy the arg into buf so we can send it to the server
  strncpy(buf, argv[3], bufsize);
 
  // Send sentence to server
  rval = write(sock, buf, strlen(buf));
  if (rval < 0){
    perror("ERROR: writing to server\n");
    return(-1);
  }

  printf("Message sent to server...\n");

  // read response from server
  bzero(buf, bufsize+1);
  rval = read(sock, buf, bufsize);
  if (rval < 0){
    perror("ERROR: reading from server\n");
    return(-1);
  }

  // print result to window
  fprintf(stdout,"Server: %s\n\n", buf);
  
  close(sock);
}

