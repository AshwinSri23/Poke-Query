#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "clientTypes.h"
#include <pthread.h>
#include <semaphore.h>
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6000

/*The client.c file contains the implementation of the Pokemon Query Client(PQC). The operations that the PQC performs are based on the option that the user chooses from the menu. The PQC is responsible for retreiving the query results made by the Pokemon Property Server(PPS) and storing those query results into a file.*/


sem_t mutex2;
 
int main() {
  int                 clientSocket;
  struct sockaddr_in  clientAddress;
  int                 status, bytesRcv;
  char                buffer[1000]; 
  
  char **clientFileNames;
  clientArguments Arg;
  pthread_t save2;
  
  
  Arg.fileNumber = 0;
  Arg.fileNames = malloc(sizeof(char*));
  Arg.success = 0;
  strcpy(Arg.result,"");

  if(sem_init(&mutex2,0,1)<0){
      printf("Error on semaphore init.");
      exit(1);
    }

  clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (clientSocket < 0) {
    printf("Could open socket.\n");
    exit(-1);
  }
   
  memset(&clientAddress, 0, sizeof(clientAddress));
  clientAddress.sin_family = AF_INET;
  clientAddress.sin_addr.s_addr = inet_addr(SERVER_IP);
  clientAddress.sin_port = htons((unsigned short) SERVER_PORT);
  
  
  status = connect(clientSocket, (struct sockaddr *) &clientAddress, sizeof(clientAddress));
  if (status < 0) {
    printf("Unable to establish a connection to the PPS!\n");
    exit(-1);
  }
  
 
  menu(buffer,clientSocket);
   
  while (1) {    
      if(strcmp(buffer,"1")==0){ 
        queryResults(buffer,clientSocket); 
        strcpy(buffer,"");
        menu(buffer,clientSocket);
       
      }else if(strcmp(buffer,"2")==0){
       writeResults(buffer,clientSocket,&Arg);
       pthread_create(&save2,NULL,writePokemon,(void*)&Arg);
       strcpy(buffer,"");
       menu(buffer,clientSocket);
       
      
     }else{
        if(strcmp(buffer,"3") == 0){
          bytesRcv = recv(clientSocket,&(Arg.success), sizeof(Arg.success), 0);
          printf("Number of queries successfully completed: %d\n",Arg.success);
          printf("List of the files that have been successfully created:\n");
          if(Arg.fileNumber>0){
            for(int i=0; i<Arg.fileNumber; i++){
              printf("%s\n",Arg.fileNames[i]);
            }
            for(int i=0; i<Arg.fileNumber; i++){
              free(Arg.fileNames[i]);
            }
            free(Arg.fileNames);
            free(Arg.fileName);
          }
         
          
          close(clientSocket);  
          printf("CLIENT: Shutting down.\n");
          pthread_exit(NULL);
          break; 
        }else{
          strcpy(buffer,"");
          menu(buffer,clientSocket);
        }
        
     }
     
     pthread_join(save2,NULL);
     strcpy(Arg.result,"");
     bytesRcv = recv(clientSocket,Arg.result,50000, 0);
     Arg.result[bytesRcv] = 0; 
  } 
}

