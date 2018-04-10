/* 
	Written by Anjan Shrestha, September 2017
	Copyright (C) - All Rights Reserved
 */

#include <stdio.h>   //for using printf 
#include <stdlib.h>   //for using exit(1)
#include <sys/types.h> 
#include <sys/socket.h>   //for using socket
#include <netinet/in.h>   //for using sprintf
#include <string.h> 
#include <netdb.h>  // for using gethostbyname() function
#include <time.h>

/* Function to create socket */
int create_socket(int socketfd)
  {
     if ((socketfd =socket(AF_INET,SOCK_STREAM,0))<0){ //creating socket
        perror("Error creating socket...");   // error message if there is problem in creating socket
        exit(1);     //exits program with error message
      }
  printf("Socket has been created successfully...\n");   // message displayed if socket is created successfully
  return socketfd;
  }

/*Function to read from socket */
char *read_socket(int socketfd, char buffer[4000])
{
  if (read(socketfd, buffer,4000)< 0){   // reading from socket and storing it into buffer
    perror("Error reading from socket");   //error message displayed
    exit(1);     //exits program with error message
   }
  printf("Message received: %s\n", buffer);   //message displayed if correctly read
  return buffer;
}

/*Function to write from socket */
char *write_socket(int socketfd,char buffer[4000])
{ 
   if ( write(socketfd,buffer,strlen(buffer))<0){   //writing to a socket with a message displayed in buffer
    perror ("Error writing to a server...\n");  //error message displayed if socket is not wrote successfully
    exit(1);    //exit program with error message
    }
   printf("Writing to a socket...\n");   //message displayed if correctly wrote
   return buffer;
}

/*Function to create cache files */
 char *cache_file_creater (char new_buf[4000], char filename[80])
   { 
          time_t currenttime;
         struct tm *finaltime;
       	 time(&currenttime);
  	 finaltime = localtime(&currenttime);   //saving current time in a file
  	
  	 strftime(filename,80,"%Y%m%d%H%M%S",finaltime);   //saving current time in a filename as per the format in question
  	 printf("Current time is: %s\n",filename);
	 printf("Saving response as a cache...\n");
         FILE *cachepointer = fopen(filename,"w");   //opening file
  	 fputs(new_buf,cachepointer);
 	  fclose(cachepointer);
         return filename;
   }

/* Checking for existence for list.txt */
int file_exist_list()
{
    FILE *fp;
    if ( fp=fopen("list.txt", "r")) {    //if there is a file
	fclose(fp);
        return 0;
        
    } else {      //if there is not a file
        return 1;
    }
}

/* Check for blacklisted websites*/
int check_blacklisted(char buffer[4000]){
    time_t now,stime,ftime;
    struct tm starttime;
    struct tm endtime;
    time(&now);          //saving current time

    FILE *blacklistpointer;
    char blacklistarray[20],starttimec[20],endtimec[20];    //declaring variable for using it later
    

    if (blacklistpointer = fopen("blacklist.txt","r")){   //opening blacklist file
           while(!feof(blacklistpointer)){
                fscanf (blacklistpointer,"%s",blacklistarray);
                fscanf (blacklistpointer,"%s",starttimec);
                fscanf (blacklistpointer,"%s%*[^\n]",endtimec);
                strptime(starttimec,"%Y%m%d%H%M%S",&starttime);
                strptime(endtimec,"%Y%m%d%H%M%S",&endtime);
                stime = mktime(&starttime);    //converting strings into time_t variable
                ftime = mktime(&endtime);     //converting strings into time_t variable
                if(strcmp(blacklistarray,buffer)==0)  //comparing to see if there is hostname in list.txt
                 {
                   
                   if(difftime(now,stime)>0){    //checking for the difference in time
                       if (difftime(now,ftime)<0){
                           printf("The website is blacklisted...\n");
		           return 1;     //return 1 if it is blacklisted
                       }
                    }
                 }
                 strcpy(blacklistarray,"");  //clearing the array
                 strcpy(starttimec,"");     //clearing the array
                 strcpy(endtimec,"");     //clearing the array

           }
       return 0;
       fclose(blacklistpointer);     //closing the blaclist.txt file
     }
     else
        {
        printf("There is no blacklist.txt file in current directory.\n");
        return 0;     //returning 0 if there is no blacklist.txt file
        }
}

//function to get the content from file
char *get_file(char buffer[4000])
{
   FILE *hostpointer,*filepointer;   //creating a pointers
   char hostname[20],filename[20];
   char chostname;
   
   if (hostpointer = fopen("list.txt","r")){   //opening list.txt file
           while(!feof(hostpointer)){
                int i=0;
                fscanf (hostpointer,"%s",hostname);   //saving hostname from list.txt file
                fscanf (hostpointer,"%s%*[^\n]",filename);   //saving filename from list.txt file

                if(strcmp(hostname,buffer)==0)
                 {
                    printf("Found the file in cache...\nReading from cache...\n");
                    filepointer = fopen(filename,"r");
                    bzero(buffer,4000);
                    chostname = getc(filepointer);
                    while(chostname != EOF)
                    {
                       buffer[i] = chostname;   //saving the content of a file to a buffer
                       i++;
                       chostname = getc(filepointer);    
                    }
                    
                    fclose(filepointer);                 
                 }
                 strcpy(hostname,"");   //clearing hostname
                 strcpy(filename,"");           // clearing filename
           }
       
       fclose(hostpointer);
       return buffer; 
     }

     else
       {
           printf("There is no list.txt in current directory...\n");
           return NULL;
       }

}

//function to check the host
int host_checker (char buffer[4000], int number)
 {
       number = 0;
       FILE *hostpointer;  //making pointer
       char hostname[20],filename[20];
       if (hostpointer = fopen("list.txt","r")){   //opening file
           while(!feof(hostpointer)){
                fscanf (hostpointer,"%s",hostname);
                fscanf (hostpointer,"%s%*[^\n]",filename);

                if(strcmp(hostname,buffer)==0)  //if there is a hostname in list.txt
                 {
                    number =1;    //making number equals to 1 if there is a hostname
                 }
             }
         }

 return number;
}
 

int checker_200(char buffer[4000])    //function to check for http 200 ok
{
   char check[4000]="";
//   bzero(check,4000);
   strncat(check,buffer,15);
   printf("\n\n\ncheck: %s\n\n\n",check);
   if(strcmp(check,"HTTP/1.1 200 OK")==0)
      return 1;    //return 1 if there is 200 ok
   else 
      return 0;

}

 /* Main Function */
int main(int argc, char *argv[]) {
     int sockfd, newsockfd, clientlength, new_sockfd;   // declaring variables
     char buffer[4000];    //declaring character array with size 4000
     struct sockaddr_in serv_addr, cli_addr,webserv_addr;
     struct hostent *web_server;
     char new_buf[4000], get[4000];
     char *p;    //declaring pointer
     char filename [80];
     int blacklistchecker,hostchecker,checker200;
     
   

     if (argc>2 || argc <2)    //if statment to check see if user enter exactly two arguments
        {
       printf ("Usage: ./pserver <portnumber>\n");  //usage messge if users inputs wrong number of arguments
       exit(1);   //exits program with error message
       } 
  
     /* creating a socket for client */
     sockfd = create_socket(sockfd);  //socket created
    
     /*Binding to socket */
     bzero( &serv_addr, sizeof(serv_addr));
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(atoi(argv[1]));
     if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0){      //bind function to bind socket
              perror("ERROR on binding");   //error message if binding is unsuccessful
              exit(1);   //exits program with error message
         }
    printf("Socket has been binded to client...\n");   //message displayed if binding is successful
  
  /* Listening for for the connections */
     listen(sockfd,6);  //function to listen for connections that can listen upto 6 connections
     printf("Now, listening for the connections...\n");  
     clientlength = sizeof(cli_addr);   //storing size of cli_addr into clientlength
    
     
     
     FILE *listpointer;
     int i = file_exist_list();    //checking for existence of a file
     if(i==1)
     {
        printf("list.txt is not in current directory...\nCreating list.txt file to save cache...\n");
        listpointer = fopen ("list.txt","w");    //creating file if there is not
        fclose(listpointer);
      }

    do{        //while loop for accepting multiple client requests one by one
   /* Accepting connection from client */  
     
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clientlength);
     if ( newsockfd<0)
        {
          perror("ERROR on accept");   //error message if there is problem on accept
          exit(1);  //exits program with error message
          }
     
      printf("Connection has been accepted with the client...\n");
  
    /* Reading from a client */
    bzero(buffer,4000);
    read_socket(newsockfd,buffer);    //reading from client and storing it into buffer
    bzero(new_buf,4000); 
    p = strtok(buffer,"\n");     // storing the value of buffer until new line to a pointer
    strcpy(new_buf,p);     //copying value from pointer to a new_buf


   // checking for blacklisted websites
   blacklistchecker = check_blacklisted(new_buf);

  if(blacklistchecker == 1)  //if blacklisted
    {
        bzero(new_buf,4000);
        strcpy(new_buf, "Website is blacklisted...\n\n");  //buffer to send to client 
    }

 else{
      printf("This website is not blacklisted...\n");
      hostchecker = host_checker(new_buf,hostchecker);

    if (hostchecker == 1)   // if there is hostname in list.txt file
       {
       get_file(new_buf);    //getting file from directory
       }
       
  else
     {
     printf("The file is not in cache...\n");
    /* Preparing get response */
      bzero(get,4000);
      sprintf(get, "GET / HTTP/1.1\r\nHost: %s\r\nConnection:Close\r\n\r\n",new_buf);   //storing the get response in get variable

    web_server = gethostbyname(new_buf);  //getting ip address for web address
 
    if (web_server == NULL) {  //if there is no webserver
        fprintf(stderr,"ERROR, no such host\n");   //error message displayed 
        exit(1);   //exits prgram with error message
       }

    bzero((char *) &webserv_addr, sizeof(webserv_addr));
    webserv_addr.sin_family = AF_INET;
    bcopy((char *)web_server->h_addr,
         (char *)&webserv_addr.sin_addr.s_addr,
         web_server->h_length);
    
    /* creating another socket for web server */   
    new_sockfd = create_socket(new_sockfd);   //creating new socket

    /*Connection to server*/
    //portnumber = 80;
    webserv_addr.sin_port = htons(80);
    if (connect(new_sockfd,(struct sockaddr *)&webserv_addr,sizeof(webserv_addr)) < 0)    //connecting to a webserver
     {
        perror("ERROR connecting");
        exit(1);
     }
     printf("Connecting to the web server...\n");

   /* sending request to web server */
    write_socket(new_sockfd, get);  
 
  /* Reading from web server */
 bzero (new_buf,4000);
 read_socket(new_sockfd,new_buf);



//list in the cache

  /* if 200 ok----Saving response as a cache */
   checker200 = checker_200(new_buf);
   
   if(checker200)   //if 200 ok
   {     
      printf("This is 200 OK, so saving it as cache...\n");
      listpointer = fopen("list.txt","a");
   cache_file_creater(new_buf,filename);  //creating cache
   fputs(buffer,listpointer);
   fputs(" ",listpointer); 
   fputs(filename,listpointer);
   fputs ("\n",listpointer);
   
   fclose(listpointer);
   
/* For checking number of lines */
   listpointer = fopen("list.txt","r");
   char read_list = getc(listpointer);
   int line_count = 0;

   while(read_list != EOF)   //counting number of lines in list.txt
   {
     if (read_list == '\n')
           line_count++;
     read_list = getc(listpointer);
   }   

     printf("Number of lines: %i\n",line_count);
   // Deleting the last cache if exceeds 5 hostnames
    if (line_count >5)
       {
          
           int i= 0;
           int del = 1,del1 = 1;
           rewind(listpointer);   //moving the pointer back to the starting

           char deletecache[20],charcache;
           charcache = getc(listpointer);
           while(charcache != EOF){
               if (charcache == ' ' && del1 ==1)
                   { 
                     charcache = getc(listpointer);
                     while (charcache != '\n'){            //while there is a new line
                          deletecache[i] = charcache;   //saving character to the deletecache array
                          i++;
                          charcache = getc(listpointer);
                        }
                     del1++;
                     printf("\ncache to delete: %s\n", deletecache);
                     
                    }
              charcache = getc(listpointer);
           }
           remove(deletecache);      //removing deletecache
         

           rewind(listpointer);   //moving file pointer to the starting
           FILE *deletepointer = fopen("delete.txt","w");
           char delete = getc(listpointer);
           while(delete != EOF)
              { 
                 
                 if( del == 1 && delete == '\n')   //moving to the next line
                    {
                        del++;
                        delete = getc(listpointer);
                        
                    } 
                 if (del != 1)  //if not the line 1
                     putc(delete,deletepointer);
                 delete = getc(listpointer);
                  
               }
           
            fclose(listpointer);
            fclose(deletepointer);
            remove("list.txt");    //removing list.txt
            rename("delete.txt","list.txt");  //moving delete.txt to list.txt
        }


/* closing connection with web server */
  close (new_sockfd);

}  //closing if statement for 200 checker
}    // closing else statement for 200 ok
}   // closing else statement for blacklisted checker


/* writing back to the client */
   write_socket(newsockfd, new_buf);
  printf("Waiting for another request...\n");

/* closing connection with client */
    close(newsockfd);

   }while(1);

   
/* closing proxy socket */
  close(sockfd);

  return 0;
}

