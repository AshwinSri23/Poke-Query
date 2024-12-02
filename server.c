#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "serverTypes.h"
#include <pthread.h>
#include <ctype.h>

#define SERVER_PORT 6000

/*The server.c file contains the implementation of the Pokemon Property Server(PPS). The operations that should be performed are based on the option that the user chooses from the Pokemon Query Client. The PPS is responsible for gathering the query results and sending it to the client. */



sem_t mutex;


int main() {
  int                 serverSocket, clientSocket;
  struct sockaddr_in  serverAddress, clientAddress;
  int                 status, bytesReceived;
  unsigned int        addrSize;  
  char                buffer[1000];
  char result[500000];
  int choice;
  pthread_t query;
  threadArguments Arg; 
  
  
  user(&Arg); 
  
  Arg.pokemonCount = 0;
  Arg.success = 0;
  Arg.fileNumber = 0;
  Arg.results = malloc(sizeof(Pokemon));
  Arg.fileNames = malloc(sizeof(char*));
  strcpy(result,"");
  
  
  serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (serverSocket < 0) {
    printf("*** SERVER ERROR: Could not open socket.\n");
    exit(-1);
  }

  
  memset(&serverAddress, 0, sizeof(serverAddress)); // zeros the struct
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
  serverAddress.sin_port = htons((unsigned short) SERVER_PORT);

  
  status = bind(serverSocket,  (struct sockaddr *)&serverAddress, sizeof(serverAddress));
  if (status < 0) {
    printf("*** SERVER ERROR: Could not bind socket.\n");
    exit(-1);
  }

  
  status = listen(serverSocket, 1);
  if (status < 0) {
    printf("*** SERVER ERROR: Could not listen on socket.\n");
    exit(-1);
  }  
  if(sem_init(&mutex,0,1)<0){
    printf("Error on semaphore init.");
    exit(1);
   }
   
   
  
  
   
   
  while (1) {
    addrSize = (unsigned int)sizeof(clientAddress);
    clientSocket = accept(serverSocket, (struct sockaddr *) &clientAddress, &addrSize);
    
    if (clientSocket < 0) {
      printf("*** SERVER ERROR: Could not accept incoming client connection.\n");
      exit(-1);
    }
    printf("SERVER: Received client connection.\n");

    while (1) {
      
     choice = 0;  
     bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
     buffer[bytesReceived] = 0; // put a 0 at the end so we can display the string
     choice = atoi(buffer);
     
     
     if(choice == 1){
        
       strcpy(buffer,""); 
       bytesReceived = recv(clientSocket, buffer, sizeof(buffer)+1, 0);
       buffer[bytesReceived] = 0; // put a 0 at the end so we can display the string
       strcpy(Arg.term,buffer);
       pthread_create(&query,NULL,readPokemon,(void*)&Arg);
        
     }else{
       if(choice == 3){
         strcpy(buffer,"");
         send(clientSocket,&Arg.success,sizeof(Arg.success), 0);
         free(Arg.results);
         for(int i=0; i<(Arg.fileNumber); i++){
           free(Arg.fileNames[i]);
          }
         free(Arg.fileNames);
         fclose(Arg.fd);
         printf("SERVER: Closing client connection.\n");
         close(clientSocket);
         close(serverSocket);
         printf("SERVER: Shutting down.\n");
         pthread_exit(NULL);
         break;
      }
        
     }
      
      
    pthread_join(query,NULL);
      
        
    for(int i=0; i<Arg.pokemonCount; i++){
      strcpy(buffer,"");
      sprintf(buffer,"%d,%s,%s,%s,%d,%d,%d,%d,%d,%d,%d,%d,%s\n",Arg.results[i].num,Arg.results[i].name,Arg.results[i].type_1,Arg.results[i].type_2,Arg.results[i].total,Arg.results[i].hp,Arg.results[i].attack,Arg.results[i].defense,Arg.results[i].spAtk,Arg.results[i].spDef,Arg.results[i].speed,Arg.results[i].gen,Arg.results[i].legend);
      strcat(result,buffer);
     }
    send(clientSocket,result,50000, 0);
    strcpy(result,"");
    }
  }
}

