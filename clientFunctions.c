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

//The clientFunctions.c file contains the implementations of all the functions that are to be used by the Pokemon Query Client. 



/*The queryResults function accepts a buffer and the number returned when creating the client socket as input-output parameters. The function is responsible for getting the preferred pokemon type from the user and sending that input to the server.*/
void queryResults(char *buffer,int clientSocket){
    strcpy(buffer,"");
    printf("Enter a term: ");
    strcpy(buffer,"");
    scanf("%s", buffer); 
    printf("CLIENT: Sending \"%s\" to server.\n", buffer);
        
    send(clientSocket, buffer, strlen(buffer), 0);
        
    strcpy(buffer,"");
  }
  
/*The menu function accepts a buffer and the number returned when creating the client socket as input-output parameters. The function is responsible for displaying the menu and informing the server on what option the user has chosen.*/
void menu(char *buffer,int clientSocket){
    strcpy(buffer,"");
    printf("1.Type Search\n2.Save results\n3.Exit program\n");
    scanf("%s", buffer);
    send(clientSocket, buffer, strlen(buffer), 0);
 } 

 
/*The writeResults function accepts a buffer,the number returned when creating the client socket, and a pointer to a clientArguments structure as input-output parameters. The function is responsible verifying the user enters a valid file name.*/ 
void writeResults(char *buffer,int clientSocket,clientArguments* Arg){
  FILE *fd2;
  strcpy(buffer,"");
  printf("Enter the name of the file: ");
  scanf("%ms",&(Arg->fileName));
  fd2 = fopen(Arg->fileName,"w");
  while(!fd2){
    free(Arg->fileName);
    printf("Unable to create the new file. Please enter the name of the file again.\n");
    scanf("%ms",&(Arg->fileName));
    fd2 = fopen(Arg->fileName,"w");
  }
  fclose(fd2);  
  strcpy(buffer,Arg->fileName);
  
            
  strcpy(buffer,"");     
        
}


/*The writePokemon function accepts a pointer to a clientArguments structure as an input-output parameter. The function is responsible storing the query results into a file.*/ 
void *writePokemon(void *Arg){
  FILE *fd3;
  
  
  clientArguments* ArgCopy = (clientArguments*)Arg;
  
  
    
  fd3 = fopen(ArgCopy->fileName,"w");
  
  
  
  if(sem_wait(&mutex2)<0){
    printf("Error on semaphore wait");
    exit(1);
  }
  
  fprintf(fd3,"%s",ArgCopy->result);
  
  
  ArgCopy->fileNames[ArgCopy->fileNumber] = malloc(100);
  strcpy(ArgCopy->fileNames[ArgCopy->fileNumber],ArgCopy->fileName);
  (ArgCopy->fileNumber)++;
  ArgCopy->fileNames = realloc(ArgCopy->fileNames,(ArgCopy->fileNumber+1)*sizeof(char*));
 
  fclose(fd3);
  if(sem_post(&mutex2)<0){
   printf("Error on sempahore post\n");
   exit(1);
  } 
}
