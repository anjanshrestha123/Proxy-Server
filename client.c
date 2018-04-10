/* 
	Written by Anjan Shrestha, September 2017
	Copyright (C) - All Rights Reserved
 */
#include <stdio.h>   // for using printf
#include <stdlib.h>   // for using exit(1)
#include <netdb.h>    //for using gethostbyname() function
#include <sys/types.h> 
#include <sys/socket.h>   //for using sockets
#include <netinet/in.h>
#include <string.h>    //for using sprintf


/* Function to create socket */
int create_socket(int socketfd)
  {
     if ((socketfd =socket(AF_INET,SOCK_STREAM,0))<0){   //creating socket
        perror("Error creating socket...");   // error message if there is error in connection
        exit(1);      //exit if there is error
      }
  printf("Socket has been created successfully...\n");  //message displayed if the socket is displayed correctly
  return socketfd;
  }

/* Function to read socket */
char *read_socket(int socketfd, char buffer[4000])
{
  if (read(socketfd, buffer,4000)<0){     //reading socket and storing it into buffer 
    perror("Error reading from socket");   //error message if there is error
    exit(1);
   }
  printf("Message from server: %s\n", buffer);   // message displayed if read is successful
  return buffer;
}

/* Function to write socket */
char *write_socket(int socketfd,char buffer[4000])
{ 
   if ( write(socketfd,buffer,strlen(buffer))<0){  //writing to a socket with message stored in buffer
    perror ("Error writing to a server...\n");   // error message if there is an error
    exit(1);
    }
   printf("Writing to a socket...\n");   // message displayed if write is successful
   return buffer;
}

int main(int argc, char *argv[])
{ 
   int clientsocketfd;   // declaring int variable 
   struct hostent *proxyserver;   
   struct sockaddr_in proxyserv_addr;  
   char clientbuffer[4000];  // declaring char array with size 4000

    if (argc > 2 ||argc < 2)     //printing usage message if user inputs more than 2 or less than 2 argument in command line
      {
      printf ("Usage: ./client <portnumber>\n");   //usage message
      exit(1);      //exits the program with error message 
      }

   /*getting url from the client */
    printf("URL: ");     // ask user for url 
    bzero (clientbuffer,4000);       
    fgets(clientbuffer,4000,stdin);      //storing user inputs in variable clientbuffer


   /* Creating socket */
    clientsocketfd = create_socket(clientsocketfd);    //creating a socket using create_socket function

  /* Connecting to server */
   proxyserver = gethostbyname("cse02");    //  getting the host ip with the gethostbyname function
   bzero ((char *) &proxyserv_addr, sizeof(proxyserv_addr));
   proxyserv_addr.sin_family = AF_INET;     
   bcopy((char *)proxyserver->h_addr, (char*)&proxyserv_addr.sin_addr.s_addr, proxyserver->h_length);
   proxyserv_addr.sin_port = htons(atoi(argv[1]));
   if (connect(clientsocketfd,(struct sockaddr *)&proxyserv_addr, sizeof(proxyserv_addr))<0){    //connecting client with server
           perror ("Error connecting to server...\n");    //error message if there is error on connection
         exit(1);
         }
   printf("Socket has been connected...\n");       // message diplayed if connection is successfull
  
  /*Writing to proxy server */
  write_socket(clientsocketfd, clientbuffer);     //sending url request to the proxy server  using write_socket function
  printf("clientbuffer: %s",clientbuffer);        // printing the url to make sure if it is send correctly

  /*Reading from Server */
  bzero(clientbuffer,4000);
  read_socket(clientsocketfd,clientbuffer);    //receiving reponse from server using read_socket function

  return 0;
}
